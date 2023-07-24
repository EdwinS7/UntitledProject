#include <windows.h>
#include <shlobj.h>
#include <thread>
#include <chrono>
#include <vector>
#include <format>

#define _USE_MATH_DEFINES

// freetype
#include ".lib/ft2build.h"
#include FT_FREETYPE_H
#include FT_BITMAP_H

// definitions
#define ALWAYS_INLINE __forceinline
using timepoint_t = std::chrono::steady_clock::time_point;

#include "cfg.hpp"

#ifdef UNTITLED_USE_LOGS
#define LOG(...) printf(__VA_ARGS__)
#else
#define LOG(...)
#endif

#ifdef UNTITLED_USE_DX9
#include <d3d9.h>
#include <d3dx9.h>
#pragma comment(lib, "d3d9.lib")
#pragma comment(lib, "d3dx9.lib")
#include ".hpp/gfx/dx9.hpp"

using texture = LPDIRECT3DTEXTURE9;
using creation_parameters = D3DDEVICE_CREATION_PARAMETERS;

#define TRIANGLE D3DPT_TRIANGLESTRIP
#define LINE D3DPT_LINESTRIP
#endif

#ifdef UNTITLED_USE_DX11
#include <d3d11.h>
#pragma comment(lib, "d3d11.lib")
#include ".hpp/gfx/dx11.hpp"

using texture = ID3D11Texture2D*;

#define TRIANGLE 1
#define LINE 0
#endif

#ifdef UNTITLED_USE_DX12

#endif

// includes
#include ".hpp/buffer/buffer.hpp"
#include ".hpp/buffer/components.hpp"

#ifdef UNTITLED_USE_INPUT
#include ".hpp/input/input.hpp"
#endif

#ifdef UNTITLED_USE_WIN32
#include ".hpp/win32/win32.hpp"
#endif

#include ".hpp/ctx/ctx.hpp"