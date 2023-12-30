#pragma once

#include "../../Common.hpp"

struct Vector2 {
	float x, y;

	Vector2( ) = default;
	Vector2( float x, float y ) : x( x ), y( y ) { }
};

namespace Untitled::Win32 {
	int CreateRenderEnvironment( const char* title, int w = 1280, int h = 800 );
	void DestroyRenderEnvironment( );
	
	int CreateLoggingEnvironment( const char* title );
	void DestroyLoggingEnvironment( );

	Vector2 GetSize( );

	int Dispatch( );

	HWND GetHwnd( );
	WNDCLASSEX GetWndClass( );
};