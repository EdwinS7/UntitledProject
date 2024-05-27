#include "Window.hpp"

inline LRESULT CALLBACK WndProc( HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam ) {
	switch ( msg ) {
	case WM_SIZE:
		if ( gGraphics->IsDeviceValid( ) && wParam != SIZE_MINIMIZED )
			gGraphics->ResetDevice( lParam );

		return 0;
	case WM_MOUSEMOVE:
		gInput->SetMousePos( Vec2<int16_t>( LOWORD( lParam ), HIWORD( lParam ) ) );
		break;
	case WM_SETCURSOR:
		SetCursor( LoadCursorA( 0, gInput->GetCursorStyle( ) ) );
		break;
	case WM_DESTROY:
		PostQuitMessage( 0 );
		return 0;
	}

	return DefWindowProc( hwnd, msg, wParam, lParam );
}

void cWin32::InitWindow( const char* title, const Vec2<int16_t>& position, const Vec2<int16_t>& size ) {
	m_WindowClass = {
		sizeof( m_WindowClass ), CS_CLASSDC, WndProc,
		0L, 0L, GetModuleHandle( NULL ),
		NULL, NULL, NULL,
		NULL, title, NULL
	};

	RegisterClassEx( &m_WindowClass );

	m_Hwnd = CreateWindow(
		m_WindowClass.lpszClassName, title, WS_OVERLAPPEDWINDOW,
		position.x, position.y,
		size.x, size.y, NULL, NULL, m_WindowClass.hInstance, NULL
	);

	ShowWindow( m_Hwnd, SW_SHOWDEFAULT );
	UpdateWindow( m_Hwnd );
}

void cWin32::ExitWindow( ) {
	::UnregisterClass( m_WindowClass.lpszClassName, m_WindowClass.hInstance );
}