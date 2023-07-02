#pragma once

namespace hacks {
	struct extrapolation_data_t {
		__forceinline constexpr extrapolation_data_t( ) = default;

		__forceinline extrapolation_data_t(
			valve::c_player* const player, const lag_record_t* const lag_record
		) : m_player{ player }, m_sim_time{ lag_record->m_sim_time }, m_flags{ lag_record->m_flags },
			m_was_in_air{ !( lag_record->m_flags & valve::e_ent_flags::on_ground ) }, m_origin{ lag_record->m_origin },
			m_velocity{ lag_record->m_velocity }, m_obb_min{ lag_record->m_obb_min }, m_obb_max{ lag_record->m_obb_max } {}

		valve::c_player*	m_player{};

		float				m_sim_time{};

		valve::e_ent_flags	m_flags{};
		bool				m_was_in_air{};

		vec3_t				m_origin{}, m_velocity{},
							m_obb_min{}, m_obb_max{};
	};

	struct aim_point_t {
		__forceinline constexpr aim_point_t( ) = default;

		__forceinline constexpr aim_point_t(
			const vec3_t& pos, const bool center,
			const int hitbox, const int hitgroup,
			const int needed_intersections
		) : m_pos{ pos }, m_center{ center },
			m_hitbox{ hitbox }, m_hitgroup{ hitgroup },
			m_needed_intersections{ needed_intersections } {}

		vec3_t		m_pos{};
		pen_data_t	m_pen_data{};
		bool		m_center{}, m_valid{};
		int			m_intersections{},
					m_hitbox{}, m_hitgroup{},
					m_needed_intersections{};
	};

	struct aim_record_t {
		player_entry_t*					m_entry{};
		std::shared_ptr< lag_record_t > m_lag_record{};
	};

	struct aim_target_t {
		__forceinline constexpr aim_target_t( ) = default;

		__forceinline aim_target_t( const aim_record_t& record )
			: m_entry{ record.m_entry }, m_lag_record{ record.m_lag_record } {}

		player_entry_t*					m_entry{};
		std::shared_ptr< lag_record_t >	m_lag_record{};

		std::vector< aim_point_t >		m_points{};
		aim_point_t*					m_best_point{};
		aim_point_t*					m_best_body_point{};
	};

	class c_aim_bot {
	private:
		std::size_t calc_points_count( const int hitgroups, const int multi_points ) const;

		void scan_point( const aim_target_t& target, aim_point_t& point, const bool ignore_dmg, const vec3_t shoot_pos ) const;

		void scan_center_points( aim_target_t& target, const int hitgroups, const vec3_t shoot_pos ) const;

		void calc_capsule_points(
			aim_target_t& target, const valve::studio_hitbox_t* const hitbox,
			const int index, const mat3x4_t& matrix, float scale
		) const;

		float calc_point_scale(
			const float spread, const float max,
			const float dist, const vec3_t& dir,
			const vec3_t& right, const vec3_t& up
		) const;

		void calc_multi_points(
			aim_target_t& target, const valve::studio_hitbox_t* const hitbox,
			const int index, const mat3x4_t& matrix, float scale
		) const;

	public:

		void scan_points(
			aim_target_t& target, const int hitgroups, const int multi_points, const bool trace, const vec3_t shoot_pos
		) const;

		void player_move( extrapolation_data_t& data ) const;

		bool select_points( aim_target_t& target, const bool additional_scan ) const;

		int config_weapon = 0;

	private:

		std::optional< aim_record_t > extrapolate( const player_entry_t& entry ) const;

		std::optional< aim_record_t > select_latest_record( const player_entry_t& entry ) const;

		std::optional< aim_record_t > select_record( const player_entry_t& entry ) const;

		aim_point_t* select_point( const aim_target_t& target, const int cmd_number ) const;

		void set_cfg_weapon( );

		void find_targets( );

		void scan_targets( );

		aim_target_t* select_target( const int cmd_number );

		vec2_t calc_spread_angle(
			const int bullets, const valve::e_item_index item_index,
			const float recoil_index, const std::size_t i
		) const;

		int calc_hit_chance(
			const aim_target_t* const target, const qangle_t& angle, const aim_point_t* const aim_point
		) const;

		struct cfg_t {
			bool	m_enabled;

			int		m_min_dmg_override_key{}, m_force_baim_key{}, m_force_safe_point_key{};
			
			struct weapon_t {
				bool	m_auto_scope{},
						m_scale_dmg_on_hp{}, m_static_point_scale{},
						m_predictive_auto_stop{};

				int		m_body_scale{}, m_head_scale{},
						m_hit_chance{}, m_auto_stop_type{}, m_min_dmg{},
						m_min_dmg_override{}, m_hitgroups{}, m_multi_points{};
			} weapon_t[6];
		};

		sdk::cfg_var_t< cfg_t >					m_cfg;

		std::array< std::vector< int >, 6u >	m_hitgroups{
			std::vector< int >{ 0 },
			{ 2, 4, 5, 6 },
			{ 3 },
			{ 13, 14, 15, 16, 17, 18 },
			{ 7, 8, 9, 10 },
			{ 11, 12 }
		};

		struct last_target_t {
			player_entry_t*					m_entry{};
			std::shared_ptr< lag_record_t > m_lag_record{};

			int								m_dmg{};
		}										m_last_target{};

		std::vector< aim_target_t >				m_targets{};
	public:
		void on_create_move( valve::user_cmd_t& user_cmd );

		__forceinline const last_target_t& last_target( ) const;

		__forceinline cfg_t& cfg( );
	};

	inline const auto g_aim_bot = std::make_unique< c_aim_bot >( );
}

#include "../../../.hpp/hacks/aim_bot/inl/aim_bot.inl"