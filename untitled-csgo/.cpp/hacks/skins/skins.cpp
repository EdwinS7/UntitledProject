#include "../../../inc.hpp"

namespace hacks {
	void c_skins::init( ) {
		std::string items{};

		if ( std::ifstream file{ xorstr_( "csgo/scripts/items/items_game_cdn.txt" ) } )
			items = { std::istreambuf_iterator{ file }, {} };

		if ( items.empty( ) )
			return;

		const auto item_schema = reinterpret_cast< valve::item_schema_t* >(
			reinterpret_cast< std::uintptr_t( __cdecl* )( ) >( g_ctx->addresses( ).m_item_system )( ) + 0x4u
			);

		for ( int i{}; i <= item_schema->m_paint_kits.m_last_element; ++i ) {
			const auto& paint_kit = item_schema->m_paint_kits.m_mem[ i ].m_value;
			if ( paint_kit->m_id == 9001
				|| paint_kit->m_id >= 10000 )
				continue;

			auto name = util::to_utf8( valve::g_localize->find_safe( paint_kit->m_item_name.m_buffer + 1 ) );
			auto name_wpn = util::to_utf8( valve::g_localize->find_safe( paint_kit->m_item_name.m_buffer + 1 ) );

			const auto pos = items.find( '_' + std::string{ paint_kit->m_name.m_buffer } + '=' );
			if ( pos != std::string::npos
				&& items.substr( pos + paint_kit->m_name.m_length ).find( '_' + std::string{ paint_kit->m_name.m_buffer } + '=' ) == std::string::npos ) {
				const auto wpn_name = items.rfind( xorstr_( "weapon_" ), pos );
				
				if ( wpn_name != std::string::npos )
					name_wpn = name + std::string( " (" + items.substr( wpn_name + 7, pos - wpn_name - 7 ) + ')' );
			}

			m_raw_paint_kits.emplace_back( paint_kit->m_id, std::move( name ) );
			m_paint_kits.emplace_back( paint_kit->m_id, std::move( name_wpn ) );

			std::sort( m_paint_kits.begin( ), m_paint_kits.end( ), [ ] ( const paint_kit_t& kit1, const paint_kit_t& kit2 ) {
				return kit1.m_name < kit2.m_name;
			} );

			std::sort( m_raw_paint_kits.begin( ), m_raw_paint_kits.end( ), [ ] ( const paint_kit_t& kit1, const paint_kit_t& kit2 ) {
				return kit1.m_name < kit2.m_name;
			} );
		}
	}

