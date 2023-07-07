#pragma once

#include "../../inc.hpp"

namespace valve {
	struct cvar_t;

	struct user_cmd_t;

	class c_player;

	class c_weapon;

	struct weapon_data_t;
}

enum e_context_flags : std::uint8_t {
	can_choke = 1u << 0u,
	choke = 1u << 1u,
	can_shoot = 1u << 2u,
	recharge = 1u << 3u,
	aim_fire = 1u << 4u,
	has_target = 1u << 5u
};
DEFINE_ENUM_FLAG_OPERATORS( e_context_flags )

class c_ctx {
private:
	struct addresses_t {
		using random_float_t = float( __cdecl* )( const float, const float );

		using random_int_t = int( __cdecl* )( const int, const int );

		using random_seed_t = void( __cdecl* )( const int );

		using angle_matrix_t = void( __fastcall* )( const qangle_t&, mat3x4_t& );

		random_float_t m_random_float{};
		random_int_t m_random_int{};
		random_seed_t m_random_seed{};

		std::uintptr_t m_reset_anim_state{}, m_update_anim_state{},
			m_set_abs_angles{}, m_set_abs_origin{},
			m_lookup_seq_act{}, m_breakable{}, m_user_cmd_calc_checksum{},
			m_set_collision_bounds{}, m_invalidate_bone_cache{}, m_lookup_bone{},
			m_trace_filter_simple_vtable{}, m_trace_filter_skip_two_entities_vtable{},
			m_write_user_cmd{}, m_weapon_system{}, m_calc_shotgun_spread{},
			m_alloc_key_values_engine{}, m_alloc_key_values_client{}, m_key_values_init{},
			m_key_values_load_from_buffer{}, m_cam_sv_cheats_ret{}, m_add_notify{}, m_ret_to_scope_arc{},
			m_ret_to_scope_lens{}, m_set_clan_tag{}, m_start_drawing{}, m_finish_drawing{},
			m_add_clred_text{}, m_calc_text_size{}, m_set_font_glyph_set{}, m_has_c4{}, m_ret_insert_into_tree{},
			m_smoke_count{}, m_disable_post_processing{}, m_crosshair_ret{}, m_ret_to_scope_clear{}, m_ret_to_scope_blurry{},
			m_ret_to_eye_pos_and_vectors{}, m_ret_to_extrapolation{}, m_item_system{},
			m_hud{}, m_find_hud_element{}, m_clear_hud_wpn_icon{};

		int* m_pred_seed{};
		valve::c_player* m_pred_player{};

		angle_matrix_t m_angle_matrix{};
	}m_addresses{};

	struct cvars_t {
		valve::cvar_t* m_r_jiggle_bones{}, * m_cl_interp{},
			* m_cl_interp_ratio{}, * m_cl_updaterate{},
			* m_ff_dmg_reduction_bullets, * m_ff_dmg_bullet_pen{},
			* m_sv_auto_bhop{}, * m_sv_clockcorrection_msecs{},
			* m_sv_maxvelocity{}, * m_sv_friction{},
			* m_sv_accelerate{}, * m_sv_accelerate_use_weapon_speed{},
			* m_weapon_accuracy_nospread{}, * m_sv_maxunlag{},
			* m_sv_enable_bhop{}, * m_sv_jump_impulse{}, * m_sv_gravity{},
			* m_weapon_recoil_scale{}, * m_weapon_accuracy_shotgun_spread_patterns{},
			* m_molotov_throw_detonate_time{}, * m_weapon_molotov_maxdetonateslope{},
			* m_cl_fullupdate{}, * m_cl_forwardspeed{};
	}m_cvars{};

	struct net_info_t {
		float	m_lerp{};

		struct {
			float m_in{}, m_out{};
		}m_latency{};
	}m_net_info{};

	vec3_t m_shoot_pos{}, m_last_sent_origin{};
	int	m_last_sent_cmd_number{}, m_last_cmd_number{};
	bool m_freeze_time{}, m_allow_anim_update{}, m_allow_setup_bones{}, m_force_bone_mask{}, m_broke_lc{};

	std::vector< int >		m_sented_cmds{};

	e_context_flags			m_flags{};

	valve::c_weapon* m_weapon{};
	valve::weapon_data_t* m_wpn_data{};

public:
	void init( );

	ALWAYS_INLINE addresses_t& addresses( );

	ALWAYS_INLINE cvars_t& cvars( );

	ALWAYS_INLINE net_info_t& net_info( );

	ALWAYS_INLINE vec3_t& shoot_pos( );

	ALWAYS_INLINE int& last_sent_cmd_number( );

	ALWAYS_INLINE int& last_cmd_number( );

	ALWAYS_INLINE bool& allow_anim_update( );

	ALWAYS_INLINE bool& allow_setup_bones( );

	ALWAYS_INLINE bool& force_bone_mask( );

	ALWAYS_INLINE std::vector< int >& sented_cmds( );

	ALWAYS_INLINE e_context_flags& flags( );

	ALWAYS_INLINE valve::c_weapon*& weapon( );

	ALWAYS_INLINE valve::weapon_data_t*& wpn_data( );

	ALWAYS_INLINE bool& freeze_time( );

	ALWAYS_INLINE bool& broke_lc( );

	ALWAYS_INLINE vec3_t& last_sent_origin( );
};

inline const auto g_ctx = std::make_unique<c_ctx>( );

#include "ctx.inl"