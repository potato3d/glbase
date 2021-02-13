#include <glb/vertex_array.h>
#include <glb/opengl.h>
#include <glb/glstate.h>

namespace glb
{
	// ---------------------------------------------------------------------------------------------------------------------------------------------------------
	// public
	// ---------------------------------------------------------------------------------------------------------------------------------------------------------

	bool vertex_array::create(glmode mode, gltype type)
	{
		_mode = mode;
		_element_type = type;
		return create();
	}

	bool vertex_array::create()
	{
		glGenVertexArrays(1, &_vao_id);
		return _vao_id != 0;
	}

	bool vertex_array::destroy()
	{
		if(_vao_id == 0)
		{
			return true;
		}
		glDeleteVertexArrays(1, &_vao_id);
		_vao_id = 0;
		return true;
	}

	void vertex_array::bind() const
	{
		glstate::bind_vertex_array(_vao_id);
	}

	void vertex_array::release() const
	{
		glstate::release_vertex_array();
	}

	void vertex_array::set_element_count(int count)
	{
		_element_count = count;
	}

	void vertex_array::set_element_byte_offset(int byte_offset)
	{
		_element_byte_offset = byte_offset;
	}

	void vertex_array::draw() const
	{
		bind();
		glDrawElements(_mode, _element_count, _element_type, GLB_BYTE_OFFSET(_element_byte_offset));
	}

	void vertex_array::draw_instanced(int instance_count) const
	{
		bind();
		glDrawElementsInstanced(_mode, _element_count, _element_type, GLB_BYTE_OFFSET(_element_byte_offset), instance_count);
	}
} // namespace glb
