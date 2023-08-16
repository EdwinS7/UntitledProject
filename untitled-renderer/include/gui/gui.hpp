#pragma once

#include "../../inc.hpp"

#ifdef UNTITLED_USE_GUI

enum window_flags {
	none = 0,
	no_background,
	no_border,
	no_header,
	no_footer,
	no_dragging
};

class c_style {
public:
	int8_t window_radius = 10;
	int8_t popup_radius = 5;

	int8_t element_radius = 3;

};

class c_dark {
public:
	color_t accent = { 211,165,176, 255 };

	color_t window = { 20, 20, 20, 255 };
	color_t window_header = { 42, 42, 42, 255 };
	color_t window_frame = { 0, 0, 0, 255 };

	color_t text_active = { 255, 255, 255, 255 };
	color_t text_inactive = { 150, 150, 150, 255 };
};

class c_gui {
public:
	c_gui::c_gui( const char* title, vector2_t<int16_t> size, window_flags flags );

	void start( );
	void end( );

private:
	const char* m_title;
	vector2_t<int16_t> m_pos;
	vector2_t<int16_t> m_size;
	window_flags m_flags;
	
	c_style m_style;
	c_dark m_color;

	vector2_t<int16_t> m_drag_advance, m_resize_advance;
	bool m_dragging = false, m_resizing = false;
	bool m_drag_outside_bounds = false, m_resize_outside_bounds = false;
};

#endif