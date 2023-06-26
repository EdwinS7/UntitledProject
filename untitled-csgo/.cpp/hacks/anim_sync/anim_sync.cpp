#include "../../../inc.hpp"

namespace hacks {
	void c_anim_sync::update(
		const player_entry_t& entry, lag_record_t* const current,
		const lag_anim_data_t* const previous, const int side
	) const {
		const auto anim_state = entry.m_player->anim_state( );

		if ( previous ) {
			entry.m_player->anim_layers( ) = previous->m_anim_layers;

			auto& prev_anim_side = previous->m_anim_sides.at( side );

			anim_state->m_foot_yaw = prev_anim_side.m_foot_yaw;
			anim_state->m_prev_foot_yaw = prev_anim_side.m_prev_foot_yaw;
			anim_state->m_move_yaw = prev_anim_side.m_move_yaw;
			anim_state->m_move_yaw_cur_to_ideal = prev_anim_side.m_move_yaw_cur_to_ideal;
			anim_state->m_move_yaw_ideal = prev_anim_side.m_move_yaw_ideal;

			anim_state->m_feet_cycle = previous->m_anim_layers.at( 6u ).m_cycle;

			const auto& layer7 = previous->m_anim_layers.at( 7u );

			anim_state->m_strafe_weight = layer7.m_weight;
			anim_state->m_strafe_sequence = layer7.m_sequence;
			anim_state->m_strafe_cycle = layer7.m_cycle;
		}
		else {
			if ( current->m_flags & valve::e_ent_flags::on_ground )
				anim_state->m_on_ground = true;

			anim_state->m_prev_update_time = current->m_sim_time - valve::g_global_vars->m_interval_per_tick;
		}

		struct backup_t {
			__forceinline constexpr backup_t( ) = default;

			__forceinline backup_t( valve::c_player* const player )
				: m_cur_time{ valve::g_global_vars->m_cur_time },
				  m_frame_time{ valve::g_global_vars->m_frame_time },
				  m_duck_amount{ player->duck_amount( ) }, m_eye_yaw{ player->eye_angles( ).y },
				  m_eflags{ player->eflags( ) }, m_flags{ player->flags( ) },
				  m_velocity{ player->velocity( ) }, m_abs_velocity{ player->abs_velocity( ) } {}

			__forceinline void restore( valve::c_player* const player ) const {
				valve::g_global_vars->m_cur_time = m_cur_time;
				valve::g_global_vars->m_frame_time = m_frame_time;

				player->duck_amount( ) = m_duck_amount;
				player->eye_angles( ).y = m_eye_yaw;

				player->eflags( ) = m_eflags;
				player->flags( ) = m_flags;

				player->velocity( ) = m_velocity;
				player->abs_velocity( ) = m_abs_velocity;
			}

			float				m_cur_time{}, m_frame_time{},
								m_duck_amount{}, m_eye_yaw{};

			std::size_t			m_eflags{};
			valve::e_ent_flags	m_flags{};

			vec3_t				m_velocity{}, m_abs_velocity{};
		} backup{ entry.m_player };

		for ( const auto& interpolated : entry.m_interpolated ) {
			valve::g_global_vars->m_cur_time = interpolated.m_sim_time;
			valve::g_global_vars->m_frame_time = valve::g_global_vars->m_interval_per_tick;

			anim_state->m_foot_yaw = anim_state->m_eye_yaw + ( side == 1 ? -60.f : side == 2 ? 60.f : 0.f );

			entry.m_player->duck_amount( ) = interpolated.m_duck_amount;
			entry.m_player->flags( ) = interpolated.m_flags;
			entry.m_player->velocity( ) = entry.m_player->abs_velocity( ) = interpolated.m_velocity;

			entry.m_player->eflags( ) &= ~0x1000u;

			anim_state->m_prev_update_frame = valve::g_global_vars->m_frame_count - 1;

			entry.m_player->client_side_anim( ) = g_ctx->allow_anim_update( ) = true;
			entry.m_player->update_client_side_anim( );
			entry.m_player->client_side_anim( ) = g_ctx->allow_anim_update( ) = false;

			backup.restore( entry.m_player );
		}

		auto& cur_anim_side = current->m_anim_sides.at( side );

		{
			cur_anim_side.m_foot_yaw = anim_state->m_foot_yaw;
			cur_anim_side.m_prev_foot_yaw = anim_state->m_prev_foot_yaw;
			cur_anim_side.m_move_yaw_ideal = anim_state->m_move_yaw_ideal;
			cur_anim_side.m_move_yaw_cur_to_ideal = anim_state->m_move_yaw_cur_to_ideal;
			cur_anim_side.m_move_yaw = anim_state->m_move_yaw;

			cur_anim_side.m_anim_layers = entry.m_player->anim_layers( );
			cur_anim_side.m_pose_params = entry.m_player->pose_params( );
		}

		entry.m_player->anim_layers( ) = current->m_anim_layers;
		entry.m_player->set_abs_angles( { 0.f, anim_state->m_foot_yaw, 0.f } );

		setup_bones( entry.m_player, cur_anim_side.m_bones, current->m_sim_time, 15 );

		const auto mdl_data = entry.m_player->mdl_data( );
		if ( !mdl_data
			|| !mdl_data->m_studio_hdr )
			return;

		cur_anim_side.m_bones_count = mdl_data->m_studio_hdr->m_bones_count;
	}

