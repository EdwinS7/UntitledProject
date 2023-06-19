#pragma once

// includes
#include "../../inc.hpp"

class c_ctx {
public:
	void update( );

	int get_framerate( );

private:

	int m_fps{ 60 };
};

inline const auto g_ctx = std::make_unique<c_ctx>( );