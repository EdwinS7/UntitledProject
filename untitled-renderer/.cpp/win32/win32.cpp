#include "../../.hpp/win32/win32.hpp"

LRESULT CALLBACK WindowProc( HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam ) {
	switch ( uMsg ) {
	case WM_SIZE:
		if ( g_gfx->valid( ) && wParam != SIZE_MINIMIZED )
			g_gfx->reset( hwnd, lParam );

		return 0;
	case WM_DESTROY:
		PostQuitMessage( 0 );
		return 0;
	}

	return DefWindowProc( hwnd, uMsg, wParam, lParam );
}

void c_win32::set_window_title(const char* title) {
	SetWindowTextA( m_hwnd, title );
}

bool c_win32::create_window( const char* title, bool full_screen, vector2_t< uint16_t > size ) {
	window_class = {
		sizeof( window_class ),
		CS_CLASSDC,
		WindowProc,
		0L,
		0L,
		GetModuleHandle( NULL ),
		NULL,
		NULL,
		NULL,
		NULL,
		title,
		NULL
	};

	RegisterClassEx( &window_class );

	m_hwnd = CreateWindow( window_class.lpszClassName, title,
		WS_OVERLAPPEDWINDOW, NULL, NULL, size.x, size.y, NULL, NULL, window_class.hInstance, NULL );

	ShowWindow( m_hwnd, SW_SHOWDEFAULT );
	UpdateWindow( m_hwnd );

#ifdef UNTITLED_USE_CONSOLE
	AllocConsole( );

	FILE* fp = nullptr;
	freopen_s( &fp, "CONIN$", "r", stdin );
	freopen_s( &fp, "CONOUT$", "w", stdout );
	freopen_s( &fp, "CONOUT$", "w", stderr );
#endif

	return true;
}

bool c_win32::message_box( const char* title, const char* caption, uint8_t type ) {
	return false;
}

HWND c_win32::get_hwnd( ) {
	return m_hwnd;
}

vector2_t< uint16_t > c_win32::get_pos( ) {
	RECT rect{ };

	if ( GetWindowRect( m_hwnd, &rect ) )
		return vector2_t< uint16_t >( rect.left, rect.top );

	throw std::runtime_error( "GetWindowRect failed" );
}

vector2_t< uint16_t > c_win32::get_size( ) {
	RECT rect{ };

	if ( GetClientRect( m_hwnd, &rect ) )
		return vector2_t< uint16_t >( rect.right - rect.left, rect.bottom - rect.top );

	throw std::runtime_error("GetClientRect failed");
}

bool c_win32::dispatch_messages( ) {
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