#pragma once

#include "../aim_bot.hpp"

namespace hacks {
	__forceinline const c_aim_bot::last_target_t& c_aim_bot::last_target( ) const { return m_last_target; }

	__forceinline c_aim_bot::cfg_t& c_aim_bot::cfg( ) { return m_cfg.value( ); }
}