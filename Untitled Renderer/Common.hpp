#include <filesystem>
#include <shlobj.h>
#include <fstream>
#include <vector>
#include <random>
#include <math.h>
#include <regex>
#include <map>

#include <d3d9.h>
#include <d3dx9.h>
#include <d3dx9tex.h>

#define _USE_MATH_DEFINES
#define CURL_STATICLIB
#define SOL_LUAJIT 1

#include <Base64.hpp>
#include <Sha256.hpp>
#include <Sol/Sol.hpp>
#include <nlohmann/json.hpp>
#include <Curl/Include/curl.h>
#include <FreeType/ft2build.h>
#include FT_FREETYPE_H
#include FT_BITMAP_H

#include "Include/Backend/Logger/Logger.hpp"
#include "Include/Backend/FileSystem/FileSystem.hpp"
#include "Include/Backend/Animation/Animation.hpp"

#include "Include/Backend/Window/Window.hpp"

#include "Include/Backend/Graphics/Components.hpp"
#include "Include/Backend/Graphics/DirectX9.hpp"

#include "Include/Backend/Buffer/Components.hpp"
#include "Include/Backend/Buffer/Buffer.hpp"

#include "Include/Backend/Camera/Camera.hpp"

#include "Include/Backend/Audio/Audio.hpp"
#include "Include/Backend/Input/Input.hpp"

#include "Include/Backend/LuaWrapper/LuaWrapper.hpp"

#include "Include/Scenes/Interface/Interface.hpp"
#include "Include/Scenes/World/World.hpp"

#include "Include/Backend/Context/Context.hpp"