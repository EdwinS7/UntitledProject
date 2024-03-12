#pragma once

#include "../../common.hpp"
#include "KeyDefinitions.hpp"

class cInput {
public:
	void PoolInput( );

	inline Vec2<int16_t> GetMousePos( );
	void SetMousePos( Vec2<int16_t> position );

	bool MouseHoveringRect( Vec2<int16_t> position, Vec2<int16_t> size );

	bool KeyPressed( int8_t key );
	bool KeyHeld( int8_t key );

	void SetCursorStyle( LPSTR id );
	inline LPSTR GetCursorStyle( );

	inline bool AnyKeyPressed( );

private:
	bool m_AnyKeyPressed;
	Vec2<int16_t> m_MousePos;

	std::pair<bool, bool> m_PreviousKeyStates[ 255 ];
	std::pair<bool, bool> m_KeyStates[ 255 ];

	LPSTR m_CursorStyle;
};

inline const auto gInput = std::make_unique<cInput>( );

#include "Input.inl"