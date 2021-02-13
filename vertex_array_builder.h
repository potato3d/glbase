#pragma once
#include <glb/vertex_attrib.h>
#include <glb/vertex_array.h>
#include <glb/vertex_specification.h>

namespace glb
{
	class vertex_array_builder
	{
	public:
		bool initialize(vertex_specification spec, glmode draw_mode, gltype element_type);
		bool finalize();

		void begin();

		template<typename t_vertex, typename t_element>
		inline void add_mesh(t_vertex* vertices, int vertex_count, t_element* elements, int element_count);

		template<typename t_attrib>
		inline void add_instance_attribs(vertex_specification::attrib_buffer_id id, t_attrib* attribs, int count);

		bool end();

		vector<vertex_array> get_vertex_arrays();

//	private:
		void _initialize_current_vao();
		void _finalize_current_vao();

//	private:
		vertex_specification _spec;
		int _current_element_count = 0;
		vertex_array _current_vao;
		vector<vertex_array> _completed_vaos;
	};

	template<typename t_vertex, typename t_element>
	void vertex_array_builder::add_mesh(t_vertex* vertices, int vertex_count, t_element* elements, int element_count)
	{
		bool need_new_vao = false;

		const int vertex_size_bytes = vertex_count * sizeof(t_vertex);
		auto& vertex_buffer = _spec.get_vertex_buffer();
		if(vertex_buffer.get_size_bytes() + vertex_size_bytes > vertex_buffer.get_capacity_bytes())
		{
			vertex_buffer.create(math::max(vertex_size_bytes, _spec.get_suggested_vertex_capacity()));
			need_new_vao = true;
		}

		const int element_size_bytes = element_count * sizeof(t_element);
		auto& element_buffer = _spec.get_element_buffer();
		if(element_buffer.get_size_bytes() + element_size_bytes > element_buffer.get_capacity_bytes())
		{
			element_buffer.create(math::max(element_size_bytes, _spec.get_suggested_element_capacity()));
			need_new_vao = true;
		}

		if(need_new_vao)
		{
			_finalize_current_vao();
			_initialize_current_vao();
		}

		for(int i = 0; i < element_count; ++i)
		{
			elements[i] += vertex_buffer.get_count();
		}

		vertex_buffer.add(vertices, vertex_count);
		element_buffer.add(elements, element_count);

		_current_element_count += element_count;
	}

	template<typename t_attrib>
	void vertex_array_builder::add_instance_attribs(vertex_specification::attrib_buffer_id id, t_attrib* attribs, int count)
	{
		const int attrib_size_bytes = count * sizeof(t_attrib);
		auto& attrib_buffer = _spec.get_attrib_buffer(id);
		if(attrib_buffer.get_size_bytes() + attrib_size_bytes >= attrib_buffer.get_capacity_bytes())
		{
			attrib_buffer.create(math::max(attrib_size_bytes, _spec.get_suggested_attrib_capacity(id)));
			_finalize_current_vao();
			_initialize_current_vao();
		}

		attrib_buffer.add(attribs, count);
	}
} // namespace glb
