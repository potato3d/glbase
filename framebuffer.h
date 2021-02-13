#pragma once
#include <glb/gltypes.h>
#include <bl/bl.h>

namespace glb
{
	class framebuffer
	{
	public:
		typedef unsigned int color_buffer_id;

		framebuffer();

		void set_use_textures(bool use);

		color_buffer_id add_color_buffer();

		void set_color_format(color_buffer_id id, glinternal_format format);
		void set_clear_color(color_buffer_id id, float r, float g, float b, float a = 1.0f);

		void set_color_buffer_to_display(color_buffer_id id);
		color_buffer_id get_color_buffer_to_display() const;

		void set_depth_format(glinternal_format format);
		void set_clear_depth(float value);
		void set_create_depth_mipmaps(bool create);

		bool initialize();
		bool finalize();

		void resize_screen(int w, int h);

		void read_color_pixels(unsigned int color_buffer_idx, gltype type, int x, int y, int width, int height, void* pixels);
		void read_depth_pixels(gltype type, int x, int y, int width, int height, void* pixels);

		void begin_frame();
		void clear();
		void display_on(gldisplay_buffer buffer);
		void end_frame();

		int update_depth_mipmaps();
		void draw_depth_mipmap(int level);
		void bind_depth_texture();

		unsigned int get_color_buffer_name(unsigned int color_buffer_id);
		unsigned int get_depth_buffer_name();

		unsigned int get_width();
		unsigned int get_height();

		unsigned int get_id();

	private:
		struct renderbuffer
		{
			unsigned int id;
			glinternal_format internal_format;
		};

		struct color
		{
			float r, g, b, a;
		};

	private:
		bool _use_textures = false;
		unsigned int _fbo_id = 0;
		unsigned int _width = 1;
		unsigned int _height = 1;
		vector<renderbuffer> _color_buffers;
		vector<unsigned int> _color_attachments;
		vector<color> _clear_colors;
		unsigned int _color_buffer_to_display = 0;
		renderbuffer _depth_buffer = {0, internal_format_depth32f};
		float _clear_depth = 0.0f;
		bool _create_depth_mipmaps = false;
		unsigned int _depth_mipmap_texture = 0;
		unsigned int _depth_mipmap_program = 0;
		unsigned int _draw_depth_mipmap_program = 0;
	};
} // namespace glb
