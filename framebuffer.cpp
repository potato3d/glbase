#include <glb/framebuffer.h>
#include <glb/opengl.h>
#include <glb/shader_program_builder.h>

namespace glb
{
#define FOR_EACH_COLOR_BUFFER for(unsigned int i = 0; i < _color_buffers.size(); ++i)

	// ---------------------------------------------------------------------------------------------------------------------------------------------------------
	// public
	// ---------------------------------------------------------------------------------------------------------------------------------------------------------

	framebuffer::framebuffer()
	{
		set_color_buffer_to_display(add_color_buffer());
	}

	void framebuffer::set_use_textures(bool use)
	{
		_use_textures = use;
	}

	framebuffer::color_buffer_id framebuffer::add_color_buffer()
	{
		_color_buffers.push_back({0, internal_format_rgba8});
		_color_attachments.push_back(GL_COLOR_ATTACHMENT0 + _color_attachments.size());
		_clear_colors.push_back({0.0f, 0.0f, 0.0f, 1.0f});
		return _color_buffers.size() - 1;
	}

	void framebuffer::set_color_format(color_buffer_id id, glinternal_format format)
	{
		_color_buffers[id].internal_format = format;
	}

	void framebuffer::set_clear_color(color_buffer_id id, float r, float g, float b, float a /*= 1.0f*/)
	{
		_clear_colors[id] = {r, g, b, a};
	}

	void framebuffer::set_color_buffer_to_display(color_buffer_id id)
	{
		_color_buffer_to_display = id;
	}

	framebuffer::color_buffer_id framebuffer::get_color_buffer_to_display() const
	{
		return _color_buffer_to_display;
	}

	void framebuffer::set_depth_format(glinternal_format format)
	{
		_depth_buffer.internal_format = format;
	}

	void framebuffer::set_clear_depth(float value)
	{
		_clear_depth = value;
	}

	void framebuffer::set_create_depth_mipmaps(bool create)
	{
		_create_depth_mipmaps = create;
	}

	bool framebuffer::initialize()
	{
		// check OpenGL version
		if(!GLEW_VERSION_3_2)
		{
			return false;
		}

		// need at least one color buffer
		if(_color_attachments.empty())
		{
			return false;
		}

		// create frame buffer
		glGenFramebuffers(1, &_fbo_id);
		glBindFramebuffer(GL_FRAMEBUFFER, _fbo_id);

		// for each color buffer
		if(_use_textures)
		{
			FOR_EACH_COLOR_BUFFER
			{
				// create
				glGenTextures(1, &_color_buffers[i].id);
				glBindTexture(GL_TEXTURE_2D, _color_buffers[i].id);
				glTexImage2D(GL_TEXTURE_2D, 0, _color_buffers[i].internal_format, _width, _height, 0, to_format(_color_buffers[i].internal_format), GL_UNSIGNED_BYTE, NULL);
				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
				glBindTexture(GL_TEXTURE_2D, 0);

				// attach
				glFramebufferTexture(GL_FRAMEBUFFER, _color_attachments[i], _color_buffers[i].id, 0);
			}
		}
		else
		{
			FOR_EACH_COLOR_BUFFER
			{
				// create
				glGenRenderbuffers(1, &_color_buffers[i].id);
				glBindRenderbuffer(GL_RENDERBUFFER, _color_buffers[i].id);
				glRenderbufferStorage(GL_RENDERBUFFER, _color_buffers[i].internal_format, _width, _height);
				glBindRenderbuffer(GL_RENDERBUFFER, 0);

				// attach
				glFramebufferRenderbuffer(GL_FRAMEBUFFER, _color_attachments[i], GL_RENDERBUFFER, _color_buffers[i].id);
			}
		}

		// create depth buffer
		if(_use_textures)
		{
			glGenTextures(1, &_depth_buffer.id);
			glBindTexture(GL_TEXTURE_2D, _depth_buffer.id);
			glTexImage2D(GL_TEXTURE_2D, 0, _depth_buffer.internal_format, _width, _height, 0, to_format(_depth_buffer.internal_format), GL_FLOAT, NULL);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
			if(_create_depth_mipmaps)
			{
				glGenTextures(1, &_depth_mipmap_texture);
				glBindTexture(GL_TEXTURE_2D, _depth_mipmap_texture);
				glTexImage2D(GL_TEXTURE_2D, 0, _depth_buffer.internal_format, _width, _height, 0, to_format(_depth_buffer.internal_format), GL_FLOAT, NULL);
				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST_MIPMAP_NEAREST);
				glGenerateMipmap(GL_TEXTURE_2D);
			}
			glBindTexture(GL_TEXTURE_2D, 0);
		}
		else
		{
			glGenRenderbuffers(1, &_depth_buffer.id);
			glBindRenderbuffer(GL_RENDERBUFFER, _depth_buffer.id);
			glRenderbufferStorage(GL_RENDERBUFFER, _depth_buffer.internal_format, _width, _height);
			glBindRenderbuffer(GL_RENDERBUFFER, 0);
		}

