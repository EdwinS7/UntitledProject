#include "../../../inc.hpp"

namespace hacks {
	void c_anti_aim::at_target( float& yaw ) const {
		if ( !m_cfg->m_at_target )
			return;

		valve::c_player* best_player{};
		auto best_value = std::numeric_limits< float >::max( );
		
		float total_yaw{};
		int valid_count{};

		const auto view_angles = valve::g_engine->view_angles( );

		for ( auto i = 1; i <= valve::g_global_vars->m_max_clients; ++i ) {
			const auto player = static_cast< valve::c_player* >(
				valve::g_entity_list->find_entity( i )
			);
			if ( !player
				|| player->dormant( )
				|| !player->alive( ) 
				|| player->friendly( )  )
				continue;

			switch ( m_cfg->m_at_target_type ) {
			case 0: {
				const auto dist = ( valve::g_local_player->origin( ) - player->origin( ) ).length( );
				if ( dist >= best_value )
					continue;

				best_value = dist;
				best_player = player;
			} break;
			case 1: {
				++valid_count;

				const auto x = player->origin( ).x - valve::g_local_player->origin( ).x;
				const auto y = player->origin( ).y - valve::g_local_player->origin( ).y;

				if ( x == 0.f
					&& y == 0.f )
					continue;

				total_yaw += math::to_deg( std::atan2( y, x ) );
			} break;
			case 2: {
				const auto fov = math::calc_fov( view_angles, g_ctx->shoot_pos( ), player->world_space_center( ) );
				if ( fov >= best_value )
					continue;

				best_value = fov;
				best_player = player;
			} break;
			}
		}

		if ( m_cfg->m_at_target_type == 1 ) {
			if ( valid_count )
				yaw = total_yaw / valid_count;

			return;
		}

		if ( !best_player )
			return;

		const auto x = best_player->origin( ).x - valve::g_local_player->origin( ).x;
		const auto y = best_player->origin( ).y - valve::g_local_player->origin( ).y;

		yaw = x == 0.f && y == 0.f ? 0.f : math::to_deg( std::atan2( y, x ) );
	}

