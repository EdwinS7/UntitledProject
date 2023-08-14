#pragma once

// includes
#include "../../inc.hpp"

class c_console {
public:
	void draw( );
	void handle( );

	void log( color_t clr, const char* msg );

private:
	std::vector<std::tuple<const char*, color_t, float>> history; // text, color, time remaining

	FILE* m_file_pointer;
};

inline const auto g_console = std::make_unique<c_console>( );