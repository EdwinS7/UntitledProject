#include "../../../inc.hpp"

namespace hacks {
	void c_movement::accelerate(
		const valve::user_cmd_t& user_cmd, const vec3_t& wishdir,
		const float wishspeed, vec3_t& velocity, float acceleration
	) const {
		const auto cur_speed = velocity.dot( wishdir );

		const auto add_speed = wishspeed - cur_speed;
		if ( add_speed <= 0.f )
			return;

		const auto v57 = std::max( cur_speed, 0.f );

		const auto ducking =
			user_cmd.m_buttons & valve::e_buttons::in_duck
			|| valve::g_local_player->ducking( )
			|| valve::g_local_player->flags( ) & valve::e_ent_flags::ducking;

		auto v20 = true;
		if ( ducking
			|| !( user_cmd.m_buttons & valve::e_buttons::in_speed ) )
			v20 = false;

		auto finalwishspeed = std::max( wishspeed, 250.f );
		auto abs_finalwishspeed = finalwishspeed;

		bool slow_down_to_fast_nigga{};

		if ( g_ctx->weapon( )
			&& g_ctx->cvars( ).m_sv_accelerate_use_weapon_speed->get_bool( ) ) {
			const auto item_index = static_cast< std::uint16_t >( g_ctx->weapon( )->item_index( ) );
			if ( g_ctx->weapon( )->zoom_lvl( ) > 0
				&& ( item_index == 11 || item_index == 38 || item_index == 9 || item_index == 8 || item_index == 39 || item_index == 40 ) )
				slow_down_to_fast_nigga = ( m_max_weapon_speed * 0.52f ) < 110.f;

			const auto modifier = std::min( 1.f, m_max_weapon_speed / 250.f );

			abs_finalwishspeed *= modifier;

			if ( ( !ducking && !v20 )
				|| slow_down_to_fast_nigga )
				finalwishspeed *= modifier;
		}

		if ( ducking ) {
			if ( !slow_down_to_fast_nigga )
				finalwishspeed *= 0.34f;

			abs_finalwishspeed *= 0.34f;
		}

		if ( v20 ) {
			if ( !slow_down_to_fast_nigga )
				finalwishspeed *= 0.52f;

			abs_finalwishspeed *= 0.52f;

			const auto abs_finalwishspeed_minus5 = abs_finalwishspeed - 5.f;
			if ( v57 < abs_finalwishspeed_minus5 ) {
				const auto v30 =
					std::max( v57 - abs_finalwishspeed_minus5, 0.f )
					/ std::max( abs_finalwishspeed - abs_finalwishspeed_minus5, 0.f );

				const auto v27 = 1.f - v30;
				if ( v27 >= 0.f )
					acceleration = std::min( v27, 1.f ) * acceleration;
				else
					acceleration = 0.f;
			}
		}

		const auto v33 = std::min(
			add_speed,
			( ( valve::g_global_vars->m_interval_per_tick * acceleration ) * finalwishspeed )
			* valve::g_local_player->surface_friction( )
		);

		velocity += wishdir * v33;

		const auto len = velocity.length( );
		if ( len
			&& len > m_max_weapon_speed )
			velocity *= m_max_weapon_speed / len;
	}

	void c_movement::walk_move(
		const valve::user_cmd_t& user_cmd, vec3_t& move,
		vec3_t& fwd, vec3_t& right, vec3_t& velocity
	) const {
		if ( fwd.z != 0.f )
			fwd.normalize( );

		if ( right.z != 0.f )
			right.normalize( );

		vec3_t wishvel{
			fwd.x * move.x + right.x * move.y,
			fwd.y * move.x + right.y * move.y,
			0.f
		};

		auto wishdir = wishvel;

		auto wishspeed = wishdir.normalize( );
		if ( wishspeed
			&& wishspeed > m_max_player_speed ) {
			wishvel *= m_max_player_speed / wishspeed;

			wishspeed = m_max_player_speed;
		}

		velocity.z = 0.f;
		accelerate( user_cmd, wishdir, wishspeed, velocity, g_ctx->cvars( ).m_sv_accelerate->get_float( ) );
		velocity.z = 0.f;

		const auto speed_sqr = velocity.length_sqr( );
		if ( speed_sqr > ( m_max_player_speed * m_max_player_speed ) )
			velocity *= m_max_player_speed / std::sqrt( speed_sqr );

		if ( velocity.length( ) < 1.f )
			velocity = {};
	}

