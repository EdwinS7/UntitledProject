#pragma once

#include "../Common.hpp"

class CMemory {
public:
	void SetupHooks( );

private:
	std::uintptr_t Module = reinterpret_cast< std::uintptr_t >( GetModuleHandleA( nullptr ) );

	template <typename hook_type>
	void CreateHook( hook_type* Hook, const std::string& Signature );

	template<typename hook_type>
	void CreateHook( hook_type* Hook, const uintptr_t& Offset );

	INLINE std::uintptr_t Scan( const std::string& signature );
	INLINE std::uintptr_t Rebase( std::uintptr_t Address );

	using PrintDecl = std::uintptr_t( __cdecl* )( int, const char*, ... );
	PrintDecl PrintDeclare;
};

inline const auto Memory = std::make_unique<CMemory>( );