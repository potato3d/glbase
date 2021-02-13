#pragma once
#include <glb/gltypes.h>

namespace glb
{
	class buffer
	{
	public:
		bool create(gltarget target, glusage usage, int capacity_bytes);
		bool create(int capacity_bytes);
		bool create();
		bool destroy();

		unsigned int get_id() const;
		int get_capacity_bytes() const;
		int get_count() const;
		int get_size_bytes() const;

		void bind() const;
		void release() const;

		void clear();

		template<typename t_value>
		inline bool can_add(int count);

		template<typename t_value>
		inline void add(const t_value& contents);

		template<typename t_value>
		inline void add(const t_value* const items, int count);

		template<typename t_value>
		inline void replace(const t_value* const items, int count, int offset = 0);

	private:
		void _add(const void* const items, int count, int size_bytes);
		void _replace(const void* const items, int size_bytes, int offset_bytes);

	private:
		unsigned int _buffer_id = 0;
		gltarget _target = target_attrib_buffer;
		glusage _usage = usage_static_draw;
		int _capacity_bytes = 0;
		int _count = 0;
		int _size_bytes = 0;
	};

	template<typename t_value>
	bool buffer::can_add(int count)
	{
		return _size_bytes + (count * sizeof(t_value)) <= _capacity_bytes;
	}

	template<typename t_value>
	void buffer::add(const t_value& contents)
	{
		add(&contents, 1);
	}

	template<typename t_value>
	void buffer::add(const t_value* const items, int count)
	{
		_add(items, count, count * sizeof(t_value));
	}

	template<typename t_value>
	void buffer::replace(const t_value* const items, int count, int offset /*= 0*/)
	{
		_replace(items, count * sizeof(t_value), offset * sizeof(t_value));
	}
} // namespace glb