	void c_anim_sync::find_server_foot_yaw(
		player_entry_t& entry,
		lag_record_t* const current, const lag_anim_data_t* const previous
	) const {
		current->m_anim_side = entry.m_prev_anim_side;

		if ( !previous )
			return;

		const auto anim_state = entry.m_player->anim_state( );
		if ( !anim_state )
			return;

		if ( current->m_sim_ticks <= 0
			|| current->m_total_cmds <= 1
			|| !( current->m_flags & valve::e_ent_flags::on_ground )
			|| !( previous->m_flags & valve::e_ent_flags::on_ground ) ) {
			current->m_reforce_safe_points = true;
			return;
		}

		/*const auto walk_speed = std::clamp( anim_state->m_speed_as_portion_of_walk_speed, 0.f, 1.f );
		const auto run_speed = ( ( 0.8f - ( anim_state->m_walk_to_run_transition * 0.3f ) ) - 1.f ) * walk_speed;

		auto body_yaw_modifier = run_speed + 1.f;

		if ( anim_state->m_duck_amount > 0.f ) {
			const auto crouch_walk_speed = std::clamp( anim_state->m_speed_as_portion_of_crouch_speed, 0.f, 1.f );

			body_yaw_modifier += ( anim_state->m_duck_amount * crouch_walk_speed ) * ( 0.5f - body_yaw_modifier );
		}

		current->m_max_delta = anim_state->m_max_body_yaw * body_yaw_modifier;*/

		// if they're jittering do the funny min 2 overlaps
		if ( current->m_eye_angles.y != previous->m_eye_angles.y 
			&& std::abs( math::angle_diff( current->m_eye_angles.y, current->m_lby ) ) != std::abs( math::angle_diff( previous->m_eye_angles.y, previous->m_lby ) ) )
			current->m_reforce_safe_points = true;

		// lby niggers min 3 overlaps aka full SP
		if ( std::abs( math::angle_diff( previous->m_eye_angles.y, current->m_eye_angles.y ) ) > 35.f )
			current->m_force_safe_points = true;

		const auto speed_2d = current->m_velocity.length_2d( );
		if ( speed_2d <= 0.1f ) {
			current->m_reforce_safe_points = true;
			// @todo: lby resolver lol
			return;
		}

		const auto& move_layer = current->m_anim_layers.at( 6u );

		if ( ( current->m_velocity.length_2d( ) <= 30.f
			&& current->m_velocity.length_2d( ) > 15.f )
			|| ( current->m_velocity.length_2d( ) < 120.f
				&& previous->m_velocity.length_2d( ) >= 120.f
				&& previous->m_anim_layers.at( 12u ).m_weight < current->m_anim_layers.at( 12u ).m_weight ) )
			current->m_force_safe_points = true;

		if ( current->m_velocity.length_2d( ) >= 120 && previous->m_velocity.length_2d( ) >= 120.f ) {
			// @todo: move-yaw resolver
			// @link: https://github.com/perilouswithadollarsign/cstrike15_src/blob/master/game/shared/cstrike15/csgo_playeranimstate.cpp#L2490
			// @info: if i want the desync side there are much better ways to do it that do not involve animlayers.
			// - Looking into that and double checking the result with my animlayer method.

		RECALC:

			const float resolved_foot_yaw = std::remainder( anim_state->m_move_yaw + current->m_max_delta, 360.f );

			if ( math::angle_diff( anim_state->m_move_yaw, resolved_foot_yaw ) < 0.f || anim_state->m_max_body_yaw == 0.f ) {
				if ( anim_state->m_min_body_yaw != 0.f )
					current->m_anim_side = entry.m_prev_anim_side = 2;
			}
			else {
				current->m_anim_side = entry.m_prev_anim_side = 1;
			}
		}
		else {
			float center_delta = std::abs( current->m_anim_sides.at( 0u ).m_anim_layers.at( 6u ).m_playback_rate - move_layer.m_playback_rate );
			float left_delta = std::abs( current->m_anim_sides.at( 1u ).m_anim_layers.at( 6u ).m_playback_rate - move_layer.m_playback_rate );
			float right_delta = std::abs( current->m_anim_sides.at( 2u ).m_anim_layers.at( 6u ).m_playback_rate - move_layer.m_playback_rate );

			center_delta = std::numeric_limits<float>::max( );

			if ( left_delta <= right_delta
				&& left_delta <= center_delta ) {
				if ( left_delta )
					current->m_reforce_safe_points = true;

				current->m_anim_side = entry.m_prev_anim_side = 1;
			}

			if ( right_delta <= left_delta
				&& right_delta <= center_delta ) {
				if ( right_delta )
					current->m_reforce_safe_points = true;

				current->m_anim_side = entry.m_prev_anim_side = 2;
			}

			if ( move_layer.m_weight == 1.f )
				current->m_anim_side = entry.m_prev_anim_side = 0;
		}
	}

