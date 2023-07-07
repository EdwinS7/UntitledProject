#pragma once

#include "../../inc.hpp"

namespace hooks {
	LRESULT CALLBACK wnd_proc( const HWND hwnd, const UINT msg, const WPARAM wparam, const LPARAM lparam );
	inline decltype( &wnd_proc ) original_wnd_proc{};

	long D3DAPI	device_reset( IDirect3DDevice9* const device, D3DPRESENT_PARAMETERS* const present_params );
	inline decltype( &device_reset ) original_device_reset{};

	long D3DAPI device_present( IDirect3DDevice9* const device, RECT* const src_rect, RECT* const dst_rect, const HWND dst_wnd_override, RGNDATA* const dirty_region );
	inline decltype( &device_present ) original_device_present{};
};