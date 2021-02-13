#pragma once
#include <glb/gltypes.h>

namespace glb
{
	struct vertex_attrib
	{
		int size;
		gltype type;
		bool normalized;
		int stride;
		unsigned int byte_offset;
	};
} // namespace glb
