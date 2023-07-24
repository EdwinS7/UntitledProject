#include "../../.hpp/input/input.hpp"

#ifdef UNTITLED_USE_INPUT

void c_input::pool_input( ) {
    for ( size_t i = 0; i < 255; i++ ) {
        bool is_pressed = ( GetAsyncKeyState( i ) & 0x8000 ) != 0;
        key_states[ i ] = std::make_pair( is_pressed, is_pressed != prev_key_states[ i ].first );

		prev_key_states[ i ] = key_states[ i ];
    }
}

vector2_t<uint16_t> c_input::get_mouse_pos( ) {
	return m_mouse_pos;
}

void c_input::set_mouse_pos( vector2_t<uint16_t> pos ) {
	m_mouse_pos = pos;
}

bool c_input::mouse_hovering_rect( vector2_t<uint16_t> pos, vector2_t<uint16_t> size ) {
	std::pair<bool, bool> m_hovered {
		get_mouse_pos( ).x > pos.x && get_mouse_pos( ).y > pos.y,
		get_mouse_pos( ).x < pos.x + size.x && get_mouse_pos( ).y < pos.y + size.y
	};

	return m_hovered.first && m_hovered.second;
}

bool c_input::key_pressed( uint8_t key ) {
	return ( std::get<0>( key_states[ key ] ) && std::get<1>( key_states[ key ] ) );
}

bool c_input::key_held( uint8_t key ) {
	return std::get<0>( key_states[ key ] );
}

#endif