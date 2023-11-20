#pragma once

#include "../../common.hpp"

class c_memory {
public:
	// @note: used only when manually given addresses.
	std::uintptr_t module = reinterpret_cast< std::uintptr_t >( GetModuleHandleA( nullptr ) );

	// @note: PE32: 0x400000, PE64: 0x140000000.
	std::uintptr_t rebase( std::uintptr_t address ) {
		return address - 0x140000000 + module;
	}

	std::uintptr_t key_valid = rebase( 0x1423FA8E4 );

	template <typename T>
	T read_memory( uintptr_t address ) {
		return *reinterpret_cast< T* >( address );
	}

	template <typename T>
	void write_memory( uintptr_t address, T data ) {
		*reinterpret_cast< T* >( address ) = data;
	}

	using RenderTextHooked = std::uintptr_t( __fastcall* )( int, int, int, int, float, const char* );
	RenderTextHooked RenderText = reinterpret_cast< RenderTextHooked >( rebase( 0x14009AFD0 ) );
};

inline const auto g_memory = std::make_unique<c_memory>( );