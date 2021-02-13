#include <glb/texture.h>
#include <glb/buffer.h>
#include <glb/glstate.h>
#include <glb/opengl.h>

namespace glb
{
	// ---------------------------------------------------------------------------------------------------------------------------------------------------------
	// public
	// ---------------------------------------------------------------------------------------------------------------------------------------------------------

	bool texture::create(int unit, gltarget target)
	{
		_unit = unit;
		_target = target;
		return create();
	}

	bool texture::create()
	{
		glGenTextures(1, &_texture_id);
		return _texture_id != 0;
	}

	bool texture::destroy()
	{
		if(_texture_id == 0)
		{
			return true;
		}
		glDeleteTextures(1, &_texture_id);
		_texture_id = 0;
		return true;
	}

	void texture::set_data_source(glinternal_format format, const buffer& data_source)
	{
		bind();
		glTexBuffer(_target, format, data_source.get_id());
		release();
	}

	void texture::bind() const
	{
		glstate::bind_texture(_unit, _target, _texture_id);
	}

	void texture::release() const
	{
		glstate::release_texture(_unit, _target);
	}
} // namespace glb
