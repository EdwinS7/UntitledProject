#pragma once

#include "../../Common.hpp"

class cMemory {
public:
	std::uintptr_t BaseAddress = reinterpret_cast< std::uintptr_t >( GetModuleHandleA( nullptr ) );

	// Signatures & Offsets.
	std::uintptr_t Scan( const std::string& Signature );
	std::uintptr_t Rebase( std::uintptr_t Address );
};

inline const auto Memory = std::make_unique<cMemory>( );