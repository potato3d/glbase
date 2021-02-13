#include <glb/shader_program_builder.h>
#include <glb/opengl.h>

namespace glb
{
	// ---------------------------------------------------------------------------------------------------------------------------------------------------------
	// global constants
	// ---------------------------------------------------------------------------------------------------------------------------------------------------------

	static string INCLUDE_STMT = "#include";

	// ---------------------------------------------------------------------------------------------------------------------------------------------------------
	// helper functions
	// ---------------------------------------------------------------------------------------------------------------------------------------------------------

	static string load_shader_file(const string& filename)
	{
		ifstream file;
		file.open(filename.data());
		if(!file)
		{
			return "";
		}

		string buff;
		string line;
		while(getline(file, line))
		{
			auto include_token = line.find(INCLUDE_STMT);
			if(include_token != string::npos)
			{
				string include_file = line.substr(include_token+INCLUDE_STMT.size());
				include_file = str::trim_all(include_file, " \"");
				include_file = path::join(path::get_dirpath(filename), include_file);
				buff += load_shader_file(include_file);
			}
			else
			{
				buff += line + "\n";
			}
		}

		return buff;
	}

	static string type_to_string(glshader type)
	{
		switch(type)
		{
		case shader_vertex:
			return "vertex";
		case shader_tess_ctrl:
			return "tessellation control";
		case shader_tess_eval:
			return "tessellation evaluation";
		case shader_geometry:
			return "geometry";
		case shader_fragment:
			return "fragment";
		case shader_compute:
			return "compute";
		default:
			return "unknown";
		};
	}

	bool upload_source(unsigned int program_id, glshader type, const char* name, const char* source)
	{
		// create shader object
		GLuint shader = glCreateShader(type);

		// assign source code
		glShaderSource(shader, 1, &source, NULL);

		// compile shader object
		glCompileShader(shader);

		// check compilation
		GLint ok = 0;
		glGetShaderiv(shader, GL_COMPILE_STATUS, &ok);
		if(!ok)
		{
			GLsizei max_len = 4096;
			GLchar info_log[4096];
			glGetShaderInfoLog(shader, max_len, &max_len, info_log);
			std::cout << "ERROR> compile shader (name: '" << name << "', type: " << type_to_string(type) << ")\n" << info_log << std::endl;
			return false;
		}

		// attach shader to program object
		glAttachShader(program_id, shader);

		// delete shader object: no longer needed
		glDeleteShader(shader);

		// global error checking
		GLenum glerr = glGetError();
		if(glerr != GL_NONE)
		{
			const char* err = opengl::get_error_string(glerr);
			if(err == NULL)
			{
				std::cout << "ERROR> attach shader: unknown error detected!" << std::endl;
			}
			else
			{
				std::cout << "ERROR> attach shader (name: '" << name << "', type: " << type_to_string(type) << ")\n" << "Errors: " << err << std::endl;
			}
			return false;
		}
		return true;
	}

	// ---------------------------------------------------------------------------------------------------------------------------------------------------------
	// public
	// ---------------------------------------------------------------------------------------------------------------------------------------------------------

	void shader_program_builder::begin()
	{
		_program.create();
	}

	bool shader_program_builder::add_file(glshader type, const char* filename)
	{
		if(!path::exists(filename))
		{
			return false;
		}
		return add_source(type, filename, load_shader_file(filename).data());
	}

	bool shader_program_builder::add_source(glshader type, const char* name, const char* source)
	{
		return upload_source(_program.get_id(), type, name, source);
	}

	void shader_program_builder::bind_draw_buffer(const char* frag_output_name, unsigned int id)
	{
		glBindFragDataLocation(_program.get_id(), id, frag_output_name);
	}

	void shader_program_builder::bind_vertex_attrib(const char* attrib_name, unsigned int id)
	{
		glBindAttribLocation(_program.get_id(), id, attrib_name);
	}

	bool shader_program_builder::end()
	{
		// link whole program object
		glLinkProgram(_program.get_id());

		// test link success
		GLint ok = GL_FALSE;
		glGetProgramiv(_program.get_id(), GL_LINK_STATUS, &ok);
		if(!ok)
		{
			GLint max_length = 4096;
			GLchar info_log[4096];
			glGetProgramInfoLog(_program.get_id(), max_length, &max_length, info_log);
			std::cout << "ERROR> shader program link error:" << std::endl << info_log << std::endl;
			return false;
		}

		// program validation
		glValidateProgram(_program.get_id());
		ok = GL_FALSE;
		glGetProgramiv(_program.get_id(), GL_VALIDATE_STATUS, &ok);
		if(!ok)
		{
			GLint max_len = 4096;
			GLchar info_log[4096];
			glGetProgramInfoLog(_program.get_id(), max_len, &max_len, info_log);
			std::cout << "ERROR> shader program validation error:" << std::endl << info_log << std::endl;
			return false;
		}

		return true;
	}

	shader_program shader_program_builder::get_shader_program()
	{
		return _program;
	}
} // namespace glb
