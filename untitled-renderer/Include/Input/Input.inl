#include "Input.hpp"

inline Vec2<int16_t> cInput::GetMousePos( ) {
	return m_MousePos;
}

inline LPSTR cInput::GetCursorStyle( ) {
	return m_CursorStyle;
}

inline bool cInput::AnyKeyPressed() {
	return m_AnyKeyPressed;
}