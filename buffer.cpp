#include <glb/buffer.h>
#include <glb/opengl.h>
#include <glb/glstate.h>

namespace glb
{
	// ---------------------------------------------------------------------------------------------------------------------------------------------------------
	// public
	// ---------------------------------------------------------------------------------------------------------------------------------------------------------

	bool buffer::create(gltarget target, glusage usage, int capacity_bytes)
	{
		_target = target;
		_usage = usage;
		_capacity_bytes = capacity_bytes;
		return create();
	}

	bool buffer::create(int capacity_bytes)
	{
		_capacity_bytes = capacity_bytes;
		return create();
	}

	bool buffer::create()
	{
		if(!GLEW_VERSION_2_1)
		{
			return false;
		}

		glGenBuffers(1, &_buffer_id);

		if(_buffer_id == 0)
		{
			return false;
		}

		clear();

		return true;
	}

	bool buffer::destroy()
	{
		if(_buffer_id == 0)
		{
			return true;
		}
		glDeleteBuffers(1, &_buffer_id);
		_buffer_id = 0;
		return true;
	}

	unsigned int buffer::get_id() const
	{
		return _buffer_id;
	}

	int buffer::get_capacity_bytes() const
	{
		return _capacity_bytes;
	}

	int buffer::get_count() const
	{
		return _count;
	}

	int buffer::get_size_bytes() const
	{
		return _size_bytes;
	}

	void buffer::bind() const
	{
		glstate::bind_buffer(_target, _buffer_id);
	}

	void buffer::release() const
	{
		glstate::release_buffer(_target);
	}

	void buffer::clear()
	{
		bind();
		glBufferData(_target, _capacity_bytes, nullptr, _usage);
		release();
		_count = 0;
		_size_bytes = 0;
	}

	// ---------------------------------------------------------------------------------------------------------------------------------------------------------
	// private
	// ---------------------------------------------------------------------------------------------------------------------------------------------------------

	void buffer::_add(const void* const items, int count, int size_bytes)
	{
		bind();
		glBufferSubData(_target, _size_bytes, size_bytes, items);
		release();
		_count += count;
		_size_bytes += size_bytes;
	}

	void buffer::_replace(const void* const items, int size_bytes, int offset_bytes)
	{
		bind();
		glBufferSubData(_target, offset_bytes, size_bytes, items);
		release();
	}
} // namespace glb
