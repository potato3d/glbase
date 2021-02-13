#pragma once

namespace glb
{
	class camera;
	class framebuffer;

	class irenderer
	{
	public:
		virtual ~irenderer(){}
		virtual bool initialize(framebuffer& fbuffer, camera& cam) = 0;
		virtual bool finalize() = 0;
		virtual void render() = 0;
	};
} // namespace glb
