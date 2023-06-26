#include "../../../inc.hpp"

namespace hacks {
	void c_lag_comp::on_net_update( ) {
		if ( !valve::g_local_player 
			|| !valve::g_engine->in_game( ) )
			return;

		const auto time_limit = valve::to_time( valve::g_client_state->m_server_tick ) - g_ctx->cvars( ).m_sv_maxunlag->get_float( );

		for ( auto i = 1; i <= valve::g_global_vars->m_max_clients; ++i ) {
			auto& entry = m_entries.at( i - 1 );

			const auto player = static_cast< valve::c_player* >(
				valve::g_entity_list->find_entity( i )
			);

			if ( player == valve::g_local_player ) {
				entry.reset( );

				continue;
			}

			if ( entry.m_player != player )
				entry.reset( );

			entry.m_player = player;

			if ( !player
				|| !player->alive( ) ) {
				entry.reset( );

                entry.m_player = player;

				continue;
			}

			if ( player->dormant( ) ) {
				entry.m_prev_anim_data = std::nullopt;
				entry.m_try_anim_resolver = true;
				entry.m_misses = 0;

				if ( entry.m_lag_records.empty( ) ) {
					entry.m_lag_records.emplace_back(
						std::make_shared< lag_record_t >( player ) 
					);

					continue;
				}

				if ( !entry.m_lag_records.back( )->m_dormant ) {
					entry.m_lag_records.clear( );

					entry.m_lag_records.emplace_back(
						std::make_shared< lag_record_t >( player )
					);

					continue;
				}

				continue;
			}

			const auto anim_state = player->anim_state( );
			if ( !anim_state ) {
				entry.reset( );

				continue;
			}

			if ( player->sim_time( ) < player->old_sim_time( ) )
				continue;

			const auto& cur_alive_loop_cycle = player->anim_layers( ).at( 11 ).m_cycle;
			if ( cur_alive_loop_cycle == entry.m_alive_loop_cycle ) {
				player->sim_time( ) = player->old_sim_time( );

				continue;
			}

			if ( player->sim_time( ) < entry.m_last_sim
				&& ( entry.m_last_sim != 0.f ) )
				entry.m_broke_sim = player->sim_time( );

			bool invalid_processing{};
			if ( entry.m_broke_sim >= player->sim_time( ) )
				invalid_processing = true;

			if ( !invalid_processing )
				entry.m_last_sim = player->sim_time( );

			entry.m_lag_records.erase(
				std::remove_if( 
					entry.m_lag_records.begin( ), entry.m_lag_records.end( ), 
					[ & ] ( const std::shared_ptr< lag_record_t >& lag_record ) -> bool {
						return lag_record->m_sim_time < time_limit;
					} 
				),
				entry.m_lag_records.end( )
			);

			entry.m_alive_loop_cycle = cur_alive_loop_cycle;

			entry.m_receive_time = valve::g_global_vars->m_real_time;

			if ( entry.m_spawn_time != player->spawn_time( ) ) {
				anim_state->reset( );

				entry.m_try_anim_resolver = true;
				entry.m_misses = entry.m_prev_anim_side = 0;
				entry.m_prev_anim_data = std::nullopt;
				entry.m_last_sim = 0.f;

				entry.m_lag_records.clear( );
			}

			entry.m_spawn_time = player->spawn_time( );

			lag_anim_data_t* previous{};
			if ( entry.m_prev_anim_data.has_value( ) )
				previous = &entry.m_prev_anim_data.value( );

			const auto current = entry.m_lag_records.emplace_back(
				std::make_shared< lag_record_t >( player )
			).get( );

			current->m_invalid = invalid_processing;

			current->correct( previous );

			g_anim_sync->on_net_update( entry, current, previous );

			entry.m_prev_anim_data.emplace( current );
		}
	}
}