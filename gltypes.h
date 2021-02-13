#pragma once

namespace glb
{
	enum glformat
	{
		format_red         = 0x1903, // GL_RED
		format_red_integer = 0x8D94, // GL_RED_INTEGER
		format_rg          = 0x8227, // GL_RG
		format_rgb         = 0x1907, // GL_RGB
		format_rgba        = 0x1908, // GL_RGBA
		format_depth       = 0x1902, // GL_DEPTH_COMPONENT
	};

	enum glinternal_format
	{
		internal_format_r8ui     = 0x8232, // GL_R8UI    - unsigned byte, not normalized
		internal_format_r32ui    = 0x8236, // GL_R32UI   - unsigned int, not normalized
		internal_format_rgba8ui  = 0x8D7C, // GL_RGBA8UI - unsigned byte, not normalized
		internal_format_rgba8    = 0x8058, // GL_RGBA8
		internal_format_r32f     = 0x822E, // GL_R32F
		internal_format_rg32f    = 0x8230, // GL_RG32F
		internal_format_rgb32f   = 0x8815, // GL_RGB32F
		internal_format_rgba32f  = 0x8814, // GL_RGBA32F
		internal_format_depth24  = 0x81A6, // GL_DEPTH_COMPONENT24
		internal_format_depth32f = 0x8CAC, // GL_DEPTH_COMPONENT32F
	};

	enum gldisplay_buffer
	{
		display_back  = 0x0405, // GL_BACK
		display_front = 0x0404  // GL_FRONT
	};

	enum gltype
	{
		type_ubyte  = 0x1401, // GL_UNSIGNED_BYTE
		type_ushort = 0x1403, // GL_UNSIGNED_SHORT
		type_uint   = 0x1405, // GL_UNSIGNED_INT
		type_float  = 0x1406, // GL_FLOAT
	};

	enum glmode
	{
		mode_triangles = 0x0004, // GL_TRIANGLES
		mode_points = 0x0000, // GL_POINTS
	};

	enum gltarget
	{
		target_attrib_buffer  = 0x8892, // GL_ARRAY_BUFFER
		target_element_buffer = 0x8893, // GL_ELEMENT_ARRAY_BUFFER
		target_uniform_buffer = 0x8A11, // GL_UNIFORM_BUFFER
		target_texture_buffer = 0x8C2A, // GL_TEXTURE_BUFFER
	};

	enum glusage
	{
		usage_static_draw  = 0x88E4, // GL_STATIC_DRAW
		usage_dynamic_draw = 0x88E8, // GL_DYNAMIC_DRAW
		usage_stream_draw  = 0x88E0, // GL_STREAM_DRAW
	};

	enum glshader
	{
		shader_vertex    = 0x8B31, // GL_VERTEX_SHADER
		shader_tess_ctrl = 0x8E88, // GL_TESS_CONTROL_SHADER
		shader_tess_eval = 0x8E87, // GL_TESS_EVALUATION_SHADER
		shader_geometry  = 0x8DD9, // GL_GEOMETRY_SHADER
		shader_fragment  = 0x8B30, // GL_FRAGMENT_SHADER
		shader_compute   = 0x91B9  // GL_COMPUTE_SHADER
	};

	inline glformat to_format(glinternal_format internal_format)
	{
		switch(internal_format)
		{
		case internal_format_r8ui:
		case internal_format_r32ui:
		case internal_format_r32f:
			return format_red;

		case internal_format_rgba8ui:
		case internal_format_rgba8:
		case internal_format_rgba32f:
			return format_rgba;

		case internal_format_rg32f:
			return format_rg;

		case internal_format_rgb32f:
			return format_rgb;

		case internal_format_depth24:
		case internal_format_depth32f:
			return format_depth;

		default:
			return format_rgba;
		}
	}
} // namespace glb
