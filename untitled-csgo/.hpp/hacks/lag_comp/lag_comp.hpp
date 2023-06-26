#pragma once

namespace hacks {
	struct lag_backup_t {
		__forceinline constexpr lag_backup_t( ) = default;

		__forceinline lag_backup_t( valve::c_player* const player );

		__forceinline void restore( valve::c_player* const player ) const;

		float			m_foot_yaw{};
		vec3_t			m_origin{}, m_abs_origin{},
						m_obb_min{}, m_obb_max{};

		valve::bones_t	m_bones{};
		int				m_bones_count{},
						m_readable_bones{}, m_writable_bones{};

		unsigned long	m_mdl_bone_counter{};
	};

	struct lag_anim_data_t;

	struct lag_record_t {
		struct anim_side_t {
			valve::bones_t			m_bones{};
			std::size_t				m_bones_count{};

			float					m_foot_yaw{}, m_prev_foot_yaw{},
									m_move_yaw_ideal{}, m_move_yaw_cur_to_ideal{},
									m_move_yaw{};

			valve::anim_layers_t	m_anim_layers{};
			valve::pose_params_t    m_pose_params{};
		};

		__forceinline constexpr lag_record_t( ) = default;

		__forceinline lag_record_t(
			valve::c_player* const player
		) : m_dormant{ player->dormant( ) },
			m_flags{ player->flags( ) },
			m_sim_ticks{ valve::to_ticks( player->sim_time( ) - player->old_sim_time( ) ) },
			m_total_cmds{ m_sim_ticks }, m_receive_tick{ valve::g_client_state->m_server_tick },
			m_sim_time_delay{ valve::g_client_state->m_server_tick - valve::to_ticks( player->sim_time( ) ) },
			m_sim_time{ player->sim_time( ) },
			m_lby{ player->lby( ) }, m_duck_amount{ player->duck_amount( ) },
			m_third_person_recoil{ player->third_person_recoil( ) }, m_lc_exploit{ m_sim_time_delay >= 64 },
			m_walking{ player->walking( ) }, m_weapon{ player->weapon( ) },
			m_last_shot_time{ m_weapon ? m_weapon->last_shot_time( ) : std::numeric_limits< float >::max( ) },
			m_eye_angles{ player->eye_angles( ) },
			m_velocity{ player->velocity( ) }, m_origin{ player->origin( ) },
			m_obb_min{ player->obb_min( ) }, m_obb_max{ player->obb_max( ) },
			m_anim_layers{ player->anim_layers( ) } {}

		__forceinline void restore( valve::c_player* const player, const int anim_index, const bool only_anim = false ) const;

		__forceinline void correct( const lag_anim_data_t* const previous );

		__forceinline bool valid( ) const;

		bool							m_dormant{}, m_trying_to_resolve{},
										m_broke_lc{}, m_extrapolated{}, m_shot{}, m_reforce_safe_points{}, m_force_safe_points{}, m_invalid{};

		valve::e_ent_flags				m_flags{};
		int								m_sim_ticks{}, m_total_cmds{}, m_anim_side{}, m_receive_tick{}, m_sim_time_delay{};
		float							m_sim_time{}, m_max_delta{}, m_lby{}, m_duck_amount{}, m_on_ground_time{}, m_third_person_recoil{};

		bool							m_lc_exploit{}, m_walking{};

		std::optional< bool >			m_jumped{};

		valve::c_weapon*				m_weapon{};
		float							m_last_shot_time{};

		qangle_t						m_eye_angles{};
		vec3_t							m_velocity{}, m_origin{}, m_obb_min{}, m_obb_max{};

		valve::anim_layers_t			m_anim_layers{};

		std::array< anim_side_t, 3u >	m_anim_sides{};
	};

	struct lag_anim_data_t {
		__forceinline constexpr lag_anim_data_t( ) = default;

		__forceinline lag_anim_data_t( const lag_record_t* const lag_record );

		struct anim_side_t {
			float	m_foot_yaw{}, m_prev_foot_yaw{},
					m_move_yaw_ideal{}, m_move_yaw_cur_to_ideal{},
					m_move_yaw{};
		};

		valve::c_weapon*				m_weapon{};

		qangle_t						m_eye_angles{};
		vec3_t							m_origin{}, m_velocity{};

		valve::e_ent_flags				m_flags{};
		float							m_sim_time{}, m_lby{}, m_duck_amount{};

		bool                            m_dormant{};

		valve::anim_layers_t			m_anim_layers{};

		std::array< anim_side_t, 3u >	m_anim_sides{};
	};

	struct player_entry_t {
		struct interpolated_t {
			__forceinline constexpr interpolated_t( ) = default;

			__forceinline constexpr interpolated_t(
				const float sim_time, const float duck_amount,
				const valve::e_ent_flags flags, const vec3_t& velocity
			) : m_sim_time{ sim_time }, m_duck_amount{ duck_amount },
				m_flags{ flags }, m_velocity{ velocity } {}

			float				m_sim_time{},
								m_duck_amount{};

			valve::e_ent_flags	m_flags{};
			vec3_t				m_velocity{};

		};

		__forceinline void reset( );

		__forceinline void interpolate(
			const lag_anim_data_t* const from, const lag_record_t* const to
		);

		valve::c_player*								m_player{};

		float											m_spawn_time{},
														m_receive_time{},
														m_alive_loop_cycle{},
														m_last_sim{},
														m_broke_sim{};

		valve::bones_t									m_bones{};

		bool											m_unk{}, m_try_anim_resolver{ true };
		int												m_misses{}, m_prev_anim_side{};

		std::optional< lag_anim_data_t >				m_prev_anim_data{};

		std::deque< std::shared_ptr< lag_record_t > >	m_lag_records{};
		std::vector< interpolated_t >					m_interpolated{};
	};

	class c_lag_comp {
	private:
		std::array< player_entry_t, 64u > m_entries{};
	public:
		void on_net_update( );

		__forceinline float calc_time_delta( const float sim_time ) const;

		__forceinline player_entry_t& entry( const std::size_t i );
	};

	inline const auto g_lag_comp = std::make_unique< c_lag_comp >( );
}

#include "../../../.hpp/hacks/lag_comp/inl/lag_comp.inl"