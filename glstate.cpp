#include <glb/glstate.h>
#include <glb/opengl.h>
#ifdef BL_OS_WINDOWS
#include <GL/wglew.h>
#else
#include <GL/glxew.h>
#endif

namespace glb
{
	// ---------------------------------------------------------------------------------------------------------------------------------------------------------
	// public
	// ---------------------------------------------------------------------------------------------------------------------------------------------------------

	void glstate::bind_buffer(gltarget target, unsigned int id)
	{
		glBindBuffer(target, id);
		_instance()._buffers[target] = id;
	}

	void glstate::release_buffer(gltarget target)
	{
		bind_buffer(target, 0);
	}

	void glstate::bind_vertex_array(unsigned int id)
	{
		glBindVertexArray(id);
		_instance()._vertex_array = id;
	}

	void glstate::release_vertex_array()
	{
		bind_vertex_array(0);
	}

	void glstate::bind_shader_program(unsigned int id)
	{
		glUseProgram(id);
		_instance()._shader_program = id;
	}

	void glstate::release_shader_program()
	{
		bind_shader_program(0);
	}

	void glstate::bind_texture(unsigned int unit, gltarget target, unsigned int id)
	{
		glActiveTexture(GL_TEXTURE0 + unit);
		glBindTexture(target, id);
		_instance()._textures[unit][target] = id;
	}

	void glstate::release_texture(unsigned int unit, gltarget target)
	{
		bind_texture(unit, target, 0);
	}

	unsigned int glstate::get_or_create_uniform_buffer_location(unsigned int buffer_id)
	{
		auto location = alg::find_idx(_instance()._uniform_buffers, buffer_id);
		if(location == _instance()._uniform_buffers.size())
		{
			_instance()._uniform_buffers.push_back(buffer_id);
			glBindBufferBase(target_uniform_buffer, location, buffer_id);
		}
		return location;
	}

	void glstate::set_depth_test(bool enabled)
	{
		if(enabled && !_instance()._depth_test_enabled)
		{
			glEnable(GL_DEPTH_TEST);
		}
		else if(!enabled && _instance()._depth_test_enabled)
		{
			glDisable(GL_DEPTH_TEST);
		}

		_instance()._depth_test_enabled = enabled;
	}

	void glstate::set_backface_culling(bool enabled)
	{
		if(enabled && !_instance()._backface_culling_enabled)
		{
			glCullFace(GL_BACK);
			glEnable(GL_CULL_FACE);
		}
		else if(!enabled && _instance()._backface_culling_enabled)
		{
			glDisable(GL_CULL_FACE);
		}

		_instance()._backface_culling_enabled = enabled;
	}

	void glstate::set_vsync_enabled(bool enabled)
	{
		if((enabled && !_instance()._vsync_enabled) || (!enabled && _instance()._vsync_enabled))
		{
#ifdef BL_OS_WINDOWS
			if(WGLEW_EXT_swap_control)
			{
				wglSwapIntervalEXT(enabled? 1 : 0);
			}
#else
			if(GLXEW_SGI_swap_control)
			{
				glXSwapIntervalSGI(enabled? 1 : 0);
			}
#endif
		}

		_instance()._vsync_enabled = enabled;
	}

	void glstate::reset_state()
	{
		release_vertex_array();
		release_shader_program();
		for(unsigned int i = 0; i < _instance()._textures.size(); ++i)
		{
			release_texture(i, target_texture_buffer);
		}
	}

	// ---------------------------------------------------------------------------------------------------------------------------------------------------------
	// private
	// ---------------------------------------------------------------------------------------------------------------------------------------------------------

	glstate::glstate()
	{
		int id = 0;
		glGetIntegerv(GL_ARRAY_BUFFER_BINDING, &id);
		_buffers.emplace(target_attrib_buffer, id);
		id = 0;
		glGetIntegerv(GL_ELEMENT_ARRAY_BUFFER_BINDING, &id);
		_buffers.emplace(target_element_buffer, id);

		id = 0;
		glGetIntegerv(GL_VERTEX_ARRAY_BINDING, &id);
		_vertex_array = id;

		id = 0;
		glGetIntegerv(GL_CURRENT_PROGRAM, &id);
		_shader_program = id;

		int max_units = 0;
		glGetIntegerv(GL_MAX_COMBINED_TEXTURE_IMAGE_UNITS, &max_units);
		_textures.resize(max_units);

		int active_unit = 0;
		glGetIntegerv(GL_ACTIVE_TEXTURE, &active_unit);

		for(unsigned int i = 0; i < _textures.size(); ++i)
		{
			glActiveTexture(GL_TEXTURE0 + i);
			id = 0;
			glGetIntegerv(GL_TEXTURE_BINDING_BUFFER, &id);
			_textures[i][target_texture_buffer] = id;
		}

		glActiveTexture(active_unit);

		int max_uniform_bindings = 0;
		glGetIntegerv(GL_MAX_UNIFORM_BUFFER_BINDINGS, &max_uniform_bindings);
		_uniform_buffers.reserve(max_uniform_bindings);

		GLboolean enabled = false;
		glGetBooleanv(GL_DEPTH_TEST, &enabled);
		_depth_test_enabled = enabled;

		enabled = false;
		glGetBooleanv(GL_CULL_FACE, &enabled);
		_backface_culling_enabled = enabled;

		// todo: any way to get this?
		_vsync_enabled = true;
	}

	glstate& glstate::_instance()
	{
		static glstate s_state;
		return s_state;
	}
} // namespace glb