	void c_movement::full_walk_move(
		const valve::user_cmd_t& user_cmd, vec3_t& move,
		vec3_t& fwd, vec3_t& right, vec3_t& velocity
	) const {
		if ( valve::g_local_player->ground_entity( ) != valve::e_ent_handle::invalid ) {
			velocity.z = 0.f;

			const auto speed = velocity.length( );
			if ( speed >= 0.1f ) {
				const auto friction = g_ctx->cvars( ).m_sv_friction->get_float( ) * valve::g_local_player->surface_friction( );
				const auto sv_stopspeed = g_ctx->cvars( ).m_sv_friction->get_float( );
				const auto control = speed < sv_stopspeed ? sv_stopspeed : speed;

				const auto new_speed = std::max( 0.f, speed - ( ( control * friction ) * valve::g_global_vars->m_interval_per_tick ) );
				if ( speed != new_speed )
					velocity *= new_speed / speed;
			}

			walk_move( user_cmd, move, fwd, right, velocity );

			velocity.z = 0.f;
		}

		const auto sv_maxvelocity = g_ctx->cvars( ).m_sv_maxvelocity->get_float( );
		for ( std::size_t i{}; i < 3u; ++i ) {
			auto& element = velocity[ i ];

			if ( element > sv_maxvelocity )
				element = sv_maxvelocity;
			else if ( element < -sv_maxvelocity )
				element = -sv_maxvelocity;
		}
	}

	void c_movement::modify_move( valve::user_cmd_t& user_cmd, vec3_t& velocity ) const {
		vec3_t fwd{}, right{};

		math::angle_vectors( user_cmd.m_view_angles, &fwd, &right );

		const auto speed_sqr = user_cmd.m_move.length_sqr( );
		if ( speed_sqr > ( m_max_player_speed * m_max_player_speed ) )
			user_cmd.m_move *= m_max_player_speed / std::sqrt( speed_sqr );

		full_walk_move( user_cmd, user_cmd.m_move, fwd, right, velocity );
	}

	void c_movement::predict_move( const valve::user_cmd_t& user_cmd, vec3_t& velocity ) const {
		vec3_t fwd{}, right{};

		math::angle_vectors( user_cmd.m_view_angles, &fwd, &right );

		auto move = user_cmd.m_move;

		const auto speed_sqr = user_cmd.m_move.length_sqr( );
		if ( speed_sqr > ( m_max_player_speed * m_max_player_speed ) )
			move *= m_max_player_speed / std::sqrt( speed_sqr );

		full_walk_move( user_cmd, move, fwd, right, velocity );
	}

	void c_movement::auto_strafe( valve::user_cmd_t& user_cmd ) {
		if ( !m_cfg->m_auto_strafe_type
			|| ( m_cfg->m_auto_strafe_type != 2 && ( user_cmd.m_move.x != 0.f || user_cmd.m_move.y != 0.f ) )
			|| valve::g_local_player->flags( ) & valve::e_ent_flags::on_ground )
			return;

		const auto& velocity = valve::g_local_player->velocity( );

		auto wish_angles = user_cmd.m_view_angles;

		if ( m_cfg->m_auto_strafe_type == 2
			&& ( user_cmd.m_move.x != 0.f || user_cmd.m_move.y != 0.f ) )
			wish_angles.y = std::remainder(
				wish_angles.y
				+ std::remainder(
					math::to_deg(
						std::atan2( user_cmd.m_move.x, user_cmd.m_move.y )
					) - 90.f, 360.f
				), 360.f
			);

		user_cmd.m_move.x = user_cmd.m_move.y = 0.f;

		const auto speed_2d = velocity.length_2d( );

		const auto ideal_strafe = std::min( 90.f, math::to_deg( std::asin( 15.f / speed_2d ) ) );

		const auto mult = m_strafe_switch ? 1.f : -1.f;

		m_strafe_switch = !m_strafe_switch;

		auto delta = std::remainder( wish_angles.y - m_prev_view_yaw, 360.f );
		if ( delta )
			user_cmd.m_move.y = delta < 0.f ? 450.f : -450.f;

		delta = std::abs( delta );

		if ( delta >= 30.f
			|| ideal_strafe >= delta ) {
			const auto vel_angle = math::to_deg( std::atan2( velocity.y, velocity.x ) );
			const auto vel_delta = std::remainder( wish_angles.y - vel_angle, 360.f );

			if ( speed_2d <= 15.f
				|| ideal_strafe >= vel_delta ) {
				if ( speed_2d <= 15.f
					|| vel_delta >= -ideal_strafe ) {
					user_cmd.m_move.y = 450.f * mult;
					wish_angles.y += ideal_strafe * mult;
				}
				else {
					user_cmd.m_move.y= 450.f;
					wish_angles.y = vel_angle - ideal_strafe;
				}
			}
			else {
				user_cmd.m_move.y = -450.f;
				wish_angles.y = vel_angle + ideal_strafe;
			}

			rotate(
				user_cmd, wish_angles,
				valve::g_local_player->flags( ),
				valve::g_local_player->move_type( )
			);
		}
	}

