#pragma once
#include <glb/gltypes.h>
#include <bl/bl.h>

namespace glb
{
	class glstate
	{
	public:
		static void bind_buffer(gltarget target, unsigned int id);
		static void release_buffer(gltarget target);

		static void bind_vertex_array(unsigned int id);
		static void release_vertex_array();

		static void bind_shader_program(unsigned int id);
		static void release_shader_program();

		static void bind_texture(unsigned int unit, gltarget target, unsigned int id);
		static void release_texture(unsigned int unit, gltarget target);

		static unsigned int get_or_create_uniform_buffer_location(unsigned int buffer_id);

		static void set_depth_test(bool enabled);
		static void set_backface_culling(bool enabled);
		static void set_vsync_enabled(bool enabled);

		static void reset_state();

	private:
		glstate();
		static glstate& _instance();

	private:
		hash_map<gltarget, unsigned int, enum_hash> _buffers;
		unsigned int _vertex_array = 0;
		unsigned int _shader_program = 0;
		vector<hash_map<gltarget, unsigned int, enum_hash>> _textures;
		vector<unsigned int> _uniform_buffers;
		bool _depth_test_enabled = false;
		bool _backface_culling_enabled = false;
		bool _vsync_enabled = true;
	};
} // namespace glb
