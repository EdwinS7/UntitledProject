#include "Input.hpp"

void cInput::PoolInput( ) {
	m_CursorStyle = IDC_ARROW;
	m_AnyKeyPressed = false;

	char Buffer[ 256 ];
    if ( GetKeyboardState( ( PBYTE ) Buffer ) ) {
        for (size_t i = 0; i < 256; i++) {
            bool isPressed = ( Buffer[ i ] & 0x80 ) != 0;

            if (isPressed)
                m_AnyKeyPressed = true;

			m_KeyStates[ i ] = { isPressed, ( isPressed != m_PreviousKeyStates[ i ].first ) };
			m_PreviousKeyStates[ i ] = m_KeyStates[ i ];
        }
    }
}

bool cInput::IsActive( ) {
	return gWindow->IsFocused( );
}

void cInput::SetMousePos( Vec2<int16_t> position ) {
	m_MousePos = position;
}

bool cInput::MouseHoveringRect( Vec2<int16_t> position, Vec2<int16_t> size ) {
	std::pair<bool, bool> m_hovered {
		GetMousePos( ).x > position.x && GetMousePos( ).y > position.y,
		GetMousePos( ).x < position.x + size.x && GetMousePos( ).y < position.y + size.y
	};

	return m_hovered.first && m_hovered.second;
}

bool cInput::KeyPressed( int8_t key ) {
	return ( std::get<0>( m_KeyStates[ key ] ) && std::get<1>( m_KeyStates[ key ] ) );
}

bool cInput::KeyHeld( int8_t key ) {
	return std::get<0>( m_KeyStates[ key ] );
}


void cInput::SetCursorStyle( LPSTR id ) {
	m_CursorStyle = id;
}