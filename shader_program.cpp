#include <glb/shader_program.h>
#include <glb/buffer.h>
#include <glb/glstate.h>
#include <glb/opengl.h>

namespace glb
{
	// ---------------------------------------------------------------------------------------------------------------------------------------------------------
	// public
	// ---------------------------------------------------------------------------------------------------------------------------------------------------------

	bool shader_program::create()
	{
		_program_id = glCreateProgram();
		return _program_id != 0;
	}

	bool shader_program::destroy()
	{
		if(_program_id == 0)
		{
			return true;
		}
		glDeleteProgram(_program_id);
		_program_id = 0;
		return true;
	}

	unsigned int shader_program::get_id() const
	{
		return _program_id;
	}

	void shader_program::set_uniform(const char* uniform_name, int value)
	{
		glProgramUniform1i(_program_id, glGetUniformLocation(_program_id, uniform_name), value);
	}

	void shader_program::bind_uniform_buffer(const char* uniform_block_name, const buffer& uniform_buffer)
	{
		const auto location = glstate::get_or_create_uniform_buffer_location(uniform_buffer.get_id());
		glUniformBlockBinding(_program_id, glGetUniformBlockIndex(_program_id, uniform_block_name), location);
	}

	void shader_program::bind() const
	{
		glstate::bind_shader_program(_program_id);
	}

	void shader_program::release() const
	{
		glstate::release_shader_program();
	}
} // namespace glb
