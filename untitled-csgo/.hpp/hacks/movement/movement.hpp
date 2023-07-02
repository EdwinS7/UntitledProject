#pragma once

namespace hacks {
	class c_movement {
	private:
		void accelerate(
			const valve::user_cmd_t& user_cmd, const vec3_t& wishdir,
			const float wishspeed, vec3_t& velocity, float acceleration
		) const;

		void walk_move(
			const valve::user_cmd_t& user_cmd, vec3_t& move,
			vec3_t& fwd, vec3_t& right, vec3_t& velocity
		) const;

		void full_walk_move(
			const valve::user_cmd_t& user_cmd, vec3_t& move,
			vec3_t& fwd, vec3_t& right, vec3_t& velocity
		) const;

		void modify_move( valve::user_cmd_t& user_cmd, vec3_t& velocity ) const;

		void predict_move( const valve::user_cmd_t& user_cmd, vec3_t& velocity ) const;

		void auto_strafe( valve::user_cmd_t& user_cmd );

		struct cfg_t {
			bool	m_slide_walk{}, m_slow_walk{}, m_remove_crouch_cooldown{},
					m_auto_bhop{}, m_fast_stop{};

			float	m_slow_walk_speed{};
			int		m_auto_strafe_type{}, m_fake_duck_key{}, m_slow_walk_key{}, m_auto_peek_key{};
		};

		sdk::cfg_var_t< cfg_t > m_cfg{ 0x05562e58u, {} };

		vec3_t					m_velocity{}, m_auto_peek_start_pos{};

		int						m_stop_type{};
		bool					m_should_fake_duck{}, m_strafe_switch{}, m_allow_early{}, m_auto_peek_is_firing{};
		float					m_max_player_speed{}, m_max_weapon_speed{}, m_prev_view_yaw{};
	public:
		bool is_peeking( int ticks = 6 );

		void fast_stop( valve::user_cmd_t& user_cmd );

		bool stop( valve::user_cmd_t& user_cmd );

		bool force_lby_update( valve::user_cmd_t& user_cmd );

		void normalize( valve::user_cmd_t& user_cmd ) const;

		void rotate(
			valve::user_cmd_t& user_cmd, const qangle_t& wish_angles,
			const valve::e_ent_flags flags, const valve::e_move_type move_type
		) const;

		void auto_peek( valve::user_cmd_t& user_cmd );

		void on_create_move( valve::user_cmd_t& user_cmd );

		__forceinline bool& should_fake_duck( );

		__forceinline bool& allow_early( );

		__forceinline float& prev_view_yaw( );

		__forceinline int& stop_type( );

		__forceinline vec3_t& auto_peek_start_pos( );

		__forceinline bool& auto_peek_is_firing( );

		__forceinline bool& forced_lby_update( );

		__forceinline cfg_t& cfg( );
	};

	inline const auto g_movement = std::make_unique< c_movement >( );
}

#include "../../../.hpp/hacks/movement/inl/movement.inl"