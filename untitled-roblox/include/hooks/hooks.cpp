#include "hooks.hpp"

void c_offsets::init( ) {
	// @ida xref: "Current identity is %d" the function call within.
	g_hooks->print = reinterpret_cast< c_hooks::print_hooked >(
		g_mem->scan(
			xorstr_( "Windows10Universal.exe" ),
			xorstr_( "55 8B EC 6A FF 68 ?? ?? ?? ?? 64 A1 00 00 00 00 50 83 EC 20 A1 ?? ?? ?? ?? 33 C5 89 45 F0 50 8D 45 F4 64 A3 00 00 00 00 8B" )
		)
	);

	// @ida xref: "Current identity is %d" the function itself.
	g_hooks->print_identity = reinterpret_cast< c_hooks::print_identity_hooked >(
		g_mem->scan(
			xorstr_( "Windows10Universal.exe" ),
			xorstr_( "55 8B EC 83 E4 F0 83 EC 38 A1 ?? ?? ?? ?? 33 C4 89 44 24 34 80 3D" )
		)
	);

	// @ida xref: [Watchdog or LuauWatchdog], after second jnz its the sub function.
	g_hooks->get_scheduler = reinterpret_cast< c_hooks::get_scheduler_hooked >(
		g_mem->scan(
			xorstr_( "Windows10Universal.exe" ),
			xorstr_( "55 8B EC 6A FF 68 ?? ?? ?? ?? 64 A1 00 00 00 00 50 83 EC 14 56 A1 ?? ?? ?? ?? 33 C5 50 8D 45 F4 64 A3 00 00 00 00 64 A1 2C 00 00 00 8B 08 A1 ?? ?? ?? ?? 3B 81 20 00 00 00 7F 2B A1 ?? ?? ?? ?? 8B 4D F4 64 89 0D 00 00 00 00 59 5E 8B E5 5D C3 8D 4D E4 E8 ?? ?? ?? ?? 68 ?? ?? ?? ?? 8D 45 E4 50 E8 ?? ?? ?? ?? 68 ?? ?? ?? ?? E8 ?? ?? ?? ?? 83 C4 04 83 3D ?? ?? ?? ?? ?? 75 BF 68 60" )
		)
	);

	// @ida xref: "defer", the code section.
	g_hooks->task_spawn = reinterpret_cast< c_hooks::task_spawn_hooked >(
		g_mem->scan(
			xorstr_( "Windows10Universal.exe" ),
			xorstr_( "55 8B EC 6A FF 68 ?? ?? ?? ?? 64 A1 00 00 00 00 50 83 EC 30 A1 ?? ?? ?? ?? 33 C5 89 45 EC 53 56 57 50 8D 45 F4 64 A3 00 00 00 00 8B 7D 08 C7 45 E8 00 00 00 00" )
		)
	);

	// @ida xref: "debug" second sub routine upwards ( use match case ), its right under that.
	g_hooks->get_state = reinterpret_cast< c_hooks::get_state_hooked >(
		g_mem->scan(
			xorstr_( "Windows10Universal.exe" ),
			xorstr_( "55 8B EC 6A FF 68 ?? ?? ?? ?? 64 A1 00 00 00 00 50 83 EC 1C A1 ?? ?? ?? ?? 33 C5 89 45 F0 53 56 57 50 8D 45 F4 64 A3 00 00 00 00 8B F9 8B 45 0C 8B 18" )
		)
	);

	// @ida xref: "oldResult, moduleRef  = ..." last sub function called.
	g_hooks->lua_vm_load = reinterpret_cast< c_hooks::lua_vm_load_hooked >(
		g_mem->scan(
			xorstr_( "Windows10Universal.exe" ),
			xorstr_( "53 8B DC 83 EC 08 83 E4 F8 83 C4 04 55 8B 6B 04 89 6C 24 04 8B EC 6A FF 68 ?? ?? ?? ?? 64 A1 00 00 00 00 50 51 53 81 EC D0 07 00 00" )
		)
	);

	g_hooks->print( print_level::info, xorstr_( "found addresses!" ) );
}