	bool c_anim_sync::setup_bones(
		valve::c_player* const player,
		valve::bones_t& bones, const float time, const int flags
	) const {
		struct backup_t {
			__forceinline constexpr backup_t( ) = default;

			__forceinline backup_t( valve::c_player* const player )
				: m_cur_time{ valve::g_global_vars->m_cur_time },
				  m_frame_time{ valve::g_global_vars->m_frame_time },
				  m_frame_count{ valve::g_global_vars->m_frame_count },
				  m_occlusion_frame{ player->occlusion_frame( ) },
				  m_ent_client_flags{ player->ent_client_flags( ) },
				  m_ik_context{ player->ik_context( ) }, m_effects{ player->effects( ) },
				  m_occlusion_flags{ player->occlusion_flags( ) } {}

			float					m_cur_time{}, m_frame_time{};
			int						m_frame_count{}, m_occlusion_frame{};

			std::uint8_t			m_ent_client_flags{};
			valve::ik_context_t*	m_ik_context{};

			std::size_t				m_effects{}, m_occlusion_flags{};
		} backup{ player };

		g_ctx->force_bone_mask( ) = flags & 4;

		valve::g_global_vars->m_cur_time = time;
		valve::g_global_vars->m_frame_time = valve::g_global_vars->m_interval_per_tick;

		if ( flags & 8 ) {
			player->effects( ) |= 8u;
			player->occlusion_flags( ) &= ~0xau;
			player->occlusion_frame( ) = 0;
		}

		if ( flags & 4 ) {
			player->ik_context( ) = nullptr;
			player->ent_client_flags( ) |= 2u;
		}

		if ( flags & 2 )
			player->last_setup_bones_frame( ) = 0;

		if ( flags & 1 ) {
			player->mdl_bone_counter( ) = 0ul;
			player->last_setup_bones_time( ) = std::numeric_limits< float >::lowest( );

			auto& bone_accessor = player->bone_accessor( );

			bone_accessor.m_writable_bones = bone_accessor.m_readable_bones = 0;
		}

		auto& jiggle_bones = g_ctx->cvars( ).m_r_jiggle_bones;

		const auto backup_jiggle_bones = jiggle_bones->get_bool( );

		jiggle_bones->set_int( false );

		g_ctx->allow_setup_bones( ) = true;
		const auto ret = player->setup_bones( bones.data( ), 128, ( ( ( flags >> 4 ) & 1 ) << 9 ) + 0xffd00, time );
		g_ctx->allow_setup_bones( ) = false;

		jiggle_bones->set_int( backup_jiggle_bones );

		if ( flags & 8 ) {
			player->effects( ) = backup.m_effects;
			player->occlusion_flags( ) = backup.m_occlusion_flags;
			player->occlusion_frame( ) = backup.m_occlusion_frame;
		}

		if ( flags & 4 ) {
			player->ik_context( ) = backup.m_ik_context;
			player->ent_client_flags( ) = backup.m_ent_client_flags;
		}

		valve::g_global_vars->m_cur_time = backup.m_cur_time;
		valve::g_global_vars->m_frame_time = backup.m_frame_time;
		valve::g_global_vars->m_frame_count = backup.m_frame_count;

		if ( !( flags & 4 ) )
			return ret;

		const auto mdl_data = player->mdl_data( );
		if ( !mdl_data
			|| !mdl_data->m_studio_hdr )
			return ret;

		const auto hitbox_set = mdl_data->m_studio_hdr->hitbox_set( player->hitbox_set_index( ) );
		if ( !hitbox_set )
			return ret;

		for ( int i{}; i < hitbox_set->m_hitboxes_count; ++i ) {
			const auto hitbox = hitbox_set->hitbox( i );
			if ( !hitbox
				|| hitbox->m_radius >= 0.f )
				continue;

			mat3x4_t rot_mat{};
			g_ctx->addresses( ).m_angle_matrix( hitbox->m_rotation, rot_mat );

			math::concat_transforms(
				bones[ hitbox->m_bone ], rot_mat, bones.at( hitbox->m_bone )
			);
		}

		return ret;
	}

