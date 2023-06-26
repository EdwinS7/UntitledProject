#include "../../../inc.hpp"

namespace hacks {
	void c_shots::on_net_update( ) {
		if ( !valve::g_local_player
			|| !valve::g_engine->in_game( ) )
			return m_elements.clear( );

		for ( auto& shot : m_elements ) {
			if ( shot.m_processed
				|| !shot.m_process_tick
				|| valve::g_global_vars->m_tick_count < shot.m_process_tick )
				continue;

			if ( shot.m_entry
				&& shot.m_lag_record
				&& shot.m_entry->m_player
				&& shot.m_entry->m_player->alive( ) ) {
				lag_backup_t lag_backup{ shot.m_entry->m_player };

				if ( shot.m_server_info.m_hurt_tick ) {
					if ( shot.m_lag_record->m_trying_to_resolve ) {
						valve::trace_t trace{};

						auto new_anim_side = shot.m_lag_record->m_anim_side;

						for ( int i{}; ; ) {
							new_anim_side = ( new_anim_side + i ) % 3;

							shot.m_lag_record->restore( shot.m_entry->m_player, new_anim_side, i != 0 );

							valve::g_engine_trace->clip_ray_to_entity(
								{ shot.m_src, shot.m_server_info.m_impact_pos },
								valve::e_mask::shot_player, shot.m_entry->m_player, &trace
							);

							if ( trace.m_hit_entity == shot.m_entry->m_player
								&& trace.m_hitgroup == shot.m_server_info.m_hitgroup )
								break;

							++i;

							if ( i >= 3 ) {
								new_anim_side = shot.m_lag_record->m_anim_side;

								break;
							}
						}

						if ( new_anim_side != shot.m_lag_record->m_anim_side ) {
							const auto wrong_anim_side = shot.m_lag_record->m_anim_side;

							shot.m_entry->m_prev_anim_side = shot.m_lag_record->m_anim_side = new_anim_side;

							valve::g_cvar->con_print( true, 0xff00ffffu,
								std::vformat( xorstr_( "[ untitled ] corrected via hitgroup anim_side from {} to {}.\n" ), std::make_format_args( wrong_anim_side, new_anim_side ) ).data( )
							);

							for ( auto& lag_record : shot.m_entry->m_lag_records ) {
								if ( !lag_record->m_trying_to_resolve
									|| lag_record->m_anim_side != wrong_anim_side )
									continue;

								lag_record->m_anim_side = new_anim_side;
							}
						}
						else
							valve::g_cvar->con_print( true, 0xff00ff00u, xorstr_( "[ untitled ] hit properly resolved shot.\n" ) );
					}
				}
				else {
					shot.m_lag_record->restore( shot.m_entry->m_player, shot.m_lag_record->m_anim_side );

					valve::trace_t trace{};

					valve::g_engine_trace->clip_ray_to_entity(
						{ shot.m_src, shot.m_server_info.m_impact_pos },
						valve::e_mask::shot_player, shot.m_entry->m_player, &trace
					);

					if ( trace.m_hit_entity == shot.m_entry->m_player ) {
						if ( shot.m_lag_record->m_extrapolated )
							valve::g_cvar->con_print( true, 0xff0000ffu, xorstr_( "[ untitled ] shot missed due to prediction.\n" ) );
						else {
							valve::g_cvar->con_print( true, 0xff0000ffu, xorstr_( "[ untitled ] shot missed due to resolver.\n" ) );

							++shot.m_entry->m_misses;

							if ( shot.m_lag_record->m_trying_to_resolve ) {
								valve::trace_t trace{};

								auto new_anim_side = shot.m_lag_record->m_anim_side;

								for ( auto i = 1; ; ) {
									new_anim_side = ( new_anim_side + i ) % 3;

									shot.m_lag_record->restore( shot.m_entry->m_player, new_anim_side, i != 1 );

									valve::g_engine_trace->clip_ray_to_entity(
										{ shot.m_src, shot.m_server_info.m_impact_pos },
										valve::e_mask::shot_player, shot.m_entry->m_player, &trace
									);

									if ( trace.m_hit_entity == shot.m_entry->m_player
										&& new_anim_side != shot.m_lag_record->m_anim_side )
										break;

									++i;

									if ( i >= 3 ) {
										new_anim_side = shot.m_lag_record->m_anim_side;

										break;
									}
								}

								if ( new_anim_side != shot.m_lag_record->m_anim_side ) {
									const auto wrong_anim_side = shot.m_lag_record->m_anim_side;

									shot.m_entry->m_prev_anim_side = shot.m_lag_record->m_anim_side = new_anim_side;

									valve::g_cvar->con_print( true, 0xff00ffffu,
										std::vformat( xorstr_( "[ untitled ] corrected anim_side from {} to {}.\n" ), std::make_format_args( wrong_anim_side, new_anim_side ) ).data( )
									);

									for ( auto& lag_record : shot.m_entry->m_lag_records ) {
										if ( !lag_record->m_trying_to_resolve
											|| lag_record->m_anim_side != wrong_anim_side )
											continue;

										lag_record->m_anim_side = new_anim_side;
									}
								}
							}
						}
					}
					else {
						// @todo: add occlusion log will do that very late in the shit since nemesis didn't have issue with occlusion iirc i might be wrong though.
						valve::g_cvar->con_print( true, 0xffccffffu, xorstr_( "[ untitled ] shot missed due to spread.\n" ) );
					}
				}

				lag_backup.restore( shot.m_entry->m_player );
			}

			shot.m_processed = true;
		}
	}

