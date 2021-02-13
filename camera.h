#pragma once
#include <glb/buffer.h>
#include <bl/bl.h>

namespace glb
{
	struct camera_uniform_block
	{
		mat4 proj_view_matrix = mat4::IDENTITY;
		mat4 normal_matrix = mat4::IDENTITY;
		mat4 view_matrix = mat4::IDENTITY;
	};

	class camera
	{
	public:
		bool initialize();
		bool finalize();

		const buffer& get_uniform_buffer() const;
		void set_perspective(float fovy, float znear, float zfar);
		void set_view(const float* const view_matrix);
		void resize_screen(int w, int h);
		void begin_frame();

		mat4 get_view() const;
		mat4 get_projection() const;

		float getZNear();
		float getZFar();

	private:
		void _update();

	private:
		camera_uniform_block _uniform_block;
		buffer _uniform_buffer;
		float _fovy = math::to_radians(60.0f);
		float _aspect = 4.0f/3.0f;
		float _znear = 0.1f;
		float _zfar = 10000.0f;
	};
} // namespace glb
