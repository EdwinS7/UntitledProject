#pragma once

#define NOMINMAX
#define WIN32_LEAN_AND_MEAN
#define _HAS_EXCEPTIONS 0

#define IMGUI_DEFINE_MATH_OPERATORS

#define IM_VEC2_CLASS_EXTRA \
        __forceinline float dot( const ImVec2& rhs ) const { return rhs.x * x + rhs.y * y; } \
        __forceinline float length_sqr( ) const { return dot( *this ); } \
        __forceinline float length( ) const { return std::sqrt( length_sqr( ) ); } \
        __forceinline float normalize( ) { const auto len = length( ); if ( len ) { x /= len; y /= len; } return len; } \
        __forceinline ImVec2 normalized( ) { auto ret = *this; ret.normalize( ); return ret; }


#define IM_VEC3_CLASS_EXTRA \
        __forceinline ImVec3 cross( const ImVec3& rhs ) const { return { ( y * rhs.z ) - ( z * rhs.y ), ( z * rhs.x ) - ( x * rhs.z ), ( x * rhs.y ) - ( y * rhs.x ) }; } \
	    __forceinline float dot( const float* const rhs ) const { return ( x * rhs[ 0u ] + y * rhs[ 1u ] + z * rhs[ 2u ] ); } \
        __forceinline float dot( const ImVec3& rhs ) const { return rhs.x * x + rhs.y * y + rhs.z * z; } \
        __forceinline float length_sqr( ) const { return dot( *this ); } \
        __forceinline float length_2d_sqr( ) const { return x * x + y * y; } \
        __forceinline float length( ) const { return std::sqrt( length_sqr( ) ); } \
        __forceinline float length_2d( ) const { return std::sqrt( length_2d_sqr( ) ); } \
        __forceinline float normalize( ) { const auto len = length( ); if ( len ) { x /= len; y /= len; z /= len; } return len; } \
        __forceinline ImVec3 normalized( ) { auto ret = *this; ret.normalize( ); return ret; } \
        __forceinline bool zero( ) { return ( x > -0.01f && x < 0.01f && y > -0.01f && y < 0.01f && z > -0.01f && z < 0.01f ); }

#if __clang__ && __cplusplus >= 201907l /* don't ask */
#define __cpp_lib_format /* fck thhisshit the dependencies ofthis lib arefuckinghuge */
#endif

#include <windows.h>
#include <intrin.h>

#include <algorithm>
#include <thread>
#include <array>
#include <deque>
#include <unordered_map>
#include <format>
#include <mutex>
#include <filesystem>
#include <type_traits>
#include <fstream>

#include <d3d9.h>

#include ".hpp/dependencies/xorstr/xorstr.hpp"

#define ALWAYS_INLINE __forceinline
#define ENCSTR( str ) xorstr_( str )
#define SDK_CFG_ID_OBJECT "untitled"

#define JSON_NOEXCEPTION

#include ".hpp/dependencies/json/json.hpp"
#include ".hpp/dependencies/cfg/cfg.hpp"

#include ".hpp/dependencies/minhook/minhook.h"

#include ".hpp/dependencies/imgui/imgui.h"
#include ".hpp/dependencies/imgui/imgui_internal.h"
#include ".hpp/dependencies/imgui/imgui_impl_dx9.h"
#include ".hpp/dependencies/imgui/imgui_impl_win32.h"

#include ".hpp/math/math.hpp"
#include ".hpp/util/util.hpp"

#include ".hpp/ctx/ctx.hpp"
#include ".hpp/valve/valve.hpp"

#include ".hpp/hacks/hacks.hpp"
#include ".hpp/hooks/hooks.hpp"