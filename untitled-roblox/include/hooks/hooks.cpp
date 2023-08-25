#include "hooks.hpp"

template <typename hook_type>
void c_offsets::create_hook( hook_type* hook, const std::string& signature ) {
	*hook = reinterpret_cast< hook_type >(
		g_mem->scan( signature )
	);
}

template <typename hook_type>
void c_offsets::create_hook( hook_type* hook, const uintptr_t& offset ) {
	*hook = reinterpret_cast< hook_type >(
		g_hooks->aslr( offset )
	);
}

void c_offsets::init( ) {
	// @ida xref: "Current identity is %d" the function call within.
	create_hook( &g_hooks->print, xorstr_( "55 8B EC 6A FF 68 ?? ?? ?? ?? 64 A1 00 00 00 00 50 83 EC 20 A1 ?? ?? ?? ?? 33 C5 89 45 F0 50 8D 45 F4 64 A3 00 00 00 00 8B" ) );

	// @ida xref: "Current identity is %d" the function itself.
	create_hook( &g_hooks->print_identity, xorstr_( "55 8B EC 83 E4 F0 83 EC 38 A1 ?? ?? ?? ?? 33 C4 89 44 24 34 80 3D" ) );

	// @ida xref: [Watchdog or LuauWatchdog], after second jnz its the sub function.
	create_hook( &g_hooks->get_scheduler, xorstr_( "55 8B EC 6A FF 68 ?? ?? ?? ?? 64 A1 00 00 00 00 50 83 EC 14 56 A1 ?? ?? ?? ?? 33 C5 50 8D 45 F4 64 A3 00 00 00 00 64 A1 2C 00 00 00 8B 08 A1 ?? ?? ?? ?? 3B 81 20 00 00 00 7F 2B A1 ?? ?? ?? ?? 8B 4D F4 64 89 0D 00 00 00 00 59 5E 8B E5 5D C3 8D 4D E4 E8 ?? ?? ?? ?? 68 ?? ?? ?? ?? 8D 45 E4 50 E8 ?? ?? ?? ?? 68 ?? ?? ?? ?? E8 ?? ?? ?? ?? 83 C4 04 83 3D ?? ?? ?? ?? ?? 75 BF 68 60" ) );

	// @ida xref: "defer", the code section.
	create_hook( &g_hooks->task_spawn, xorstr_( "55 8B EC 6A FF 68 ?? ?? ?? ?? 64 A1 00 00 00 00 50 83 EC 30 A1 ?? ?? ?? ?? 33 C5 89 45 EC 53 56 57 50 8D 45 F4 64 A3 00 00 00 00 8B 7D 08 C7 45 E8 00 00 00 00" ) );

	// @ida xref: "debug" second sub routine upwards ( use match case ), its right under that.
	create_hook( &g_hooks->get_state, xorstr_( "55 8B EC 6A FF 68 ?? ?? ?? ?? 64 A1 00 00 00 00 50 83 EC 1C A1 ?? ?? ?? ?? 33 C5 89 45 F0 53 56 57 50 8D 45 F4 64 A3 00 00 00 00 8B F9 8B 45 0C 8B 18" ) );

	// @ida xref: "oldResult, moduleRef  = ..." last sub function called.
	create_hook( &g_hooks->lua_vm_load, xorstr_( "53 8B DC 83 EC 08 83 E4 F8 83 C4 04 55 8B 6B 04 89 6C 24 04 8B EC 6A FF 68 ?? ?? ?? ?? 64 A1 00 00 00 00 50 51 53 81 EC D0 07 00 00" ) );

	// @ida xref: "tables cannot be cyclic" decompile then looks for the next unk_xxxxxxx, done.
	create_hook( &g_hooks->lua_nil_object, 0x2D05080 );

	// @ida xref: "{\"type\":\"table\",\"cat\":%d,\"size\":%d" its the first dword being utilized after the sub function. its right above lua_nil_object.
	create_hook( &g_hooks->lua_dummy_node, 0x2D05060 );

	// @ida xref: "assertion failed!" its the first unk_xxxxxxx used.
	create_hook( &g_hooks->pseudo2, xorstr_( "8B C1 81 FA EE D8 FF FF" ) );

	g_hooks->print( print_level::info, xorstr_( "found addresses!" ) );
}