	constexpr std::size_t hash( const char* str ) {
		auto hash = 0x811c9dc5u;

		char v4{};
		do {
			v4 = *str++;
			hash = 0x1000193u * ( hash ^ v4 );
		} while ( v4 );

		return hash;
	}

	void c_shots::on_new_event( valve::game_event_t* const event ) {
		if ( !valve::g_local_player )
			return;

		switch ( util::hash( event->name( ) ) ) {
		case 0xf8dba51u/* player_footstep */: {
			const auto& cfg = g_visuals->cfg( );
			if ( !cfg.m_player.m_sounds )
				return;

			const auto entity = valve::g_entity_list->find_entity( valve::g_engine->index_for_uid( event->get_int( xorstr_( "userid" ) ) ) );
			if ( !entity
				|| entity->dormant( )
				|| entity == valve::g_local_player
				|| ( entity->friendly( ) && !cfg.m_player.m_teammates ) )
				return;

			g_visuals->player_sounds( ).emplace_back( entity->origin( ) );
		} break;
		case 0xbded60d0u/* player_hurt */: {
			if ( valve::g_engine->index_for_uid( event->get_int( xorstr_( "attacker" ) ) ) != valve::g_local_player->index( ) )
				return;

			const auto victim = valve::g_entity_list->find_entity( valve::g_engine->index_for_uid( event->get_int( xorstr_( "userid" ) ) ) );
			if ( !victim )
				return;

			const auto hitgroup = event->get_int( xorstr_( "hitgroup" ) );
			if ( hitgroup == 10 )
				return;

			if ( const auto player_info = valve::g_engine->player_info( victim->index( ) ); player_info.has_value( ) )
				valve::g_cvar->con_print( true, 0xff00ff00u,
					std::vformat( xorstr_( "[ untitled ] hurt {} for {} in {}.\n" ), std::make_format_args( player_info.value( ).m_name, event->get_int( xorstr_( "dmg_health" ) ), translate_hitgroup( hitgroup ) ) ).data( )
				);

			if ( g_visuals->cfg( ).m_player.m_hit_marker_sound ) /* idc */
				valve::g_engine->exec_cmd( xorstr_( "play buttons\\arena_switch_press_02.wav" ) );

			g_visuals->last_hurt_time( ) = valve::g_global_vars->m_cur_time;

			const auto shot = last_unprocessed( );
			if ( !shot
				|| ( shot->m_entry && shot->m_target_index != victim->index( ) ) )
				return;

			shot->m_server_info.m_hitgroup = hitgroup;
			shot->m_server_info.m_hurt_tick = valve::g_client_state->m_server_tick;
		} break;
		case 0xe64effdau/* weapon_fire */: {
			if ( valve::g_engine->index_for_uid( event->get_int( xorstr_( "userid" ) ) ) != valve::g_local_player->index( ) )
				return;

			if ( m_elements.empty( ) )
				return;

			const auto shot = std::find_if(
				m_elements.begin( ), m_elements.end( ),
				[ ] ( const shot_t& shot ) {
					return shot.m_cmd_number != -1 && !shot.m_server_info.m_fire_tick
						&& std::abs( valve::g_client_state->m_cmd_ack - shot.m_cmd_number ) <= 17;
				}
			);

			if ( shot == m_elements.end( ) )
				return;

			shot->m_process_tick = valve::g_global_vars->m_tick_count + 1;
			shot->m_server_info.m_fire_tick = valve::g_client_state->m_server_tick;
		} break;
		case 0x9b5f9138u/* bullet_impact */: {
			if ( valve::g_engine->index_for_uid( event->get_int( xorstr_( "userid" ) ) ) != valve::g_local_player->index( ) )
				return;

			const vec3_t impact_pos{
				event->get_float( xorstr_( "x" ) ),
				event->get_float( xorstr_( "y" ) ),
				event->get_float( xorstr_( "z" ) )
			};

			if ( const auto shot = last_unprocessed( ) )
				shot->m_server_info.m_impact_pos = impact_pos;

			const auto& cfg = g_visuals->cfg( );
			if ( !cfg.m_effects.m_bullet_tracers
				&& !cfg.m_effects.m_bullet_impacts )
				return;

			auto& vis_impacts = g_visuals->bullet_impacts( );

			if ( !vis_impacts.empty( )
				&& vis_impacts.back( ).m_time == valve::g_global_vars->m_cur_time )
				vis_impacts.back( ).m_final = false;

			vis_impacts.emplace_back(
				valve::g_global_vars->m_cur_time,
				valve::g_local_player->origin( ) + valve::g_local_player->view_offset( ),
				impact_pos
			);
		} break;
		case 0x19180a27u/* round_freeze_end */: g_ctx->freeze_time( ) = false; break;
		case 0x2301969du/* round_prestart */:
			g_ctx->freeze_time( ) = true;

			for ( std::size_t i{}; i < 64u; ++i ) {
				auto& entry = g_lag_comp->entry( i );

				entry.m_misses = 0;
				entry.m_try_anim_resolver = true;
			}

			break;
		}
	}
}