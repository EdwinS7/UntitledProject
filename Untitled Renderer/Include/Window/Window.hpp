#pragma once
#include "../../common.hpp"

class cWindow {
public:

	int CreateRenderEnvironment( const char* title, const Vec2< int16_t > size );
	void SetRenderEnvironmentTitle( const char* title );
	void DestroyRenderEnvironment( );

	int CreateLoggingEnvironment( const char* title );
	void SetLoggingEnvironmentTitle( const char* title );
	void DestroyLoggingEnvironment();

	bool Dispatch( );

	inline Vec2<int16_t> GetPos( );
	inline Vec2<int16_t> GetSize( );
	inline Rect<int16_t> GetClipRect( );
	inline Rect<int16_t> GetRect( );
	inline HWND GetHwnd( );
private:

	HWND m_Hwnd;
	WNDCLASSEX m_WindowClass;
};

inline const auto gWin32 = std::make_unique<cWindow>( );

#include "Window.inl"