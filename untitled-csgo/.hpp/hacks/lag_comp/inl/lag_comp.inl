#pragma once

#include "../lag_comp.hpp"

namespace hacks {
	__forceinline lag_backup_t::lag_backup_t( valve::c_player* const player ) {
		if ( const auto anim_state = player->anim_state( ) )
			m_foot_yaw = anim_state->m_foot_yaw;

		m_origin = player->origin( );
		m_abs_origin = player->get_abs_origin( );

		m_obb_min = player->obb_min( );
		m_obb_max = player->obb_max( );

		const auto& bone_accessor = player->bone_accessor( );

		m_readable_bones = bone_accessor.m_readable_bones;
		m_writable_bones = bone_accessor.m_writable_bones;

		const auto& bone_cache = player->bone_cache( );

		std::memcpy(
			m_bones.data( ),
			bone_cache.m_mem.m_ptr,
			bone_cache.m_size * sizeof( mat3x4_t )
		);

		m_bones_count = bone_cache.m_size;

		m_mdl_bone_counter = player->mdl_bone_counter( );
	}

	__forceinline void lag_backup_t::restore( valve::c_player* const player ) const {
		player->origin( ) = m_origin;

		player->set_abs_origin( m_abs_origin );

		player->set_collision_bounds( m_obb_min, m_obb_max );

		player->set_abs_angles( { 0.f, m_foot_yaw, 0.f } );

		auto& bone_accessor = player->bone_accessor( );

		bone_accessor.m_readable_bones = m_readable_bones;
		bone_accessor.m_writable_bones = m_writable_bones;

		std::memcpy(
			player->bone_cache( ).m_mem.m_ptr,
			m_bones.data( ), m_bones_count * sizeof( mat3x4_t )
		);

		player->mdl_bone_counter( ) = m_mdl_bone_counter;
	}

	__forceinline void lag_record_t::restore( valve::c_player* const player, const int anim_index, const bool only_anim ) const {
		if ( !only_anim ) {
			player->origin( ) = m_origin;

			player->set_abs_origin( m_origin );

			player->set_collision_bounds( m_obb_min, m_obb_max );
		}

		const auto& anim_side = m_anim_sides.at( anim_index );

		player->set_abs_angles( { 0.f, anim_side.m_foot_yaw, 0.f } );

		std::memcpy(
			player->bone_cache( ).m_mem.m_ptr,
			anim_side.m_bones.data( ), anim_side.m_bones_count * sizeof( mat3x4_t )
		);

		player->mdl_bone_counter( ) = **reinterpret_cast< unsigned long** >(
			g_ctx->addresses( ).m_invalidate_bone_cache + 0xau
		);
	}