		// attach depth buffer
		if(_use_textures)
		{
			glFramebufferTexture(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, _depth_buffer.id, 0);
		}
		else
		{
			glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, _depth_buffer.id);
		}

		// check FBO configuration
		GLenum status = glCheckFramebufferStatus(GL_FRAMEBUFFER);

		// cleanup
		glBindFramebuffer(GL_FRAMEBUFFER, 0);

		if(status != GL_FRAMEBUFFER_COMPLETE)
		{
			return false;
		}

		// create shader program to update depth mipmaps later
		if(_create_depth_mipmaps)
		{
			glb::shader_program_builder depth_mipmap_shader_builder;
			depth_mipmap_shader_builder.begin();
			if(!depth_mipmap_shader_builder.add_file(glb::shader_geometry, "../shaders/depth_mipmap.geom"))
			{
				return false;
			}
			if(!depth_mipmap_shader_builder.add_file(glb::shader_vertex, "../shaders/depth_mipmap.vert"))
			{
				return false;
			}
			if(!depth_mipmap_shader_builder.add_file(glb::shader_fragment, "../shaders/depth_mipmap.frag"))
			{
				return false;
			}
			if(!depth_mipmap_shader_builder.end())
			{
				return false;
			}
			_depth_mipmap_program = depth_mipmap_shader_builder.get_shader_program().get_id();

			glb::shader_program_builder draw_depth_mipmap_shader_builder;
			draw_depth_mipmap_shader_builder.begin();
			if(!draw_depth_mipmap_shader_builder.add_file(glb::shader_geometry, "../shaders/depth_mipmap.geom"))
			{
				return false;
			}
			if(!draw_depth_mipmap_shader_builder.add_file(glb::shader_vertex, "../shaders/depth_mipmap.vert"))
			{
				return false;
			}
			if(!draw_depth_mipmap_shader_builder.add_file(glb::shader_fragment, "../shaders/draw_depth_mipmap.frag"))
			{
				return false;
			}
			if(!draw_depth_mipmap_shader_builder.end())
			{
				return false;
			}

			_draw_depth_mipmap_program = draw_depth_mipmap_shader_builder.get_shader_program().get_id();
		}

		return true;
	}

	bool framebuffer::finalize()
	{
		// bind framebuffer so when its renderbuffers are deleted they also get detached
		glBindFramebuffer(GL_FRAMEBUFFER, _fbo_id);
		if(_use_textures)
		{
			FOR_EACH_COLOR_BUFFER
			{
				glDeleteTextures(1, &_color_buffers[i].id);
			}
		}
		else
		{
			FOR_EACH_COLOR_BUFFER
			{
				glDeleteRenderbuffers(1, &_color_buffers[i].id);
			}

		}
		if(_use_textures)
		{
			glDeleteTextures(1, &_depth_buffer.id);
		}
		else
		{
			glDeleteRenderbuffers(1, &_depth_buffer.id);
		}
		glBindFramebuffer(GL_FRAMEBUFFER, 0);
		glDeleteFramebuffers(1, &_fbo_id);
		return true;
	}

	void framebuffer::resize_screen(int w, int h)
	{
		_width = w;
		_height = h;

		// resize color buffers
		if(_use_textures)
		{
			FOR_EACH_COLOR_BUFFER
			{
				glBindTexture(GL_TEXTURE_2D, _color_buffers[i].id);
				glTexImage2D(GL_TEXTURE_2D, 0, _color_buffers[i].internal_format, _width, _height, 0, to_format(_color_buffers[i].internal_format), GL_UNSIGNED_BYTE, NULL);
			}
		}
		else
		{
			FOR_EACH_COLOR_BUFFER
			{
				glBindRenderbuffer(GL_RENDERBUFFER, _color_buffers[i].id);
				glRenderbufferStorage(GL_RENDERBUFFER, _color_buffers[i].internal_format, _width, _height);
			}
		}

		// resize depth buffer
		if(_depth_buffer.id > 0)
		{
			if(_use_textures)
			{
				glBindTexture(GL_TEXTURE_2D, _depth_buffer.id);
				glTexImage2D(GL_TEXTURE_2D, 0, _depth_buffer.internal_format, _width, _height, 0, to_format(_depth_buffer.internal_format), GL_FLOAT, NULL);
				if(_create_depth_mipmaps)
				{
					glBindTexture(GL_TEXTURE_2D, _depth_mipmap_texture);
					glTexImage2D(GL_TEXTURE_2D, 0, _depth_buffer.internal_format, _width, _height, 0, to_format(_depth_buffer.internal_format), GL_FLOAT, NULL);
					glGenerateMipmap(GL_TEXTURE_2D); // reallocate mipmap memory
				}
			}
			else
			{
				glBindRenderbuffer(GL_RENDERBUFFER, _depth_buffer.id);
				glRenderbufferStorage(GL_RENDERBUFFER, _depth_buffer.internal_format, _width, _height);
			}
		}

		// cleanup
		if(_use_textures)
		{
			glBindTexture(GL_TEXTURE_2D, 0);
		}
		else
		{
			glBindRenderbuffer(GL_RENDERBUFFER, 0);
		}
	}

	void framebuffer::read_color_pixels(unsigned int color_idx, gltype type, int x, int y, int width, int height, void* pixels)
	{
		// set attachment to read from
		glBindFramebuffer(GL_READ_FRAMEBUFFER, _fbo_id);
		glReadBuffer(_color_attachments[color_idx]);

		// get pixel data
		glReadPixels(x, y, width, height, to_format(_color_buffers[color_idx].internal_format), type, pixels);

		// cleanup
		glBindFramebuffer(GL_READ_FRAMEBUFFER, 0);
		glReadBuffer(GL_BACK);
	}

	void framebuffer::read_depth_pixels(gltype type, int x, int y, int width, int height, void* pixels)
	{
		// set attachment to read from
		glBindFramebuffer(GL_READ_FRAMEBUFFER, _fbo_id);
		glReadBuffer(_color_attachments[0]);

		// get pixel data
		glReadPixels(x, y, width, height, to_format(_depth_buffer.internal_format), type, pixels);

		// cleanup
		glBindFramebuffer(GL_READ_FRAMEBUFFER, 0);
		glReadBuffer(GL_BACK);
	}

	void framebuffer::begin_frame()
	{
		// bind frame buffer to draw
		glBindFramebuffer(GL_DRAW_FRAMEBUFFER, _fbo_id);

		// set draw buffers to color buffers
		glDrawBuffers(_color_attachments.size(), _color_attachments.data());

		// adjust to draw to entire image
		glViewport(0, 0, _width, _height);
	}

	void framebuffer::clear()
	{
		// clear color buffers separately so each gets a different color
		// todo: could do this by rendering a full-screen quad with multiple outputs
		FOR_EACH_COLOR_BUFFER
		{
			glClearBufferfv(GL_COLOR, i, (const float*)&_clear_colors[i]);
		}

		// clear depth
		if(_depth_buffer.id > 0)
		{
			glClear(GL_DEPTH_BUFFER_BIT);
		}
	}

	void framebuffer::display_on(gldisplay_buffer buffer)
	{
		GLB_PRINT_GL_ERROR;

		// setup read from frame buffer
		glBindFramebuffer(GL_READ_FRAMEBUFFER, _fbo_id);

		GLB_PRINT_GL_ERROR;

		glReadBuffer(_color_attachments[_color_buffer_to_display]);

		GLB_PRINT_GL_ERROR;

		// setup write to given gl buffer
		glBindFramebuffer(GL_DRAW_FRAMEBUFFER, 0);

		GLB_PRINT_GL_ERROR;

		glDrawBuffer(buffer);

		GLB_PRINT_GL_ERROR;

		// copy from frame buffer to given gl buffer
		glBlitFramebuffer(0, 0, _width, _height, 0, 0, _width, _height, GL_COLOR_BUFFER_BIT, GL_NEAREST);

		GLB_PRINT_GL_ERROR;

		// cleanup
		glBindFramebuffer(GL_READ_FRAMEBUFFER, 0);

		GLB_PRINT_GL_ERROR;

		glReadBuffer(GL_BACK);

		GLB_PRINT_GL_ERROR;
	}

	void framebuffer::end_frame()
	{
		glBindFramebuffer(GL_DRAW_FRAMEBUFFER, 0);
	}

	int framebuffer::update_depth_mipmaps()
	{
		// copy current depth buffer to mipmap texture
		glCopyImageSubData(_depth_buffer.id, GL_TEXTURE_2D, 0, 0, 0, 0, _depth_mipmap_texture, GL_TEXTURE_2D, 0, 0, 0, 0, _width, _height, 1);

		// activate hi-z program
		glUseProgram(_depth_mipmap_program);

		// disable color buffer as we will render only a depth image
		glColorMask(GL_FALSE, GL_FALSE, GL_FALSE, GL_FALSE);

		// bind depth mipmap texture
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, _depth_mipmap_texture);

		// we have to disable depth testing but allow depth writes
		glDepthFunc(GL_ALWAYS);

		// calculate the number of mipmap levels for NPOT texture
		int numLevels = 1 + (int)floorf(log2f(math::max(_width, _height)));
		int currentWidth = _width;
		int currentHeight = _height;

		for(int i = 1; i < numLevels; ++i)
		{
			glProgramUniform1i(_depth_mipmap_program, 0, i-1);
			glProgramUniform1i(_depth_mipmap_program, 1, (currentHeight%2 == 0) && (currentWidth%2 == 0));

			// calculate next viewport size
			currentWidth /= 2;
			currentHeight /= 2;

			// ensure that the viewport size is always at least 1x1
			currentWidth = currentWidth > 0 ? currentWidth : 1;
			currentHeight = currentHeight > 0 ? currentHeight : 1;
			glViewport(0, 0, currentWidth, currentHeight);

			// bind next level for rendering but first restrict fetches only to previous level
			glFramebufferTexture(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, _depth_mipmap_texture, i);

			// dummy draw command as the full screen quad is generated completely by a geometry shader
			glDrawArrays(GL_POINTS, 0, 1);
		}

		// reset the framebuffer configuration
		glFramebufferTexture(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, _depth_buffer.id, 0);

		// reenable color buffer writes, reset viewport and reenable depth test
		glDepthFunc(GL_LEQUAL);
		glColorMask(GL_TRUE, GL_TRUE, GL_TRUE, GL_TRUE);
		glViewport(0, 0, _width, _height);

		return numLevels;
	}

	void framebuffer::draw_depth_mipmap(int level)
	{
		glProgramUniform1ui(_draw_depth_mipmap_program, 0, level);
		glUseProgram(_draw_depth_mipmap_program);
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, _depth_mipmap_texture);
		glDisable(GL_DEPTH_TEST);
		glDrawArrays(GL_POINTS, 0, 1);
		glEnable(GL_DEPTH_TEST);
	}

	void framebuffer::bind_depth_texture()
	{
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, _depth_mipmap_texture);
	}

	unsigned int framebuffer::get_color_buffer_name(unsigned int color_buffer_id)
	{
		return _color_buffers[color_buffer_id].id;
	}

	unsigned int framebuffer::get_depth_buffer_name()
	{
		return _depth_buffer.id;
	}

	unsigned int framebuffer::get_width()
	{
		return _width;
	}

	unsigned int framebuffer::get_height()
	{
		return _height;
	}

	unsigned int framebuffer::get_id()
	{
		return _fbo_id;
	}

#undef FOR_EACH_COLOR_BUFFER
} // namespace glb
