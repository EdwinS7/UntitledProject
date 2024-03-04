#include "Window.hpp"

LRESULT CALLBACK WndProc( HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam ) {
	switch ( msg ) {
	case WM_SIZE:
		if ( gGraphics->Valid( ) && wParam != SIZE_MINIMIZED )
			gGraphics->Reset( lParam );

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

void cWindow::SetRenderEnvironmentTitle(const char* title) {
	SetWindowTextA( m_Hwnd, title );
}

int cWindow::CreateRenderEnvironment( const char* title, const Vec2< int16_t > size ) {
	m_WindowClass = {
		sizeof( m_WindowClass ),
		CS_CLASSDC,
		WndProc,
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

	RegisterClassEx( &m_WindowClass );

	RECT DesktopResolution;
	const HWND Desktop = GetDesktopWindow( );
	GetWindowRect( Desktop, &DesktopResolution );

	m_Hwnd = CreateWindow( m_WindowClass.lpszClassName, title,
		WS_OVERLAPPEDWINDOW, ( DesktopResolution.right / 2 ) - ( size.x / 2 ), ( DesktopResolution.bottom / 2 ) - ( size.y / 2 ), size.x, size.y, NULL, NULL, m_WindowClass.hInstance, NULL );

	return ( ShowWindow( m_Hwnd, SW_SHOWDEFAULT ) && UpdateWindow( m_Hwnd ) );
}

void cWindow::DestroyRenderEnvironment( ) {
	::UnregisterClass( m_WindowClass.lpszClassName, m_WindowClass.hInstance );
}

int cWindow::CreateLoggingEnvironment( const char* title ) {
	if ( !title )
		return 0;

	AllocConsole( );
	SetConsoleTitleA( title );

	FILE* fp = nullptr;
	freopen_s( &fp, "CONIN$", "r", stdin );
	freopen_s( &fp, "CONOUT$", "w", stdout );
	freopen_s( &fp, "CONOUT$", "w", stderr );

	std::printf( std::vformat( "[ Win32 ] Console created ( name: {} )\n", std::make_format_args( title ) ).c_str( ) );

	return 1;
}

void cWindow::SetLoggingEnvironmentTitle( const char* title ) {
	SetConsoleTitle( title );
}

void cWindow::DestroyLoggingEnvironment( ) {
	ShowWindow( GetConsoleWindow( ), SW_HIDE );
	FreeConsole( );
}

bool cWindow::Dispatch( ) {
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