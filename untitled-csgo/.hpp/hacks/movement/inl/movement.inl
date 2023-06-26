#pragma once

#include "../movement.hpp"

namespace hacks {
	__forceinline bool& c_movement::should_fake_duck( ) {
		return m_should_fake_duck;
	}

	__forceinline bool& c_movement::allow_early( ) {
		return m_allow_early;
	}

	__forceinline float& c_movement::prev_view_yaw( ) {
		return m_prev_view_yaw;
	}

	__forceinline int& c_movement::stop_type( ) {
		return m_stop_type;
	}

	__forceinline vec3_t& c_movement::auto_peek_start_pos( ) {
		return m_auto_peek_start_pos;
	}

	__forceinline bool& c_movement::auto_peek_is_firing( ) {
		return m_auto_peek_is_firing;
	}

	__forceinline c_movement::cfg_t& c_movement::cfg( ) {
		return m_cfg.value( );
	}
}