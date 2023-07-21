#pragma once

class c_util {
public:
	template< typename T >
	ALWAYS_INLINE T get_method( void* thisptr, uintptr_t idx );

	ALWAYS_INLINE uintptr_t absolute( uintptr_t ptr );

	ALWAYS_INLINE std::string to_utf8( const std::wstring_view str );

	ALWAYS_INLINE std::wstring to_utf16( const std::string_view str );

	ALWAYS_INLINE constexpr std::uint32_t hash( const char* str );
};

inline const auto g_util = std::make_unique< c_util >( );

#include "impl/util.inl"