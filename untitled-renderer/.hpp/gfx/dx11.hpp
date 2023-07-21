#pragma once

// includes
#include "../../inc.hpp"

#ifdef UNTITLED_USE_DX11

class c_gfx {
public:
	void set_render_states( ID3D11Device* device );

	bool create_device( HWND hwnd );

	void create_texture( texture* resource, const std::vector<BYTE> bytes, const vector2_t<uint16_t> size );

	void begin_scene( );

	void end_scene( );

	void render_draw_data( );

	bool valid( );

	void reset( );

	void release( );

private:
	ID3D11Device* m_device;
	IDXGISwapChain* m_swap_chain;
	ID3D11DeviceContext* m_context;
	ID3D11RenderTargetView* m_target;

	template <typename type>
	void safe_release( type*& obj );
};

inline const auto g_gfx = std::make_unique<c_gfx>( );

#endif // __UNTITLED_DX11__