	bool c_movement::is_peeking( int ticks ) {
		bool found{ false };

		if ( !valve::g_local_player || !g_ctx->weapon( ) )
			return false;

		if ( valve::g_local_player->velocity( ).length_2d( ) < 7.f )
			return false;

		for ( int i = 1; i <= valve::g_global_vars->m_max_clients; ++i ) {
			if ( !valve::g_local_player || !valve::g_local_player->alive( ) )
				continue;

			auto entry = hacks::g_lag_comp->entry( i - 1 );
			if ( !entry.m_player || !entry.m_player->alive( )
				|| entry.m_player == valve::g_local_player 
				|| entry.m_player->dormant( ) || entry.m_player->friendly( ) )
				continue;

			if ( entry.m_lag_records.empty( ) )
				continue;

			auto& lag_record = entry.m_lag_records.back( );
			if ( !lag_record )
				continue;

			aim_target_t target{};

			target.m_entry = const_cast< player_entry_t* >( &entry );
			target.m_lag_record = lag_record;

			vec3_t next_shot_pos = g_ctx->shoot_pos( ) + ( valve::g_local_player->velocity( ) * valve::g_global_vars->m_interval_per_tick ) * ticks;
			g_aim_bot->scan_points( target, ( 1 << 1 | 1 << 3 | 1 << 4 ), ( 1 << 1 | 1 << 3 | 1 << 4 ), true, next_shot_pos );

			if ( g_aim_bot->select_points( target, false ) ) {
				found = true;
				break;
			}
		}

		return found;
	}

	void c_movement::fast_stop( valve::user_cmd_t& user_cmd ) {
		if ( !m_cfg->m_fast_stop ||
			valve::g_local_player->move_type( ) == valve::e_move_type::ladder ||
			( user_cmd.m_buttons & valve::e_buttons::in_move_left ) ||
			( user_cmd.m_buttons & valve::e_buttons::in_move_right ) ||
			( user_cmd.m_buttons & valve::e_buttons::in_forward ) ||
			( user_cmd.m_buttons & valve::e_buttons::in_back ) ||
			!( valve::g_local_player->flags( ) & valve::e_ent_flags::on_ground ) )
			return;

		float max_speed = g_ctx->weapon( ) ? g_ctx->weapon( )->max_speed( ) : 260.f;

		if ( valve::g_local_player->velocity( ).length_2d( ) <= max_speed * 0.34f ) {
			user_cmd.m_move.x = user_cmd.m_move.y = 0.f;
			return;
		}

		qangle_t res_ang{};

		math::vector_angles( valve::g_local_player->velocity( ) * -1.f, res_ang );

		res_ang.y = user_cmd.m_view_angles.y - res_ang.y;
		res_ang.x = user_cmd.m_view_angles.x - res_ang.x;

		vec3_t res_vec{};

		math::angle_vectors( res_ang, &res_vec );

		user_cmd.m_move.x = std::clamp( res_vec.x, -450.f, 450.0f );
		user_cmd.m_move.y = std::clamp( res_vec.y, -450.f, 450.0f );
	}

