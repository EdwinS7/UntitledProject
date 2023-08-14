#pragma once

// includes
#include "../../inc.hpp"

#ifdef UNTITLED_USE_CONSOLE

enum log_level {
	LOG_DEFAULT = 0,
	LOG_INFO,
	LOG_WARNING,
	LOG_ERROR,
	LOG_SUCCESS
};

class c_console {
public:
	void create( const char* title, vector2_t<uint16_t> size );
	void log( int level, const char* msg );

private:
	FILE* file_pointer;
};

inline const auto g_console = std::make_unique<c_console>( );

#endif