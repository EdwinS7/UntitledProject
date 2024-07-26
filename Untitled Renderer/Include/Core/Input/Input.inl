#include "Input.hpp"

inline void cInput::SetMousePos( Vec2<int16_t> position ) {
	m_MousePosition = position;
}

inline Vec2<int16_t> cInput::GetMousePos( ) const {
	return m_MousePosition;
}

inline void cInput::SetMouseWheelDelta(float delta) {
	m_MouseWheelDelta = delta;
}

inline float cInput::GetMouseWheelDelta() const {
	return m_MouseWheelDelta;
}

inline void cInput::SetCursorStyle( MousePointer id ) {
	m_CursorStyle = id;
}

inline MousePointer cInput::GetCursorStyle( ) const {
	return m_CursorStyle;
}

inline bool cInput::AnyKeyPressed() const {
	return m_AnyKeyPressed;
}