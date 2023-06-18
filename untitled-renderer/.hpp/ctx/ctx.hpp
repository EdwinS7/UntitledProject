#pragma once

// includes
#include "../../inc.hpp"

class c_ctx {
public:
	int get_framerate( );

private:

};

inline const auto g_ctx = std::make_unique<c_ctx>( );