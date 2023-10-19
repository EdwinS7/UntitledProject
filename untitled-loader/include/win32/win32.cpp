#include "win32.hpp"

extern IMGUI_IMPL_API LRESULT ImGui_ImplWin32_WndProcHandler( HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam );

LRESULT CALLBACK WndProc( HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam ) {
	if ( ImGui_ImplWin32_WndProcHandler( hwnd, msg, wParam, lParam ) )
		return true;

	switch ( msg ) {
	case WM_SIZE:
		if ( wParam == SIZE_MINIMIZED )
			return 0;

		Graphics->SetResizeWidth( ( UINT ) LOWORD( lParam ) );
		Graphics->SetResizeHeight( ( UINT ) HIWORD( lParam ) );

		return 0;
	case WM_SYSCOMMAND:
		if ( ( wParam & 0xfff0 ) == SC_KEYMENU )
			return 0;

		break;
	case WM_DESTROY:
		::PostQuitMessage( 0 );
		return 0;
	}

	return DefWindowProc( hwnd, msg, wParam, lParam );
}

int CWin32::CreateRenderEnvironment( const char* title ) {
	if ( !title ) {
		LOG( "CreateRenderEnvironment-> title is required!" );
		return 0;
	}

	m_WndClass = { 
		sizeof( m_WndClass ),
		CS_CLASSDC,
		WndProc,
		0L, 0L,
		GetModuleHandle( NULL ),
		NULL, 
		NULL,
		NULL, 
		NULL,
		title, 
		NULL
	};

	RegisterClassEx( &m_WndClass );

	m_Hwnd = CreateWindow( 
		m_WndClass.lpszClassName, 
		title, 
		WS_OVERLAPPEDWINDOW, 
		0, 
		0,
		1280,
		800, 
		0, 
		0, 
		m_WndClass.hInstance,
		0 
	);

	ShowWindow( m_Hwnd, SW_SHOWDEFAULT );
	UpdateWindow( m_Hwnd );

	return 1;
}

void CWin32::DestroyRenderEnvironment( ) {
	::UnregisterClass( m_WndClass.lpszClassName, m_WndClass.hInstance );

}

int CWin32::CreateLoggingEnvironment( const char* title ) {
	if ( !title )
		return 0;

	AllocConsole( );
	SetConsoleTitleA( title );

	FILE* fp = nullptr;
	freopen_s( &fp, "CONIN$", "r", stdin );
	freopen_s( &fp, "CONOUT$", "w", stdout );
	freopen_s( &fp, "CONOUT$", "w", stderr );

	return 1;
}

void CWin32::DestroyLoggingEnvironment( ) {
	ShowWindow( GetConsoleWindow( ), SW_HIDE );
	FreeConsole( );
}

int CWin32::Dispatch( ) {
	MSG msg;

	std::memset( &msg, 0, sizeof( MSG ) );
	if ( PeekMessage( &msg, nullptr, 0u, 0u, PM_REMOVE ) ) {
		TranslateMessage( &msg );
		DispatchMessage( &msg );

		if ( msg.message == WM_QUIT )
			return 0;
	}

	return 1;
}