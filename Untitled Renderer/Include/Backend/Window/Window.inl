#include "Window.hpp"

inline bool cWin32::IsMinimized() {
    return IsIconic(m_Hwnd) != 0;
}

inline bool cWin32::IsFocused() {
    return m_Hwnd == GetForegroundWindow();
}

inline void cWin32::SetFullscreen( bool fullscreen ) {
    static Rect<int16_t> PreviousRect;
    static bool PreviousRectSet = false;

    if ( !gWindow->GetFullscreen( ) && fullscreen ) {
        PreviousRect = gWindow->GetRect( );
        PreviousRectSet = true;
    }

    if ( fullscreen ) {
        LONG_PTR Style = GetWindowLongPtr( m_Hwnd, GWL_STYLE );
        Style &= ~( WS_CAPTION | WS_THICKFRAME | WS_MINIMIZE | WS_MAXIMIZE | WS_SYSMENU );
        SetWindowLongPtr( m_Hwnd, GWL_STYLE, Style );

        HMONITOR Monitor = MonitorFromWindow( m_Hwnd, MONITOR_DEFAULTTONEAREST );
        MONITORINFO MonitorInfo;

        MonitorInfo.cbSize = sizeof( MonitorInfo );
        GetMonitorInfo( Monitor, &MonitorInfo );

        SetWindowPos( m_Hwnd, HWND_TOP, MonitorInfo.rcMonitor.left, MonitorInfo.rcMonitor.top,
            MonitorInfo.rcMonitor.right - MonitorInfo.rcMonitor.left,
            MonitorInfo.rcMonitor.bottom - MonitorInfo.rcMonitor.top, SWP_FRAMECHANGED );

        m_Fullscreen = true;
    }
    else {
        SetWindowLongPtr( m_Hwnd, GWL_STYLE, WS_OVERLAPPEDWINDOW );
        SetWindowPos( m_Hwnd, HWND_TOP, PreviousRect.x, PreviousRect.y, PreviousRect.w, PreviousRect.h, SWP_NOZORDER | SWP_FRAMECHANGED );

        ShowWindow( m_Hwnd, SW_SHOWDEFAULT );
        UpdateWindow( m_Hwnd );

        PreviousRectSet = false;
        m_Fullscreen = false;
    }
}

inline bool cWin32::GetFullscreen( ) const {
    LONG_PTR Style = GetWindowLongPtr( m_Hwnd, GWL_STYLE );
    return !( Style & WS_CAPTION ) && !( Style & WS_THICKFRAME );
}

inline void cWin32::SetPos( Vec2<int16_t>& pos ) {
    SetWindowPos( m_Hwnd, NULL, pos.x, pos.y, 0, 0, SWP_NOSIZE | SWP_NOZORDER );
}

inline Vec2<int16_t> cWin32::GetPos( ) const {
    RECT rect{};

    if (GetWindowRect(m_Hwnd, &rect)) {
        return Vec2<int16_t>(
            static_cast<int16_t>(rect.left),
            static_cast<int16_t>(rect.top)
        );
    }
        
    return Vec2<int16_t>( );
}

inline void cWin32::SetSize( Vec2<int16_t>& size ) {
    SetWindowPos( m_Hwnd, NULL, 0, 0, size.x, size.y, SWP_NOMOVE | SWP_NOZORDER );
}

inline Vec2<int16_t> cWin32::GetSize( ) const {
    RECT rect{};

    bool Result = GetFullscreen() ? 
        GetWindowRect( m_Hwnd, &rect ) : GetClientRect( m_Hwnd, &rect );

    if ( Result ) {
        return Vec2<int16_t>(
            static_cast<int16_t>(rect.right - rect.left),
            static_cast<int16_t>(rect.bottom - rect.top)
        );
    }
       
    return Vec2<int16_t>( );
}

inline void cWin32::SetRect( const Rect<int16_t>& rect ) {
    SetWindowPos( m_Hwnd, NULL, rect.x, rect.y, rect.w, rect.h, SWP_NOZORDER );
}

inline Rect<int16_t> cWin32::GetRect( ) const {
    Vec2<int16_t> pos = GetPos( );
    Vec2<int16_t> size = GetSize( );

    return Rect<int16_t>( 
        pos.x, pos.y, 
        size.x, size.y
    );
}

inline Rect<int16_t> cWin32::GetClipRect( ) const {
    Vec2<int16_t> size = GetSize( );
    return Rect<int16_t>( 0, 0, size.x, size.y );
}

inline HWND cWin32::GetHandle( ) const {
    return m_Hwnd;
}