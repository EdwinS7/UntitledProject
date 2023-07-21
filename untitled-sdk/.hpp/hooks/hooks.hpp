#pragma once

#include "../../inc.hpp"

namespace hooks {
	LRESULT CALLBACK wnd_proc( HWND, UINT, WPARAM, LPARAM );
	inline decltype( &wnd_proc ) original_wnd_proc{};

	HRESULT CALLBACK resize_buffers( IDXGISwapChain*, UINT, UINT, UINT, DXGI_FORMAT, UINT );
	inline decltype( &resize_buffers ) original_resize_buffers{};

	HRESULT CALLBACK present( IDXGISwapChain*, UINT, UINT );
	inline decltype( &present ) original_present{};
};