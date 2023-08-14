#include "gui.hpp"

c_gui::c_gui( const char* title, vector2_t<uint16_t> size ) {
	m_title = title;

	m_pos = vector2_t<uint16_t>(
		g_win32->get_size( ).x * 0.5f - size.x * 0.5f,
		g_win32->get_size( ).y * 0.5f - size.y * 0.5f
	);

	m_size = size;
}

void c_gui::start( ) {
	//g_buffer->filled_rectangle( m_pos, m_size, m_color.window, m_style.window_radius );
	//g_buffer->filled_rectangle( m_pos, vector2_t<uint16_t>(m_size.x, 25), m_color.window_header, m_style.window_radius );
	//g_buffer->rectangle( m_pos, m_size, m_color.window_frame, m_style.window_radius );
}

void c_gui::end( ) {

}