	void c_anim_sync::on_net_update(
		player_entry_t& entry,
		lag_record_t* const current, const lag_anim_data_t* const previous
	) const {
		struct anim_backup_t {
			__forceinline constexpr anim_backup_t( ) = default;

			__forceinline anim_backup_t( valve::c_player* const player )
				: m_anim_state{ *player->anim_state( ) }, m_abs_yaw{ m_anim_state.m_foot_yaw },
				  m_anim_layers{ player->anim_layers( ) }, m_pose_params{ player->pose_params( ) } {}

			__forceinline void restore( valve::c_player* const player ) const {
				*player->anim_state( ) = m_anim_state;

				player->set_abs_angles( { 0.f, m_abs_yaw, 0.f } );

				player->anim_layers( ) = m_anim_layers;
				player->pose_params( ) = m_pose_params;
			}

			valve::anim_state_t		m_anim_state{};

			float					m_abs_yaw{};

			valve::anim_layers_t	m_anim_layers{};
			valve::pose_params_t	m_pose_params{};
		} anim_backup{ entry.m_player };

		entry.interpolate( previous, current ); {
			entry.m_player->set_abs_origin( current->m_origin );

			if ( !entry.m_player->friendly( ) ) {
				const auto player_info = valve::g_engine->player_info( entry.m_player->index( ) );
				if ( player_info.has_value( )
					&& !player_info.value( ).m_fake_player ) {
					update( entry, current, previous, 2 );

					anim_backup.restore( entry.m_player );

					update( entry, current, previous, 1 );

					anim_backup.restore( entry.m_player );

					update( entry, current, previous, 0 );

					anim_backup.restore( entry.m_player );

					current->m_trying_to_resolve = true;
				}
			}

			if ( current->m_trying_to_resolve )
				find_server_foot_yaw( entry, current, previous );

			update( entry, current, previous, current->m_anim_side );

			entry.m_player->anim_layers( ) = current->m_anim_layers;

			entry.m_player->pose_params( ) = current->m_anim_sides.at( current->m_anim_side ).m_pose_params;

			g_ctx->allow_setup_bones( ) = true;
			entry.m_player->setup_bones( entry.m_bones.data( ), 128, 0xfff00, valve::g_global_vars->m_cur_time );
			g_ctx->allow_setup_bones( ) = false;
		}
		entry.m_interpolated.clear( );
	}

