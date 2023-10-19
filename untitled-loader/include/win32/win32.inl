#include "win32.hpp"
#pragma once

INLINE HWND CWin32::GetHwnd( ) {
	return m_Hwnd;
}

INLINE WNDCLASSEX CWin32::GetWndClass( ) {
	return m_WndClass;
}