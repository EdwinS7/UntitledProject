#include "ctx.hpp"

ALWAYS_INLINE c_ctx::addresses_t& c_ctx::addresses( ) {
	return m_addresses;
}

ALWAYS_INLINE c_ctx::cvars_t& c_ctx::cvars( ) {
	return m_cvars;
}

ALWAYS_INLINE c_ctx::net_info_t& c_ctx::net_info( ) {
	return m_net_info;
}

ALWAYS_INLINE vec3_t& c_ctx::shoot_pos( ) {
	return m_shoot_pos;
}

ALWAYS_INLINE int& c_ctx::last_sent_cmd_number( ) {
	return m_last_sent_cmd_number;
}

ALWAYS_INLINE int& c_ctx::last_cmd_number( ) {
	return m_last_cmd_number;
}

ALWAYS_INLINE bool& c_ctx::allow_anim_update( ) {
	return m_allow_anim_update;
}

ALWAYS_INLINE bool& c_ctx::allow_setup_bones( ) {
	return m_allow_setup_bones;
}

ALWAYS_INLINE bool& c_ctx::force_bone_mask( ) {
	return m_force_bone_mask;
}

ALWAYS_INLINE std::vector< int >& c_ctx::sented_cmds( ) {
	return m_sented_cmds;
}

ALWAYS_INLINE e_context_flags& c_ctx::flags( ) {
	return m_flags;
}

ALWAYS_INLINE valve::c_weapon*& c_ctx::weapon( ) {
	return m_weapon;
}

ALWAYS_INLINE valve::weapon_data_t*& c_ctx::wpn_data( ) {
	return m_wpn_data;
}

ALWAYS_INLINE bool& c_ctx::freeze_time( ) {
	return m_freeze_time;
}

ALWAYS_INLINE bool& c_ctx::broke_lc( ) {
	return m_broke_lc;
}

ALWAYS_INLINE vec3_t& c_ctx::last_sent_origin( ) {
	return m_last_sent_origin;
}