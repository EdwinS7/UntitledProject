#include "Window.hpp"

HWND Hwnd;
WNDCLASSEX WndClass;
int ActiveWindow = 0;

extern IMGUI_IMPL_API LRESULT ImGui_ImplWin32_WndProcHandler( HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam );

LRESULT CALLBACK WndProc( HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam ) {
	if ( ImGui_ImplWin32_WndProcHandler( hwnd, msg, wParam, lParam ) )
		return true;

	switch ( msg ) {
	case WM_SIZE:
		if ( wParam == SIZE_MINIMIZED )
			return 0;

		Untitled::Graphics::SetResizeWidth( ( UINT ) LOWORD( lParam ) );
		Untitled::Graphics::SetResizeHeight( ( UINT ) HIWORD( lParam ) );

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

int Untitled::Win32::CreateRenderEnvironment( const char* title, int w, int h ) {
	WndClass = {
		sizeof( WndClass ),
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

	RegisterClassEx( &WndClass );

	Hwnd = CreateWindow( 
		WndClass.lpszClassName, 
		title, 
		WS_OVERLAPPEDWINDOW, 
		0, 
		0,
		w,
		h, 
		0, 
		0, 
		WndClass.hInstance,
		0 
	);

	ShowWindow( Hwnd, SW_SHOWDEFAULT );
	UpdateWindow( Hwnd );

	return 1;
}

void Untitled::Win32::DestroyRenderEnvironment( ) {
	::UnregisterClass( WndClass.lpszClassName, WndClass.hInstance );

}

int Untitled::Win32::CreateLoggingEnvironment( const char* title ) {
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

void Untitled::Win32::DestroyLoggingEnvironment( ) {
	ShowWindow( GetConsoleWindow( ), SW_HIDE );
	FreeConsole( );
}

Vector2 Untitled::Win32::GetSize( ) {
	RECT rect;

	if ( GetClientRect( Hwnd, &rect ) )
		return Vector2( rect.right - rect.left, rect.bottom - rect.top );

	return Vector2( );
}

int Untitled::Win32::Dispatch( ) {
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

HWND Untitled::Win32::GetHwnd( ) {
	return Hwnd;
}

WNDCLASSEX Untitled::Win32::GetWndClass( ) {
	return WndClass;
}