#include <functional>
#include <algorithm>
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

#include "Thirdparty/ft2build.h"
#include FT_FREETYPE_H
#include FT_BITMAP_H

#include <d3d9.h>
#include <d3dx9.h>
#pragma comment(lib, "d3d9.lib")
#pragma comment(lib, "d3dx9.lib")

#define TRIANGLE D3DPT_TRIANGLESTRIP
#define LINE D3DPT_LINESTRIP

#include "Definitions.hpp"
#include "Include/Graphics/DirectX9.hpp"

// includes
#include "Include/Buffer/Buffer.hpp"
#include "Include/Buffer/Components.hpp"

#include "Include/Input/Input.hpp"
#include "Include/Window/Window.hpp"
#include "Include/Interface/Interface.hpp"

#include "Include/context/Context.hpp"