	int c_skins::correct_sequence( const valve::e_item_index index, const int seq ) const {
		enum e_sequence : int {
			sequence_default_draw = 0,
			sequence_default_idle1 = 1,
			sequence_default_idle2 = 2,
			sequence_default_light_miss1 = 3,
			sequence_default_light_miss2 = 4,
			sequence_default_heavy_miss1 = 9,
			sequence_default_heavy_hit1 = 10,
			sequence_default_heavy_backstab = 11,
			sequence_default_lookat1 = 12,

			sequence_bufferly_draw = 0,
			sequence_bufferly_draw2 = 1,
			sequence_butterly_lookat1 = 13,
			sequence_butterly_lookat3 = 15,

			sequence_falchion_idle1 = 1,
			sequence_falchion_heavy_miss1 = 8,
			sequence_falchion_heavy_miss1_noflip = 9,
			sequence_falchion_lookat1 = 12,
			sequence_falchion_lookat2 = 13,

			sequence_daggers_idle1 = 1,
			sequence_daggers_light_miss1 = 2,
			sequence_daggers_light_miss5 = 6,
			sequence_daggers_heavy_miss2 = 11,
			sequence_daggers_heavy_miss1 = 12,

			sequence_bowie_idle1 = 1,
		};

		switch ( index ) {
		case valve::e_item_index::knife_butterfly:
		{
			switch ( seq ) {
			case sequence_default_draw:
				return g_ctx->addresses( ).m_random_int( sequence_bufferly_draw, sequence_bufferly_draw2 );
				break;
			case sequence_default_lookat1:
				return g_ctx->addresses( ).m_random_int( sequence_butterly_lookat1, sequence_butterly_lookat3 );
				break;
			}

			return seq + 1;
		} break;
		case valve::e_item_index::knife_falchion:
		{
			switch ( seq ) {
			case sequence_default_idle2: return sequence_falchion_idle1; break;
			case sequence_default_heavy_miss1:
				return g_ctx->addresses( ).m_random_int( sequence_falchion_heavy_miss1, sequence_falchion_heavy_miss1_noflip );
				break;
			case sequence_default_lookat1:
				return g_ctx->addresses( ).m_random_int( sequence_falchion_lookat1, sequence_falchion_lookat2 );
				break;
			case sequence_default_draw:
			case sequence_default_idle1:
				return seq;
				break;
			}

			return seq - 1;
		} break;
		case valve::e_item_index::knife_push:
		{
			switch ( seq ) {
			case sequence_default_idle2: return sequence_daggers_idle1; break;
			case sequence_default_light_miss1:
			case sequence_default_light_miss2:
				return g_ctx->addresses( ).m_random_int( sequence_daggers_light_miss1, sequence_daggers_light_miss5 );
				break;
			case sequence_default_heavy_miss1:
				return g_ctx->addresses( ).m_random_int( sequence_daggers_heavy_miss2, sequence_daggers_heavy_miss1 );
				break;
			case sequence_default_heavy_hit1:
			case sequence_default_heavy_backstab:
			case sequence_default_lookat1:
				return seq + 3;
				break;
			case sequence_default_draw:
			case sequence_default_idle1:
				return seq;
				break;
			}

			return seq + 2;
		} break;
		case valve::e_item_index::knife_survival_bowie:
		{
			switch ( seq ) {
			case sequence_default_draw:
			case sequence_default_idle1:
				return seq;
				break;
			case sequence_default_idle2: return sequence_bowie_idle1; break;
			}

			return seq - 1;
		} break;
		case valve::e_item_index::knife_ursus:
		case valve::e_item_index::knife_skeleton:
		case valve::e_item_index::knife_outdoor:
		case valve::e_item_index::knife_cord:
		case valve::e_item_index::knife_canis:
		{
			switch ( seq ) {
			case sequence_default_draw:
				return g_ctx->addresses( ).m_random_int( sequence_bufferly_draw, sequence_bufferly_draw2 );
			case sequence_default_lookat1:
				return g_ctx->addresses( ).m_random_int( sequence_butterly_lookat1, 14 );
				break;
			}

			return seq + 1;
		} break;
		case valve::e_item_index::knife_stiletto:
			if ( seq == sequence_default_lookat1 )
				return g_ctx->addresses( ).m_random_int( 12, 13 );

			break;
		case valve::e_item_index::knife_widowmaker:
			if ( seq == sequence_default_lookat1 )
				return g_ctx->addresses( ).m_random_int( 14, 15 );

			break;
		default: break;
		}

		return seq;
	}

	bool c_skins::set_paint_kit( valve::c_weapon* const weapon, const bool is_knife ) {
		if ( is_knife ) {
			if ( !m_cfg->m_knife_index ) {
				const auto updated = m_old_knife_index != m_cfg->m_knife_index;

				m_old_knife_index = m_cfg->m_knife_index;

				return updated;
			}

			const auto paint_kit = m_cfg->m_paint_kits.at( 0u );

			weapon->item_quality( ) = 9;
			weapon->item_id_high( ) = -1;

			weapon->account_id( ) = weapon->owner_xuid_low( );

			weapon->fallback_stat_trak( ) = 69;
			weapon->fallback_paint_kit( ) = paint_kit;
			weapon->fallback_wear( ) = std::numeric_limits< float >::epsilon( );

			const auto model_index = valve::g_model_info->lookup_model_index( m_knife_mdls.at( m_cfg->m_knife_index - 1u ).data( ) );

			const auto item_index = m_knives.at( m_cfg->m_knife_index ).m_index;
			if ( item_index != weapon->item_index( ) ) {
				weapon->item_index( ) = item_index;

				weapon->set_model_index( model_index );

				const auto networkable = weapon->networkable( );

				using fn_t = void( __thiscall* )( const std::uintptr_t, const int );

				( *reinterpret_cast< fn_t** >( networkable ) )[ 6u ]( networkable, 0 );
			}

			if ( const auto view_model = valve::g_local_player->view_model( ) )
				if ( const auto view_model_weapon = valve::g_entity_list->find_entity( view_model->view_model_weapon_handle( ) ); view_model_weapon == weapon )
					if ( const auto world_model = valve::g_entity_list->find_entity( weapon->world_model_handle( ) ) ) {
						view_model->model_index( ) = model_index;
						world_model->model_index( ) = model_index + 1;
					}

			auto updated = m_old_paint_kits.at( 0u ) != paint_kit;

			m_old_paint_kits.at( 0u ) = paint_kit;

			if ( !updated )
				updated = m_old_knife_index != m_cfg->m_knife_index;

			m_old_knife_index = m_cfg->m_knife_index;

			return updated;
		}

		const auto result = std::find_if( m_weapons.begin( ), m_weapons.end( ), [ & ] ( const weapon_t& item ) {
			return item.m_index == weapon->item_index( );
		} );

		if ( result == m_weapons.end( ) )
			return false;

		const auto index = std::distance( m_weapons.begin( ), result );

		const auto paint_kit = m_cfg->m_paint_kits.at( index );
		if ( paint_kit ) {
			weapon->item_quality( ) = 9;
			weapon->item_id_high( ) = -1;

			weapon->account_id( ) = weapon->owner_xuid_low( );

			weapon->fallback_paint_kit( ) = paint_kit;
			weapon->fallback_wear( ) = std::numeric_limits< float >::epsilon( );
		}

		const auto updated = m_old_paint_kits.at( index ) != paint_kit;

		m_old_paint_kits.at( index ) = paint_kit;

		return updated;
	}

