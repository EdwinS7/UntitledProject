#pragma once

#include "../../common.hpp"

class CWin32 {
public:
	int CreateRenderEnvironment( const char* title );
	void DestroyRenderEnvironment( );
	
	int CreateLoggingEnvironment( const char* title );
	void DestroyLoggingEnvironment( );

	int Dispatch( );

	INLINE HWND GetHwnd( );
	INLINE WNDCLASSEX GetWndClass( );

private:
	HWND m_Hwnd;
	WNDCLASSEX m_WndClass;

	int m_ActiveWindow = 0;

};

inline const auto Win32 = std::make_unique<CWin32>( );

#include "win32.inl"