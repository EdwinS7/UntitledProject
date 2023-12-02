#pragma once

#include "../../Common.hpp"

namespace Hooking {
	// Main
	int Init( );

	// MinHook
	int MH_Init( );
	int MH_Hook( void* Address, void* Detour, void** Original );
	int MH_Enable( );

	// Function hooking
	template <typename hook_type>
	void CreateHook( hook_type* Hook, const std::string& Signature );

	template<typename hook_type>
	void CreateHook( hook_type* Hook, const uintptr_t& Offset );

	// Hooks
	namespace Hooks {
		int __fastcall IsKeyValid( char* key );
		inline decltype( &IsKeyValid ) IsKeyValidOriginal{};

		class cSurface {
		public:
			using TextDefinition = std::uintptr_t( __fastcall* )( __int64 a1, __int64 a2, __int64 a3, __int64 a4, const char* a5 );
			TextDefinition Text;
		};

		inline const auto Surface = std::make_unique<cSurface>( );
	}
}