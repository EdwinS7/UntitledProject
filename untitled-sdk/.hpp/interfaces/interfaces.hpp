#pragma once

#include "../../inc.hpp"

namespace interfaces {
	class c_render {
	public:
		PAD( 0x178 );
		IDXGISwapChain* pSwapChain;
	};

	inline c_render* m_render = nullptr;
	inline IDXGISwapChain* m_swap_chain = nullptr;
};