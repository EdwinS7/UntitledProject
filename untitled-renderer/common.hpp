#include <windows.h>
#include <Uxtheme.h>
#include <dwmapi.h>
#include <shlobj.h>
#include <cassert>
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

#include "config.hpp"

#ifdef UNTITLED_USE_DX9
#include <d3d9.h>
#include <d3dx9.h>
#pragma comment(lib, "d3d9.lib")
#pragma comment(lib, "d3dx9.lib")

using texture = LPDIRECT3DTEXTURE9;
using creation_parameters = D3DDEVICE_CREATION_PARAMETERS;

#define TRIANGLE D3DPT_TRIANGLESTRIP
#define LINE D3DPT_LINESTRIP

#include "include/graphics/dx9.hpp"
#endif

// includes
#include "include/buffer/buffer.hpp"
#include "include/buffer/components.hpp"

#include "include/user input/input.hpp"
#include "include/win32/win32.hpp"
#include "include/user interface/gui.hpp"
#include "include/context/context.hpp"