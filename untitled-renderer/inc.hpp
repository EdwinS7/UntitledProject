#include <windows.h>
#include <iostream>
#include <cstdint>
#include <cassert>
#include <thread>
#include <vector>
#include <string>
#include <format>

// @note: used for M_PI & other PI variables.
#define _USE_MATH_DEFINES
#include <math.h>

// @note: these are used only because I like them like this.
using hwnd_t = HWND;
using byte_t = BYTE;
using rect_t = RECT;
using dword_t = DWORD;
using ulong_t = unsigned long;
using wndclass_t = WNDCLASSEX;

// @note: you can use ( #define UNTITLED_USE_CONSOLE ) for logging & error handling.
// @note: you can use ( #define UNTITLED_USE_WIN32 ) for a render enviornment.
/* Graphics API's ( you must have the library installed for what you choose! )
#define UNTITLED_USE_DX9
#define UNTITLED_USE_DX11
#define UNTITLED_USE_DX12
#define UNTITLED_USE_GL2
#define UNTITLED_USE_GL3
*/

#define UNTITLED_USE_CONSOLE
#define UNTITLED_USE_WIN32
#define UNTITLED_USE_DX9

// @note: add more games if you known any, only add known popular games.
/* supported games: cs:go, css, call of duty, call of duty 2, hl2, tf2, any source engine game */
#ifdef UNTITLED_USE_DX9
	#include <d3d9.h>
	#include <d3dx9.h>
	#pragma comment(lib, "d3d9.lib")
	#pragma comment(lib, "d3dx9.lib")
	#include ".hpp/gfx/dx9.hpp"

	using GfxTexture = LPDIRECT3DTEXTURE9;
	using IDirect3DParamaters9 = D3DPRESENT_PARAMETERS;
#endif

/* supported games: roblox, eft, cs2, 7 days to die, fortnite */
#ifdef UNTITLED_USE_DX11
	#include <d3d11.h>
	#pragma comment(lib, "d3d11.lib")
	#include ".hpp/gfx/dx11.hpp"

	using GfxTexture = ID3D11Texture2D*;
#endif

/* supported games: fortnite */
#ifdef UNTITLED_USE_DX12

#endif

/* supported games: mxs */
#ifdef UNTITLED_USE_GL2

#endif

/* supported games: unknown */
#ifdef UNTITLED_USE_GL3
	#include <GLFW/glfw3.h>
	#pragma comment (lib, "glfw3.lib")
	#include ".hpp/gfx/gl3.hpp"
#endif

// includes
#include ".hpp/buffer/buffer.hpp"
#include ".hpp/buffer/components.hpp"

#ifdef UNTITLED_USE_WIN32
	#include ".hpp/win32/win32.hpp"
#endif


#include ".hpp/ctx/ctx.hpp"