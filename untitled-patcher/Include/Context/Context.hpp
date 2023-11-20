#pragma once

#include "../Common.hpp"

class CContext {
public:
	void Init( );
	
private:
	void FixImports( std::uintptr_t RVA );
	void FixRelocations( std::uintptr_t Cheat, std::uintptr_t Stub );

	void AllocateConsole( );

};

inline const auto Context = std::make_unique<CContext>( );