	void c_skins::on_frame_stage( const valve::e_frame_stage stage ) {
		if ( !valve::g_local_player
			|| !valve::g_engine->in_game( )
			|| !valve::g_local_player->alive( ) ) {
			m_force_update = m_in_force_update = m_update_hud = false;

			return;
		}

		if ( stage == valve::e_frame_stage::net_update_end
			&& valve::g_client_state->m_delta_tick > 0 )
			m_in_force_update = false;

		if ( stage != valve::e_frame_stage::net_update_post_data_update_start )
			return;

		const auto info = valve::g_engine->player_info( valve::g_local_player->index( ) );
		if ( !info.has_value( ) )
			return;

		std::vector< valve::c_weapon* > update_queue{};

		const auto weapons = valve::g_local_player->weapons( );

		for ( std::size_t i{}; weapons[ i ] != valve::e_ent_handle::invalid; ++i ) {
			const auto weapon = static_cast< valve::c_weapon* >( valve::g_entity_list->find_entity( weapons[ i ] ) );
			if ( !weapon
				|| weapon->owner_xuid_low( ) != info.value( ).m_xuid_low
				|| weapon->owner_xuid_high( ) != info.value( ).m_xuid_high )
				continue;

			const auto client_class = weapon->client_class( );
			if ( !client_class )
				continue;

			update_queue.emplace_back( weapon );

			if ( !set_paint_kit( weapon, client_class->m_class_id == valve::e_class_id::knife ) )
				continue;

			m_force_update = true;
		}

		if ( !m_force_update
			|| valve::g_client_state->m_delta_tick == -1
			|| std::abs( valve::g_global_vars->m_cur_time - m_last_update_time ) < 1.f ) {
			if ( m_update_hud
				&& !m_in_force_update ) {
				for ( auto& weapon : update_queue ) {
					weapon->custom_material_initialized( ) = false;

					auto& vec0 = weapon->custom_materials( );
					for ( int i{}; i < vec0.m_size; ++i )
						vec0.m_mem.m_ptr[ i ] = nullptr;

					vec0.m_size = 0;

					auto& vec1 = weapon->custom_materials2( );
					for ( int i{}; i < vec1.m_size; ++i )
						vec1.m_mem.m_ptr[ i ] = nullptr;

					vec1.m_size = 0;

					auto& vec2 = weapon->visuals_data_processors( );
					for ( int i{}; i < vec2.m_size; ++i ) {
						auto& element = vec2.m_mem.m_ptr[ i ];
						if ( !element )
							continue;

						element->unref( );
						element = nullptr;
					}

					vec2.m_size = 0;

					const auto networkable = weapon->networkable( );

					using fn_t = void( __thiscall* )( const std::uintptr_t, const int );

					( *reinterpret_cast< fn_t** >( networkable ) )[ 7u ]( networkable, 0 );

					( *reinterpret_cast< fn_t** >( networkable ) )[ 5u ]( networkable, 0 );
				}

				using find_t = std::uintptr_t* ( __thiscall* )( const std::uintptr_t, const char* );

				if ( const auto hud_weapons =
					reinterpret_cast< find_t >( g_ctx->addresses( ).m_find_hud_element )(
						g_ctx->addresses( ).m_hud, xorstr_( "CCSGO_HudWeaponSelection" )
						) - 0x28u )
					for ( std::size_t i{}; i < *( hud_weapons + 0x20u ); ++i )
						i = reinterpret_cast< int( __thiscall* )( void*, int ) >( g_ctx->addresses( ).m_clear_hud_wpn_icon )( hud_weapons, i );

				m_update_hud = false;
			}

			return;
		}

		valve::g_client_state->m_delta_tick = -1;

		m_force_update = false;
		m_in_force_update = m_update_hud = true;

		m_last_update_time = valve::g_global_vars->m_cur_time;
	}
}