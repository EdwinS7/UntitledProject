#include "Window.hpp"

inline bool cWin32::DispatchMessages( ) {
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

inline bool cWin32::IsFocused( ) {
    return true; //IsIconic( m_Hwnd );
}

inline Vec2<int16_t> cWin32::GetPos( ) {
    RECT rect{};
    if ( GetWindowRect( m_Hwnd, &rect ) )
        return Vec2<int16_t>( static_cast< int16_t >( rect.left ), static_cast< int16_t >( rect.top ) );
    return Vec2<int16_t>( );
}

inline Vec2<int16_t> cWin32::GetSize( ) {
    RECT rect{};
    if ( GetClientRect( m_Hwnd, &rect ) )
        return Vec2<int16_t>( static_cast< int16_t >( rect.right - rect.left ), static_cast< int16_t >( rect.bottom - rect.top ) );
    return Vec2<int16_t>( );
}

inline Rect<int16_t> cWin32::GetRect( ) {
    Vec2<int16_t> pos = GetPos( );
    Vec2<int16_t> size = GetSize( );
    return Rect<int16_t>( pos.x, pos.y, static_cast< int16_t >( pos.x + size.x ), static_cast< int16_t >( pos.y + size.y ) );
}

inline Rect<int16_t> cWin32::GetClipRect( ) {
    Vec2<int16_t> size = GetSize( );
    return Rect<int16_t>( 0, 0, size.x, size.y );
}

inline HWND cWin32::GetHandle( ) {
    return m_Hwnd;
}