	void c_anim_sync::update_local( const qangle_t& view_angles, const bool no_view_model ) {
		const auto anim_state = valve::g_local_player->anim_state( );
		if ( !anim_state )
			return;

		const auto backup_cur_time = valve::g_global_vars->m_cur_time;
		const auto backup_frame_time = valve::g_global_vars->m_frame_time;

		valve::g_global_vars->m_cur_time = valve::to_time( valve::g_local_player->tick_base( ) );
		valve::g_global_vars->m_frame_time = valve::g_global_vars->m_interval_per_tick;

		valve::g_local_player->set_local_view_angles( view_angles );

		anim_state->m_prev_update_frame = valve::g_global_vars->m_frame_count - 1;

		const auto backup_abs_velocity = valve::g_local_player->abs_velocity( );

		valve::g_local_player->abs_velocity( ) = valve::g_local_player->velocity( );

		const auto backup_eflags = valve::g_local_player->eflags( );

		valve::g_local_player->eflags( ) &= ~0x1000u;
		
		const auto backup_client_side_anim = valve::g_local_player->client_side_anim( );

		valve::g_local_player->client_side_anim( ) = g_ctx->allow_anim_update( ) = true;

		if ( no_view_model )
			anim_state->update( view_angles.x, view_angles.y );
		else
			valve::g_local_player->update_client_side_anim( );

		valve::g_local_player->client_side_anim( ) = backup_client_side_anim;
		
		g_ctx->allow_anim_update( ) = false;

		valve::g_local_player->eflags( ) = backup_eflags;

		valve::g_local_player->abs_velocity( ) = backup_abs_velocity;

		valve::g_global_vars->m_cur_time = backup_cur_time;
		valve::g_global_vars->m_frame_time = backup_frame_time;

		const auto walk_speed = std::clamp( anim_state->m_speed_as_portion_of_walk_speed, 0.f, 1.f );
		const auto run_speed = ( ( 0.8f - ( anim_state->m_walk_to_run_transition * 0.3f ) ) - 1.f ) * walk_speed;

		auto body_yaw_modifier = run_speed + 1.f;

		if ( anim_state->m_duck_amount > 0.f ) {
			const auto crouch_walk_speed = std::clamp( anim_state->m_speed_as_portion_of_crouch_speed, 0.f, 1.f );

			body_yaw_modifier += ( anim_state->m_duck_amount * crouch_walk_speed ) * ( 0.5f - body_yaw_modifier );
		}

		m_local_data.m_max_body_yaw = anim_state->m_max_body_yaw * body_yaw_modifier;
		m_local_data.m_min_body_yaw = anim_state->m_min_body_yaw * body_yaw_modifier;
	}

