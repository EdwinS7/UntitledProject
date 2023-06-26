#include "../../inc.hpp"

int __stdcall DllMain( _In_ HINSTANCE instance, _In_ DWORD reason, _In_ LPVOID reserved ) {
	if ( reason != DLL_PROCESS_ATTACH )
		return 0;

	DisableThreadLibraryCalls( instance );

	std::jthread{ [ ] ( ) {
		g_ctx->init( );
	} }.detach( );

	return 1;
}

std::uintptr_t find_byte_seq( const code_section_t& section, const std::string& seq ) {
	using seq_byte_t = std::pair< std::uint8_t, bool >;

	std::string str{};
	std::vector< seq_byte_t > byte_seq{};

	std::stringstream stream{ seq };
	while ( stream >> str ) {
		if ( str[ 0u ] == '?' ) {
			byte_seq.emplace_back( 0u, true );

			continue;
		}

		if ( !std::isxdigit( str[ 0u ] )
			|| !std::isxdigit( str[ 1u ] ) )
			continue;

		byte_seq.emplace_back(
			static_cast< std::uint8_t >(
				std::strtoul( str.data( ), nullptr, 16 )
				), false
		);
	}

	const auto end = reinterpret_cast< std::uint8_t* >( section.m_base + section.m_size );

	const auto ret = std::search(
		reinterpret_cast< std::uint8_t* >( section.m_base ), end, byte_seq.begin( ), byte_seq.end( ),
		[ ] ( const std::uint8_t byte, const seq_byte_t& seq_byte ) {
			return std::get< bool >( seq_byte ) || byte == std::get< std::uint8_t >( seq_byte );
		}
	);

	return reinterpret_cast< std::uintptr_t >( ret );
}

