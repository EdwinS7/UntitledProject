#include "../../.hpp/hooks/hooks.hpp"

long D3DAPI	hooks::device_reset( IDirect3DDevice9* const device, D3DPRESENT_PARAMETERS* const present_params ) {
	ImGui_ImplDX9_InvalidateDeviceObjects( );

	const auto original = original_device_reset( device, present_params );

	ImGui_ImplDX9_CreateDeviceObjects( );

	return original;
}

long D3DAPI hooks::device_present( IDirect3DDevice9* const device, RECT* const src_rect, RECT* const dst_rect, const HWND dst_wnd_override, RGNDATA* const dirty_region ) {
	ImGui_ImplDX9_NewFrame( );
	ImGui_ImplWin32_NewFrame( );

	ImGui::NewFrame( );

	if ( g_gui->is_open )
		g_gui->draw( );

	ImGui::Render( );

	ImGui_ImplDX9_RenderDrawData( ImGui::GetDrawData( ) );

	return original_device_present( device, src_rect, dst_rect, dst_wnd_override, dirty_region );
}