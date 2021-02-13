#pragma once
#include <glb/gltypes.h>
#include <glb/shader_program.h>

namespace glb
{
	class shader_program_builder
	{
	public:
		void begin();

		bool add_file(glshader type, const char* filename);
		bool add_source(glshader type, const char* name, const char* source);

		void bind_vertex_attrib(const char* attrib_name, unsigned int id);
		void bind_draw_buffer(const char* frag_output_name, unsigned int id);

		bool end();

		shader_program get_shader_program();

	private:
		shader_program _program;
	};
} // namespace glb
