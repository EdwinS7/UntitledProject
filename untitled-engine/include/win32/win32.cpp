#include "win32.hpp"

void c_win32::setWindowTitle( const char* title ) {
	if ( !m_pCreated )
		return;
		
	SetWindowTextA( m_pHwnd, title );
}

void c_win32::createWindow( const char* title, Vector2 size ) {
	if ( m_pCreated )
		return;

	m_pWindowClass = {
		sizeof( m_pWindowClass ),
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

	RegisterClassEx( &m_pWindowClass );

	m_pHwnd = CreateWindow( m_pWindowClass.lpszClassName, title,
		WS_OVERLAPPEDWINDOW, NULL, NULL, size.x, size.y, NULL, NULL, m_pWindowClass.hInstance, NULL );

	ShowWindow( m_pHwnd, SW_SHOWDEFAULT );
	UpdateWindow( m_pHwnd );

	m_pCreated = true;
}

bool c_win32::dispatch( ) {
	if ( !m_pCreated )
		return false;

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