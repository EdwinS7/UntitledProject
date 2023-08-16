#pragma once

// includes
#include "../../inc.hpp"

class c_win32 {
public:
	void set_window_title( const char* title );

	void create_window( const char* title, const vector2_t< uint16_t > size );

	void create_message_box( const char* title, const char* caption, uint8_t type );

	bool dispatch_messages( );

	void set_rect( const vector2_t<uint16_t> pos, const vector2_t<uint16_t> size );

	vector2_t<uint16_t> get_pos( );

	vector2_t<uint16_t> get_size( );

	HWND get_hwnd( );
private:

	HWND m_hwnd;
	WNDCLASSEX m_window_class;
};

inline const auto g_win32 = std::make_unique<c_win32>( );