	bool c_movement::stop( valve::user_cmd_t& user_cmd ) {
		static qangle_t wish_ang{};
		bool predict_available{};

		if ( g_ctx->weapon( ) ) {
			if ( g_ctx->wpn_data( ) ) {
				if ( g_ctx->wpn_data( )->m_type == 8
					|| g_ctx->wpn_data( )->m_type == 7
					|| g_ctx->wpn_data( )->m_type == 0 )
					return false;
			}
		}

		if ( g_ctx->flags( ) & e_context_flags::can_shoot
			&& hacks::g_aim_bot->cfg( ).m_predictive_auto_stop
			&& m_allow_early
			&& is_peeking( 2 ) )
			predict_available = true;

		if ( !predict_available )
			if ( !hacks::g_movement->stop_type( ) )
				return false;

		auto stop_type = hacks::g_movement->stop_type( );

		hacks::g_movement->stop_type( ) = 0;

		if ( predict_available )
			stop_type = 1;

		if ( !( valve::g_local_player->flags( ) & valve::e_ent_flags::on_ground )
			|| user_cmd.m_buttons & valve::e_buttons::in_jump )
			return false;
		
		if ( !g_ctx->weapon( ) )
			return false;

		if ( !g_ctx->wpn_data( ) )
			return false;

		auto max_speed = valve::g_local_player->scoped( ) ?
				g_ctx->wpn_data( )->m_max_speed_alt :
				g_ctx->wpn_data( )->m_max_speed;

		float target_speed{};

		if ( stop_type == 1 )
			target_speed = 36.5f;

		vec3_t cur_velocity{ valve::g_local_player->velocity( ) };

		if ( predict_available )
			predict_move( user_cmd, cur_velocity );

		const auto speed_2d = cur_velocity.length_2d( );

		if ( stop_type == 2 ) {
			if ( speed_2d < 1.f ) {
				user_cmd.m_move.x = user_cmd.m_move.y = 0.f;

				return false;
			}
		}
		else if ( target_speed >= speed_2d ) {
			m_max_player_speed = m_max_weapon_speed = target_speed;

			modify_move( user_cmd, cur_velocity );

			return false;
		}

		user_cmd.m_buttons &= ~valve::e_buttons::in_speed;

		auto finalwishspeed = std::min( max_speed, 250.f );

		const auto ducking =
			user_cmd.m_buttons & valve::e_buttons::in_duck
			|| valve::g_local_player->flags( ) & valve::e_ent_flags::ducking;

		bool slow_down_to_fast_nigga{};

		if ( g_ctx->cvars( ).m_sv_accelerate_use_weapon_speed->get_int( ) ) {
			const auto item_index = static_cast< std::uint16_t >( g_ctx->weapon( )->item_index( ) );
			if ( g_ctx->weapon( )->zoom_lvl( ) > 0
				&& ( item_index == 11 || item_index == 38 || item_index == 9 || item_index == 8 || item_index == 39 || item_index == 40 ) )
				slow_down_to_fast_nigga = ( max_speed * 0.52f ) < 110.f;

			if ( !ducking
				|| slow_down_to_fast_nigga )
				finalwishspeed *= std::min( 1.f, max_speed / 250.f );
		}

		if ( ducking
			&& !slow_down_to_fast_nigga )
			finalwishspeed *= 0.34f;

		finalwishspeed =
			( ( valve::g_global_vars->m_interval_per_tick * g_ctx->cvars( ).m_sv_accelerate->get_float( ) ) * finalwishspeed )
			* valve::g_local_player->surface_friction( );

		if ( stop_type == 1 ) {
			if ( max_speed * 0.52f <= speed_2d ) {
				qangle_t dir{};
				math::vector_angles( cur_velocity *= -1.f, dir );

				dir.y = user_cmd.m_view_angles.y - dir.y;

				vec3_t dir_ang_handler{};

				math::angle_vectors( dir, &dir_ang_handler, nullptr, nullptr );

				user_cmd.m_move.x = dir_ang_handler.x * finalwishspeed;
				user_cmd.m_move.y = dir_ang_handler.y * finalwishspeed;
			}
			else {
				m_max_player_speed = m_max_weapon_speed = target_speed;

				modify_move( user_cmd, cur_velocity );
			}
		}
		else {
			qangle_t dir{};
			math::vector_angles( cur_velocity *= -1.f, dir );

			dir.y = user_cmd.m_view_angles.y - dir.y;

			vec3_t dir_ang_handler{};

			math::angle_vectors( dir, &dir_ang_handler, nullptr, nullptr );

			user_cmd.m_move.x = dir_ang_handler.x * finalwishspeed;
			user_cmd.m_move.y = dir_ang_handler.y * finalwishspeed;
		}

		return true;
	}

