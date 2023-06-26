#pragma once

namespace hacks {
	class c_visuals {
	private:
		struct bbox_t {
			vec2_t m_min{}, m_size{};
		};

		struct grenade_simulation_t {
			__forceinline grenade_simulation_t( ) = default;

			__forceinline grenade_simulation_t(
				valve::c_player* const owner, const valve::e_item_index index,
				const vec3_t& origin, const vec3_t& velocity, const float throw_time, const int offset
			) : m_owner{ owner }, m_index{ index } { predict( origin, velocity, throw_time, offset ); }

			void predict( const vec3_t& origin, const vec3_t& velocity, const float throw_time, const int offset );

			bool physics_simulate( );

			void physics_trace_entity(
				const vec3_t& src, const vec3_t& dst,
				const valve::e_mask mask, valve::trace_t& trace
			);

			void physics_push_entity( const vec3_t& push, valve::trace_t& trace );

			void perform_fly_collision_resolution( valve::trace_t& trace );

			void think( );

			void detonate( const bool bounced );

			void update_path( const bool bounced );

			bool										m_detonated{};
			valve::c_player*							m_owner{};
			vec3_t										m_origin{}, m_velocity{};
			valve::c_entity*							m_last_hit_entity{}, *m_last_breakable{};
			float										m_detonate_time{}, m_expire_time{};
			valve::e_item_index							m_index{};
			int											m_tick{}, m_next_think_tick{},
														m_last_update_tick{}, m_bounces_count{}, m_collision_group{};
			std::vector< std::pair< vec3_t, bool > >	m_path{};
		};

		using throwed_grenades_t = std::unordered_map< valve::e_ent_handle, grenade_simulation_t >;

		bool add_grenade_simulation( const grenade_simulation_t& sim, const bool warning ) const;

		bool calc_bbox( valve::c_player* const player, bbox_t& bbox ) const;

		std::optional< valve::bones_t > try_to_lerp_bones( const int index ) const;

		valve::material_t* create_material(
			const std::string_view name,
			const std::string_view shader, const std::string_view data
		) const;

		void override_material(
			const int type, const bool ignore_z,
			const float r, const float g, const float b, const float a
		) const;

		void add_line(
			const vec2_t& from, const vec2_t& to, const std::uint32_t clr
		) const;

		void add_rect(
			const vec2_t& pos, const vec2_t& size, const std::uint32_t clr
		) const;

		void add_rect_filled(
			const vec2_t& pos, const vec2_t& size, const std::uint32_t clr
		) const;

		void add_text(
			vec2_t pos, const unsigned long font,
			const std::string_view str, const std::uint32_t clr, const int flags
		) const;

		void add_text(
			vec2_t pos, const unsigned long font,
			const std::wstring_view str, const std::uint32_t clr, const int flags
		) const;

		void add_vertices(
			const valve::vertex_t* const vertices,
			const std::size_t vertices_count, const std::uint32_t clr
		) const;

		void add_circle(
			const vec2_t& pos, const float radius,
			const int segments_count, const std::uint32_t clr
		) const;

		void add_filled_circle(
			const vec2_t& pos, const float radius,
			const int segments_count, const std::uint32_t clr
		) const;

		void add_beam_ring( const vec3_t& pos, const float* const clr ) const;

		void add_beam( const vec3_t& from, const vec3_t& to, const float* const clr ) const;

		void add_grenade_warning_beam( const vec3_t& from, const vec3_t& to, const float* const clr ) const;

		void oof_arrow( const vec3_t& pos ) const;

		void handle_player( valve::c_player* const player ) const;

		void handle_projectile( valve::c_entity* const entity, const valve::e_class_id class_id );

		void handle_weapon( valve::c_weapon* const weapon ) const;

		struct cfg_t {
			struct {
				bool	m_teammates{}, m_dormant{}, m_bbox{},
						m_health_bar{}, m_name{}, m_flags{},
						m_wpn_text{}, m_ammo{}, m_dist{},
						m_glow{}, m_hit_marker{}, m_hit_marker_sound{},
						m_sounds{}, m_money{}, m_skeleton{}, m_oof_arrow{};

				int		m_oof_arrow_size{ 12 }, m_oof_arrow_dist{ 50 };

				float	m_bbox_clr[ 3u ], m_name_clr[ 3u ],
						m_wpn_clr[ 3u ]{}, m_ammo_clr[ 3u ]{},
						m_sounds_clr[ 3u ]{}, m_glow_clr[ 4u ]{},
						m_skeleton_clr[ 3u ]{}, m_oof_arrow_clr[ 3u ]{};
			} m_player{};

			struct {
				bool	m_player{}, m_player_behind_wall{},
						m_teammate{}, m_teammate_behind_wall{},
						m_local_player{}, m_local_player_fake{},
						m_on_shot{}, m_ragdoll{}, m_hands{},
						m_wpn_view_model{}, m_weapons{}, m_shadow{}, m_props{};

