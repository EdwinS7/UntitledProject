#pragma once

// includes
#include "../../inc.hpp"

#ifdef UNTITLED_USE_DX9

class c_gfx {
public:
	bool create( HWND hwnd, const bool device_only = false );
	void set_render_states( IDirect3DDevice9* device );
	void reset( const HWND hWnd, const LPARAM lParam );
	void render_draw_data( );

	bool valid( );
	void draw( );
	void release( );

	// utilities
	void create_texture_from_file( texture* resource, const char* file_name );
	void create_texture_from_bytes( texture* resource, const std::vector<BYTE> bytes, const vector2_t<uint16_t> size );

	IDirect3DDevice9* get_device( );

private:
	IDirect3D9* m_d3d;
	IDirect3DDevice9* m_device;
	D3DPRESENT_PARAMETERS m_parameters;

	int	m_vertex_buffer_size{ 5000 },
		m_index_buffer_size{ 10000 };

	IDirect3DVertexBuffer9* m_vertex_buffer;
	IDirect3DIndexBuffer9* m_index_buffer;

	template <typename type>
	void safe_release( type*& obj );
};

inline const auto g_gfx = std::make_unique<c_gfx>( );

#endif // __UNTITLED_DX9__