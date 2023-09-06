#pragma once

#include "../../common.hpp"

class c_utilities {
public:
	void open_console( const std::string& title );

};

inline const auto g_utilities = std::make_unique<c_utilities>( );