				int		m_player_mdl_type{}, m_teammate_mdl_type{},
						m_local_player_mdl_type{}, m_local_player_fake_mdl_type{},
						m_on_shot_mdl_type{}, m_shadow_mdl_type{}, m_weapons_mdl_type{},
						m_hands_mdl_type{}, m_wpn_view_model_mdl_type{};

				float	m_on_shot_mdl_time{},
						m_player_clr[ 4u ]{}, m_player_behind_wall_clr[ 4u ]{},
						m_teammate_clr[ 4u ]{}, m_teammate_behind_wall_clr[ 4u ]{},
						m_local_player_clr[ 4u ]{}, m_local_player_fake_clr[ 4u ]{},
						m_on_shot_clr[ 4u ]{}, m_hands_clr[ 4u ]{},
						m_wpn_view_model_clr[ 4u ]{}, m_weapons_clr[ 4u ]{}, m_shadow_clr[ 4u ], m_props_clr[ 3u ]{};
			} m_clred_mdls{};

			struct {
				bool	m_radar{}, m_dropped_wpns{},
						m_grenades{}, m_inaccuracy{}, m_recoil{},
						m_crosshair{}, m_bomb{},
						m_grenade_trajectory{}, m_grenade_warning{},
						m_spectators{}, m_penetration_reticle{};

				int		m_indicators{};

				float	m_dropped_wpns_clr[ 3u ]{},
						m_grenades_clr[ 3u ]{},
						m_inaccuracy_clr[ 3u ]{}, m_bomb_clr[ 4u ]{},
						m_grenade_trajectory_clr[ 3u ]{};
			} m_other{};

			struct {
				bool	m_remove_flash{}, m_remove_smoke{},
						m_remove_fog{}, m_remove_skybox{},
						m_disable_post_processing{}, m_remove_scope_overlay{},
						m_force_thirdperson_alive{}, m_force_thirdperson_dead{},
						m_disable_rendering_of_teammates{}, m_disable_rendering_of_ragdolls{},
						m_bullet_tracers{}, m_bullet_impacts{}, m_fov_changer{}, m_viewmodel_fov_changer{},
						m_aspect_ratio_changer{};

				int		m_recoil_adjustment{},
						m_transparent_walls{ 100 },
						m_transparent_props{ 100 },
						m_brightness_adjustment{},
						m_force_thirdperson_key{},
						m_fov{ 90 }, m_viewmodel_fov{ 60 },
						m_thirdperson_distance{ 150 },
						m_aspect_ratio{ 100 };

				float	m_bullet_tracers_clr[ 3u ]{}, m_brightess_mod[ 3u ]{};
			} m_effects{};
		};

		sdk::cfg_var_t< cfg_t >			m_cfg{ 0x2f607ebeu, {} };

		struct {
			unsigned long	m_verdana12{}, m_verdana26{},
							m_verdana26n{}, m_small_fonts8{};
		}								m_fonts{};

		struct bullet_impact_t {
			__forceinline bullet_impact_t( ) = default;

			__forceinline bullet_impact_t(
				const float time, const vec3_t& from, const vec3_t& pos
			) : m_time{ time }, m_from{ from }, m_pos{ pos } {}

			float	m_time{};
			bool	m_final{ true };

			vec3_t	m_from{}, m_pos{};
		};

		struct shot_mdl_t {
			float						m_time{};
			valve::bones_t				m_bones{};
			mat3x4_t					m_world_matrix{};

			valve::model_render_info_t	m_info{};
			valve::draw_model_state_t	m_state{};
		};

		int								m_backup_smoke_count{};

		std::vector< vec3_t >			m_player_sounds{};
		std::vector< bullet_impact_t >	m_bullet_impacts{};

		float							m_last_hurt_time{};
		std::optional< pen_data_t >		m_pen_data{};

		grenade_simulation_t			m_grenade_trajectory{};
		throwed_grenades_t				m_throwed_grenades{};

		std::vector< shot_mdl_t >		m_shot_mdls{};
		bool							m_rendering_shot_mdl{};
	public:
		void init( );

		void on_paint( );

		void on_render_start( );

		void on_render_end( );

		void on_create_move( const valve::user_cmd_t& cmd );

		void on_override_view( valve::view_setup_t* const setup );

		void on_post_screen_effects( );

		bool on_draw_model(
			valve::studio_render_t* const ecx, const std::uintptr_t edx,
			const std::uintptr_t a0, const valve::draw_model_info_t& info,
			mat3x4_t* const bones, float* const a3, float* const a4, const vec3_t& origin, int flags
		) const;

		void on_calc_view(
			valve::c_player* const ecx, const std::uintptr_t edx,
			vec3_t& origin, qangle_t& angles, float& z_near, float& z_far, float& fov
		);

		void add_shot_mdl( valve::c_player* const player, const std::shared_ptr< lag_record_t >& lag_record );

		__forceinline float& last_hurt_time( );

		__forceinline std::vector< vec3_t >& player_sounds( );

		__forceinline std::vector< bullet_impact_t >& bullet_impacts( );

		__forceinline cfg_t& cfg( );
	};

	inline const auto g_visuals = std::make_unique< c_visuals >( );
}

#include "../../../.hpp/hacks/visuals/inl/visuals.inl"