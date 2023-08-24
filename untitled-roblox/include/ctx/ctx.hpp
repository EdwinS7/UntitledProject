#pragma once

#include "../../inc.hpp"

class c_ctx {
public:
	void init( );
	void listener( );

	uintptr_t script_context, lua_state;
};

inline const auto g_ctx = std::make_unique<c_ctx>( );