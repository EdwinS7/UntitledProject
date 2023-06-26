#pragma once

#include "skins.hpp"

namespace hacks {
	__forceinline const std::array< c_skins::weapon_t, c_skins::k_max_wpns >& c_skins::weapons( ) {
		return m_weapons;
	}

	__forceinline const std::array< c_skins::weapon_t, c_skins::k_max_knives >& c_skins::knives( ) {
		return m_knives;
	}

	__forceinline const std::vector< c_skins::paint_kit_t >& c_skins::paint_kits( ) {
		return m_paint_kits;
	}

	__forceinline const std::vector< c_skins::paint_kit_t >& c_skins::raw_paint_kits( ) {
		return m_raw_paint_kits;
	}

	__forceinline std::atomic< bool >& c_skins::force_update( ) {
		return m_force_update;
	}

	__forceinline c_skins::cfg_t& c_skins::cfg( ) { return m_cfg.value( ); }
}