#include <glb/vertex_specification.h>
#include <glb/opengl.h>

namespace glb
{
	// ---------------------------------------------------------------------------------------------------------------------------------------------------------
	// static
	// ---------------------------------------------------------------------------------------------------------------------------------------------------------

	const vertex_specification::attrib_buffer_id vertex_specification::INVALID = math::limit_pos<vertex_specification::attrib_buffer_id>();

	// ---------------------------------------------------------------------------------------------------------------------------------------------------------
	// public
	// ---------------------------------------------------------------------------------------------------------------------------------------------------------

	bool vertex_specification::setup_vertex_buffer(glusage usage, int suggested_capacity_bytes)
	{
		_suggested_element_capacity = suggested_capacity_bytes;
		return _vertex_buffer.contents.create(target_attrib_buffer, usage, suggested_capacity_bytes);
	}

	bool vertex_specification::setup_element_buffer(glusage usage, int suggested_capacity_bytes)
	{
		_suggested_vertex_capacity = suggested_capacity_bytes;
		return _element_buffer.create(target_element_buffer, usage, suggested_capacity_bytes);
	}

	vertex_specification::attrib_buffer_id vertex_specification::add_attrib_buffer(glusage usage, int suggested_capacity_bytes)
	{
		buffer generic_buffer;
		if(!generic_buffer.create(target_attrib_buffer, usage, suggested_capacity_bytes))
		{
			return INVALID;
		}
		_suggested_attrib_capacities.push_back(suggested_capacity_bytes);
		_attrib_buffers.push_back({generic_buffer, {}});
		return _attrib_buffers.size() - 1;
	}

	unsigned int vertex_specification::add_vertex_attrib(const vertex_attrib& attrib)
	{
		_vertex_buffer.attribs.push_back({_overall_attrib_count, attrib, 0});
		return _overall_attrib_count++;
	}

	unsigned int vertex_specification::add_instanced_attrib(attrib_buffer_id id, const vertex_attrib& attrib, unsigned int instancing_rate /*= 1*/)
	{
		_attrib_buffers[id].attribs.push_back({_overall_attrib_count, attrib, instancing_rate});
		return _overall_attrib_count++;
	}

	void vertex_specification::create_buffers()
	{
		_vertex_buffer.contents.create();
		_element_buffer.create();
		for(auto& b: _attrib_buffers)
		{
			b.contents.create();
		}
	}

	void vertex_specification::bind_to_current_vertex_array() const
	{
		_element_buffer.bind();

		_vertex_buffer.contents.bind();
		for(const auto& a : _vertex_buffer.attribs)
		{
			glEnableVertexAttribArray(a.id);
			glVertexAttribPointer(a.id, a.attrib.size, a.attrib.type, a.attrib.normalized, a.attrib.stride, GLB_BYTE_OFFSET(a.attrib.byte_offset));
		}
		_vertex_buffer.contents.release();

		for(auto& b : _attrib_buffers)
		{
			b.contents.bind();
			for(const auto& a : b.attribs)
			{
				glEnableVertexAttribArray(a.id);
				glVertexAttribPointer(a.id, a.attrib.size, a.attrib.type, a.attrib.normalized, a.attrib.stride, GLB_BYTE_OFFSET(a.attrib.byte_offset));
				glVertexAttribDivisor(a.id, a.instancing_rate);
			}
			b.contents.release();
		}
	}

	buffer& vertex_specification::get_element_buffer()
	{
		return _element_buffer;
	}

	buffer& vertex_specification::get_vertex_buffer()
	{
		return _vertex_buffer.contents;
	}

	buffer& vertex_specification::get_attrib_buffer(attrib_buffer_id id)
	{
		return _attrib_buffers[id].contents;
	}

	int vertex_specification::get_suggested_vertex_capacity() const
	{
		return _suggested_vertex_capacity;
	}

	int vertex_specification::get_suggested_element_capacity() const
	{
		return _suggested_element_capacity;
	}

	int vertex_specification::get_suggested_attrib_capacity(attrib_buffer_id id) const
	{
		return _suggested_attrib_capacities[id];
	}
} // namespace glb
