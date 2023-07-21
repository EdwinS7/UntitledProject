#pragma once

#include "../util.hpp"

template< typename T >
ALWAYS_INLINE T c_util::get_method( void* thisptr, uintptr_t idx ) {
	return reinterpret_cast< T >( ( *static_cast< uintptr_t** >( thisptr ) )[ idx ] );
}

ALWAYS_INLINE uintptr_t c_util::absolute( uintptr_t ptr ) {
	return ptr + *( PDWORD ) ptr + 0x4;
}

ALWAYS_INLINE std::string c_util::to_utf8( const std::wstring_view str ) {
	if ( str.empty( ) )
		return {};

	const auto len = WideCharToMultiByte( CP_UTF8, 0, str.data( ), str.size( ), 0, 0, 0, 0 );

	std::string ret{};

	ret.resize( len );

	WideCharToMultiByte( CP_UTF8, 0, str.data( ), str.size( ), ret.data( ), len, 0, 0 );

	return ret;
}

ALWAYS_INLINE std::wstring c_util::to_utf16( const std::string_view str ) {
	if ( str.empty( ) )
		return {};

	const auto len = MultiByteToWideChar( CP_UTF8, 0, str.data( ), str.size( ), nullptr, 0 );

	std::wstring ret{};

	ret.resize( len );

	MultiByteToWideChar( CP_UTF8, 0, str.data( ), str.size( ), ret.data( ), len );

	return ret;
}

ALWAYS_INLINE constexpr std::uint32_t c_util::hash( const char* str ) {
	auto hash = 0x811c9dc5u;

	char v4{};
	do {
		v4 = *str++;
		hash = 0x1000193u * ( hash ^ v4 );
	} while ( v4 );

	return hash;
}