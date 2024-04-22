#pragma once
#include "../../common.hpp"

class cWin32 {
public:
	HWND InitWindow( const char* title, const Vec2<int16_t> size = { 1280, 720 } );
	void SetWindowTitle( const char* title );
	bool DispatchMessages( );
	void ExitWindow( );

	inline Vec2<int16_t> GetPos( );
	inline Vec2<int16_t> GetSize( );
	inline Rect<int16_t> GetClipRect( );
	inline Rect<int16_t> GetRect( );
	inline HWND GetHwnd( );
private:

	HWND m_Hwnd;
	WNDCLASSEX m_WindowClass;
};

inline const auto gWindow = std::make_unique<cWin32>( );

#include "Window.inl"