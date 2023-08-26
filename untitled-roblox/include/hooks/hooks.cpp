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
	create_hook( &g_hooks->print, 0x118A5C0 );

	// @ida xref: "Current identity is %d" the function itself.
	create_hook( &g_hooks->print_identity, 0x89EDE0 );

	// @ida xref: [Watchdog or LuauWatchdog], after second jnz its the sub function.
	create_hook( &g_hooks->get_scheduler, 0xBD15B0 );

	// @ida xref: "defer", the function itself.
	create_hook( &g_hooks->task_defer, 0x980070 );

	// @ida xref: "spawn", the function itself.
	create_hook( &g_hooks->task_spawn, 0x980270 );

	// @ida xref: "debug" second sub routine upwards ( use match case ), its right under that.
	create_hook( &g_hooks->get_state, 0x87F1C0 );

	// @ida xref: "oldResult, moduleRef  = ..." last sub function called.
	create_hook( &g_hooks->lua_vm_load, 0x980FE0 );

	// @ida xref: "tables cannot be cyclic" decompile then looks for the next unk_xxxxxxx, done.
	create_hook( &g_hooks->lua_nil_object, 0x2D05080 );

	// @ida xref: "{\"type\":\"table\",\"cat\":%d,\"size\":%d" its the first dword being utilized after the sub function. its right above lua_nil_object.
	create_hook( &g_hooks->lua_dummy_node, 0x2D05060 );

	// @ida xref: "assertion failed!" its the first unk_xxxxxxx used.
	create_hook( &g_hooks->pseudo2, 0x1813A10 );

	// @ida xref: ??? also wrong.
	create_hook( &g_hooks->pushkclosure, 0x18A11E0 );
}