	bool c_anti_aim::auto_direction( float& yaw ) {
		if ( !m_cfg->m_auto_dir_type )
			return false;

		valve::c_player* best_player{};
		auto best_fov = std::numeric_limits< float >::max( );

		const auto view_angles = valve::g_engine->view_angles( );

		for ( auto i = 1; i <= valve::g_global_vars->m_max_clients; ++i ) {
			const auto player = static_cast< valve::c_player* >(
				valve::g_entity_list->find_entity( i )
			);
			if ( !player
				|| player->dormant( )
				|| !player->alive( )
				|| player->friendly( ) )
				continue;

			const auto fov = math::calc_fov( view_angles, g_ctx->shoot_pos( ), player->world_space_center( ) );
			if ( fov >= best_fov )
				continue;

			best_fov = fov;
			best_player = player;
		}

		if ( !best_player )
			return false;

		struct angle_data_t {
			__forceinline constexpr angle_data_t( ) = default;

			__forceinline angle_data_t( const float yaw ) : m_yaw{ yaw } {}

			int		m_dmg{};
			float	m_yaw{}, m_dist{};
			bool	m_can_do_dmg{};
		};

		std::array< angle_data_t, 3u > angles{
			{
				{ std::remainder( yaw + 180.f, 360.f ) },
				{ std::remainder( yaw + 90.f, 360.f ) },
				{ std::remainder( yaw - 90.f, 360.f) }
			}
		};

		constexpr auto k_range = 30.f;

		auto enemy_shoot_pos = best_player->origin( );

		enemy_shoot_pos.z += 64.f;

		bool valid{};

		const auto& local_shoot_pos = g_ctx->shoot_pos( );
		for ( auto& angle : angles ) {
			const auto rad_yaw = math::to_rad( angle.m_yaw );

			const auto pen_data = g_auto_wall->fire_emulated(
				best_player, valve::g_local_player, enemy_shoot_pos,
				{
					local_shoot_pos.x + std::cos( rad_yaw ) * k_range,
					local_shoot_pos.y + std::sin( rad_yaw ) * k_range,
					local_shoot_pos.z
				}
			);

			if ( pen_data.m_dmg < 1 )
				continue;

			angle.m_dmg = pen_data.m_dmg;

			angle.m_can_do_dmg = angle.m_dmg > 0;

			if ( !angle.m_can_do_dmg )
				continue;

			valid = true;
		}

		if ( valid ) {
			float best_dmg{};
			std::size_t best_index{};
			
			for ( std::size_t i{}; i < angles.size( ); ++i ) {
				const auto& angle = angles.at( i );
				if ( !angle.m_can_do_dmg
					|| angle.m_dmg <= best_dmg )
					continue;

				best_dmg = angle.m_dmg;
				best_index = i;
			}

			const auto& best_angle = angles.at( best_index );

			if ( m_cfg->m_auto_dir_type == 1 )
				yaw = best_angle.m_yaw;
			else {
				yaw = std::remainder( yaw + 180.f, 360.f );

				const auto diff = math::angle_diff( yaw, best_angle.m_yaw );

				m_auto_dir_side = ( m_cfg->m_auto_dir_type == 2 ? diff >= 0.f : diff < 0.f ) + 1;
			}

			return true;
		}

		valid = false;

		constexpr auto k_step = 4.f;

		for ( auto& angle : angles ) {
			const auto rad_yaw = math::to_rad( angle.m_yaw );

			const vec3_t dst{
				local_shoot_pos.x + std::cos( rad_yaw ) * k_range,
				local_shoot_pos.y + std::sin( rad_yaw ) * k_range,
				local_shoot_pos.z
			};

			auto dir = dst - enemy_shoot_pos;
			
			const auto len = dir.normalize( );
			if ( len <= 0.f )
				continue;

			for ( float i{}; i < len; i += k_step ) {
				const auto contents = valve::g_engine_trace->point_contents( local_shoot_pos + dir * i, valve::e_mask::shot_hull );
				if ( !( contents & valve::e_mask::shot_hull ) )
					continue;

				auto mult = 1.f;

				if ( i > ( len * 0.5f ) )
					mult = 1.25f;

				if ( i > ( len * 0.75f ) )
					mult = 1.25f;

				if ( i > ( len * 0.9f ) )
					mult = 2.f;

				angle.m_dist += k_step * mult;

				valid = true;
			}
		}

		if ( !valid )
			return false;

		if ( std::abs( angles.at( 0u ).m_dist - angles.at( 1u ).m_dist ) >= 10.f
			|| std::abs( angles.at( 0u ).m_dist - angles.at( 2u ).m_dist ) >= 10.f ) {
			std::sort(
				angles.begin( ), angles.end( ),
				[ ] ( const angle_data_t& a, const angle_data_t& b ) {
					return a.m_dist > b.m_dist;
				}
			);

			const auto& best_angle = angles.front( );
			if ( best_angle.m_dist > 400.f )
				return false;

			if ( m_cfg->m_auto_dir_type == 1 )
				yaw = best_angle.m_yaw;
			else {
				yaw = std::remainder( yaw + 180.f, 360.f );

				const auto diff = math::angle_diff( yaw, best_angle.m_yaw );

				m_auto_dir_side = ( m_cfg->m_auto_dir_type == 2 ? diff >= 0.f : diff < 0.f ) + 1;
			}

			return true;
		}

		return false;
	}

