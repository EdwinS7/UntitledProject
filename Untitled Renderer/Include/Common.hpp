#pragma once

#include <corecrt_math_defines.h>
#include <winsock2.h>
#include <ws2tcpip.h>
#include <filesystem>
#include <shlobj.h>
#include <intrin.h>
#include <fstream>
#include <vector>
#include <thread>
#include <random>
#include <regex>
#include <map>

#include <d3d9.h>
#include <d3dx9.h>
#include <d3dx9tex.h>

#define CURL_STATICLIB

//#define SOL_CHECK_ARGUMENTS 1
#define SOL_ALL_SAFETIES_ON 1
#define SOL_LUAJIT 1

#include <Base64.hpp>
#include <Sha256.hpp>
#include <Sol/Sol.hpp>
#include <nlohmann/json.hpp>
#include <Curl/Include/curl.h>
#include <FreeType/ft2build.h>
#include FT_FREETYPE_H
#include FT_BITMAP_H