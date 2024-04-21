#include "Logger.hpp"

inline Vec2< int16_t > cConsole::GetPos( ) {
	RECT rect{ };

	if ( GetWindowRect( GetConsoleWindow( ), &rect ) )
		return Vec2< int16_t >( rect.left, rect.top );

	return Vec2< int16_t >( );
}

inline Vec2< int16_t > cConsole::GetSize( ) {
	RECT rect{ };

	if ( GetClientRect( GetConsoleWindow( ), &rect ) )
		return Vec2< int16_t >( rect.right - rect.left, rect.bottom - rect.top );

	return Vec2< int16_t >( );
}

inline Rect<int16_t> cConsole::GetClipRect( ) {
	return Rect<int16_t>( 0, 0, GetSize( ).x, GetSize( ).y );
}

inline Rect<int16_t> cConsole::GetRect( ) {
	return Rect<int16_t>( GetPos( ).x, GetPos( ).y, GetPos( ).x + GetSize( ).x, GetPos( ).y + GetSize( ).y );
}

inline HWND cConsole::GetHwnd( ) {
	return GetConsoleWindow( );
}