void c_ctx::init( ) {
#ifdef _DEBUG
	if ( !AllocConsole( ) )
		return;

	FILE* file{};
	if ( freopen_s( &file, "CONOUT$", "w", stdout ) )
		return;

	SetConsoleTitle( "untitled" );
#endif
	while ( !GetModuleHandle( xorstr_( "serverbrowser.dll" ) ) )
		std::this_thread::sleep_for( std::chrono::milliseconds{ 200 } );

	if ( MH_Initialize( ) != MH_OK )
		return;

	const auto d3d9_device = **reinterpret_cast< IDirect3DDevice9*** >(
		find_byte_seq( GetModuleHandle( xorstr_( "shaderapidx9.dll" ) ), xorstr_( "A1 ? ? ? ? 50 8B 08 FF 51 0C" ) ) + 1u
	);

#ifdef _DEBUG
	std::printf( "initializing imgui...\n" );
#endif
	/* init imgui */
	{
		D3DDEVICE_CREATION_PARAMETERS creation_params{};
		if ( d3d9_device->GetCreationParameters( &creation_params ) != D3D_OK )
			return;

		ImGui::CreateContext( );
		ImGui::StyleColorsClassic( );

		ImGui::GetStyle( ).WindowMinSize = { 450, 350 };
		ImGui::GetStyle( ).WindowRounding = 5.f;
		ImGui::GetStyle( ).FrameRounding = 5.f;
		ImGui::GetStyle( ).ChildRounding = 5.f;
		ImGui::GetStyle( ).PopupRounding = 5.f;
		ImGui::GetStyle( ).GrabRounding = 5.f;
		ImGui::GetStyle( ).ScrollbarRounding = 3.f;
		ImGui::GetStyle( ).TabRounding = 5.f;

		auto& imgui_io = ImGui::GetIO( );

		imgui_io.IniFilename = imgui_io.LogFilename = nullptr;

		ImGui_ImplWin32_Init( creation_params.hFocusWindow );
		ImGui_ImplDX9_Init( d3d9_device );

		hooks::orig_wnd_proc = reinterpret_cast< WNDPROC >(
			SetWindowLong(
				creation_params.hFocusWindow, GWL_WNDPROC,
				reinterpret_cast< LONG >( hooks::wnd_proc )
			)
		);
	}

	const auto cl_code_section = code_section_t{ GetModuleHandle( xorstr_( "client.dll" ) ) };
	const auto eng_code_section = code_section_t{ GetModuleHandle( xorstr_( "engine.dll" ) ) };
	const auto vgui_code_section = code_section_t{ GetModuleHandle( xorstr_( "vguimatsurface.dll" ) ) };

#ifdef _DEBUG
	std::printf( "initializing valve interfaces...\n" );
#endif
	/* init valve stuff */
	{
		valve::g_local_player = *reinterpret_cast< valve::c_local_player* >(
			find_byte_seq( cl_code_section, xorstr_( "8B 0D ? ? ? ? 83 FF FF 74 07" ) ) + 0x2u
		);

		valve::g_client = *reinterpret_cast< valve::c_client** >(
			find_byte_seq( cl_code_section, xorstr_( "A1 ? ? ? ? B9 ? ? ? ? FF 90 ? ? ? ? 8B 35 ? ? ? ?" ) ) + 0x1u
		);

		valve::g_global_vars = **reinterpret_cast< valve::global_vars_t*** >(
			( *reinterpret_cast< std::uintptr_t** >( valve::g_client ) )[ 11u ] + 0xau
		);

		valve::g_engine = **reinterpret_cast< valve::c_engine*** >(
			find_byte_seq( cl_code_section, xorstr_( "8B 0D ? ? ? ? 8B 01 8B 40 38 FF" ) ) + 0x2u
		);

		valve::g_entity_list = **reinterpret_cast< valve::c_entity_list*** >(
			find_byte_seq( eng_code_section, xorstr_( "8B 0D ? ? ? ? 88 45 FC 6A 00" ) ) + 0x2u
		);

		valve::g_client_state = **reinterpret_cast< valve::client_state_t*** >(
			find_byte_seq( eng_code_section, xorstr_( "A1 ? ? ? ? 8B 88 ? ? ? ? 85 C9 75 07" ) ) + 0x1u
		);
			
		valve::g_move_helper = **reinterpret_cast< valve::c_move_helper*** >(
			find_byte_seq( cl_code_section, xorstr_( "8B 0D ? ? ? ? 8B 45 ? 51 8B D4 89 02 8B 01" ) ) + 0x2u
		);

		valve::g_prediction = *reinterpret_cast< valve::prediction_t** >(
			find_byte_seq( cl_code_section, xorstr_( "A1 ? ? ? ? 8B 40 38 FF D0 84 C0" ) ) + 0x1u
		);
			
		valve::g_movement = *reinterpret_cast< valve::c_movement** >(
			find_byte_seq( cl_code_section, xorstr_( "A1 ? ? ? ? B9 ? ? ? ? FF 50 04 EB 07" ) ) + 0x1u
		);
			
		valve::g_cvar = **reinterpret_cast< valve::c_cvar*** >(
			find_byte_seq( cl_code_section, xorstr_( "8B 0D ? ? ? ? 68 ? ? ? ? 8B 01 FF 50 44 8B C8 89" ) ) + 0x2u
		);

		valve::g_engine_trace = **reinterpret_cast< valve::c_engine_trace*** >(
			find_byte_seq( cl_code_section, xorstr_( "A1 ? ? ? ? F3 0F 11 45 ? 8B 30" ) ) + 0x1u
		);
			
		valve::g_surface_data = **reinterpret_cast< valve::c_surface_data*** >(
			find_byte_seq( cl_code_section, xorstr_( "8B 0D ? ? ? ? 0F BF 45 C8" ) ) + 0x2u
		);

		valve::g_input = *reinterpret_cast< valve::input_t** >(
			find_byte_seq( cl_code_section, xorstr_( "B9 ? ? ? ? FF 75 08 FF 50 0C" ) ) + 0x1u
		);

		valve::g_game_event_mgr = **reinterpret_cast< valve::c_game_event_mgr*** >(
			find_byte_seq( cl_code_section, xorstr_( "8B 0D ? ? ? ? 50 8B 11 FF 52 24 FF 15 ? ? ? ?" ) ) + 0x2u
		);

		valve::g_studio_render = **reinterpret_cast< valve::studio_render_t*** >(
			find_byte_seq( cl_code_section, xorstr_( "8B 0D ? ? ? ? 8D 54 24 40 52 8B 01 FF 50 34" ) ) + 0x2u
		);

		valve::g_mat_system = **reinterpret_cast< valve::c_mat_system*** >(
			find_byte_seq( cl_code_section, xorstr_( "8B 0D ?? ?? ?? ?? 8B 01 8B 80 ?? ?? ?? ?? FF D0 8B F0 89 74 24 48" ) ) + 0x2u
		);

		valve::g_view_render = **reinterpret_cast< valve::view_render_t*** >(
			find_byte_seq( cl_code_section, xorstr_( "8B 0D ? ? ? ? 8B 01 FF 50 4C 8B 06" ) ) + 0x2u
		);

		valve::g_surface = **reinterpret_cast< valve::c_surface*** >(
			find_byte_seq( cl_code_section, xorstr_( "8B 0D ? ? ? ? FF B7 ? ? ? ? 8B 01 FF 90 ? ? ? ? 8B 7C 24 1C" ) ) + 0x2u
		);

		/* idc */
		valve::g_key_values_system = reinterpret_cast< valve::c_key_values_system*( __cdecl* )( ) >( 
			GetProcAddress( GetModuleHandleA( xorstr_( "vstdlib.dll" ) ), xorstr_( "KeyValuesSystem" ) )
		)( );

		valve::g_localize = reinterpret_cast< valve::c_localize*( __cdecl* )( const char*, int* ) >(
			GetProcAddress( GetModuleHandle( xorstr_( "localize.dll" ) ), xorstr_( "CreateInterface" ) )
		)( xorstr_( "Localize_001" ), nullptr );

		valve::g_font_mgr = *reinterpret_cast< valve::c_font_mgr** >(
			find_byte_seq( vgui_code_section, xorstr_( "74 1D 8B 0D ? ? ? ? 68 ? ? ? ?" ) ) + 0x4u
		);

		valve::g_model_info = *reinterpret_cast< valve::c_model_info** >(
			find_byte_seq( eng_code_section, xorstr_( "B9 ? ? ? ? 8B 44 B8 04 83 C0 04 50 A1 ? ? ? ? FF 50 08 8B D0 8B 4C BB 0C" ) ) + 0x1u
		);

		valve::g_beams = *reinterpret_cast< valve::c_beams** >(
			find_byte_seq( cl_code_section, xorstr_( "B9 ? ? ? ? A1 ? ? ? ? FF 10 A1 ? ? ? ? B9" ) ) + 0x1u
		);

		valve::g_model_render = reinterpret_cast< valve::c_model_render*( __cdecl* )( const char*, int* ) >(
			GetProcAddress( GetModuleHandle( xorstr_( "engine.dll" ) ), xorstr_( "CreateInterface" ) )
		)( xorstr_( "VEngineModel016" ), nullptr );

		valve::g_mdl_cache = reinterpret_cast< valve::c_mdl_cache*( __cdecl* )( const char*, int* ) >(
			GetProcAddress( GetModuleHandle( xorstr_( "datacache.dll" ) ), xorstr_( "CreateInterface" ) )
		)( xorstr_( "MDLCache004" ), nullptr );

		valve::g_glow_mgr = *reinterpret_cast< valve::glow_mgr_t** >(
			find_byte_seq( cl_code_section, xorstr_( "0F 11 05 ? ? ? ? 83 C8 01" ) ) + 0x3u
		);

		valve::g_debug_overlay = reinterpret_cast< valve::c_debug_overlay*( __cdecl* )( const char*, int* ) >(
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

            auto start = cl_code_section.m_base;
            auto size = cl_code_section.m_size;

            std::uintptr_t addr{};
            for ( ; ; ) {
                addr = find_byte_seq( { size, start }, xorstr_( "CC CC CC CC C7 05 ? ? ? ? ? ? ? ? C7 05 ? ? ? ? ? ? ? ? C3 CC" ) );
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
    }

#ifdef _DEBUG
	std::printf( "initializing addresses...\n" );
#endif
		
	/* init addresses */
	{
		const auto vstdlib_dll = GetModuleHandle( xorstr_( "vstdlib.dll" ) );

		m_addresses.m_random_float = reinterpret_cast< addresses_t::random_float_t >( GetProcAddress( vstdlib_dll, xorstr_( "RandomFloat" ) ) );
		m_addresses.m_random_int = reinterpret_cast< addresses_t::random_int_t >( GetProcAddress( vstdlib_dll, xorstr_( "RandomInt" ) ) );
		m_addresses.m_random_seed = reinterpret_cast< addresses_t::random_seed_t >( GetProcAddress( vstdlib_dll, xorstr_( "RandomSeed" ) ) );

		m_addresses.m_reset_anim_state = find_byte_seq( cl_code_section, xorstr_( "56 6A 01 68 ? ? ? ? 8B F1" ) );
		m_addresses.m_update_anim_state = find_byte_seq(
			cl_code_section, xorstr_( "55 8B EC 83 E4 F8 83 EC 18 56 57 8B F9 F3 0F 11 54 24" )
		);
		m_addresses.m_set_abs_angles = find_byte_seq( cl_code_section, xorstr_( "55 8B EC 83 E4 F8 83 EC 64 53 56 57 8B F1" ) );
		m_addresses.m_set_abs_origin = find_byte_seq( cl_code_section, xorstr_( "55 8B EC 83 E4 F8 51 53 56 57 8B F1 E8 ? ? ? ? 8B 7D" ) );
		m_addresses.m_lookup_seq_act = find_byte_seq( cl_code_section, xorstr_( "55 8B EC 53 8B 5D 08 56 8B F1 83" ) );
		m_addresses.m_breakable = find_byte_seq( cl_code_section, xorstr_( "55 8B EC 51 56 8B F1 85 F6 74 68 83 BE" ) );
		m_addresses.m_user_cmd_calc_checksum = find_byte_seq( cl_code_section, xorstr_( "53 8B D9 83 C8" ) );

		m_addresses.m_pred_seed = *reinterpret_cast< int** >(
			find_byte_seq( cl_code_section, xorstr_( "A3 ? ? ? ? 66 0F 6E 86" ) ) + 0x1u
		);

		m_addresses.m_pred_player = *reinterpret_cast< valve::c_player** >(
			find_byte_seq( cl_code_section, xorstr_( "89 35 ? ? ? ? F3 0F 10 48" ) ) + 0x2u
		);

		const auto angle_matrix_rel = find_byte_seq( cl_code_section, xorstr_( "E8 ? ? ? ? 8B 07 89 46 0C" ) );
		m_addresses.m_angle_matrix = reinterpret_cast< addresses_t::angle_matrix_t >(
			angle_matrix_rel + 0x1u + sizeof( std::uintptr_t ) + *reinterpret_cast< std::ptrdiff_t* >( angle_matrix_rel + 0x1u )
		);

		m_addresses.m_set_collision_bounds = find_byte_seq(
			cl_code_section, xorstr_( "53 8B DC 83 EC 08 83 E4 F8 83 C4 04 55 8B 6B 04 89 6C 24 04 8B EC 83 EC 18 56 57 8B 7B 08 8B D1 8B 4B 0C" )
		);

		m_addresses.m_invalidate_bone_cache = find_byte_seq( cl_code_section, xorstr_( "80 3D ? ? ? ? ? 74 16 A1 ? ? ? ? 48 C7 81" ) );
		m_addresses.m_lookup_bone = find_byte_seq( cl_code_section, xorstr_( "55 8B EC 53 56 8B F1 57 83 BE ?? ?? ?? ?? ?? 75 14 8B 46 04 8D 4E 04 FF 50 20" ) );
		m_addresses.m_trace_filter_simple_vtable = *reinterpret_cast< std::uintptr_t* >(
			find_byte_seq( cl_code_section, xorstr_( "55 8B EC 83 E4 F0 83 EC 7C 56 52" ) ) + 0x3du
		);

		m_addresses.m_trace_filter_skip_two_entities_vtable = *reinterpret_cast< std::uintptr_t* >(
			find_byte_seq( cl_code_section, xorstr_( "C7 45 ? ? ? ? ? 89 45 E4 8B 01" ) ) + 0x3u
		);

		m_addresses.m_write_user_cmd = find_byte_seq( cl_code_section, xorstr_( "55 8B EC 83 E4 F8 51 53 56 8B D9 8B 0D" ) );
		m_addresses.m_weapon_system = *reinterpret_cast< std::uintptr_t* >(
			find_byte_seq( cl_code_section, xorstr_( "8B 35 ? ? ? ? FF 10 0F B7 C0" ) ) + 0x2u
		);

		const auto calc_shotgun_spread_rel = find_byte_seq( cl_code_section, xorstr_( "E8 ? ? ? ? EB 38 83 EC 08" ) );

		m_addresses.m_calc_shotgun_spread =
			calc_shotgun_spread_rel + 0x1u + sizeof( std::uintptr_t )
			+ *reinterpret_cast< std::ptrdiff_t* >( calc_shotgun_spread_rel + 0x1u );

		const auto alloc_key_values_engine_rel = find_byte_seq( eng_code_section, xorstr_( "E8 ? ? ? ? 83 C4 08 84 C0 75 10 FF 75 0C" ) ) + 0x1u;

		m_addresses.m_alloc_key_values_engine =
			( alloc_key_values_engine_rel + 0x4u
			+ *reinterpret_cast< std::int32_t* >( alloc_key_values_engine_rel ) ) + 0x4Au;

		const auto alloc_key_values_client_rel = find_byte_seq( cl_code_section, xorstr_( "E8 ? ? ? ? 83 C4 08 84 C0 75 10" ) ) + 0x1;

		m_addresses.m_alloc_key_values_client =
			( alloc_key_values_client_rel + 0x4u
			+ *reinterpret_cast< std::int32_t* >( alloc_key_values_client_rel ) ) + 0x3Eu;

		m_addresses.m_key_values_init = find_byte_seq( cl_code_section, xorstr_( "55 8B EC 56 8B F1 33 C0 8B 4D 0C 81 26 ? ? ? ? 89 46" ) );
		m_addresses.m_key_values_load_from_buffer = find_byte_seq( cl_code_section, xorstr_( "55 8B EC 83 E4 F8 83 EC 34 53 8B 5D 0C 89" ) );
		m_addresses.m_cam_sv_cheats_ret = find_byte_seq( cl_code_section, xorstr_( "85 C0 75 30 38 87" ) );
		m_addresses.m_add_notify = find_byte_seq( eng_code_section, xorstr_( "55 8B EC 81 EC 10 08 00 00 80 3D" ) );
		m_addresses.m_ret_to_scope_arc = find_byte_seq( cl_code_section, xorstr_( "6A 00 FF 50 3C 8B 0D ? ? ? ? FF B7 ? ? ? ?" ) ) + 0x5u;
		m_addresses.m_ret_to_scope_lens = find_byte_seq( cl_code_section, xorstr_( "FF 50 3C 8B 4C 24 20" ) ) + 0x3u;
		m_addresses.m_set_clan_tag = find_byte_seq( eng_code_section, xorstr_( "53 56 57 8B DA 8B F9 FF 15" ) );
		m_addresses.m_start_drawing = find_byte_seq( vgui_code_section, xorstr_( "55 8B EC 83 E4 C0 83 EC 38" ) );
		m_addresses.m_finish_drawing = find_byte_seq( vgui_code_section, xorstr_( "8B 0D ? ? ? ? 56 C6 05 ? ? ? ? ?" ) );
		m_addresses.m_add_clred_text = find_byte_seq( vgui_code_section, xorstr_( "55 8B EC 81 EC ? ? ? ? 56 57 FF 75 10" ) );
		m_addresses.m_calc_text_size = find_byte_seq( vgui_code_section, xorstr_( "55 8B EC 83 EC 18 83 7D 0C 00" ) );
		m_addresses.m_set_font_glyph_set = find_byte_seq( vgui_code_section, xorstr_( "55 8B EC 83 EC 08 8B 45 08 53" ) );
		m_addresses.m_has_c4 = find_byte_seq( cl_code_section, xorstr_( "56 8B F1 85 F6 74 31" ) );
		m_addresses.m_ret_insert_into_tree = find_byte_seq( cl_code_section, xorstr_( "89 44 24 14 EB 08 C7 44 24 ? ? ? ? ? 8B 45" ) );
		m_addresses.m_smoke_count = find_byte_seq( cl_code_section, xorstr_( "A3 ? ? ? ? 57 8B CB" ) ) + 0x1u;
		m_addresses.m_disable_post_processing = find_byte_seq( cl_code_section, xorstr_( "80 3D ? ? ? ? ? 53 56 57 0F 85" ) ) + 0x2u;
		m_addresses.m_crosshair_ret = find_byte_seq( cl_code_section, xorstr_( "85 C0 0F 84 ? ? ? ? E8 ? ? ? ? 99" ) );
		m_addresses.m_ret_to_scope_clear = find_byte_seq( cl_code_section, xorstr_( "0F 82 ? ? ? ? FF 50 3C" ) ) + 0x9u;
		m_addresses.m_ret_to_scope_blurry = find_byte_seq( cl_code_section, xorstr_( "FF B7 ? ? ? ? 8B 01 FF 90 ? ? ? ? 8B 4C 24 24" ) ) - 0x6u;
		m_addresses.m_ret_to_eye_pos_and_vectors = find_byte_seq( cl_code_section, xorstr_( "8B 55 0C 8B C8 E8 ? ? ? ? 83 C4 08 5E 8B E5" ) );
		m_addresses.m_ret_to_extrapolation = find_byte_seq( cl_code_section, xorstr_( "0F B6 0D ? ? ? ? 84 C0 0F 44 CF 88 0D ? ? ? ?" ) );
		m_addresses.m_item_system = find_byte_seq( cl_code_section, xorstr_( "A1 ? ? ? ? 85 C0 75 ? A1 ? ? ? ? 56 68 E8 07 00 00" ) );
		m_addresses.m_hud = *reinterpret_cast< std::uintptr_t* >( find_byte_seq( cl_code_section, xorstr_( "B9 ? ? ? ? 68 ? ? ? ? E8 ? ? ? ? 89" ) ) + 0x1u );
		m_addresses.m_find_hud_element = find_byte_seq( cl_code_section, xorstr_( "55 8B EC 53 8B 5D 08 56 57 8B F9 33 F6 39 77 28" ) );
		m_addresses.m_clear_hud_wpn_icon = find_byte_seq( cl_code_section, xorstr_( "55 8B EC 51 53 56 8B 75 08 8B D9 57 6B" ) );
	}

#ifdef _DEBUG
	std::printf( "initializing cvars...\n" );
#endif
	/* init cvars */
	{
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

		/* temp solution for dbg @needfix */
		valve::g_cvar->find_var( xorstr_( "developer" ) )->set_int( 1 );
		valve::g_cvar->find_var( xorstr_( "con_filter_enable" ) )->set_int( 2 );
		valve::g_cvar->find_var( xorstr_( "con_filter_text" ) )->set_str( xorstr_( "[ untitled ]" ) );
	}

#ifdef _DEBUG
	std::printf( "setting up event listener...\n" );
#endif
	{
		valve::g_game_event_mgr->add_listener( &hooks::g_event_listener, xorstr_( "bullet_impact" ), false );
		valve::g_game_event_mgr->add_listener( &hooks::g_event_listener, xorstr_( "player_hurt" ), false );
		valve::g_game_event_mgr->add_listener( &hooks::g_event_listener, xorstr_( "weapon_fire" ), false );
		valve::g_game_event_mgr->add_listener( &hooks::g_event_listener, xorstr_( "round_freeze_end" ), false );
		valve::g_game_event_mgr->add_listener( &hooks::g_event_listener, xorstr_( "round_prestart" ), false );
		valve::g_game_event_mgr->add_listener( &hooks::g_event_listener, xorstr_( "player_footstep" ), false );
	}

	hacks::g_visuals->init( );
	hacks::g_skins->init( );

#ifdef _DEBUG
	std::printf( "setting up hooks...\n" );
#endif
	{
		auto& vel_mod_prop = std::get< valve::recv_prop_t* >( valve::g_net_vars->entry( xorstr_( "CCSPlayer->m_flVelocityModifier" ) ) );

		hooks::orig_velocity_modifier = reinterpret_cast< decltype( hooks::orig_velocity_modifier ) >( vel_mod_prop->m_proxy_fn );

		vel_mod_prop->m_proxy_fn = reinterpret_cast< std::uintptr_t >( &hooks::velocity_modifier );

		auto& sequence_prop = std::get< valve::recv_prop_t* >( valve::g_net_vars->entry( xorstr_( "CBaseViewModel->m_nSequence" ) ) );

		hooks::orig_sequence = reinterpret_cast< decltype( hooks::orig_sequence ) >( sequence_prop->m_proxy_fn );

		sequence_prop->m_proxy_fn = reinterpret_cast< std::uintptr_t >( &hooks::sequence );
	}

	if ( MH_CreateHook(
		( *reinterpret_cast< LPVOID** >( d3d9_device ) )[ 16u ],
		reinterpret_cast< LPVOID >( &hooks::d3d9_reset ),
		reinterpret_cast< LPVOID* >( &hooks::orig_d3d9_reset )
	) != MH_OK )
		return;

	if ( MH_CreateHook(
		( *reinterpret_cast< LPVOID** >( d3d9_device ) )[ 17u ],
		reinterpret_cast< LPVOID >( &hooks::d3d9_present ),
		reinterpret_cast< LPVOID* >( &hooks::orig_d3d9_present )
	) != MH_OK )
		return;

	if ( MH_CreateHook(
		( *reinterpret_cast< LPVOID** >( valve::g_key_values_system ) )[ 2u ],
		reinterpret_cast< LPVOID >( &hooks::alloc_key_values_memory ),
		reinterpret_cast< LPVOID* >( &hooks::orig_alloc_key_values_memory )
	) != MH_OK )
		return;

	if ( MH_CreateHook(
		reinterpret_cast< LPVOID >(
			find_byte_seq( eng_code_section, xorstr_( "55 8B EC 83 EC 40 53 8B D9 8B 0D ? ? ? ? 89 5D F8" ) )
		),
		reinterpret_cast< LPVOID >( &hooks::paint ),
		reinterpret_cast< LPVOID* >( &hooks::orig_paint )
	) != MH_OK )
		return;

	if ( MH_CreateHook(
		( *reinterpret_cast< LPVOID** >( valve::g_cvar->find_var( xorstr_( "sv_cheats" ) ) ) )[ 13u ],
		reinterpret_cast< LPVOID >( &hooks::sv_cheats_get_int ),
		reinterpret_cast< LPVOID* >( &hooks::orig_sv_cheats_get_int )
	) != MH_OK )
		return;

	if ( MH_CreateHook(
		( *reinterpret_cast< LPVOID** >( valve::g_studio_render ) )[ 29u ],
		reinterpret_cast< LPVOID >( &hooks::draw_model ),
		reinterpret_cast< LPVOID* >( &hooks::orig_draw_model )
	) != MH_OK )
		return;

	if ( MH_CreateHook(
		( *reinterpret_cast< LPVOID** >( valve::g_surface ) )[ 67u ],
		reinterpret_cast< LPVOID >( &hooks::lock_cursor ),
		reinterpret_cast< LPVOID* >( &hooks::orig_lock_cursor )
	) != MH_OK )
		return;

	if ( MH_CreateHook(
		( *reinterpret_cast< LPVOID** >( valve::g_surface ) )[ 116u ],
		reinterpret_cast< LPVOID >( &hooks::on_screen_size_change ),
		reinterpret_cast< LPVOID* >( &hooks::orig_on_screen_size_change )
	) != MH_OK )
		return;

	if ( MH_CreateHook(
		( *reinterpret_cast< LPVOID** >( valve::g_surface ) )[ 15u ],
		reinterpret_cast< LPVOID >( &hooks::draw_set_clr ),
		reinterpret_cast< LPVOID* >( &hooks::orig_draw_set_clr )
	) != MH_OK )
		return;

	if ( MH_CreateHook(
		( *reinterpret_cast< LPVOID** >( valve::g_client ) )[ 22u ],
		reinterpret_cast< LPVOID >( &hooks::create_move_proxy ),
		reinterpret_cast< LPVOID* >( &hooks::orig_create_move )
	) != MH_OK )
		return;

	if ( MH_CreateHook(
		( *reinterpret_cast< LPVOID** >( valve::g_client ) )[ 37u ],
		reinterpret_cast< LPVOID >( &hooks::frame_stage_notify ),
		reinterpret_cast< LPVOID* >( &hooks::orig_frame_stage_notify )
	) != MH_OK )
		return;

	if ( MH_CreateHook(
		reinterpret_cast< LPVOID >(
			find_byte_seq( cl_code_section, xorstr_( "55 8B EC 83 E4 F0 B8 ? ? ? ? E8 ? ? ? ? 56 57 8B F9 8B 0D ? ? ? ?" ) )
		),
		reinterpret_cast< LPVOID >( &hooks::setup_bones ),
		reinterpret_cast< LPVOID* >( &hooks::orig_setup_bones )
	) != MH_OK )
		return;

	if ( MH_CreateHook(
		reinterpret_cast< LPVOID >(
			find_byte_seq( cl_code_section, xorstr_( "55 8B EC 51 56 8B F1 80 BE ? ? ? ? ? 74 36" ) )
		),
		reinterpret_cast< LPVOID >( &hooks::update_client_side_anim ),
		reinterpret_cast< LPVOID* >( &hooks::orig_update_client_side_anim )
	) != MH_OK )
		return;

	if ( MH_CreateHook(
		reinterpret_cast< LPVOID >(
			find_byte_seq( cl_code_section, xorstr_( "55 8B EC 83 E4 F8 81 EC ? ? ? ? 53 56 8B F1 57 89 74 24 1C" ) )
		),
		reinterpret_cast< LPVOID >( &hooks::do_extra_bones_processing ),
		reinterpret_cast< LPVOID* >( &hooks::orig_do_extra_bones_processing )
	) != MH_OK )
		return;

	if ( MH_CreateHook(
		reinterpret_cast< LPVOID >(
			find_byte_seq( cl_code_section, xorstr_( "55 8B EC 57 8B F9 8B 0D ? ? ? ? 8B 01 8B 80 ? ? ? ?" ) )
		),
		reinterpret_cast< LPVOID >( &hooks::accumulate_layers ),
		reinterpret_cast< LPVOID* >( &hooks::orig_accumulate_layers )
	) != MH_OK )
		return;

	if ( MH_CreateHook(
		reinterpret_cast< LPVOID >(
			find_byte_seq( cl_code_section, xorstr_( "55 8B EC 83 E4 F0 B8 ? ? ? ? E8 ? ? ? ? 56 8B 75 08 57 8B F9 85 F6" ) )
		),
		reinterpret_cast< LPVOID >( &hooks::standard_blending_rules ),
		reinterpret_cast< LPVOID* >( &hooks::orig_standard_blending_rules )
	) != MH_OK )
		return;

	if ( MH_CreateHook(
		reinterpret_cast< LPVOID >(
			find_byte_seq(
				eng_code_section,
				xorstr_( "55 8B EC 8B 45 08 89 81 ? ? ? ? 8B 45 0C 89 81 ? ? ? ? 5D C2 08 00 CC CC CC CC CC CC CC 56" )
			)
		),
		reinterpret_cast< LPVOID >( &hooks::packet_start ),
		reinterpret_cast< LPVOID* >( &hooks::orig_packet_start )
	) != MH_OK )
		return;

	if ( MH_CreateHook(
		reinterpret_cast< LPVOID >(
			find_byte_seq(
				eng_code_section,
				xorstr_( "56 8B F1 E8 ? ? ? ? 8B 8E ? ? ? ? 3B 8E ? ? ? ?" )
			)
		),
		reinterpret_cast< LPVOID >( &hooks::packet_end ),
		reinterpret_cast< LPVOID* >( &hooks::orig_packet_end )
	) != MH_OK )
		return;

	if ( MH_CreateHook(
		reinterpret_cast< LPVOID >(
			find_byte_seq(
				cl_code_section,
				xorstr_( "56 8B F1 8B 8E ? ? ? ? 83 F9 FF 74 23" )
			)
		),
		reinterpret_cast< LPVOID >( &hooks::physics_simulate ),
		reinterpret_cast< LPVOID* >( &hooks::orig_physics_simulate )
	) != MH_OK )
		return;

	if ( MH_CreateHook(
		reinterpret_cast< LPVOID >(
			find_byte_seq(
				cl_code_section,
				xorstr_( "55 8B EC 83 EC 10 53 56 8B F1 57 80 BE ? ? ? ? ? 75 41" )
			)
		),
		reinterpret_cast< LPVOID >( &hooks::on_latch_interpolated_vars ),
		reinterpret_cast< LPVOID* >( &hooks::orig_on_latch_interpolated_vars )
	) != MH_OK )
		return;

	if ( MH_CreateHook(
		reinterpret_cast< LPVOID >(
			find_byte_seq(
				cl_code_section,
				xorstr_( "56 8B F1 E8 ? ? ? ? 3B F0" )
			)
		),
		reinterpret_cast< LPVOID >( &hooks::should_interpolate ),
		reinterpret_cast< LPVOID* >( &hooks::orig_should_interpolate )
	) != MH_OK )
		return;

	if ( MH_CreateHook(
		( *reinterpret_cast< LPVOID** >( valve::g_client ) )[ 24u ],
		reinterpret_cast< LPVOID >( &hooks::write_user_cmd_delta_to_buffer ),
		reinterpret_cast< LPVOID* >( &hooks::orig_write_user_cmd_delta_to_buffer )
	) != MH_OK )
		return;

	const auto modify_eye_pos_rel = find_byte_seq( cl_code_section, xorstr_( "E8 ? ? ? ? 8B 06 8B CE FF 90 ? ? ? ? 85 C0 74 50" ) );
	if ( MH_CreateHook(
		reinterpret_cast< LPVOID >(
			modify_eye_pos_rel + 0x1u + sizeof( std::uintptr_t )
			+ *reinterpret_cast< std::ptrdiff_t* >( modify_eye_pos_rel + 0x1u )
		),
		reinterpret_cast< LPVOID >( &hooks::modify_eye_pos ),
		reinterpret_cast< LPVOID* >( &hooks::orig_modify_eye_pos )
	) != MH_OK )
		return;

	if ( MH_CreateHook(
		reinterpret_cast< LPVOID >(
			find_byte_seq(
				eng_code_section,
				xorstr_( "55 8B EC 83 E4 F8 83 EC 4C A1 ? ? ? ? 80 B8 ? ? ? ? ?" )
			)
		),
		reinterpret_cast< LPVOID >( &hooks::process_temp_entities ),
		reinterpret_cast< LPVOID* >( &hooks::orig_process_temp_entities )
	) != MH_OK )
		return;

	if ( MH_CreateHook(
		( *reinterpret_cast< LPVOID** >( valve::g_client ) )[ 7u ],
		reinterpret_cast< LPVOID >( &hooks::level_shutdown ),
		reinterpret_cast< LPVOID* >( &hooks::orig_level_shutdown )
	) != MH_OK )
		return;

	if ( MH_CreateHook(
		( *reinterpret_cast< LPVOID** >( valve::g_engine ) )[ 101u ],
		reinterpret_cast< LPVOID >( &hooks::get_screen_aspect_ratio ),
		reinterpret_cast< LPVOID* >( &hooks::orig_get_screen_aspect_ratio )
	) != MH_OK )
		return;

	if ( MH_CreateHook(
		reinterpret_cast< LPVOID >(
			find_byte_seq(
				cl_code_section,
				xorstr_( "55 8B EC 83 EC 14 53 8B 5D 0C 56 57 85 DB 74" )
			)
		),
		reinterpret_cast< LPVOID >( &hooks::glow_effect_spectator ),
		reinterpret_cast< LPVOID* >( &hooks::orig_glow_effect_spectator )
	) != MH_OK )
		return;

	if ( MH_CreateHook(
		reinterpret_cast< LPVOID >(
			find_byte_seq(
				GetModuleHandle( xorstr_( "materialsystem.dll" ) ),
				xorstr_( "55 8B EC 83 EC ? 56 8B F1 8A 46" )
			)
		),
		reinterpret_cast< LPVOID >( &hooks::get_clr_modulation ),
		reinterpret_cast< LPVOID* >( &hooks::orig_get_clr_modulation )
	) != MH_OK )
		return;

	if ( MH_CreateHook(
		reinterpret_cast< LPVOID >(
			find_byte_seq(
				eng_code_section,
				xorstr_( "8B 0D ? ? ? ? 81 F9 ? ? ? ? 75 ? A1 ? ? ? ? 35 ? ? ? ? EB ? 8B 01 FF 50 ? 83 F8 ? 0F 85 ? ? ? ? 8B 0D" )
			)
		),
		reinterpret_cast< LPVOID >( &hooks::is_using_static_prop_debug_modes ),
		reinterpret_cast< LPVOID* >( &hooks::orig_is_using_static_prop_debug_modes )
	) != MH_OK )
		return;

	//if ( MH_CreateHook(
	//	( *reinterpret_cast< LPVOID** >( valve::g_engine->bsp_tree_query( ) ) )[ 6u ],
	//	reinterpret_cast< LPVOID >( &hooks::list_leaves_in_box ),
	//	reinterpret_cast< LPVOID* >( &hooks::orig_list_leaves_in_box )
	//) != MH_OK )
	//	return;

	if ( MH_CreateHook(
		( *reinterpret_cast< LPVOID** >( valve::g_mat_system ) )[ 21u ],
		reinterpret_cast< LPVOID >( &hooks::override_cfg ),
		reinterpret_cast< LPVOID* >( &hooks::orig_override_cfg )
	) != MH_OK )
		return;

	const auto client_mode = **reinterpret_cast< std::uintptr_t*** >(
		( *reinterpret_cast< std::uintptr_t** >( valve::g_client ) )[ 10u ] + 0x5u
	);

	if ( MH_CreateHook(
		reinterpret_cast< LPVOID >(
			find_byte_seq( cl_code_section, xorstr_( "53 56 8B F1 8A DA 8B 0D ? ? ? ? 81 F9 ? ? ? ? 75 0C A1 ? ? ? ? 35 ? ? ? ? EB 05 8B 01" ) )
		),
		reinterpret_cast< LPVOID >( &hooks::fog_enabled ),
		reinterpret_cast< LPVOID* >( &hooks::orig_fog_enabled )
	) != MH_OK )
		return;

	if ( MH_CreateHook(
		( *reinterpret_cast< LPVOID** >( client_mode ) )[ 18u ],
		reinterpret_cast< LPVOID >( &hooks::override_view ),
		reinterpret_cast< LPVOID* >( &hooks::orig_override_view )
	) != MH_OK )
		return;
		
	if ( MH_CreateHook(
		( *reinterpret_cast< LPVOID** >( client_mode ) )[ 44u ],
		reinterpret_cast< LPVOID >( &hooks::do_post_screen_space_effects ),
		reinterpret_cast< LPVOID* >( &hooks::orig_do_post_screen_space_effects )
	) != MH_OK )
		return;

	if ( MH_CreateHook(
		( *reinterpret_cast< LPVOID** >( valve::g_cvar->find_var( xorstr_( "weapon_debug_spread_show" ) ) ) )[ 13u ],
		reinterpret_cast< LPVOID >( &hooks::weapon_debug_spread_show_get_int ),
		reinterpret_cast< LPVOID* >( &hooks::orig_weapon_debug_spread_show_get_int )
	) != MH_OK )
		return;

	if ( MH_CreateHook(
		reinterpret_cast< LPVOID >(
			find_byte_seq( cl_code_section, xorstr_( "55 8B EC 83 EC 14 53 56 57 FF 75 18" ) )
		),
		reinterpret_cast< LPVOID >( &hooks::calc_view ),
		reinterpret_cast< LPVOID* >( &hooks::orig_calc_view )
	) != MH_OK )
		return;

	if ( MH_CreateHook(
		reinterpret_cast< LPVOID >(
			find_byte_seq( cl_code_section, xorstr_( "56 8B F1 85 F6 74 32" ) )
		),
		reinterpret_cast< LPVOID >( &hooks::get_eye_angles ),
		reinterpret_cast< LPVOID* >( &hooks::orig_get_eye_angles )
	) != MH_OK )
		return;

	if ( MH_CreateHook(
		( *reinterpret_cast< LPVOID** >( valve::g_engine ) )[ 90u ],
		reinterpret_cast< LPVOID >( &hooks::is_paused ),
		reinterpret_cast< LPVOID* >( &hooks::orig_is_paused )
	) != MH_OK )
		return;

	if ( MH_EnableHook( MH_ALL_HOOKS ) != MH_OK )
		return;

#ifdef _DEBUG
	std::printf( "done!\n" );
#endif
}

bool c_ctx::will_shoot( valve::c_weapon* const weapon, const valve::user_cmd_t& user_cmd ) const {
	if ( !weapon )
		return false;

	const auto item_index = weapon->item_index( );
	const auto wpn_data = weapon->wpn_data( );

	if ( ( item_index < valve::e_item_index::flashbang || item_index > valve::e_item_index::inc_grenade )
		&& !( user_cmd.m_buttons & valve::e_buttons::in_attack )
		&& ( !( user_cmd.m_buttons & valve::e_buttons::in_attack2 )
			|| weapon->next_secondary_attack( ) >= valve::g_global_vars->m_cur_time
			|| ( item_index != valve::e_item_index::revolver && ( !wpn_data || wpn_data->m_unk_type != 1 ) )
			)
		)
		return false;

	return true;
}

bool c_ctx::can_shoot(
	valve::c_weapon* const weapon, const valve::user_cmd_t& user_cmd,
	bool r8_ret, const int shift_amount, const bool normal
) const {
	const auto& anim_data = hacks::g_anim_sync->local_data( );
	if ( !normal
		&& anim_data.m_shot_valid_wpn
		&& anim_data.m_shot_cmd_number > valve::g_client_state->m_last_cmd_out
		&& anim_data.m_shot_cmd_number < ( valve::g_client_state->m_last_cmd_out + 100 ) )
		return false;

	if ( !weapon
		|| m_freeze_time
		|| !valve::g_local_player 
		|| user_cmd.m_weapon_select
		|| valve::g_local_player->flags( ) & valve::e_ent_flags::frozen 
		|| !valve::g_local_player->alive( ) )
		return false;

	if ( !g_ctx->wpn_data( ) )
		return false;

	const auto item_index = weapon->item_index( );

	const auto v10 = static_cast< std::size_t >( g_ctx->wpn_data( )->m_unk_type - 2 ) <= 5 || item_index == valve::e_item_index::taser;

	const auto cur_time = valve::to_time( 
		v10 ? valve::g_local_player->tick_base( ) - shift_amount : valve::g_local_player->tick_base( )
	);

	if ( valve::g_local_player->next_attack( ) > cur_time )
		return false;

	if ( item_index == valve::e_item_index::c4 )
		return true;

	if ( item_index >= valve::e_item_index::flashbang
		&& item_index <= valve::e_item_index::inc_grenade )
		return weapon->throw_time( ) > 0.f && cur_time > weapon->throw_time( );

	if ( ( v10 || item_index == valve::e_item_index::taser ) && ( weapon->ammo( ) <= 0 || weapon->in_reload( ) ) )
		return false;

	if ( weapon->next_primary_attack( ) > cur_time )
		return false;

	return item_index == valve::e_item_index::revolver ? r8_ret : true;
}

void c_ctx::add_packet( const int cmd_number ) {
	const auto user_cmd = &valve::g_input->m_cmds[ cmd_number % 150 ];
	if ( !user_cmd ) {
		m_last_sent_cmd_number = cmd_number;
		m_sented_cmds.emplace_back( cmd_number );

		return;
	}

	m_sented_cmds.emplace_back( cmd_number );

	for ( auto& shot : hacks::g_shots->elements( ) ) {
		if ( shot.m_cmd_number != -1 )
			continue;

		shot.m_cmd_number = cmd_number;
	}
}
