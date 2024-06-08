#include "Window.hpp"

inline LRESULT CALLBACK WndProc( HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam ) {
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
		( WindowSize.y / 2 ) - (size.x / 2), ( WindowSize.x / 2) - (size.y / 2), size.x, size.y, NULL, NULL,
		m_WindowClass.hInstance, NULL
	);
	
	ShowWindow( m_Hwnd, SW_SHOWDEFAULT );
	UpdateWindow( m_Hwnd );

    SetFullscreen( fullscreen );

    AllocConsole( );
    SetConsoleTitleA( title );

    FILE* fp = nullptr;
    freopen_s( &fp, "CONIN$", "r", stdin );
    freopen_s( &fp, "CONOUT$", "w", stdout );
    freopen_s( &fp, "CONOUT$", "w", stderr );
}