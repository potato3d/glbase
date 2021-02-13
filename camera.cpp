#include <glb/camera.h>
#include <glb/opengl.h>

namespace glb
{
	// ---------------------------------------------------------------------------------------------------------------------------------------------------------
	// public
	// ---------------------------------------------------------------------------------------------------------------------------------------------------------

	bool camera::initialize()
	{
		if(!GLEW_VERSION_3_1)
		{
			return false;
		}

		return _uniform_buffer.create(target_uniform_buffer, usage_stream_draw, sizeof(_uniform_block));
	}

	bool camera::finalize()
	{
		return _uniform_buffer.destroy();
	}

	const buffer& camera::get_uniform_buffer() const
	{
		return _uniform_buffer;
	}

	void camera::set_perspective(float fovy, float znear, float zfar)
	{
		_fovy = fovy;
		_znear = znear;
		_zfar = zfar;
		_update();
	}

	void camera::set_view(const float* const view_matrix)
	{
		_uniform_block.view_matrix = mat4(view_matrix);
		_uniform_block.normal_matrix = _uniform_block.view_matrix.to_normal_matrix();
		_update();
	}

	void camera::resize_screen(int w, int h)
	{
		_aspect = static_cast<float>(w)/static_cast<float>(h);
		_update();
	}

	void camera::begin_frame()
	{
		_uniform_buffer.clear();
		_uniform_buffer.add(_uniform_block);
	}

	mat4 camera::get_view() const
	{
		return _uniform_block.view_matrix;
	}

	mat4 camera::get_projection() const
	{
		return mat4::perspective(_fovy, _aspect, _znear, _zfar);
	}

	float camera::getZNear()
	{
		return _znear;
	}

	float camera::getZFar()
	{
		return _zfar;
	}

	// ---------------------------------------------------------------------------------------------------------------------------------------------------------
	// private
	// ---------------------------------------------------------------------------------------------------------------------------------------------------------

	void camera::_update()
	{
		_uniform_block.proj_view_matrix = mat4::perspective(_fovy, _aspect, _znear, _zfar).mul(_uniform_block.view_matrix);
	}
} // namespace glb
