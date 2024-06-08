#pragma once
#ifndef WINDOW_H
#define WINDOW_H

#include "../../../Common.hpp"

class cWin32 {
public:
	void Init( const char* title, const Vec2<int16_t>& size = { 1280, 720 }, const bool fullscreen = true );
	bool DispatchMessages( );

	inline bool IsMinimized( );
	inline bool IsFocused( );

	void SetFullscreen( const bool fullscreen );
	inline bool GetFullscreen( ) const;

	inline void SetPos( const Vec2<int16_t>& pos );
	inline Vec2<int16_t> GetPos( ) const;

	inline void SetSize( const Vec2<int16_t>& size );
	inline Vec2<int16_t> GetSize( ) const;

	inline Rect<int16_t> GetRect( ) const;
	inline Rect<int16_t> GetClipRect( ) const;

	inline HWND GetHandle( ) const;

private:
	HWND m_Hwnd;
	WNDCLASSEX m_WindowClass;

	bool m_Fullscreen{ true };
};

inline const auto gWindow = std::make_unique<cWin32>( );

#include "Window.inl"
#endif