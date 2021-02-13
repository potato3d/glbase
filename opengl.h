#pragma once
#include <GL/glew.h>
#include <bl/bl.h>

namespace glb
{
	class opengl
	{
	public:
		static bool initialize();
		static bool check_extension(const char* name);
		static const char* get_error_string(GLenum errorCode);
	};
} // namespace glb

#define GLB_PRINT_GL_ERROR if(auto err = glGetError()) cout << __FILE__ << ":" << __LINE__ << ": " << glb::opengl::get_error_string(err) << endl
#define GLB_BYTE_OFFSET(bytes) static_cast<char*>(0) + (bytes)