	int c_anti_aim::select_side( ) {
		if ( !valve::g_client_state->m_choked_cmds
			&& m_prev_tick_count != valve::g_global_vars->m_tick_count )
			++m_side_counter;

		m_prev_tick_count = valve::g_global_vars->m_tick_count;

		const auto shot_cmd = g_anim_sync->local_data( ).m_shot_cmd_number;
		if ( shot_cmd > valve::g_client_state->m_last_cmd_out
			&& shot_cmd <= ( valve::g_client_state->m_last_cmd_out + valve::g_client_state->m_choked_cmds + 1 ) ) {
			switch ( m_cfg->m_on_shot_side ) {
			case 1: return 2; break;
			case 2: return 1; break;
			case 3: {
				const auto& local_data = g_eng_pred->local_data( ).at( valve::g_client_state->m_last_cmd_out % 150 );

				return ( math::angle_diff( local_data.m_anim_state.m_eye_yaw, local_data.m_anim_state.m_foot_yaw ) <= 0.f ) + 1;
			} break;
			case 4: return m_on_shot_side_counter = ( m_on_shot_side_counter != 2 ) + 1; break;
			}

			return 0;
		}

		if ( m_cfg->m_desync ) {
			if ( m_auto_dir_side )
				return m_auto_dir_side;

			if ( m_cfg->m_jitter_side )
				return 2 - ( m_side_counter % 2 );

			return m_cfg->m_side_invert_key && LOWORD( GetKeyState( m_cfg->m_side_invert_key ) ) ? 2 : 1;
		}

		return 0;
	}

	void c_anti_aim::choke( valve::user_cmd_t* const user_cmd ) {
		if ( g_exploits->charged( )
			|| g_exploits->shift( )
			|| g_ctx->freeze_time( )
			|| g_movement->should_fake_duck( )
			|| valve::g_client_state->m_choked_cmds > 14
			|| !( g_ctx->flags( ) & e_context_flags::can_choke )
			|| valve::g_local_player->flags( ) & valve::e_ent_flags::frozen )
			return;

		if ( enabled( user_cmd )
			&& valve::g_client_state->m_choked_cmds < 1 )
			g_ctx->flags( ) |= e_context_flags::choke;

		if ( !m_cfg->m_fake_lag.m_enabled )
			return;

		bool should_choke{};
		if ( m_cfg->m_fake_lag.m_conditions & 1 )
			should_choke = true;

		if ( !should_choke
			&& m_cfg->m_fake_lag.m_conditions & 2 
			&& valve::g_local_player->velocity( ).length( ) > 2.f )
			should_choke = true;

		if ( !should_choke
			&& m_cfg->m_fake_lag.m_conditions & 4
			&& !( valve::g_local_player->flags( ) & valve::e_ent_flags::on_ground ) )
			should_choke = true;

		if ( should_choke
			&& m_choke_start_cmd_number != valve::g_client_state->m_last_cmd_out )
			m_choke_start_cmd_number = valve::g_client_state->m_last_cmd_out;

		if ( g_ctx->flags( ) & e_context_flags::choke
			|| ( !should_choke && m_choke_start_cmd_number != valve::g_client_state->m_last_cmd_out ) )
			return;

		if ( m_cfg->m_fake_lag.m_adaptive ) {
			const auto& local_data = g_eng_pred->local_data( ).at( valve::g_client_state->m_last_cmd_out % 150 );
			if ( local_data.m_spawn_time == valve::g_local_player->spawn_time( ) ) {
				if ( ( local_data.m_pred_net_vars.m_abs_origin - valve::g_local_player->get_abs_origin( ) ).length_2d_sqr( ) >= 4096.f )
					g_ctx->flags( ) &= ~e_context_flags::choke;
				else
					g_ctx->flags( ) |= e_context_flags::choke;
			}

			return;
		}

		if ( m_cfg->m_fake_lag.m_limit > 14 )
			m_cfg->m_fake_lag.m_limit = 14;

		if ( valve::g_client_state->m_choked_cmds >= m_cfg->m_fake_lag.m_limit )
			g_ctx->flags( ) &= ~e_context_flags::choke;
		else
			g_ctx->flags( ) |= e_context_flags::choke;
	}

