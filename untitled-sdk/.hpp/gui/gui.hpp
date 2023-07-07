#pragma once

#include "../../inc.hpp"

class c_gui {
public:
	void draw( );

	bool is_open{ true };

private:

};

inline const auto g_gui = std::make_unique<c_gui>( );