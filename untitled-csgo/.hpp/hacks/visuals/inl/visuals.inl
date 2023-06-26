#pragma once

#include "../visuals.hpp"

namespace hacks {
	__forceinline float& c_visuals::last_hurt_time( ) { return m_last_hurt_time; }

	__forceinline std::vector< vec3_t >& c_visuals::player_sounds( ) { return m_player_sounds; }

	__forceinline std::vector< c_visuals::bullet_impact_t >& c_visuals::bullet_impacts( ) { return m_bullet_impacts; }

	__forceinline c_visuals::cfg_t& c_visuals::cfg( ) { return m_cfg.value( ); }
}