	bool c_movement::force_lby_update( valve::user_cmd_t& user_cmd ) {
		if ( !g_anti_aim->cfg( ).m_enabled
			|| !g_anti_aim->cfg( ).m_desync 
			|| user_cmd.m_buttons & valve::e_buttons::in_jump
			|| !( valve::g_local_player->flags( ) & valve::e_ent_flags::on_ground ) )
			return false;

		m_max_weapon_speed = 260.f;
		if ( g_ctx->weapon( ) )
			m_max_weapon_speed = g_ctx->weapon( )->max_speed( );

		m_max_player_speed = valve::g_local_player->max_speed( );

		if ( valve::g_local_player->velocity( ).length_2d_sqr( ) > 2.f
			|| !valve::g_local_player->anim_state( ) )
			return false;

		user_cmd.m_buttons &= ~valve::e_buttons::in_speed;

		float duck_amount{};
		if ( user_cmd.m_buttons & valve::e_buttons::in_duck )
			duck_amount = std::min(
				1.f,
				valve::g_local_player->duck_amount( )
				+ ( valve::g_global_vars->m_interval_per_tick * 0.8f ) * valve::g_local_player->duck_speed( )
			);
		else
			duck_amount =
				valve::g_local_player->duck_amount( )
				- std::max( 1.5f, valve::g_local_player->duck_speed( ) ) * valve::g_global_vars->m_interval_per_tick;

		float move{};
		if ( user_cmd.m_buttons & valve::e_buttons::in_duck
			|| valve::g_local_player->ducking( )
			|| valve::g_local_player->flags( ) & valve::e_ent_flags::anim_ducking )
			move = 1.1f / ( ( ( duck_amount * 0.34f ) + 1.f ) - duck_amount );
		else
			move = 1.1f;

		if ( std::abs( user_cmd.m_move.x ) > move
			|| std::abs( user_cmd.m_move.y ) > move )
			return false;

		if ( !( user_cmd.m_number & 1 ) )
			move *= -1.f;

		user_cmd.m_move.x = move;

		return true;
	}

	void c_movement::normalize( valve::user_cmd_t& user_cmd ) const {
		if ( m_cfg->m_anti_untrusted ) {
			user_cmd.m_view_angles.x = std::remainder( user_cmd.m_view_angles.x, 360.f );
			user_cmd.m_view_angles.y = std::remainder( user_cmd.m_view_angles.y, 360.f );
			user_cmd.m_view_angles.z = std::remainder( user_cmd.m_view_angles.z, 360.f );

			user_cmd.m_view_angles.x = std::clamp( user_cmd.m_view_angles.x, -89.f, 89.f );
			user_cmd.m_view_angles.y = std::clamp( user_cmd.m_view_angles.y, -180.f, 180.f );
			user_cmd.m_view_angles.z = std::clamp( user_cmd.m_view_angles.z, -90.f, 90.f );

			user_cmd.m_move.x = std::clamp( user_cmd.m_move.x, -450.f, 450.f );
			user_cmd.m_move.y = std::clamp( user_cmd.m_move.y, -450.f, 450.f );
			user_cmd.m_move.z = std::clamp( user_cmd.m_move.z, -320.f, 320.f );
		}

		if ( valve::g_local_player->move_type( ) != valve::e_move_type::walk )
			return;

		user_cmd.m_buttons &= ~(
			valve::e_buttons::in_forward
			| valve::e_buttons::in_back
			| valve::e_buttons::in_move_right
			| valve::e_buttons::in_move_left
		);

		if ( user_cmd.m_move.x != 0.f )
			user_cmd.m_buttons |=
			( m_cfg->m_slide_walk ? user_cmd.m_move.x < 0.f: user_cmd.m_move.x > 0.f )
			? valve::e_buttons::in_forward : valve::e_buttons::in_back;

		if ( user_cmd.m_move.y == 0.f )
			return;

		user_cmd.m_buttons |=
			( m_cfg->m_slide_walk ? user_cmd.m_move.y < 0.f : user_cmd.m_move.y > 0.f )
			? valve::e_buttons::in_move_right : valve::e_buttons::in_move_left;
	}

