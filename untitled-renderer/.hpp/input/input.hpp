#pragma once

// includes
#include "../../inc.hpp"

#ifdef UNTITLED_USE_INPUT

class c_input {
public:
	void pool_input( );

	vector2_t<uint16_t> get_mouse_pos( );

	void set_mouse_pos( vector2_t<uint16_t> pos );

	bool mouse_hovering_rect( vector2_t<uint16_t> pos, vector2_t<uint16_t> size );

	bool key_pressed( uint8_t key );

	bool key_held( uint8_t key );

private:
	vector2_t<uint16_t> m_mouse_pos;

	std::map<uint8_t, bool> prev_key_states;
	std::map<uint8_t, std::pair<bool, bool>> key_states;
};

inline const auto g_input = std::make_unique<c_input>( );

#endif