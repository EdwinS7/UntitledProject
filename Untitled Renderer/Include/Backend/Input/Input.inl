#include "Input.hpp"

inline void cInput::SetMousePos( Vec2<int16_t> position ) {
	m_MousePos = position;
}

inline Vec2<int16_t> cInput::GetMousePos( ) const {
	return m_MousePos;
}

inline void cInput::SetCursorStyle( LPSTR id ) {
	m_CursorStyle = id;
}

inline LPSTR cInput::GetCursorStyle( ) const {
	return m_CursorStyle;
}

inline bool cInput::AnyKeyPressed() const {
	return m_AnyKeyPressed;
}