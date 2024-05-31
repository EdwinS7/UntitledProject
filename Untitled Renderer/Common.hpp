#include <corecrt_math_defines.h>
#include <unordered_map>
#include <filesystem>
#include <shlobj.h>
#include <fstream>
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

#include "Thirdparty/sol/sol.hpp"

#include "Thirdparty/ft2build.h"
#include FT_FREETYPE_H
#include FT_BITMAP_H

#include "Include/Backend/Graphics/DirectX9.hpp"

#include "Include/Backend/Buffer/Buffer.hpp"
#include "Include/Backend/Buffer/Components.hpp"

#include "Include/Backend/Input/Input.hpp"
#include "Include/Backend/Window/Window.hpp"

#include "Include/Backend/Wrapper/Wrapper.hpp"

#include "Include/Scenes/Interface/Interface.hpp"
#include "Include/Scenes/World/World.hpp"

#include "Include/Backend/Context/Context.hpp"