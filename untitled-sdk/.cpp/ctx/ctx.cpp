#include "../../.hpp/ctx/ctx.hpp"

void c_ctx::init( ) {
#ifdef _DEBUG
	if ( !AllocConsole( ) )
		return;

	FILE* file{};
	if ( freopen_s( &file, "CONOUT$", "w", stdout ) )
		return;

	SetConsoleTitle( "untitled sdk" );

	std::printf( "waiting for serverbrowser.dll...\n" );
#endif

	while ( !GetModuleHandle( xorstr_( "serverbrowser.dll" ) ) )
		std::this_thread::sleep_for( std::chrono::milliseconds{ 200 } );

#ifdef _DEBUG
	std::printf( "initializing minhook...\n" );
#endif

	if ( MH_Initialize( ) != MH_OK )
		return;

#ifdef _DEBUG
	std::printf( "retreiving device context...\n" );
#endif

	const auto device = **reinterpret_cast< IDirect3DDevice9*** >(
		g_mem->find_pattern( GetModuleHandle( xorstr_( "shaderapidx9.dll" ) ), xorstr_( "A1 ? ? ? ? 50 8B 08 FF 51 0C" ) ) + 1u
	);

#ifdef _DEBUG
	std::printf( "initializing imgui...\n" );
#endif

	D3DDEVICE_CREATION_PARAMETERS creation_params{};
	if ( device->GetCreationParameters( &creation_params ) != D3D_OK )
		return;

	ImGui::CreateContext( );
	ImGui::StyleColorsDark( );

	ImGui::GetStyle( ).WindowMinSize = { 600.f, 440.f };
	ImGui::GetStyle( ).WindowRounding = 5.f;
	ImGui::GetStyle( ).FrameRounding = 5.f;
	ImGui::GetStyle( ).ChildRounding = 3.f;
	ImGui::GetStyle( ).PopupRounding = 3.f;
	ImGui::GetStyle( ).GrabRounding = 3.f;
	ImGui::GetStyle( ).ScrollbarRounding = 3.f;
	ImGui::GetStyle( ).TabRounding = 3.f;

	auto& imgui_io = ImGui::GetIO( );

	imgui_io.IniFilename = imgui_io.LogFilename = nullptr;

	ImGui_ImplWin32_Init( creation_params.hFocusWindow );
	ImGui_ImplDX9_Init( device );

	hooks::original_wnd_proc = reinterpret_cast< WNDPROC >(
		SetWindowLong(
			creation_params.hFocusWindow, GWL_WNDPROC,
			reinterpret_cast< LONG >( hooks::wnd_proc )
		)
	);

#ifdef _DEBUG
	std::printf( "getting code sections...\n" );
#endif

	const auto client = code_section_t{ GetModuleHandle( xorstr_( "client.dll" ) ) };
	const auto engine = code_section_t{ GetModuleHandle( xorstr_( "engine.dll" ) ) };
	const auto valve_gui = code_section_t{ GetModuleHandle( xorstr_( "vguimatsurface.dll" ) ) };

#ifdef _DEBUG
	std::printf( "initializing interfaces...\n" );
#endif

	valve::g_local_player = *reinterpret_cast< valve::c_local_player* >(
			g_mem->find_pattern( client, xorstr_( "8B 0D ? ? ? ? 83 FF FF 74 07" ) ) + 0x2u
		);

	valve::g_client = *reinterpret_cast< valve::c_client** >(
		g_mem->find_pattern( client, xorstr_( "A1 ? ? ? ? B9 ? ? ? ? FF 90 ? ? ? ? 8B 35 ? ? ? ?" ) ) + 0x1u
	);

	valve::g_global_vars = **reinterpret_cast< valve::global_vars_t*** >(
		( *reinterpret_cast< std::uintptr_t** >( valve::g_client ) )[ 11u ] + 0xau
	);

	valve::g_engine = **reinterpret_cast< valve::c_engine*** >(
		g_mem->find_pattern( client, xorstr_( "8B 0D ? ? ? ? 8B 01 8B 40 38 FF" ) ) + 0x2u
	);

	valve::g_entity_list = **reinterpret_cast< valve::c_entity_list*** >(
		g_mem->find_pattern( engine, xorstr_( "8B 0D ? ? ? ? 88 45 FC 6A 00" ) ) + 0x2u
	);

	valve::g_client_state = **reinterpret_cast< valve::client_state_t*** >(
		g_mem->find_pattern( engine, xorstr_( "A1 ? ? ? ? 8B 88 ? ? ? ? 85 C9 75 07" ) ) + 0x1u
	);

	valve::g_move_helper = **reinterpret_cast< valve::c_move_helper*** >(
		g_mem->find_pattern( client, xorstr_( "8B 0D ? ? ? ? 8B 45 ? 51 8B D4 89 02 8B 01" ) ) + 0x2u
	);

	valve::g_prediction = *reinterpret_cast< valve::prediction_t** >(
		g_mem->find_pattern( client, xorstr_( "A1 ? ? ? ? 8B 40 38 FF D0 84 C0" ) ) + 0x1u
	);

	valve::g_movement = *reinterpret_cast< valve::c_movement** >(
		g_mem->find_pattern( client, xorstr_( "A1 ? ? ? ? B9 ? ? ? ? FF 50 04 EB 07" ) ) + 0x1u
	);

	valve::g_cvar = **reinterpret_cast< valve::c_cvar*** >(
		g_mem->find_pattern( client, xorstr_( "8B 0D ? ? ? ? 68 ? ? ? ? 8B 01 FF 50 44 8B C8 89" ) ) + 0x2u
	);

	valve::g_engine_trace = **reinterpret_cast< valve::c_engine_trace*** >(
		g_mem->find_pattern( client, xorstr_( "A1 ? ? ? ? F3 0F 11 45 ? 8B 30" ) ) + 0x1u
	);

	valve::g_surface_data = **reinterpret_cast< valve::c_surface_data*** >(
		g_mem->find_pattern( client, xorstr_( "8B 0D ? ? ? ? 0F BF 45 C8" ) ) + 0x2u
	);

	valve::g_input = *reinterpret_cast< valve::input_t** >(
		g_mem->find_pattern( client, xorstr_( "B9 ? ? ? ? FF 75 08 FF 50 0C" ) ) + 0x1u
	);

	valve::g_game_event_mgr = **reinterpret_cast< valve::c_game_event_mgr*** >(
		g_mem->find_pattern( client, xorstr_( "8B 0D ? ? ? ? 50 8B 11 FF 52 24 FF 15 ? ? ? ?" ) ) + 0x2u
	);

	valve::g_studio_render = **reinterpret_cast< valve::studio_render_t*** >(
		g_mem->find_pattern( client, xorstr_( "8B 0D ? ? ? ? 8D 54 24 40 52 8B 01 FF 50 34" ) ) + 0x2u
	);

	valve::g_mat_system = **reinterpret_cast< valve::c_mat_system*** >(
		g_mem->find_pattern( client, xorstr_( "8B 0D ?? ?? ?? ?? 8B 01 8B 80 ?? ?? ?? ?? FF D0 8B F0 89 74 24 48" ) ) + 0x2u
	);

	valve::g_view_render = **reinterpret_cast< valve::view_render_t*** >(
		g_mem->find_pattern( client, xorstr_( "8B 0D ? ? ? ? 8B 01 FF 50 4C 8B 06" ) ) + 0x2u
	);

	valve::g_surface = **reinterpret_cast< valve::c_surface*** >(
		g_mem->find_pattern( client, xorstr_( "8B 0D ? ? ? ? FF B7 ? ? ? ? 8B 01 FF 90 ? ? ? ? 8B 7C 24 1C" ) ) + 0x2u
	);

	/* idc */
	valve::g_key_values_system = reinterpret_cast< valve::c_key_values_system * ( __cdecl* )( ) >(
		GetProcAddress( GetModuleHandleA( xorstr_( "vstdlib.dll" ) ), xorstr_( "KeyValuesSystem" ) )
	)( );

	valve::g_localize = reinterpret_cast< valve::c_localize * ( __cdecl* )( const char*, int* ) >(
		GetProcAddress( GetModuleHandle( xorstr_( "localize.dll" ) ), xorstr_( "CreateInterface" ) )
	)( xorstr_( "Localize_001" ), nullptr );

	valve::g_font_mgr = *reinterpret_cast< valve::c_font_mgr** >(
		g_mem->find_pattern( valve_gui, xorstr_( "74 1D 8B 0D ? ? ? ? 68 ? ? ? ?" ) ) + 0x4u
	);

	valve::g_model_info = *reinterpret_cast< valve::c_model_info** >(
		g_mem->find_pattern( engine, xorstr_( "B9 ? ? ? ? 8B 44 B8 04 83 C0 04 50 A1 ? ? ? ? FF 50 08 8B D0 8B 4C BB 0C" ) ) + 0x1u
	);

	valve::g_beams = *reinterpret_cast< valve::c_beams** >(
		g_mem->find_pattern( client, xorstr_( "B9 ? ? ? ? A1 ? ? ? ? FF 10 A1 ? ? ? ? B9" ) ) + 0x1u
	);

	valve::g_model_render = reinterpret_cast< valve::c_model_render * ( __cdecl* )( const char*, int* ) >(
		GetProcAddress( GetModuleHandle( xorstr_( "engine.dll" ) ), xorstr_( "CreateInterface" ) )
	)( xorstr_( "VEngineModel016" ), nullptr );

	valve::g_mdl_cache = reinterpret_cast< valve::c_mdl_cache * ( __cdecl* )( const char*, int* ) >(
		GetProcAddress( GetModuleHandle( xorstr_( "datacache.dll" ) ), xorstr_( "CreateInterface" ) )
	)( xorstr_( "MDLCache004" ), nullptr );

	valve::g_glow_mgr = *reinterpret_cast< valve::glow_mgr_t** >(
		g_mem->find_pattern( client, xorstr_( "0F 11 05 ? ? ? ? 83 C8 01" ) ) + 0x3u
	);

	valve::g_debug_overlay = reinterpret_cast< valve::c_debug_overlay * ( __cdecl* )( const char*, int* ) >(
		GetProcAddress( GetModuleHandle( xorstr_( "engine.dll" ) ), xorstr_( "CreateInterface" ) )
	)( xorstr_( "VDebugOverlay004" ), nullptr );
#ifdef _DEBUG
	std::printf( "parsing client classes...\n" );
#endif
	valve::g_net_vars->parse_client_classes( );

#ifdef _DEBUG
	std::printf( "parsing data_maps...\n" );
#endif

	{
		std::vector< valve::data_map_t* > data_maps{};

		auto start = client.m_base;
		auto size = client.m_size;

		std::uintptr_t addr{};
		for ( ; ; ) {
			addr = g_mem->find_pattern( { size, start }, xorstr_( "CC CC CC CC C7 05 ? ? ? ? ? ? ? ? C7 05 ? ? ? ? ? ? ? ? C3 CC" ) );
			if ( addr == ( start + size ) )
				break;

			const auto data_map = reinterpret_cast< valve::data_map_t* >(
				*reinterpret_cast< std::uintptr_t* >( addr + 0x6u ) - 0x4u
				);
			if ( data_map
				&& data_map->m_size
				&& data_map->m_size <= 200
				&& data_map->m_description
				&& data_map->m_name )
				data_maps.emplace_back( data_map );

			size = ( start + size ) - ( addr + 1u );

			start = addr + 1u;
		}

		if ( data_maps.empty( ) )
			return;

		valve::g_net_vars->parse_data_maps( data_maps );
	}

#ifdef _DEBUG
	std::printf( "initializing addresses...\n" );
#endif

	const auto vstdlib_dll = GetModuleHandle( xorstr_( "vstdlib.dll" ) );

	m_addresses.m_random_float = reinterpret_cast< addresses_t::random_float_t >( GetProcAddress( vstdlib_dll, xorstr_( "RandomFloat" ) ) );
	m_addresses.m_random_int = reinterpret_cast< addresses_t::random_int_t >( GetProcAddress( vstdlib_dll, xorstr_( "RandomInt" ) ) );
	m_addresses.m_random_seed = reinterpret_cast< addresses_t::random_seed_t >( GetProcAddress( vstdlib_dll, xorstr_( "RandomSeed" ) ) );

	m_addresses.m_reset_anim_state = g_mem->find_pattern( client, xorstr_( "56 6A 01 68 ? ? ? ? 8B F1" ) );
	m_addresses.m_update_anim_state = g_mem->find_pattern(
		client, xorstr_( "55 8B EC 83 E4 F8 83 EC 18 56 57 8B F9 F3 0F 11 54 24" )
	);
	m_addresses.m_set_abs_angles = g_mem->find_pattern( client, xorstr_( "55 8B EC 83 E4 F8 83 EC 64 53 56 57 8B F1" ) );
	m_addresses.m_set_abs_origin = g_mem->find_pattern( client, xorstr_( "55 8B EC 83 E4 F8 51 53 56 57 8B F1 E8 ? ? ? ? 8B 7D" ) );
	m_addresses.m_lookup_seq_act = g_mem->find_pattern( client, xorstr_( "55 8B EC 53 8B 5D 08 56 8B F1 83" ) );
	m_addresses.m_breakable = g_mem->find_pattern( client, xorstr_( "55 8B EC 51 56 8B F1 85 F6 74 68 83 BE" ) );
	m_addresses.m_user_cmd_calc_checksum = g_mem->find_pattern( client, xorstr_( "53 8B D9 83 C8" ) );

	m_addresses.m_pred_seed = *reinterpret_cast< int** >(
		g_mem->find_pattern( client, xorstr_( "A3 ? ? ? ? 66 0F 6E 86" ) ) + 0x1u
	);

	m_addresses.m_pred_player = *reinterpret_cast< valve::c_player** >(
		g_mem->find_pattern( client, xorstr_( "89 35 ? ? ? ? F3 0F 10 48" ) ) + 0x2u
	);

	const auto angle_matrix_rel = g_mem->find_pattern( client, xorstr_( "E8 ? ? ? ? 8B 07 89 46 0C" ) );
	m_addresses.m_angle_matrix = reinterpret_cast< addresses_t::angle_matrix_t >(
		angle_matrix_rel + 0x1u + sizeof( std::uintptr_t ) + *reinterpret_cast< std::ptrdiff_t* >( angle_matrix_rel + 0x1u )
	);

	m_addresses.m_set_collision_bounds = g_mem->find_pattern(
		client, xorstr_( "53 8B DC 83 EC 08 83 E4 F8 83 C4 04 55 8B 6B 04 89 6C 24 04 8B EC 83 EC 18 56 57 8B 7B 08 8B D1 8B 4B 0C" )
	);

	m_addresses.m_invalidate_bone_cache = g_mem->find_pattern( client, xorstr_( "80 3D ? ? ? ? ? 74 16 A1 ? ? ? ? 48 C7 81" ) );
	m_addresses.m_lookup_bone = g_mem->find_pattern( client, xorstr_( "55 8B EC 53 56 8B F1 57 83 BE ?? ?? ?? ?? ?? 75 14 8B 46 04 8D 4E 04 FF 50 20" ) );
	m_addresses.m_trace_filter_simple_vtable = *reinterpret_cast< std::uintptr_t* >(
		g_mem->find_pattern( client, xorstr_( "55 8B EC 83 E4 F0 83 EC 7C 56 52" ) ) + 0x3du
	);

	m_addresses.m_trace_filter_skip_two_entities_vtable = *reinterpret_cast< std::uintptr_t* >(
		g_mem->find_pattern( client, xorstr_( "C7 45 ? ? ? ? ? 89 45 E4 8B 01" ) ) + 0x3u
	);

	m_addresses.m_write_user_cmd = g_mem->find_pattern( client, xorstr_( "55 8B EC 83 E4 F8 51 53 56 8B D9 8B 0D" ) );
	m_addresses.m_weapon_system = *reinterpret_cast< std::uintptr_t* >(
		g_mem->find_pattern( client, xorstr_( "8B 35 ? ? ? ? FF 10 0F B7 C0" ) ) + 0x2u
	);

	const auto calc_shotgun_spread_rel = g_mem->find_pattern( client, xorstr_( "E8 ? ? ? ? EB 38 83 EC 08" ) );

	m_addresses.m_calc_shotgun_spread =
		calc_shotgun_spread_rel + 0x1u + sizeof( std::uintptr_t )
		+ *reinterpret_cast< std::ptrdiff_t* >( calc_shotgun_spread_rel + 0x1u );

	const auto alloc_key_values_engine_rel = g_mem->find_pattern( engine, xorstr_( "E8 ? ? ? ? 83 C4 08 84 C0 75 10 FF 75 0C" ) ) + 0x1u;

	m_addresses.m_alloc_key_values_engine =
		( alloc_key_values_engine_rel + 0x4u
		+ *reinterpret_cast< std::int32_t* >( alloc_key_values_engine_rel ) ) + 0x4Au;

	const auto alloc_key_values_client_rel = g_mem->find_pattern( client, xorstr_( "E8 ? ? ? ? 83 C4 08 84 C0 75 10" ) ) + 0x1;

	m_addresses.m_alloc_key_values_client =
		( alloc_key_values_client_rel + 0x4u
		+ *reinterpret_cast< std::int32_t* >( alloc_key_values_client_rel ) ) + 0x3Eu;

	m_addresses.m_key_values_init = g_mem->find_pattern( client, xorstr_( "55 8B EC 56 8B F1 33 C0 8B 4D 0C 81 26 ? ? ? ? 89 46" ) );
	m_addresses.m_key_values_load_from_buffer = g_mem->find_pattern( client, xorstr_( "55 8B EC 83 E4 F8 83 EC 34 53 8B 5D 0C 89" ) );
	m_addresses.m_cam_sv_cheats_ret = g_mem->find_pattern( client, xorstr_( "85 C0 75 30 38 87" ) );
	m_addresses.m_add_notify = g_mem->find_pattern( engine, xorstr_( "55 8B EC 81 EC 10 08 00 00 80 3D" ) );
	m_addresses.m_ret_to_scope_arc = g_mem->find_pattern( client, xorstr_( "6A 00 FF 50 3C 8B 0D ? ? ? ? FF B7 ? ? ? ?" ) ) + 0x5u;
	m_addresses.m_ret_to_scope_lens = g_mem->find_pattern( client, xorstr_( "FF 50 3C 8B 4C 24 20" ) ) + 0x3u;
	m_addresses.m_set_clan_tag = g_mem->find_pattern( engine, xorstr_( "53 56 57 8B DA 8B F9 FF 15" ) );
	m_addresses.m_start_drawing = g_mem->find_pattern( valve_gui, xorstr_( "55 8B EC 83 E4 C0 83 EC 38" ) );
	m_addresses.m_finish_drawing = g_mem->find_pattern( valve_gui, xorstr_( "8B 0D ? ? ? ? 56 C6 05 ? ? ? ? ?" ) );
	m_addresses.m_add_clred_text = g_mem->find_pattern( valve_gui, xorstr_( "55 8B EC 81 EC ? ? ? ? 56 57 FF 75 10" ) );
	m_addresses.m_calc_text_size = g_mem->find_pattern( valve_gui, xorstr_( "55 8B EC 83 EC 18 83 7D 0C 00" ) );
	m_addresses.m_set_font_glyph_set = g_mem->find_pattern( valve_gui, xorstr_( "55 8B EC 83 EC 08 8B 45 08 53" ) );
	m_addresses.m_has_c4 = g_mem->find_pattern( client, xorstr_( "56 8B F1 85 F6 74 31" ) );
	m_addresses.m_ret_insert_into_tree = g_mem->find_pattern( client, xorstr_( "89 44 24 14 EB 08 C7 44 24 ? ? ? ? ? 8B 45" ) );
	m_addresses.m_smoke_count = g_mem->find_pattern( client, xorstr_( "A3 ? ? ? ? 57 8B CB" ) ) + 0x1u;
	m_addresses.m_disable_post_processing = g_mem->find_pattern( client, xorstr_( "80 3D ? ? ? ? ? 53 56 57 0F 85" ) ) + 0x2u;
	m_addresses.m_crosshair_ret = g_mem->find_pattern( client, xorstr_( "85 C0 0F 84 ? ? ? ? E8 ? ? ? ? 99" ) );
	m_addresses.m_ret_to_scope_clear = g_mem->find_pattern( client, xorstr_( "0F 82 ? ? ? ? FF 50 3C" ) ) + 0x9u;
	m_addresses.m_ret_to_scope_blurry = g_mem->find_pattern( client, xorstr_( "FF B7 ? ? ? ? 8B 01 FF 90 ? ? ? ? 8B 4C 24 24" ) ) - 0x6u;
	m_addresses.m_ret_to_eye_pos_and_vectors = g_mem->find_pattern( client, xorstr_( "8B 55 0C 8B C8 E8 ? ? ? ? 83 C4 08 5E 8B E5" ) );
	m_addresses.m_ret_to_extrapolation = g_mem->find_pattern( client, xorstr_( "0F B6 0D ? ? ? ? 84 C0 0F 44 CF 88 0D ? ? ? ?" ) );
	m_addresses.m_item_system = g_mem->find_pattern( client, xorstr_( "A1 ? ? ? ? 85 C0 75 ? A1 ? ? ? ? 56 68 E8 07 00 00" ) );
	m_addresses.m_hud = *reinterpret_cast< std::uintptr_t* >( g_mem->find_pattern( client, xorstr_( "B9 ? ? ? ? 68 ? ? ? ? E8 ? ? ? ? 89" ) ) + 0x1u );
	m_addresses.m_find_hud_element = g_mem->find_pattern( client, xorstr_( "55 8B EC 53 8B 5D 08 56 57 8B F9 33 F6 39 77 28" ) );
	m_addresses.m_clear_hud_wpn_icon = g_mem->find_pattern( client, xorstr_( "55 8B EC 51 53 56 8B 75 08 8B D9 57 6B" ) );

#ifdef _DEBUG
	std::printf( "initializing cvars...\n" );
#endif

	m_cvars.m_r_jiggle_bones = valve::g_cvar->find_var( xorstr_( "r_jiggle_bones" ) );
	m_cvars.m_cl_interp = valve::g_cvar->find_var( xorstr_( "cl_interp" ) );
	m_cvars.m_cl_interp_ratio = valve::g_cvar->find_var( xorstr_( "cl_interp_ratio" ) );
	m_cvars.m_cl_updaterate = valve::g_cvar->find_var( xorstr_( "cl_updaterate" ) );
	m_cvars.m_ff_dmg_reduction_bullets = valve::g_cvar->find_var( xorstr_( "ff_damage_reduction_bullets" ) );
	m_cvars.m_ff_dmg_bullet_pen = valve::g_cvar->find_var( xorstr_( "ff_damage_bullet_penetration" ) );
	m_cvars.m_sv_auto_bhop = valve::g_cvar->find_var( xorstr_( "sv_autobunnyhopping" ) );
	m_cvars.m_sv_clockcorrection_msecs = valve::g_cvar->find_var( xorstr_( "sv_clockcorrection_msecs" ) );
	m_cvars.m_sv_maxvelocity = valve::g_cvar->find_var( xorstr_( "sv_maxvelocity" ) );
	m_cvars.m_sv_friction = valve::g_cvar->find_var( xorstr_( "sv_friction" ) );
	m_cvars.m_sv_accelerate = valve::g_cvar->find_var( xorstr_( "sv_accelerate" ) );
	m_cvars.m_sv_accelerate_use_weapon_speed = valve::g_cvar->find_var( xorstr_( "sv_accelerate_use_weapon_speed" ) );
	m_cvars.m_weapon_accuracy_nospread = valve::g_cvar->find_var( xorstr_( "weapon_accuracy_nospread" ) );
	m_cvars.m_sv_maxunlag = valve::g_cvar->find_var( xorstr_( "sv_maxunlag" ) );
	m_cvars.m_sv_enable_bhop = valve::g_cvar->find_var( xorstr_( "sv_enablebunnyhopping" ) );
	m_cvars.m_sv_jump_impulse = valve::g_cvar->find_var( xorstr_( "sv_jump_impulse" ) );
	m_cvars.m_sv_gravity = valve::g_cvar->find_var( xorstr_( "sv_gravity" ) );
	m_cvars.m_weapon_recoil_scale = valve::g_cvar->find_var( xorstr_( "weapon_recoil_scale" ) );
	m_cvars.m_weapon_accuracy_shotgun_spread_patterns = valve::g_cvar->find_var( xorstr_( "weapon_accuracy_shotgun_spread_patterns" ) );
	m_cvars.m_molotov_throw_detonate_time = valve::g_cvar->find_var( xorstr_( "molotov_throw_detonate_time" ) );
	m_cvars.m_weapon_molotov_maxdetonateslope = valve::g_cvar->find_var( xorstr_( "weapon_molotov_maxdetonateslope" ) );
	m_cvars.m_cl_fullupdate = valve::g_cvar->find_var( xorstr_( "cl_fullupdate" ) );
	m_cvars.m_cl_forwardspeed = valve::g_cvar->find_var( xorstr_( "cl_forwardspeed" ) );

	valve::g_cvar->find_var( xorstr_( "developer" ) )->set_int( 1 );
	valve::g_cvar->find_var( xorstr_( "con_filter_enable" ) )->set_int( 2 );
	valve::g_cvar->find_var( xorstr_( "con_filter_text" ) )->set_str( xorstr_( "[ untitled ]" ) );

#ifdef _DEBUG
	std::printf( "setting up hooks...\n" );
#endif

	if ( MH_CreateHook(
		( *reinterpret_cast< LPVOID** >( device ) )[ 16u ],
		reinterpret_cast< LPVOID >( &hooks::device_reset ),
		reinterpret_cast< LPVOID* >( &hooks::original_device_reset )
	) != MH_OK )
		return;

	if ( MH_CreateHook(
		( *reinterpret_cast< LPVOID** >( device ) )[ 17u ],
		reinterpret_cast< LPVOID >( &hooks::device_present ),
		reinterpret_cast< LPVOID* >( &hooks::original_device_present )
	) != MH_OK )
		return;

	if ( MH_EnableHook( MH_ALL_HOOKS ) != MH_OK )
		return;

#ifdef _DEBUG
	std::printf( "done!\n" );
#endif
}

int __stdcall DllMain( _In_ HINSTANCE instance, _In_ DWORD reason, _In_ LPVOID reserved ) {
	if ( reason != DLL_PROCESS_ATTACH )
		return 0;

	DisableThreadLibraryCalls( instance );

	std::jthread{ [ ]( ) {
		g_ctx->init( );
	} }.detach( );

	return 1;
}