#pragma once
#include "../../inc.hpp"

class c_offsets {
public:
	std::uintptr_t print = 0x118A5C0; // @ida xref: "Current identity is %d" the function call within.
	std::uintptr_t print_identity = 0x89EDE0; // @ida xref: "Current identity is %d" the function itself.
	std::uintptr_t get_scheduler = 0xBD15B0; // @ida xref: [Watchdog or LuauWatchdog], after second jnz get call sub_xxxxxx.
	std::uintptr_t task_defer = 0x980070; // @ida xref: "Maximum re-entrancy depth (%i) exceeded calling task.defer", the function within.
	std::uintptr_t get_state = 0x87F1C0; // @ida xref: "challenge" sub_7.
	std::uintptr_t lua_vm_load = 0x980FE0; // @ida xref: "oldResult, moduleRef  = ..." 3rd sub function downwards.

	std::uintptr_t top = 20;
	std::uintptr_t base = 8;
};

inline const auto g_offsets = std::make_unique<c_offsets>( );

class c_hooks {
public:
	std::uintptr_t base = reinterpret_cast< std::uintptr_t >( GetModuleHandleA( nullptr ) );

	std::uintptr_t ASLR( std::uintptr_t address ) {
		return address - 0x400000 + base;
	}

	using print_hooked = std::uintptr_t( __cdecl* )( int, const char*, ... );
	print_hooked print = reinterpret_cast< print_hooked >( ASLR( g_offsets->print ) );

	using print_identity_hooked = std::uintptr_t( __cdecl* )( std::uintptr_t );
	print_identity_hooked print_identity = reinterpret_cast< print_identity_hooked >( ASLR( g_offsets->print_identity ) );

	using get_scheduler_hooked = std::uintptr_t( __cdecl* ) ( );
	get_scheduler_hooked get_scheduler = reinterpret_cast< get_scheduler_hooked >( ASLR( g_offsets->get_scheduler ) );

	using task_defer_hooked = std::uintptr_t( __cdecl* )( std::uintptr_t );
	task_defer_hooked task_defer = reinterpret_cast< task_defer_hooked >( ASLR( g_offsets->task_defer ) );

	using get_state_hooked = std::uintptr_t( __thiscall* )( std::uintptr_t, int*, int* );
	get_state_hooked get_state = reinterpret_cast< get_state_hooked >( ASLR( g_offsets->get_state ) );

	using lua_vm_load_hooked = std::uintptr_t( __fastcall* )( std::uintptr_t, std::string*, const char*, int );
	lua_vm_load_hooked lua_vm_load = reinterpret_cast< lua_vm_load_hooked >( ASLR( g_offsets->lua_vm_load ) );

	using r_lua_setfield_t = uintptr_t( __stdcall* )( uintptr_t, int, const char* );
	r_lua_setfield_t r_lua_setfield = reinterpret_cast< r_lua_setfield_t >( ASLR( 0x18AC6A0 ) );

	using r_lua_pushcclosure_t = uintptr_t( __stdcall* )( uintptr_t, int, int, int, int );
	r_lua_pushcclosure_t r_lua_pushcclosure = reinterpret_cast< r_lua_pushcclosure_t >( ASLR( 0x18AB690 ) );
};

inline const auto g_hooks = std::make_unique<c_hooks>( );