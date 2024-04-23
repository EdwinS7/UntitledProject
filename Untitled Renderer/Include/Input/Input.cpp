#include "Input.hpp"

void cInput::PoolInput( ) {
	m_CursorStyle = IDC_ARROW;
	m_AnyKeyPressed = false;

    for ( size_t i = 0; i < 255; i++ ) {
        bool IsPressed = ( GetAsyncKeyState( i ) & 0x8000 ) != 0;
		if ( IsPressed )
			m_AnyKeyPressed = true;

		m_KeyStates[ i ] = std::make_pair( IsPressed, IsPressed != m_PreviousKeyStates[ i ].first );
		m_PreviousKeyStates[ i ] = m_KeyStates[ i ];
    }
}

bool cInput::IsActive( ) {
	return GetFocus( ) == gWindow->GetHwnd( );
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