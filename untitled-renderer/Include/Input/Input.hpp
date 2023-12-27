#pragma once

#include "../../common.hpp"
#include "KeyDefinitions.hpp"

class c_input {
public:
	void PoolInput( );

	inline Vec2<int16_t> GetMousePos( );
	void SetMousePos( Vec2<int16_t> pos );

	bool MouseHoveringRect( Vec2<int16_t> pos, Vec2<int16_t> size );

	bool KeyPressed( int8_t key );
	bool KeyHeld( int8_t key );

private:
	Vec2<int16_t> m_MousePos;

	std::pair<bool, bool> m_PreviousKeyStates[ 255 ];
	std::pair<bool, bool> m_KeyStates[ 255 ];
};

inline const auto g_input = std::make_unique<c_input>( );

#include "Input.inl"