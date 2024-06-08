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

void cWin32::Init( const char* title, const Vec2<int16_t>& size, const bool console ) {
	if ( console ) {
		AllocConsole( );
		SetConsoleTitleA( title );

		FILE* fp = nullptr;
		freopen_s( &fp, "CONIN$", "r", stdin );
		freopen_s( &fp, "CONOUT$", "w", stdout );
		freopen_s( &fp, "CONOUT$", "w", stderr );
	}

	m_WindowClass = {
		sizeof( m_WindowClass ), CS_CLASSDC, WndProc,
		0L, 0L, GetModuleHandle( NULL ),
		NULL, NULL, NULL,
		NULL, title, NULL
	};

	RegisterClassEx( &m_WindowClass );

	RECT DesktopRect;
	int Horizontal = 0, Vertical = 0;
	const HWND Desktop = GetDesktopWindow( );
	GetWindowRect( Desktop, &DesktopRect );
	Horizontal = DesktopRect.right;
	Vertical = DesktopRect.bottom;

	m_Hwnd = CreateWindow(
		m_WindowClass.lpszClassName, title, WS_OVERLAPPEDWINDOW,
		( Horizontal / 2 ) - (size.x / 2), ( Vertical / 2) - (size.y / 2), size.x, size.y, NULL, NULL,
		m_WindowClass.hInstance, NULL
	);
	
	ShowWindow( m_Hwnd, SW_SHOWDEFAULT );
	UpdateWindow( m_Hwnd );
}