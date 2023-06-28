#pragma once

// includes
#include "../../inc.hpp"

class c_win32 {
public:
	void set_window_title( const char* title );

	bool create_window( const char* title, bool full_screen, vector2_t< uint16_t > size );

	bool message_box( const char* title, const char* caption, uint8_t type );

	HWND get_hwnd( );

	vector2_t<uint16_t> get_pos( );

	vector2_t<uint16_t> get_size( );

	bool dispatch_messages( );
private:

	HWND m_hwnd;
	WNDCLASSEX window_class;
};

inline const auto g_win32 = std::make_unique<c_win32>( );