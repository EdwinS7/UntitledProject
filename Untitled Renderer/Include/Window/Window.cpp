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

void cWin32::InitWindow( const char* title, const Vec2< int16_t > size ) {
	m_WindowClass = {
		sizeof( m_WindowClass ), CS_CLASSDC, WndProc,
		0L, 0L, GetModuleHandle( NULL ),
		NULL, NULL, NULL,
		NULL, title, NULL
	};

	RegisterClassEx( &m_WindowClass );

	RECT DesktopResolution;
	const HWND Desktop = GetDesktopWindow( );
	GetWindowRect( Desktop, &DesktopResolution );

	m_Hwnd = CreateWindow( 
		m_WindowClass.lpszClassName, title, WS_OVERLAPPEDWINDOW, 
		DesktopResolution.right / 2 - size.x / 2, DesktopResolution.bottom / 2 - size.y / 2,
		size.x, size.y, NULL, NULL, m_WindowClass.hInstance, NULL
	);

	ShowWindow( m_Hwnd, SW_SHOWDEFAULT );
	UpdateWindow( m_Hwnd );
}

void cWin32::ExitWindow( ) {
	::UnregisterClass( m_WindowClass.lpszClassName, m_WindowClass.hInstance );
}

bool cWin32::DispatchMessages( ) {
	MSG msg;

	std::memset( &msg, 0, sizeof( MSG ) );
	if ( PeekMessage( &msg, nullptr, 0u, 0u, PM_REMOVE ) ) {
		TranslateMessage( &msg );
		DispatchMessage( &msg );

		if ( msg.message == WM_QUIT )
			return false;
	}

	return true;
}