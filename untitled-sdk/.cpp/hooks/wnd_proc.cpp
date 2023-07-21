#include "../../.hpp/hooks/hooks.hpp"

extern IMGUI_IMPL_API LRESULT ImGui_ImplWin32_WndProcHandler( HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam );

LRESULT __stdcall hooks::wnd_proc( HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam ) {
	if ( msg == WM_KEYUP && wparam == VK_INSERT )
		g_gui->is_open = !g_gui->is_open;

	if ( g_gui->is_open ) {
		ImGui_ImplWin32_WndProcHandler( hwnd, msg, wparam, lparam );

		return 1;
	}

	return CallWindowProc( original_wnd_proc, hwnd, msg, wparam, lparam );
}