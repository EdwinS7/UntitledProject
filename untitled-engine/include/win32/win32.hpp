#pragma once

// includes
#include "../../common.hpp"

class c_win32 {
public:
	void setWindowTitle( const char* title );
	void createWindow( const char* title, Vector2 size );
	bool dispatch( );

	Vector2 getPosition( );
	Vector2 getSize( );

	HWND getHwnd( );

private:
	Vector2 m_pPos, m_pSize;

	HWND m_pHwnd;
	WNDCLASSEX m_pWindowClass;

	bool m_pCreated;
};

inline const auto g_win32 = std::make_unique<c_win32>( );

#include "win32.inl"