#pragma once
#ifndef INPUT_H
#define INPUT_H

#include "../../../Common.hpp"
#include "KeyDefinitions.hpp"

class cInput {
public:
	void PoolInput( );
	bool IsActive( );

	bool MouseHoveringRect( Vec2<int16_t> position, Vec2<int16_t> size );

	bool KeyPressed( int8_t key ) const;
	bool KeyHeld( int8_t key ) const;

	inline void SetMousePos( Vec2<int16_t> position );
	inline Vec2<int16_t> GetMousePos( ) const;

	inline void SetCursorStyle( LPSTR id );
	inline LPSTR GetCursorStyle( ) const;

	inline bool AnyKeyPressed( ) const;

private:
	bool m_AnyKeyPressed{};
	Vec2<int16_t> m_MousePos;

	std::pair<bool, bool> m_PreviousKeyStates[ 255 ];
	std::pair<bool, bool> m_KeyStates[ 255 ];

	LPSTR m_CursorStyle{};
};

inline const auto gInput = std::make_unique<cInput>( );

#include "Input.inl"
#endif