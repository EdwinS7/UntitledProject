#pragma once
#include "../../inc.hpp"

class c_offsets {
public:
	template <typename hook_type>
	void create_hook( hook_type* hook, const std::string& signature );

	template<typename hook_type>
	void create_hook( hook_type* hook, const uintptr_t& offset );

	void init( );
	
	// @example: v12 = ( *( _DWORD *) ( a2 + 20 /*top*/ ) - *( _DWORD *) ( a2 + 8 /*base*/ ) ) >> 4;
	// @ida xref: "Argument 2 missing or nil", generate pseudo code and look for a2 + ... both can be found in the same line.
	std::uintptr_t top = 20;
	std::uintptr_t base = 8;
};

inline const auto g_offsets = std::make_unique<c_offsets>( );

class c_hooks {
public:

	// @note: used only when manually given addresses.
	std::uintptr_t base = reinterpret_cast< std::uintptr_t >( GetModuleHandleA( nullptr ) );

	std::uintptr_t aslr( std::uintptr_t address ) {
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

	using lua_nil_object_hooked = std::uintptr_t( __fastcall* )( );
	lua_nil_object_hooked lua_nil_object;

	using lua_dummy_node_hooked = std::uintptr_t( __fastcall* )( );
	lua_dummy_node_hooked lua_dummy_node;

	using pseudo2_hooked = std::uintptr_t( __fastcall* )( std::uintptr_t, int );
	pseudo2_hooked pseudo2;
};

inline const auto g_hooks = std::make_unique<c_hooks>( );