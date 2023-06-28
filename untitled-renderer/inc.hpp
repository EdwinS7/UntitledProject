#include <windows.h>
#include <iostream>
#include <shlobj.h>
#include <cstdint>
#include <cassert>
#include <thread>
#include <vector>
#include <string>
#include <format>
#include <math.h>

#define _USE_MATH_DEFINES

// @note: freetype font rasterizing
#include ".lib/ft2build.h"
#include FT_FREETYPE_H
#include FT_BITMAP_H

// @note: custom definitions
#define ALWAYS_INLINE __forceinline

// @note: redefinitions
using ulong_t = unsigned long;
using timepoint_t = std::chrono::steady_clock::time_point;

/* __FEATURES__
#define UNTITLED_USE_CONSOLE ( creates a debug console used for logging)
#define UNTITLED_USE_WIN32 ( creates a Win32 window for rendering )
*/

/* __APIs__
#define UNTITLED_USE_DX9 ( DirectX 9 June 2010 )
#define UNTITLED_USE_DX11 ( DirectX 11 )
#define UNTITLED_USE_DX12 ( DirectX 12 )
#define UNTITLED_USE_GL2 ( OpenGL 2 )
#define UNTITLED_USE_GL3 ( OpenGL 3 )
*/

// @note: demo settings
#define UNTITLED_USE_WIN32
#define UNTITLED_USE_DX9

#ifdef UNTITLED_USE_DX9
	#include <d3d9.h>
	#include <d3dx9.h>
	#pragma comment(lib, "d3d9.lib")
	#pragma comment(lib, "d3dx9.lib")
	#include ".hpp/gfx/dx9.hpp"

	using texture = LPDIRECT3DTEXTURE9;
	using creation_paramaters = D3DDEVICE_CREATION_PARAMETERS;

	#define TRIANGLE D3DPT_TRIANGLESTRIP
	#define LINE D3DPT_LINESTRIP
#endif

#ifdef UNTITLED_USE_DX11
	#include <d3d11.h>
	#pragma comment(lib, "d3d11.lib")
	#include ".hpp/gfx/dx11.hpp"

	using GfxTexture = ID3D11Texture2D*;
#endif

#ifdef UNTITLED_USE_DX12

#endif

#ifdef UNTITLED_USE_GL2

#endif

#ifdef UNTITLED_USE_GL3
	#include <GLFW/glfw3.h>
	#pragma comment (lib, "glfw3.lib")
	#include ".hpp/gfx/gl3.hpp"
#endif

// @note: header file includes
#include ".hpp/buffer/buffer.hpp"
#include ".hpp/buffer/components.hpp"

#ifdef UNTITLED_USE_WIN32
	#include ".hpp/win32/win32.hpp"
#endif

#include ".hpp/ctx/ctx.hpp"