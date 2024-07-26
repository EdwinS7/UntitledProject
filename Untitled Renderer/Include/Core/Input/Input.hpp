#pragma once
#ifndef INPUT_H
#define INPUT_H

#include <Common.hpp>
#include "KeyDefinitions.hpp"
#include "../Window/Window.hpp"

enum class MousePointer {
    Arrow = 32512, IBeam = 32513, Wait = 32514,
    Cross = 32515, UpArrow = 32516, Size = 32640,
    Icon = 32641, SizeNWSE = 32642,
    SizeNESW = 32643, SizeWE = 32644,
    SizeNS = 32645, SizeAll = 32646, No = 32648,
	Hand = 32649, AppStarting = 32650, Help = 32651,
	Pin = 32671, Person = 32672
};

class cInput {
public:
	void PoolInput( );
	bool IsActive( );

	bool MouseHoveringRect( Vec2<int16_t> position, Vec2<int16_t> size );

	bool KeyPressed( int8_t key ) const;
	bool KeyHeld( int8_t key ) const;

	inline void SetMousePos( Vec2<int16_t> position );
	inline Vec2<int16_t> GetMousePos( ) const;

	inline void SetMouseWheelDelta( float delta );
	inline float GetMouseWheelDelta( ) const;

	inline void SetCursorStyle( MousePointer id );
	inline MousePointer GetCursorStyle( ) const;

	inline bool AnyKeyPressed( ) const;

private:
	Vec2<int16_t> m_MousePosition;

	float m_MouseWheelDelta{ 0.f };
	bool m_AnyKeyPressed{ false };

	MousePointer m_CursorStyle{ MousePointer::Arrow };

	std::pair<bool, bool> m_PreviousKeyStates[ 255 ];
	std::pair<bool, bool> m_KeyStates[ 255 ];
};

inline const auto gInput = std::make_unique<cInput>( );

#include "Input.inl"
#endif