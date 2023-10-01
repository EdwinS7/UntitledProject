#include "win32.hpp"

ALWAYS_INLINE Vector2 c_win32::getPosition() {
	return m_pPos;
}

ALWAYS_INLINE Vector2 c_win32::getSize() {
	return m_pSize;
}

ALWAYS_INLINE HWND c_win32::getHwnd() {
	return m_pHwnd;
}

ALWAYS_INLINE LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam) {
	switch (uMsg) {
	case WM_SIZE:

		return 0;
	case WM_MOUSEMOVE:
		
		break;
	case WM_DESTROY:
		PostQuitMessage(0);
		return 0;
	}

	return DefWindowProc(hwnd, uMsg, wParam, lParam);
}