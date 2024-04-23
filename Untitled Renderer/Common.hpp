#include <corecrt_math_defines.h>
#include <shlobj.h>
#include <thread>
#include <vector>
#include <format>

#include <d3d9.h>
#include <d3dx9.h>
#include <d3dx9tex.h>

/*#include <d3d11.h>
#include <d3dx11.h>
#include <d3dx11tex.h>
#include <d3dx11core.h>
#include <d3dx11async.h>*/

#define VERTEX ( D3DFVF_XYZRHW | D3DFVF_DIFFUSE | D3DFVF_TEX1 )
#define TRIANGLE D3D11_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP
#define LINE D3D11_PRIMITIVE_TOPOLOGY_LINESTRIP

#include "Thirdparty/ft2build.h"
#include FT_FREETYPE_H
#include FT_BITMAP_H

#include "Definitions.hpp"
#include "Include/Graphics/DirectX9.hpp"

#include "Include/Buffer/Buffer.hpp"
#include "Include/Buffer/Components.hpp"

#include "Include/Input/Input.hpp"
#include "Include/Window/Window.hpp"
#include "Include/Interface/Interface.hpp"

#include "Include/context/Context.hpp"