#include <glb/opengl.h>

namespace glb
{
	// ---------------------------------------------------------------------------------------------------------------------------------------------------------
	// public
	// ---------------------------------------------------------------------------------------------------------------------------------------------------------

	bool opengl::initialize()
	{
		auto ret = glewInit();
		if(ret == GLEW_OK)
		{
			return true;
		}
		else
		{
			cout << "ERROR> could not initialize OpenGL '" << glewGetErrorString(ret) << "'" << endl;
			return false;
		}
	}

	bool opengl::check_extension(const char* name)
	{
		if(glewIsSupported(name))
		{
			return true;
		}
		else
		{
			cout << "ERROR> unsupported extension '" << name << "'" << endl;
			return false;
		}
	}

	const char* opengl::get_error_string(GLenum errorCode)
	{
		switch(errorCode)
		{
		case GL_NO_ERROR:
			return "no error";
		case GL_INVALID_ENUM:
			return "invalid enumerant";
		case GL_INVALID_VALUE:
			return "invalid value";
		case GL_INVALID_OPERATION:
			return "invalid operation";
		case GL_STACK_OVERFLOW:
			return "stack overflow";
		case GL_STACK_UNDERFLOW:
			return "stack underflow";
		case GL_OUT_OF_MEMORY:
			return "out of memory";
		case GL_TABLE_TOO_LARGE:
			return "table too large";
		case GL_INVALID_FRAMEBUFFER_OPERATION:
			return "invalid framebuffer operation";
		default:
			return nullptr;
		}
	}
} // namespace glb
