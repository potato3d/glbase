#pragma once
#include <glb/camera.h>
#include <glb/framebuffer.h>

namespace glb
{
	class irenderer;

	class engine
	{
	public:
		bool initialize(irenderer* renderer);
		bool finalize();
		void resize_screen(int w, int h);
		void set_view(const float* const view_matrix);
		void render();

	private:
		camera _camera;
		framebuffer _framebuffer;
		irenderer* _renderer = nullptr;
	};
} // namespace glb