	void c_anti_aim::select_yaw( float& yaw, const int side ) {
		m_auto_dir_side = 0;

		if ( !valve::g_client_state->m_choked_cmds )
			m_choke_cycle_switch = !m_choke_cycle_switch;

		if ( !m_cfg->m_yaw_type )
			return;

		yaw = valve::g_engine->view_angles( ).y;

		if ( m_cfg->m_manual_left_key
			&& LOWORD( GetKeyState( m_cfg->m_manual_left_key ) ) )
			yaw += 90.f;
		else if ( m_cfg->m_manual_right_key
			&& LOWORD( GetKeyState( m_cfg->m_manual_right_key ) ) )
			yaw -= 90.f;
		else {
			at_target( yaw );
			if ( !auto_direction( yaw ) ) {
				switch ( m_cfg->m_yaw_type ) {
				case 1: yaw += 180.f; break;
				case 2: yaw += 120.f + std::fmod( valve::g_global_vars->m_cur_time * 120.f, 120.f ); break;
				case 3: yaw = std::fmod( valve::g_global_vars->m_cur_time * 180.f, 360.f ); break;
				case 4: yaw = std::fmod( valve::g_global_vars->m_cur_time * 720.f, 360.f ); break;
				case 5: yaw = g_ctx->addresses( ).m_random_float( -180.f, 180.f ); break;
				case 6: yaw = 69.f; break;
				}
			}

			yaw +=
				( side == 2 ? m_cfg->m_yaw_offset_inverted : m_cfg->m_yaw_offset )
				+ m_cfg->m_jitter_amount * ( m_choke_cycle_switch ? 0.5f : -0.5f );

		}
	}

	void c_anti_aim::set_pitch( valve::user_cmd_t& user_cmd ) {
		const auto anim_state = valve::g_local_player->anim_state( );
		if ( !anim_state
			|| !enabled( &user_cmd ) 
			|| ( g_ctx->flags( ) & e_context_flags::can_shoot
				&& g_ctx->will_shoot( g_ctx->weapon( ), user_cmd ) ) )
			return;

		switch ( m_cfg->m_pitch ) {
		case 1: user_cmd.m_view_angles.x = anim_state->m_max_body_pitch; break;
		case 2: user_cmd.m_view_angles.x = -89.f; break;
		case 3: user_cmd.m_view_angles.x = 0.f; break;
		}
	}

	void c_anti_aim::process( valve::user_cmd_t& user_cmd, const float yaw, const int side, const int choked_cmds ) const {
		const auto anim_state = valve::g_local_player->anim_state( );
		if ( !anim_state
			|| !enabled( &user_cmd ) )
			return;

		const auto shot_cmd_number = g_anim_sync->local_data( ).m_shot_cmd_number;
		if ( user_cmd.m_number == shot_cmd_number )
			return;

		if ( !side ) {
			user_cmd.m_view_angles.y = std::remainder( yaw, 360.f );

			return;
		}

		const auto in_shot = shot_cmd_number > valve::g_client_state->m_last_cmd_out
			&& shot_cmd_number <= ( valve::g_client_state->m_last_cmd_out + valve::g_client_state->m_choked_cmds + 1 );

		const auto speed = ( ( anim_state->m_walk_to_run_transition * 20.f ) + 30.f ) * valve::g_global_vars->m_interval_per_tick;
		
		auto delta = side == 1 ? 60.f : -60.f;
		if ( !in_shot ) {
			const auto upper_limit = m_cfg->m_desync_limit + speed;
			if ( delta > upper_limit )
				delta = upper_limit;
			else {
				const auto lower_limit = ( m_cfg->m_desync_limit * -1.f ) - speed;
				if ( lower_limit > delta )
					delta = lower_limit;
			}
		}

		if ( choked_cmds <= 0 ) {
			user_cmd.m_view_angles.y = std::remainder( yaw, 360.f );

			return;
		}

		const auto& local_data = g_eng_pred->local_data( ).at( valve::g_client_state->m_last_cmd_out % 150 );
		if ( std::abs( delta - 5.f ) > std::abs( math::angle_diff( local_data.m_anim_state.m_eye_yaw, local_data.m_anim_state.m_foot_yaw ) ) )
			delta = std::copysign( 120.f, delta );

		user_cmd.m_view_angles.y = std::remainder( yaw - delta, 360.f );
	}
}