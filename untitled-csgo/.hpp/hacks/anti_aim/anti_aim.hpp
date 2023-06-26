#pragma once

namespace hacks {
	class c_anti_aim {
	private:
		void at_target( float& yaw ) const;

		bool auto_direction( float& yaw );

		struct cfg_t {
			struct {
				bool	m_enabled{}, m_adaptive{};
				int		m_conditions{}, m_limit{};
			}		m_fake_lag{};

			bool	m_enabled{},
					m_jitter_side{},
					m_at_target{}, m_desync{};

			float	m_yaw_offset{}, m_jitter_amount{},
					m_yaw_offset_inverted{}, m_desync_limit{ 60.f };

			int		m_pitch{},
					m_on_shot_side{}, m_side_invert_key{},
					m_manual_left_key{}, m_manual_right_key{},
					m_at_target_type{}, m_auto_dir_type{}, m_yaw_type{};
		};

		sdk::cfg_var_t< cfg_t > m_cfg{ 0xe01e4dbbu, {} };

		bool					m_choke_cycle_switch{};
		int						m_prev_tick_count{}, m_auto_dir_side{},
								m_side_counter{}, m_on_shot_side_counter{}, m_choke_start_cmd_number{};
	public:
		int select_side( );

		void select_yaw( float& yaw, const int side );

		void choke( valve::user_cmd_t* const user_cmd );

		void set_pitch( valve::user_cmd_t& user_cmd );

		void process( valve::user_cmd_t& user_cmd, const float yaw, const int side, const int choked_cmds ) const;

		__forceinline bool enabled( const valve::user_cmd_t* const user_cmd ) const;

		__forceinline cfg_t& cfg( );
	};

	inline const auto g_anti_aim = std::make_unique< c_anti_aim >( );
}

#include "../../../.hpp/hacks/anti_aim/inl/anti_aim.inl"