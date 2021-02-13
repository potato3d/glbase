#pragma once
#include <glb/vertex_attrib.h>
#include <glb/buffer.h>
#include <bl/bl.h>

namespace glb
{
	class vertex_specification
	{
	public:
		typedef unsigned int attrib_buffer_id;
		static const attrib_buffer_id INVALID;

		bool setup_vertex_buffer(glusage usage, int suggested_capacity_bytes);
		bool setup_element_buffer(glusage usage, int suggested_capacity_bytes);
		attrib_buffer_id add_attrib_buffer(glusage usage, int suggested_capacity_bytes);

		unsigned int add_vertex_attrib(const vertex_attrib& attrib);
		unsigned int add_instanced_attrib(attrib_buffer_id id, const vertex_attrib& attrib, unsigned int instancing_rate = 1);

		void create_buffers();
		void bind_to_current_vertex_array() const;

		buffer& get_vertex_buffer();
		buffer& get_element_buffer();
		buffer& get_attrib_buffer(attrib_buffer_id id);

		int get_suggested_vertex_capacity() const;
		int get_suggested_element_capacity() const;
		int get_suggested_attrib_capacity(attrib_buffer_id id) const;

	private:
		struct id_attrib
		{
			unsigned int id;
			vertex_attrib attrib;
			unsigned int instancing_rate;
		};

		struct buffer_attrib_binding
		{
			buffer contents;
			vector<id_attrib> attribs;
		};

	private:
		unsigned int _overall_attrib_count = 0;
		buffer_attrib_binding _vertex_buffer;
		buffer _element_buffer;
		vector<buffer_attrib_binding> _attrib_buffers;
		int _suggested_vertex_capacity = 0;
		int _suggested_element_capacity = 0;
		vector<int> _suggested_attrib_capacities;
	};
} // namespace glb