	void c_anim_sync::update_local_real( valve::user_cmd_t& user_cmd ) {
		const auto anim_state = valve::g_local_player->anim_state( );
		if ( !anim_state )
			return;

		const auto backup_anim_layers = valve::g_local_player->anim_layers( );

		if ( !g_anti_aim->enabled( &user_cmd ) ) {
			g_movement->normalize( user_cmd );

			const auto& prev_local_data = g_eng_pred->local_data( ).at( ( user_cmd.m_number - 1 ) % 150 );
			if ( prev_local_data.m_spawn_time == valve::g_local_player->spawn_time( ) )
				prev_local_data.restore_anim( false );

			update_local( user_cmd.m_view_angles, false );

			g_eng_pred->local_data( ).at( user_cmd.m_number % 150 ).store_anim( );

			valve::g_local_player->anim_layers( ) = backup_anim_layers;

			return;
		}

		const auto side = g_anti_aim->select_side( );

		auto yaw = user_cmd.m_view_angles.y;
		g_anti_aim->select_yaw( yaw, side );
		
		const auto& first_local_data = g_eng_pred->local_data( ).at( valve::g_client_state->m_last_cmd_out % 150 );
		if ( first_local_data.m_spawn_time == valve::g_local_player->spawn_time( ) )
			first_local_data.restore_anim( false );

		qangle_t shot_cmd_view_angles{};

		auto i = 1;
		auto choked_cmds = valve::g_client_state->m_choked_cmds;

		const auto total_cmds = choked_cmds + 1;
		if ( total_cmds < 1 ) {
			valve::g_local_player->anim_layers( ) = backup_anim_layers;

			return;
		}

		for ( ; i <= total_cmds; ++i, --choked_cmds ) {
			const auto& cur_local_data = g_eng_pred->local_data( ).at( ( valve::g_client_state->m_last_cmd_out + i ) % 150 );
			if ( cur_local_data.m_spawn_time == valve::g_local_player->spawn_time( ) )
				break;
		}

		const auto shot_in_this_cycle =
			m_local_data.m_shot_cmd_number > valve::g_client_state->m_last_cmd_out
			&& m_local_data.m_shot_cmd_number <= ( valve::g_client_state->m_last_cmd_out + total_cmds );

		for ( ; i <= total_cmds; ++i, --choked_cmds ) {
			const auto j = ( valve::g_client_state->m_last_cmd_out + i ) % 150;

			auto& cur_user_cmd = valve::g_input->m_cmds[ j ];
			auto& cur_local_data = g_eng_pred->local_data( ).at( j );

			if ( cur_local_data.m_net_vars.m_move_type != valve::e_move_type::ladder
				&& cur_local_data.m_pred_net_vars.m_move_type != valve::e_move_type::ladder ) {
				const auto old_view_angles = cur_user_cmd.m_view_angles;

				g_anti_aim->process( cur_user_cmd, yaw, side, choked_cmds );

				if ( cur_user_cmd.m_view_angles.x != old_view_angles.x
					|| cur_user_cmd.m_view_angles.y != old_view_angles.y
					|| cur_user_cmd.m_view_angles.z != old_view_angles.z )
					g_movement->rotate(
						cur_user_cmd, old_view_angles,
						cur_local_data.m_pred_net_vars.m_flags,
						cur_local_data.m_pred_net_vars.m_move_type
					);
			}

			if ( !choked_cmds )
				g_movement->normalize( cur_user_cmd );

			valve::g_local_player->origin( ) = cur_local_data.m_pred_net_vars.m_origin;
			valve::g_local_player->move_state( ) = cur_local_data.m_pred_net_vars.m_move_state;
			valve::g_local_player->strafing( ) = cur_local_data.m_pred_net_vars.m_strafing;
			valve::g_local_player->move_type( ) = cur_local_data.m_pred_net_vars.m_move_type;
			valve::g_local_player->scoped( ) = cur_local_data.m_pred_net_vars.m_scoped;
			valve::g_local_player->walking( ) = cur_local_data.m_pred_net_vars.m_walking;
			valve::g_local_player->lby( ) = cur_local_data.m_pred_net_vars.m_lby;

			valve::g_local_player->set_collision_bounds(
				cur_local_data.m_pred_net_vars.m_obb_min,
				cur_local_data.m_pred_net_vars.m_obb_max
			);

			valve::g_local_player->velocity( ) = cur_local_data.m_pred_net_vars.m_velocity;
			valve::g_local_player->abs_velocity( ) = cur_local_data.m_pred_net_vars.m_abs_velocity;
			valve::g_local_player->third_person_recoil( ) = cur_local_data.m_pred_net_vars.m_third_person_recoil;
			valve::g_local_player->duck_amount( ) = cur_local_data.m_pred_net_vars.m_duck_amount;
			valve::g_local_player->flags( ) = cur_local_data.m_pred_net_vars.m_flags;

			valve::g_local_player->tick_base( ) = cur_local_data.m_pred_net_vars.m_tick_base;

			auto cur_view_angles = cur_user_cmd.m_view_angles;

			if ( shot_in_this_cycle ) {
				if ( cur_user_cmd.m_number == m_local_data.m_shot_cmd_number )
					shot_cmd_view_angles = cur_view_angles;

				if ( cur_user_cmd.m_number > m_local_data.m_shot_cmd_number )
					cur_view_angles = shot_cmd_view_angles;
			}

			const auto last_user_cmd = cur_user_cmd.m_number == user_cmd.m_number;

			update_local( cur_view_angles, !last_user_cmd );

			cur_local_data.m_user_cmd = cur_user_cmd;

			cur_local_data.store_anim( );

			if ( last_user_cmd )
				continue;

			valve::g_input->m_verified_cmds[ j ] = { cur_user_cmd, cur_user_cmd.calc_checksum( ) };
		}

		valve::g_local_player->anim_layers( ) = backup_anim_layers;
	}

