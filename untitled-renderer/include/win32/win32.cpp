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
		g_input->set_mouse_pos( vector2_t<uint16_t>( GET_X_LPARAM( lParam ), GET_Y_LPARAM( lParam ) ) );
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

void c_win32::create_window( const char* title, const vector2_t< uint16_t > size ) {
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
	
	g_console->log( color_t(240, 240, 240, 255), "[ win32 ] window created\n" );
}

void c_win32::create_message_box( const char* title, const char* caption, uint8_t type ) {
	g_console->log( color_t(240, 240, 240, 255), "[ win32 ] message box created\n" );
	MessageBox( m_hwnd, title, caption, type );
	g_console->log( color_t(240, 240, 240, 255), "[ win32 ] message box destroyed\n" );
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

void c_win32::set_rect( const vector2_t< uint16_t > pos, const vector2_t< uint16_t > size ) {
	SetWindowPos( m_hwnd, NULL, pos.x, pos.y, size.x, size.y, NULL );
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

	throw std::runtime_error( "GetClientRect failed" );
}

HWND c_win32::get_hwnd( ) {
	return m_hwnd;
}