	__forceinline void lag_record_t::correct( const lag_anim_data_t* const previous ) {
		/* tf reisw */
		if ( !previous
			|| ( ( m_origin - previous->m_origin ).length_sqr( ) > 4096.f ) )
			m_broke_lc = true;

		if ( std::abs( m_third_person_recoil + m_eye_angles.x - 180.f ) < 0.1f )
			m_eye_angles.x = 89.f;

		const auto& cur_alive_loop_layer = m_anim_layers.at( 11u );

		if ( !previous
			|| previous->m_dormant ) {
			if ( !previous ) {
				if ( ( m_flags & valve::e_ent_flags::on_ground ) ) {
					auto max_speed = m_weapon ? std::max( 0.1f, m_weapon->max_speed( ) ) : 260.f;

					if ( m_anim_layers.at( 6u ).m_weight > 0.f && m_anim_layers.at( 6u ).m_playback_rate > 0.f
						&& m_velocity.length_2d( ) > 0.f ) {
						if ( ( m_flags & valve::e_ent_flags::ducking ) )
							max_speed *= 0.34f;
						else if ( m_walking )
							max_speed *= 0.52f;

						const auto move_multiplier = m_anim_layers.at( 6u ).m_weight * max_speed;
						const auto speed_multiplier = move_multiplier / m_velocity.length_2d( );

						m_velocity.x *= speed_multiplier;
						m_velocity.y *= speed_multiplier;
					}
				}
			}

			return;
		}

		if ( previous && !previous->m_dormant ) {
			const auto& prev_alive_loop_layer = previous->m_anim_layers.at( 11u );

			if ( prev_alive_loop_layer.m_playback_rate == cur_alive_loop_layer.m_playback_rate ) {
				if ( previous->m_weapon == m_weapon
					&& cur_alive_loop_layer.m_playback_rate > 0.f
					&& cur_alive_loop_layer.m_cycle > prev_alive_loop_layer.m_cycle ) {
					const auto v16 = m_sim_ticks + 1;
					const auto v17 = valve::to_ticks(
						( cur_alive_loop_layer.m_cycle - prev_alive_loop_layer.m_cycle )
						/ cur_alive_loop_layer.m_playback_rate
					);

					if ( v17 > v16
						&& v17 <= 19 )
						m_total_cmds = v17;
				}
			}

			const auto total_cmds_time = valve::to_time( m_total_cmds );
			if ( total_cmds_time > 0.f
				&& total_cmds_time < 1.f )
				m_velocity = ( m_origin - previous->m_origin ) / total_cmds_time;

			if ( m_total_cmds >= 2 ) {
				const auto anim_time = m_sim_time - total_cmds_time;

				if ( m_flags & valve::e_ent_flags::on_ground ) {
					if ( !( previous->m_flags & valve::e_ent_flags::on_ground ) ) {
						const auto& cycle = m_anim_layers.at( 5u ).m_cycle;
						if ( cycle < 0.999f ) {
							const auto playback_rate = m_anim_layers.at( 5u ).m_playback_rate;
							if ( playback_rate > 0.f ) {
								const auto land_time = m_sim_time - ( cycle / playback_rate );
								if ( land_time > anim_time
									&& m_sim_time <= land_time ) {
									m_on_ground_time = land_time;
									m_jumped = false;
								}
							}
						}
					}
				}
				else {
					if ( !( previous->m_flags & valve::e_ent_flags::on_ground ) ) {
						const auto& cycle = m_anim_layers.at( 4u ).m_cycle;
						if ( cycle < 0.999f ) {
							const auto playback_rate = m_anim_layers.at( 4u ).m_playback_rate;
							if ( playback_rate > 0.f ) {
								const auto on_ground_time = m_sim_time - ( cycle / playback_rate );
								if ( on_ground_time > anim_time
									&& m_sim_time <= on_ground_time ) {
									m_on_ground_time = on_ground_time;
									m_jumped = true;
								}
							}
						}
					}
				}
			}
		}

		if ( m_weapon
			&& m_last_shot_time > ( m_sim_time - valve::to_time( m_total_cmds ) )
			&& m_sim_time >= m_last_shot_time )
			if ( const auto wpn_data = m_weapon->wpn_data( ); wpn_data && ( static_cast< std::size_t >( wpn_data->m_unk_type - 2 ) <= 5 || m_weapon->item_index( ) == valve::e_item_index::taser ) )
				m_shot = true;

		// s-o esoterik for his gay ass velocity fix
		if ( m_total_cmds >= 2 ) {
			if ( ( m_flags & valve::e_ent_flags::on_ground )
				&& ( !previous || ( previous->m_flags & valve::e_ent_flags::on_ground ) ) ) {
				if ( m_anim_layers.at( 6u ).m_playback_rate == 0.f )
					m_velocity = {};
				else {
					if ( m_weapon == previous->m_weapon ) {
						if ( cur_alive_loop_layer.m_weight > 0.f && cur_alive_loop_layer.m_weight < 1.f ) {
							const auto speed_2d = m_velocity.length_2d( );
							const auto max_speed = m_weapon ? std::max( 0.1f, m_weapon->max_speed( ) ) : 260.f;
							if ( speed_2d ) {
								const auto v23 = ( 1.f - cur_alive_loop_layer.m_weight ) * 0.35f;

								if ( v23 > 0.f && v23 < 1.f ) {
									const auto speed_as_portion_of_run_top_speed = ( ( v23 + 0.55f ) * max_speed ) / speed_2d;
									if ( speed_as_portion_of_run_top_speed ) {
										m_velocity.x *= speed_as_portion_of_run_top_speed;
										m_velocity.y *= speed_as_portion_of_run_top_speed;
									}
								}
							}
						}
					}
				}
			}

			if ( ( m_flags & valve::e_ent_flags::on_ground )
				&& ( previous->m_flags & valve::e_ent_flags::on_ground ) ) {
				if ( m_anim_layers.at( 6u ).m_playback_rate == 0.f )
					m_velocity = {};
				else {
					if ( m_anim_layers.at( 6u ).m_weight >= 0.1f ) {
						if ( ( cur_alive_loop_layer.m_weight <= 0.f || cur_alive_loop_layer.m_weight >= 1.f )
							&& m_velocity.length_2d( ) > 0.f ) {
							const bool valid_6th_layer = ( m_anim_layers.at( 6u ).m_weight < 1.f )
								&& ( m_anim_layers.at( 6u ).m_weight >= previous->m_anim_layers.at( 6u ).m_weight );
							const auto max_spd = m_weapon ? std::max( 0.1f, m_weapon->max_speed( ) ) : 260.f;

							if ( valid_6th_layer ) {
								const auto max_duck_speed = max_spd * 0.34f;
								const auto speed_multiplier = std::max( 0.f, ( max_spd * 0.52f ) - ( max_spd * 0.34f ) );
								const auto duck_modifier = 1. - m_duck_amount;
								const auto speed_via_6th_layer = ( ( ( duck_modifier * speed_multiplier ) + max_duck_speed ) * m_anim_layers.at( 6u ).m_weight ) / m_velocity.length_2d( );

								m_velocity.x *= speed_via_6th_layer;
								m_velocity.y *= speed_via_6th_layer;
							}
						}
					}
					else {
						if ( m_anim_layers.at( 6u ).m_weight ) {
							auto weight = m_anim_layers.at( 6 ).m_weight;
							const auto length_2d = m_velocity.length_2d( );

							if ( m_flags & valve::e_ent_flags::ducking )
								weight *= 0.34f;
							else {
								if ( m_walking ) {
									weight *= 0.52f;
								}
							}
							if ( length_2d ) {
								m_velocity.x = ( m_velocity.x / length_2d ) * weight;
								m_velocity.y = ( m_velocity.y / length_2d ) * weight;
							}
						}
					}
				}
			}
		}
	}

