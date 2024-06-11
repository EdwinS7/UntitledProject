#include "Window.hpp"

LRESULT CALLBACK WndProc( HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam );

void cWin32::Init( const std::string& title, Vec2<int16_t> size, bool fullscreen ) {
    AllocConsole( );
    SetConsoleTitleA( title.c_str( ) );

    FILE* fp = nullptr;
    freopen_s( &fp, "CONIN$", "r", stdin );
    freopen_s( &fp, "CONOUT$", "w", stdout );
    freopen_s( &fp, "CONOUT$", "w", stderr );

    HANDLE hStdOut = GetStdHandle( STD_OUTPUT_HANDLE );

    for ( const auto& Log : gLogger->GetLogs( LogLevel::END ) ) {
        std::cout << Log << "\n";
    }

    m_Fullscreen = fullscreen;

    m_WindowClass = {
        sizeof( m_WindowClass ), CS_CLASSDC, WndProc,
        0L, 0L, GetModuleHandle( NULL ),
        NULL, NULL, NULL,
        NULL, title.c_str( ), NULL
    };

    RegisterClassEx( &m_WindowClass );

    RECT DesktopRect;
    GetWindowRect( GetDesktopWindow( ), &DesktopRect );

    Vec2<int16_t> WindowSize = { static_cast< int16_t >( DesktopRect.right ), static_cast< int16_t >( DesktopRect.bottom ) };

    DWORD WindowStyle = fullscreen ? WS_POPUP : WS_OVERLAPPEDWINDOW;

    m_Hwnd = CreateWindow(
        m_WindowClass.lpszClassName, title.c_str( ), WindowStyle,
        fullscreen ? 0 : ( WindowSize.x / 2 ) - ( size.x / 2 ),
        fullscreen ? 0 : ( WindowSize.y / 2 ) - ( size.y / 2 ),
        size.x, size.y, NULL, NULL, m_WindowClass.hInstance, NULL
    );

    if ( fullscreen ) {
        SetWindowLongPtr( m_Hwnd, GWL_STYLE, WindowStyle & ~WS_CAPTION & ~WS_THICKFRAME );
        SetWindowPos( m_Hwnd, HWND_NOTOPMOST, 0, 0, WindowSize.x, WindowSize.y, SWP_FRAMECHANGED | SWP_SHOWWINDOW );
    }

    ShowWindow( m_Hwnd, SW_SHOWDEFAULT );
    UpdateWindow( m_Hwnd );
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

// WndProc, Defined at the top of Window.cpp ( this file )
LRESULT CALLBACK WndProc( HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam ) {
    static bool IsResizing = false;

    switch ( msg ) {
    case WM_SIZE:
        if ( !IsResizing && gGraphics->GetDevice( ) && wParam != SIZE_MINIMIZED ) {
            gGraphics->ResetDevice( hwnd );
        }
        return 0;

    case WM_ENTERSIZEMOVE:
        IsResizing = true;
        return 0;

    case WM_EXITSIZEMOVE:
        if ( gGraphics->GetDevice( ) ) {
            gGraphics->ResetDevice( hwnd );
        }
        IsResizing = false;
        return 0;

    case WM_MOUSEMOVE:
        gInput->SetMousePos( Vec2<int16_t>( LOWORD( lParam ), HIWORD( lParam ) ) );
        break;

    case WM_SETCURSOR:
        SetCursor( LoadCursorA( 0, LPCSTR( gInput->GetCursorStyle( ) ) ) );
        break;

    case WM_DESTROY:
        PostQuitMessage( 0 );
        return 0;
    }

    return DefWindowProc( hwnd, msg, wParam, lParam );
}