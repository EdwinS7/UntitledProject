#include "Window.hpp"

inline Vec2< int16_t > cWindow::GetPos( ) {
	RECT rect{ };

	if ( GetWindowRect( m_Hwnd, &rect ) )
		return Vec2< int16_t >( rect.left, rect.top );

	std::printf( "[ Win32 ] get_pos failed!" );
}

inline Vec2< int16_t > cWindow::GetSize( ) {
	RECT rect{ };

	if ( GetClientRect( m_Hwnd, &rect ) )
		return Vec2< int16_t >( rect.right - rect.left, rect.bottom - rect.top );

	std::printf( "[ Win32 ] get_size failed!" );
}

inline RECT cWindow::GetCanvasRect( ) {
	return RECT( 0, 0, GetSize( ).x, GetSize( ).y );
}

inline RECT cWindow::GetRect( ) {
	return RECT( GetPos().x, GetPos().y, GetPos( ).x + GetSize( ).x, GetPos( ).y + GetSize( ).y );
}

inline HWND cWindow::GetHwnd( ) {
	return m_Hwnd;
}