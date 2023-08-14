#pragma once

#include "../../inc.hpp"

#ifdef UNTITLED_USE_GUI

class c_style {
public:
	uint8_t window_radius = 5;
	uint8_t popup_radius = 5;

	uint8_t element_radius = 3;

};

class c_dark {
public:
	color_t accent = { 255, 107, 107, 255 };

	color_t window = { 15, 15, 15, 255 };
	color_t window_frame = { 0, 0, 0, 255 };
	color_t window_header = { 25, 25, 25, 255 };
};

class c_gui {
public:
	c_gui::c_gui( const char* title, vector2_t<uint16_t> size );

	void start( );

	void end( );

private:
	const char* m_title;
	vector2_t<uint16_t> m_pos;
	vector2_t<uint16_t> m_size;
	
	c_style m_style;
	c_dark m_color;
};

//inline const auto g_input = std::make_unique<c_input>( );

#endif