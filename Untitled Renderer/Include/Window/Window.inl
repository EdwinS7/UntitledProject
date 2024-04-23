#include "Window.hpp"

inline Vec2< int16_t > cWin32::GetPos( ) {
	RECT rect{ };

	if ( GetWindowRect( m_Hwnd, &rect ) )
		return Vec2< int16_t >( rect.left, rect.top );

	return Vec2< int16_t >( );
}

inline Vec2< int16_t > cWin32::GetSize( ) {
	RECT rect{ };

	if ( GetClientRect( m_Hwnd, &rect ) )
		return Vec2< int16_t >( rect.right - rect.left, rect.bottom - rect.top );

	return Vec2< int16_t >( );
}

inline Rect<int16_t> cWin32::GetRect( ) {
	return Rect<int16_t>( GetPos().x, GetPos().y, GetPos( ).x + GetSize( ).x, GetPos( ).y + GetSize( ).y );
}

inline Rect<int16_t> cWin32::GetClipRect( ) {
	return Rect<int16_t>( 0, 0, GetSize( ).x, GetSize( ).y );
}

inline HWND cWin32::GetHwnd( ) {
	return m_Hwnd;
}