	void c_movement::rotate(
		valve::user_cmd_t& user_cmd, const qangle_t& wish_angles,
		const valve::e_ent_flags flags, const valve::e_move_type move_type
	) const {
		if ( user_cmd.m_view_angles.z != 0.f
			&& !( flags & valve::e_ent_flags::on_ground ) )
			user_cmd.m_move.y = 0.f;

		auto move_2d = vec2_t( user_cmd.m_move.x, user_cmd.m_move.y );

		if ( const auto speed_2d = move_2d.length( ) ) {
			const auto delta = user_cmd.m_view_angles.y - wish_angles.y;

			vec2_t v1;

			math::sin_cos(
				math::to_rad(
					std::remainder(
						math::to_deg(
							std::atan2( move_2d.y / speed_2d, move_2d.x / speed_2d )
						) + delta, 360.f
					)
				), v1.x, v1.y
			);

			const auto cos_x = std::cos(
				math::to_rad(
					std::remainder(
						math::to_deg( std::atan2( 0.f, speed_2d ) ), 360.f
					)
				)
			);

			move_2d.x = cos_x * v1.y * speed_2d;
			move_2d.y = cos_x * v1.x * speed_2d;

			if ( move_type == valve::e_move_type::ladder ) {
				if ( wish_angles.x < 45.f
					&& std::abs( delta ) <= 65.f
					&& user_cmd.m_view_angles.x >= 45.f ) {
					move_2d.x *= -1.f;
				}
			}
			else if ( std::abs( user_cmd.m_view_angles.x ) > 90.f )
				move_2d.x *= -1.f;
		}

		user_cmd.m_move.x = move_2d.x;
		user_cmd.m_move.y = move_2d.y;
	
		user_cmd.m_buttons &= ~(
			valve::e_buttons::in_forward
			| valve::e_buttons::in_back
			| valve::e_buttons::in_move_right
			| valve::e_buttons::in_move_left
		);

		if ( move_type == valve::e_move_type::ladder ) {
			if ( std::abs( user_cmd.m_move.x ) > 200.f )
				user_cmd.m_buttons |=
					user_cmd.m_move.x > 0.f
					? valve::e_buttons::in_forward : valve::e_buttons::in_back;

			if ( std::abs( user_cmd.m_move.y ) <= 200.f )
				return;

			user_cmd.m_buttons |=
				user_cmd.m_move.y > 0.f
				? valve::e_buttons::in_move_right : valve::e_buttons::in_move_left;

			return;
		}

		if ( m_cfg->m_slide_walk
			&& move_type == valve::e_move_type::walk ) {
			if ( user_cmd.m_move.x != 0.f )
				user_cmd.m_buttons |=
					user_cmd.m_move.x < 0.f
					? valve::e_buttons::in_forward : valve::e_buttons::in_back;

			if ( user_cmd.m_move.y == 0.f )
				return;

			user_cmd.m_buttons |=
				user_cmd.m_move.y < 0.f
				? valve::e_buttons::in_move_right : valve::e_buttons::in_move_left;

			return;
		}

		if ( user_cmd.m_move.x != 0.f )
			user_cmd.m_buttons |=
				user_cmd.m_move.x > 0.f
				? valve::e_buttons::in_forward : valve::e_buttons::in_back;

		if ( user_cmd.m_move.y == 0.f )
			return;

		user_cmd.m_buttons |=
			user_cmd.m_move.y > 0.f
			? valve::e_buttons::in_move_right : valve::e_buttons::in_move_left;
	}

