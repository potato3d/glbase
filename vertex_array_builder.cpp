#include <glb/vertex_array_builder.h>
#include <glb/opengl.h>
#include <glb/glstate.h>

namespace glb
{
	// ---------------------------------------------------------------------------------------------------------------------------------------------------------
	// public
	// ---------------------------------------------------------------------------------------------------------------------------------------------------------

	bool vertex_array_builder::initialize(vertex_specification spec, glmode draw_mode, gltype element_type)
	{
		if(!GLEW_VERSION_3_0)
		{
			return false;
		}
		_spec = spec;
		return _current_vao.create(draw_mode, element_type);
	}

	bool vertex_array_builder::finalize()
	{
		return _current_vao.destroy();
	}

	void vertex_array_builder::begin()
	{
		_completed_vaos.clear();
		_spec.create_buffers();
		_initialize_current_vao();
	}

	bool vertex_array_builder::end()
	{
		_finalize_current_vao();
		return !_completed_vaos.empty();
	}

	vector<vertex_array> vertex_array_builder::get_vertex_arrays()
	{
		return _completed_vaos;
	}

	// ---------------------------------------------------------------------------------------------------------------------------------------------------------
	// private
	// ---------------------------------------------------------------------------------------------------------------------------------------------------------

	void vertex_array_builder::_initialize_current_vao()
	{
		_current_vao.create();
		_current_vao.bind();
		_spec.bind_to_current_vertex_array();
		_current_vao.release();
		_current_vao.set_element_byte_offset(_spec.get_element_buffer().get_size_bytes());
		_current_element_count = 0;
	}

	void vertex_array_builder::_finalize_current_vao()
	{
		_current_vao.set_element_count(_current_element_count);
		_completed_vaos.push_back(_current_vao);
	}
} // namespace glb
