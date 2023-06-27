#pragma once

// includes
#include "../../inc.hpp"

#ifdef UNTITLED_USE_DX9

#define TRIANGLE D3DPT_TRIANGLESTRIP
#define LINE D3DPT_LINESTRIP

class c_gfx {
public:
	void set_render_states( IDirect3DDevice9* device );

	void create_context( );

	bool create_device( hwnd_t hwnd );

	IDirect3DParamaters9* get_parameters( );

	GfxTexture create_texture( const byte_t bytes[], const vector2_t<uint16_t> size );

	void begin_scene( );

	void end_scene( );

	void render_draw_data( );

	void draw( );

	void release( );

private:
	IDirect3D9* m_d3d;
	IDirect3DDevice9* m_device;
	IDirect3DParamaters9 m_parameters;

	int	m_vertex_buffer_size{ 5000 },
		m_index_buffer_size{ 10000 };

	IDirect3DVertexBuffer9* m_vertex_buffer;
	IDirect3DIndexBuffer9* m_index_buffer;

	template <typename type>
	void safe_release( type*& obj );
};

inline const auto g_gfx = std::make_unique<c_gfx>( );

#endif // __UNTITLED_DX9__