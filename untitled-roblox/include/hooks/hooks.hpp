#pragma once
#include "../../inc.hpp"

class c_offsets {
public:
	void init( );
	
	// @note: requires manual update atm.
	std::uintptr_t top = 20;
	std::uintptr_t base = 8;
};

inline const auto g_offsets = std::make_unique<c_offsets>( );

class c_hooks {
public:

	// @note: used only when manually given addresses.
	std::uintptr_t base = reinterpret_cast< std::uintptr_t >( GetModuleHandleA( nullptr ) );

	std::uintptr_t ASLR( std::uintptr_t address ) {
		return address - 0x400000 + base;
	}

	using print_hooked = std::uintptr_t( __cdecl* )( int, const char*, ... );
	print_hooked print;

	using print_identity_hooked = std::uintptr_t( __cdecl* )( std::uintptr_t );
	print_identity_hooked print_identity;

	using get_scheduler_hooked = std::uintptr_t( __cdecl* ) ( );
	get_scheduler_hooked get_scheduler;

	using task_spawn_hooked = std::uintptr_t( __cdecl* )( std::uintptr_t );
	task_spawn_hooked task_spawn;

	using get_state_hooked = std::uintptr_t( __thiscall* )( std::uintptr_t, int*, int* );
	get_state_hooked get_state;

	using lua_vm_load_hooked = std::uintptr_t( __fastcall* )( std::uintptr_t, std::string*, const char*, int );
	lua_vm_load_hooked lua_vm_load;

	using r_lua_setfield_t = uintptr_t( __stdcall* )( uintptr_t, int, const char* );
	r_lua_setfield_t r_lua_setfield;

	using r_lua_pushcclosure_t = uintptr_t( __stdcall* )( uintptr_t, int, int, int, int );
	r_lua_pushcclosure_t r_lua_pushcclosure;
};

inline const auto g_hooks = std::make_unique<c_hooks>( );