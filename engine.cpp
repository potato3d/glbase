#include <glb/engine.h>
#include <glb/glstate.h>
#include <glb/irenderer.h>
#include <glb/opengl.h>

namespace glb
{
	// ---------------------------------------------------------------------------------------------------------------------------------------------------------
	// public
	// ---------------------------------------------------------------------------------------------------------------------------------------------------------

	bool engine::initialize(irenderer* renderer)
	{
		if(!opengl::initialize())
		{
			return false;
		}

		if(!_camera.initialize())
		{
			return false;
		}

		if(!renderer->initialize(_framebuffer, _camera))
		{
			return false;
		}

		if(!_framebuffer.initialize())
		{
			return false;
		}

		_renderer = renderer;

		glstate::set_depth_test(true);
		glstate::set_backface_culling(true);
		glstate::set_vsync_enabled(false);

		GLB_PRINT_GL_ERROR;

		return true;
	}

	bool engine::finalize()
	{
		if(!_renderer->finalize())
		{
			return false;
		}
		return _framebuffer.finalize() && _camera.finalize();
	}

	void engine::resize_screen(int w, int h)
	{
		if(w == (int)_framebuffer.get_width() && h == (int)_framebuffer.get_height())
		{
			return;
		}
		_camera.resize_screen(w, h);
		_framebuffer.resize_screen(w, h);
	}

	void engine::set_view(const float* const view_matrix)
	{
		_camera.set_view(view_matrix);
	}

	void engine::render()
	{
		// reset glstate
		glstate::reset_state();

		// setup framebuffer
		_framebuffer.begin_frame();
		_framebuffer.clear();

		// upload global data
		_camera.begin_frame();

		// render
		_renderer->render();

		// display framebuffer
		_framebuffer.display_on(display_back);
		_framebuffer.end_frame();

		// check for any gl errors
		GLB_PRINT_GL_ERROR;

		// reset glstate
		glstate::reset_state();
	}
} // namespace glb
