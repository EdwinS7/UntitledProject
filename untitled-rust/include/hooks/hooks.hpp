#pragma once

#include "../../common.hpp"

class c_hooks {
public:
	void init( );

	HRESULT __stdcall present( IDXGISwapChain*, UINT, UINT );
	HRESULT __stdcall resize_buffers( IDXGISwapChain*, UINT, UINT, UINT, DXGI_FORMAT, UINT );
};

inline const auto g_hooks = std::make_unique<c_hooks>( );