#pragma once

#include "../../inc.hpp"

class c_ctx {
public:
	void init( );

	HWND m_hwnd;
	HWND m_old_hwnd;
	ID3D11Device* m_device;
	ID3D11Texture2D* m_buffer;
	ID3D11DeviceContext* m_context;
	ID3D11RenderTargetView* m_render_target_view;
private:
	

};

inline const auto g_ctx = std::make_unique< c_ctx >( );

#include "ctx.inl"