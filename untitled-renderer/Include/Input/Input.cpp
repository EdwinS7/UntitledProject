#include "input.hpp"

void c_input::PoolInput( ) {
    for ( size_t i = 0; i < 255; i++ ) {
        bool is_pressed = ( GetAsyncKeyState( i ) & 0x8000 ) != 0;
		m_KeyStates[ i ] = std::make_pair( is_pressed, is_pressed != m_PreviousKeyStates[ i ].first );

		m_PreviousKeyStates[ i ] = m_KeyStates[ i ];
    }
}



void c_input::SetMousePos( Vec2<int16_t> pos ) {
	m_MousePos = pos;
}

bool c_input::MouseHoveringRect( Vec2<int16_t> pos, Vec2<int16_t> size ) {
	std::pair<bool, bool> m_hovered {
		GetMousePos( ).x > pos.x && GetMousePos( ).y > pos.y,
		GetMousePos( ).x < pos.x + size.x && GetMousePos( ).y < pos.y + size.y
	};

	return m_hovered.first && m_hovered.second;
}

bool c_input::KeyPressed( int8_t key ) {
	return ( std::get<0>( m_KeyStates[ key ] ) && std::get<1>( m_KeyStates[ key ] ) );
}

bool c_input::KeyHeld( int8_t key ) {
	return std::get<0>( m_KeyStates[ key ] );
}