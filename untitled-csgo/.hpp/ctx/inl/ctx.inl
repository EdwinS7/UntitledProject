#pragma once

#include "../ctx.hpp"

__forceinline c_ctx::addresses_t& c_ctx::addresses( ) {
	return m_addresses;
}

__forceinline c_ctx::cvars_t& c_ctx::cvars( ) {
	return m_cvars;
}

__forceinline c_ctx::net_info_t& c_ctx::net_info( ) {
	return m_net_info;
}

__forceinline vec3_t& c_ctx::shoot_pos( ) {
	return m_shoot_pos;
}

__forceinline int& c_ctx::last_sent_cmd_number( ) {
	return m_last_sent_cmd_number;
}

__forceinline int& c_ctx::last_cmd_number( ) {
	return m_last_cmd_number;
}

__forceinline bool& c_ctx::allow_anim_update( ) {
	return m_allow_anim_update;
}

__forceinline bool& c_ctx::allow_setup_bones( ) {
	return m_allow_setup_bones;
}

__forceinline bool& c_ctx::force_bone_mask( ) {
	return m_force_bone_mask;
}

__forceinline std::vector< int >& c_ctx::sented_cmds( ) {
	return m_sented_cmds;
}

__forceinline e_context_flags& c_ctx::flags( ) {
	return m_flags;
}

__forceinline valve::c_weapon*& c_ctx::weapon( ) {
	return m_weapon;
}

__forceinline valve::weapon_data_t*& c_ctx::wpn_data( ) {
	return m_wpn_data;
}

__forceinline bool& c_ctx::freeze_time( ) {
	return m_freeze_time;
}

__forceinline bool& c_ctx::broke_lc( ) {
	return m_broke_lc;
}

__forceinline vec3_t& c_ctx::last_sent_origin( ) {
	return m_last_sent_origin;
}