	void c_movement::auto_peek( valve::user_cmd_t& user_cmd ) {
		if ( !g_ctx->weapon( ) || !g_ctx->wpn_data( )
			|| ( static_cast< std::size_t >( g_ctx->wpn_data( )->m_unk_type - 2 ) > 5
				&& g_ctx->weapon( )->item_index( ) != valve::e_item_index::taser )
			|| valve::g_local_player->move_type( ) == valve::e_move_type::ladder
			|| valve::g_local_player->move_type( ) == valve::e_move_type::noclip )
			return;

		auto wish_angles = user_cmd.m_view_angles;

		if ( hacks::g_movement->cfg( ).m_auto_peek_key
			&& LOWORD( GetKeyState( hacks::g_movement->cfg( ).m_auto_peek_key ) ) ) {
			if ( m_auto_peek_start_pos.zero( ) ) {
				m_auto_peek_start_pos = valve::g_local_player->get_abs_origin( );

				if ( !( valve::g_local_player->flags( ) & valve::e_ent_flags::on_ground ) ) {
					valve::trace_t trace;
					valve::trace_filter_world_only_t filter;
					valve::g_engine_trace->trace_ray( { m_auto_peek_start_pos, m_auto_peek_start_pos - vec3_t( 0.0f, 0.0f, 1000.0f ) },
						valve::e_mask::solid, &filter, &trace );

					if ( trace.m_fraction < 1.f )
						m_auto_peek_start_pos = trace.m_end_pos + vec3_t( 0.f, 0.f, 2.f );
				}
			}
			else {
				if ( user_cmd.m_buttons & valve::e_buttons::in_attack ) {
					if ( g_ctx->weapon( )->item_index( ) != valve::e_item_index::revolver )
						m_auto_peek_is_firing = true;
				}

				if ( m_auto_peek_is_firing ) {
					const auto diff = valve::g_local_player->get_abs_origin( ) - m_auto_peek_start_pos;

					if ( diff.length_2d( ) > 0.5f ) {
						user_cmd.m_buttons &= ~valve::e_buttons::in_jump;
						wish_angles = math::calc_ang( valve::g_local_player->get_abs_origin( ), m_auto_peek_start_pos );

						user_cmd.m_move.x = g_ctx->cvars( ).m_cl_forwardspeed->get_float( ) - ( 1.2f * ( user_cmd.m_number % 2 ) != 1 ? ( 14 - valve::g_client_state->m_choked_cmds ) : valve::g_client_state->m_choked_cmds );
						user_cmd.m_move.y = 0.0f;
					}
					else {
						m_auto_peek_is_firing = false;
						m_auto_peek_start_pos = { 0, 0, 0 };
					}

					rotate(
							user_cmd, wish_angles,
							valve::g_local_player->flags( ),
							valve::g_local_player->move_type( )
					);
				}
			}
		}
		else {
			m_auto_peek_is_firing = false;
			m_auto_peek_start_pos = { 0, 0, 0 };
		}
	}

