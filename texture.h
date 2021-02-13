#pragma once
#include <glb/gltypes.h>
#include <bl/bl.h>

namespace glb
{
	class buffer;

	class texture
	{
	public:
		bool create(int unit, gltarget target);
		bool create();
		bool destroy();

		void set_data_source(glinternal_format format, const buffer& data_source);

		void bind() const;
		void release() const;

	private:
		unsigned int _unit = 0;
		unsigned int _texture_id = 0;
		gltarget _target = target_texture_buffer;
	};
} // namespace glb
