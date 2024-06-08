#include "Window.hpp"

LRESULT CALLBACK WndProc( HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam );

void cWin32::Init( const char* title, const Vec2<int16_t>& size, const bool fullscreen ) {
	m_WindowClass = {
		sizeof( m_WindowClass ), CS_CLASSDC, WndProc,
		0L, 0L, GetModuleHandle( NULL ),
		NULL, NULL, NULL,
		NULL, title, NULL
	};

	RegisterClassEx( &m_WindowClass );

    Vec2<int16_t> WindowSize;

	RECT DesktopRect;
	GetWindowRect( GetDesktopWindow( ), &DesktopRect );

    WindowSize = { static_cast< int16_t >( DesktopRect.right ), static_cast< int16_t >( DesktopRect.bottom ) };

	m_Hwnd = CreateWindow(
		m_WindowClass.lpszClassName, title, WS_OVERLAPPEDWINDOW,
        fullscreen ? 0 : ( WindowSize.y / 2 ) - (size.x / 2), fullscreen ? 0 : ( WindowSize.x / 2) - (size.y / 2), size.x, size.y, NULL, NULL,
		m_WindowClass.hInstance, NULL
	);

    if ( fullscreen ) {
        LONG_PTR Style = GetWindowLongPtr( m_Hwnd, GWL_STYLE );
        Style &= ~( WS_CAPTION | WS_THICKFRAME | WS_MINIMIZE | WS_MAXIMIZE | WS_SYSMENU );
        SetWindowLongPtr( m_Hwnd, GWL_STYLE, Style );
    }
	
	ShowWindow( m_Hwnd, SW_SHOWDEFAULT );
	UpdateWindow( m_Hwnd );

    // Just for developers rn
    AllocConsole( );
    SetConsoleTitleA( title );

    FILE* fp = nullptr;
    freopen_s( &fp, "CONIN$", "r", stdin );
    freopen_s( &fp, "CONOUT$", "w", stdout );
    freopen_s( &fp, "CONOUT$", "w", stderr );
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

LRESULT CALLBACK WndProc( HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam ) {
    static bool isResizing = false;

    switch ( msg ) {
    case WM_SIZE:
        if ( !isResizing && gGraphics->GetDevice( ) && wParam != SIZE_MINIMIZED ) {
            gGraphics->UpdatePresentationParameters( lParam );
            gGraphics->ResetDevice( );
        }
        return 0;

    case WM_ENTERSIZEMOVE:
        isResizing = true;
        return 0;

    case WM_EXITSIZEMOVE:
        if ( gGraphics->GetDevice( ) ) {
            RECT Rect;
            GetClientRect( hwnd, &Rect );

            gGraphics->UpdatePresentationParameters(
                MAKELPARAM( Rect.right - Rect.left, Rect.bottom - Rect.top )
            );

            gGraphics->ResetDevice( );
        }
        isResizing = false;
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