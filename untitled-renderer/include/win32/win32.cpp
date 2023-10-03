#include "win32.hpp"

#define GET_X_LPARAM(lp) ((int)(short)LOWORD(lp))
#define GET_Y_LPARAM(lp) ((int)(short)HIWORD(lp))

LRESULT CALLBACK WindowProc( HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam ) {
	switch ( uMsg ) {
	case WM_SIZE:
		if ( g_gfx->valid( ) && wParam != SIZE_MINIMIZED )
			g_gfx->reset( lParam );

		return 0;
	case WM_MOUSEMOVE:
		g_input->set_mouse_pos( vector2_t<int16_t>( GET_X_LPARAM( lParam ), GET_Y_LPARAM( lParam ) ) );
		break;
	case WM_DESTROY:
		PostQuitMessage( 0 );
		return 0;
	}

	return DefWindowProc( hwnd, uMsg, wParam, lParam );
}

void c_win32::set_window_title(const char* title) {
	SetWindowTextA( m_hwnd, title );
}

void c_win32::create_window( const char* title, const vector2_t< int16_t > size ) {
	m_window_class = {
		sizeof( m_window_class ),
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

	RegisterClassEx( &m_window_class );

	m_hwnd = CreateWindow( m_window_class.lpszClassName, title,
		WS_OVERLAPPEDWINDOW, NULL, NULL, size.x, size.y, NULL, NULL, m_window_class.hInstance, NULL );

#ifdef UNTITLED_USE_LOGS
	AllocConsole( );
	SetConsoleTitleA( title );

	FILE* fp = nullptr;
	freopen_s( &fp, "CONIN$", "r", stdin );
	freopen_s( &fp, "CONOUT$", "w", stdout );
	freopen_s( &fp, "CONOUT$", "w", stderr );

	std::printf( std::vformat( "[ Win32 ] Console created ( name: {} )\n", std::make_format_args( title ) ).c_str( ) );
#endif

	ShowWindow( m_hwnd, SW_SHOWDEFAULT );
	UpdateWindow( m_hwnd );
}

void c_win32::create_message_box( const char* title, const char* caption, int8_t type ) {
	MessageBox( m_hwnd, title, caption, type );
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

void c_win32::set_rect( const vector2_t< int16_t > pos, const vector2_t< int16_t > size ) {
	SetWindowPos( m_hwnd, NULL, pos.x, pos.y, size.x, size.y, NULL );
}

vector2_t< int16_t > c_win32::get_pos( ) {
	RECT rect{ };

	if ( GetWindowRect( m_hwnd, &rect ) )
		return vector2_t< int16_t >( rect.left, rect.top );

	std::printf( "[ Win32 ] get_pos failed!" );
}

vector2_t< int16_t > c_win32::get_size( ) {
	RECT rect{ };

	if ( GetClientRect( m_hwnd, &rect ) )
		return vector2_t< int16_t >( rect.right - rect.left, rect.bottom - rect.top );

	std::printf( "[ Win32 ] get_size failed!" );
}

HWND c_win32::get_hwnd( ) {
	return m_hwnd;
}