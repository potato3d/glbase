#pragma once

namespace glb
{
	class buffer;

	class shader_program
	{
	public:
		bool create();
		bool destroy();

		unsigned int get_id() const;

		void set_uniform(const char* uniform_name, int value);
		void bind_uniform_buffer(const char* uniform_block_name, const buffer& uniform_buffer);

		void bind() const;
		void release() const;

	private:
		unsigned int _program_id = 0;
	};
} // namespace glb