	__forceinline bool lag_record_t::valid( ) const {
		if ( g_lag_comp->calc_time_delta( m_sim_time ) >= 0.2f )
			return false;

		return true;
	}

	__forceinline lag_anim_data_t::lag_anim_data_t( const lag_record_t* const lag_record ) {
		m_weapon = lag_record->m_weapon;

		m_eye_angles = lag_record->m_eye_angles;
		m_origin = lag_record->m_origin;
		m_velocity = lag_record->m_velocity;

		m_flags = lag_record->m_flags;
		m_sim_time = lag_record->m_sim_time;
		m_lby = lag_record->m_lby;
		m_duck_amount = lag_record->m_duck_amount;

		m_dormant = lag_record->m_dormant;

		m_anim_layers = lag_record->m_anim_layers;

		for ( std::size_t i{}; i < m_anim_sides.size( ); ++i ) {
			auto& to = m_anim_sides.at( i );
			const auto& from = lag_record->m_anim_sides.at( i );

			to.m_foot_yaw = from.m_foot_yaw;
			to.m_prev_foot_yaw = from.m_prev_foot_yaw;
			to.m_move_yaw_ideal = from.m_move_yaw_ideal;
			to.m_move_yaw_cur_to_ideal = from.m_move_yaw_cur_to_ideal;
			to.m_move_yaw = from.m_move_yaw;
		}
	}

