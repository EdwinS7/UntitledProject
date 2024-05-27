#pragma once
#ifndef WINDOW_H
#define WINDOW_H

#include "../../Common.hpp"

class cWin32 {
public:
	void InitWindow( const char* title, const Vec2<int16_t>& position = { 0, 0 }, const Vec2<int16_t>& size = { 1280, 720 } );
	void ExitWindow( );

	inline bool DispatchMessages( );
	inline Vec2<int16_t> GetPos( );
	inline Vec2<int16_t> GetSize( );
	inline Rect<int16_t> GetRect( );
	inline Rect<int16_t> GetClipRect( );
	inline HWND GetHandle( );
	inline bool IsFocused( );
private:

	HWND m_Hwnd;
	WNDCLASSEX m_WindowClass;
};

inline const auto gWindow = std::make_unique<cWin32>( );

#include "Window.inl"
#endif