	void c_anim_sync::setup_local_bones( ) {
		const auto anim_state = valve::g_local_player->anim_state( );
		if ( !anim_state )
			return;

		const auto& local_data = g_eng_pred->local_data( ).at( g_ctx->last_sent_cmd_number( ) % 150 );
		if ( local_data.m_spawn_time != valve::g_local_player->spawn_time( ) )
			return;

		struct anim_backup_t {
			__forceinline anim_backup_t( )
				: m_anim_state{ *valve::g_local_player->anim_state( ) },
				  m_abs_yaw{ m_anim_state.m_foot_yaw },
				  m_anim_layers{ valve::g_local_player->anim_layers( ) },
				  m_pose_params{ valve::g_local_player->pose_params( ) } {}

			__forceinline void restore( ) const {
				valve::g_local_player->set_abs_angles( { 0.f, m_abs_yaw, 0.f } );

				*valve::g_local_player->anim_state( ) = m_anim_state;

				valve::g_local_player->anim_layers( ) = m_anim_layers;
				valve::g_local_player->pose_params( ) = m_pose_params;
			}

			valve::anim_state_t		m_anim_state{};

			float					m_abs_yaw{};

			valve::anim_layers_t	m_anim_layers{};
			valve::pose_params_t	m_pose_params{};
		} anim_backup{};

		local_data.restore_anim( true );

		if ( m_local_data.m_fake.m_spawn_time == 0.f
			|| m_local_data.m_fake.m_spawn_time != valve::g_local_player->spawn_time( ) ) {
			m_local_data.m_fake.m_anim_state = *valve::g_local_player->anim_state( );

			m_local_data.m_fake.m_spawn_time = valve::g_local_player->spawn_time( );
		}

		if ( !valve::g_client_state->m_choked_cmds
			&& valve::g_global_vars->m_cur_time != m_local_data.m_fake.m_anim_state.m_prev_update_time ) {
			*valve::g_local_player->anim_state( ) = m_local_data.m_fake.m_anim_state;

			anim_state->m_foot_yaw = local_data.m_user_cmd.m_view_angles.y;

			anim_state->update(
				local_data.m_user_cmd.m_view_angles.x,
				local_data.m_user_cmd.m_view_angles.y
			);

			m_local_data.m_fake.m_abs_yaw = anim_state->m_foot_yaw;
			m_local_data.m_fake.m_anim_state = *valve::g_local_player->anim_state( );
			m_local_data.m_fake.m_anim_layers = valve::g_local_player->anim_layers( );
			m_local_data.m_fake.m_pose_params = valve::g_local_player->pose_params( );
		}

		local_data.restore_anim( true );

		valve::g_local_player->anim_layers( ) = m_local_data.m_fake.m_anim_layers;
		valve::g_local_player->pose_params( ) = m_local_data.m_fake.m_pose_params;

		valve::g_local_player->set_abs_angles( { 0.f, m_local_data.m_fake.m_abs_yaw, 0.f } );

		setup_bones( valve::g_local_player, m_local_data.m_fake.m_bones, valve::g_global_vars->m_cur_time, 11 );

		local_data.restore_anim( true );

		setup_bones( valve::g_local_player, m_local_data.m_real.m_bones, valve::g_global_vars->m_cur_time, 27 );

		anim_backup.restore( );
	}
}