	__forceinline void player_entry_t::reset( ) {
		m_player = nullptr;

		m_alive_loop_cycle = -1.f;
		m_misses = m_prev_anim_side = 0;
		m_prev_anim_data = std::nullopt;
		m_try_anim_resolver = true;

		m_lag_records.clear( );
	}

	__forceinline void player_entry_t::interpolate(
		const lag_anim_data_t* const from, const lag_record_t* const to
	) {
		if ( !from
			|| to->m_total_cmds <= 1
			|| to->m_total_cmds >= 20 ) {
			m_interpolated.emplace_back(
				to->m_sim_time, to->m_duck_amount,
				to->m_flags, to->m_velocity
			);

			return;
		}

		m_interpolated.reserve( to->m_total_cmds );

		const auto anim_time = to->m_sim_time - valve::to_time( to->m_total_cmds );
		
		const auto duck_amount_delta = to->m_duck_amount - from->m_duck_amount;
		const auto velocity_delta = to->m_velocity - from->m_velocity;

		const auto interpolate_velocity =
			to->m_anim_layers.at( 6u ).m_playback_rate == 0.f || from->m_anim_layers.at( 6u ).m_playback_rate == 0.f
			|| ( ( to->m_velocity.length_2d( ) >= 1.1f ) && ( from->m_velocity.length_2d( ) >= 1.1f ) );

		for ( auto i = 1; i <= to->m_total_cmds; ++i ) {
			if ( i == to->m_total_cmds ) {
				m_interpolated.emplace_back(
					to->m_sim_time, to->m_duck_amount,
					to->m_flags, to->m_velocity
				);

				break;
			}

			const auto lerp = i / static_cast< float >( to->m_total_cmds );

			auto& interpolated = m_interpolated.emplace_back( );

			interpolated.m_flags = to->m_flags;

			if ( to->m_jumped.has_value( ) ) {
				interpolated.m_flags &= ~valve::e_ent_flags::on_ground;

				if ( from->m_flags & valve::e_ent_flags::on_ground )
					interpolated.m_flags |= valve::e_ent_flags::on_ground;
			}

			interpolated.m_sim_time = anim_time + valve::to_time( i );
			interpolated.m_duck_amount = from->m_duck_amount + duck_amount_delta * lerp;

			if ( interpolate_velocity )
				interpolated.m_velocity = from->m_velocity + velocity_delta * lerp;
			else
				interpolated.m_velocity = { ( i & 1 ) ? 1.1f : -1.1f, 0.f, 0.f };

			if ( to->m_jumped.has_value( ) ) {
				if ( to->m_jumped.value( ) ) {
					if ( ( valve::to_ticks( interpolated.m_sim_time ) + 1 ) == valve::to_ticks( to->m_on_ground_time ) )
						interpolated.m_flags |= valve::e_ent_flags::on_ground;
				}
				else if ( interpolated.m_sim_time >= to->m_on_ground_time )
					interpolated.m_flags |= valve::e_ent_flags::on_ground;
			}
		}
	}

	__forceinline float c_lag_comp::calc_time_delta( const float sim_time ) const {
		const auto& net_info = g_ctx->net_info( );

		const auto correct = std::clamp(
			net_info.m_lerp + net_info.m_latency.m_in + net_info.m_latency.m_out,
			0.f, g_ctx->cvars( ).m_sv_maxunlag->get_float( )
		);

		auto tick_base = valve::g_local_player->tick_base( );
		if ( g_exploits->next_shift_amount( ) > 0 )
			tick_base -= g_exploits->next_shift_amount( );

		return std::abs( correct - ( valve::to_time( tick_base ) - sim_time ) );
	}

	__forceinline player_entry_t& c_lag_comp::entry( const std::size_t i ) { return m_entries.at( i ); }
}