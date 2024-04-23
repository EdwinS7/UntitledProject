#include "Window.hpp"

inline Vec2<int16_t> cWin32::GetPos( ) {
	RECT Rect{ };

	if ( GetWindowRect( m_Hwnd, &Rect ) )
		return Vec2<int16_t>( static_cast< int16_t >( Rect.left ), static_cast< int16_t >( Rect.top ) );

	return Vec2<int16_t>( );
}

inline Vec2<int16_t> cWin32::GetSize( ) {
    RECT Rect{};

    if ( GetClientRect( m_Hwnd, &Rect ) )
        return Vec2<int16_t>( static_cast< int16_t >( Rect.right - Rect.left ), static_cast< int16_t >( Rect.bottom - Rect.top ) );

    return Vec2<int16_t>( );
}

inline Rect<int16_t> cWin32::GetRect( ) {
    Vec2<int16_t> pos = GetPos( );
    Vec2<int16_t> size = GetSize( );
    return Rect<int16_t>( pos.x, pos.y, static_cast< int16_t >( pos.x + size.x ), static_cast< int16_t >( pos.y + size.y ) );
}

inline Rect<int16_t> cWin32::GetClipRect( ) {
    Vec2<int16_t> Size = GetSize( );
    return Rect<int16_t>( 0, 0, Size.x, Size.y );
}

inline HWND cWin32::GetHwnd( ) {
    return m_Hwnd;
}