	void c_movement::on_create_move( valve::user_cmd_t& user_cmd ) {
		hacks::g_movement->auto_peek( user_cmd );

		if ( valve::g_local_player->move_type( ) != valve::e_move_type::walk )
			return;

		m_max_player_speed = valve::g_local_player->max_speed( );
		m_max_weapon_speed = g_ctx->weapon( ) ? g_ctx->weapon( )->max_speed( ) : 260.f;

		m_velocity = valve::g_local_player->velocity( );

		predict_move( user_cmd, m_velocity );

		if ( m_cfg->m_remove_crouch_cooldown )
			user_cmd.m_buttons |= valve::e_buttons::in_bullrush;

		if ( m_should_fake_duck ) {
			if ( g_exploits->type( ) ) {
				if ( valve::g_local_player->duck_amount( ) > 0.5f )
					user_cmd.m_buttons |= valve::e_buttons::in_duck;
				else
					user_cmd.m_buttons &= ~valve::e_buttons::in_duck;
			}
			else {
				user_cmd.m_buttons |= valve::e_buttons::in_bullrush;

				const auto pred_duck_amount = std::min( 1.f, valve::g_local_player->duck_speed( ) * valve::to_time( 7 ) );
				if ( valve::g_local_player->duck_amount( ) <= pred_duck_amount ) {
					if ( valve::g_client_state->m_choked_cmds < 7 )
						user_cmd.m_buttons &= ~valve::e_buttons::in_duck;
					else
						user_cmd.m_buttons |= valve::e_buttons::in_duck;

					if ( g_ctx->flags( ) & e_context_flags::can_choke )
						g_ctx->flags( ) |= e_context_flags::choke;
					else
						g_ctx->flags( ) &= ~e_context_flags::choke;
				}
				else {
					user_cmd.m_buttons &= ~valve::e_buttons::in_duck;

					g_ctx->flags( ) &= ~e_context_flags::choke;
				}
			}
		}

		const auto stopped = stop( user_cmd );
		const auto forced_lby_update = force_lby_update( user_cmd );

		if ( !stopped ) {
			if ( m_cfg->m_slow_walk
				&& HIWORD( GetKeyState( m_cfg->m_slow_walk_key ) )
				&& !( user_cmd.m_buttons & valve::e_buttons::in_jump ) 
				&& valve::g_local_player->flags( ) & valve::e_ent_flags::on_ground 
				&& ( user_cmd.m_move.x != 0.f || user_cmd.m_move.y != 0.f )
				&& m_cfg->m_slow_walk_speed <= m_velocity.length( ) ) {
				m_max_player_speed = m_max_weapon_speed = m_cfg->m_slow_walk_speed;

				auto velocity = valve::g_local_player->velocity( );

				modify_move( user_cmd, velocity );
			}

			if ( !forced_lby_update ) {
				if ( g_anti_aim->cfg( ).m_enabled
					&& ( user_cmd.m_move.x != 0.f || user_cmd.m_move.y != 0.f )
					&& !( user_cmd.m_buttons & valve::e_buttons::in_duck )
					&& !m_should_fake_duck
					&& valve::g_local_player->flags( ) & valve::e_ent_flags::on_ground
					&& !( user_cmd.m_buttons & valve::e_buttons::in_jump )
					&& g_ctx->weapon( ) && g_ctx->wpn_data( ) ) {
					m_max_player_speed = valve::g_local_player->max_speed( );
					m_max_weapon_speed = g_ctx->weapon( ) ? g_ctx->weapon( )->max_speed( ) : 260.f;

					auto velocity = valve::g_local_player->velocity( );

					predict_move( user_cmd, velocity );

					const auto pred_speed_2d = velocity.length_2d( );
					if ( pred_speed_2d > 1.1f
						&& std::abs( pred_speed_2d - valve::g_local_player->velocity( ).length_2d( ) ) < 2.f ) {
						const auto len_2d = user_cmd.m_move.length_2d( );
						const auto drop = g_ctx->addresses( ).m_random_float( 1.f, 20.f );
						const auto mod = ( std::min( g_ctx->weapon( )->max_speed( ), len_2d ) - drop ) / len_2d;

						user_cmd.m_move.x *= mod;
						user_cmd.m_move.y *= mod;
					}
				}

				if ( m_cfg->m_auto_bhop
					&& user_cmd.m_buttons & valve::e_buttons::in_jump
					&& !g_ctx->cvars( ).m_sv_auto_bhop->get_bool( ) ) {
					if ( valve::g_local_player->flags( ) & valve::e_ent_flags::on_ground )
						user_cmd.m_buttons |= valve::e_buttons::in_jump;
					else
						user_cmd.m_buttons &= ~valve::e_buttons::in_jump;

				}

				auto_strafe( user_cmd );
			}
		}

		fast_stop( user_cmd );

		user_cmd.m_buttons &= ~(
			valve::e_buttons::in_forward
			| valve::e_buttons::in_back
			| valve::e_buttons::in_move_right
			| valve::e_buttons::in_move_left
		);

		if ( m_cfg->m_slide_walk
			&& valve::g_local_player->move_type( ) == valve::e_move_type::walk ) {
			if ( user_cmd.m_move.x != 0.f )
				user_cmd.m_buttons |=
					user_cmd.m_move.x < 0.f
						? valve::e_buttons::in_forward : valve::e_buttons::in_back;

			if ( user_cmd.m_move.y == 0.f )
				return;

			user_cmd.m_buttons |=
				user_cmd.m_move.y < 0.f
					? valve::e_buttons::in_move_right : valve::e_buttons::in_move_left;

			return;
		}

		if ( user_cmd.m_move.x != 0.f )
			user_cmd.m_buttons |=
				user_cmd.m_move.x > 0.f
					? valve::e_buttons::in_forward : valve::e_buttons::in_back;

		if ( user_cmd.m_move.y == 0.f )
			return;

		user_cmd.m_buttons |=
			user_cmd.m_move.y > 0.f
				? valve::e_buttons::in_move_right : valve::e_buttons::in_move_left;
	}
}