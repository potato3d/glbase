#pragma once
#include <glb/gltypes.h>

namespace glb
{
	class vertex_array
	{
	public:
		bool create(glmode mode, gltype type);
		bool create();
		bool destroy();

		void bind() const;
		void release() const;

		void set_element_count(int count);
		void set_element_byte_offset(int byte_offset);

		void draw() const;
		void draw_instanced(int instance_count) const;

	private:
		unsigned int _vao_id = 0;
		glmode _mode = mode_triangles;
		gltype _element_type = type_uint;
		int _element_count = 0;
		int _element_byte_offset = 0;
	};
} // namespace glb
