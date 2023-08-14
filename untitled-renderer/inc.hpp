#include <windows.h>
#include <shlobj.h>
#include <thread>
#include <chrono>
#include <vector>
#include <format>

#define _USE_MATH_DEFINES

// freetype
#include "third-party/ft2build.h"
#include FT_FREETYPE_H
#include FT_BITMAP_H

// definitions
#define ALWAYS_INLINE __forceinline
using timepoint_t = std::chrono::steady_clock::time_point;

#include "cfg.hpp"

#ifdef UNTITLED_USE_DX9
#include <d3d9.h>
#include <d3dx9.h>
#pragma comment(lib, "d3d9.lib")
#pragma comment(lib, "d3dx9.lib")

using texture = LPDIRECT3DTEXTURE9;
using creation_parameters = D3DDEVICE_CREATION_PARAMETERS;

#define TRIANGLE D3DPT_TRIANGLESTRIP
#define LINE D3DPT_LINESTRIP

#include "include/gfx/dx9.hpp"
#endif

#ifdef UNTITLED_USE_DX11
#include "include/gfx/dx11.hpp"
#endif

// includes
#include "include/console/console.hpp"
#include "include/buffer/buffer.hpp"
#include "include/buffer/components.hpp"

#include "include/input/input.hpp"
#include "include/win32/win32.hpp"
#include "include/gui/gui.hpp"
#include "include/ctx/ctx.hpp"