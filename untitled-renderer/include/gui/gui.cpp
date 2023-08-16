#include "gui.hpp"

c_gui::c_gui( const char* title, vector2_t<int16_t> size, window_flags flags ) {
	m_title = title;

	m_pos = vector2_t<int16_t>(
		g_win32->get_size( ).x * 0.5f - size.x * 0.5f,
		g_win32->get_size( ).y * 0.5f - size.y * 0.5f
	);

	m_size = size;
	m_flags = flags;
}

void c_gui::start( ) {
	if ( !m_flags & no_background )
		g_buffer->filled_rectangle( m_pos, m_size, m_color.window, m_style.window_radius );

	if ( !m_flags & no_header ) {
		g_buffer->filled_rectangle( m_pos, vector2_t<int16_t>( m_size.x, 20 ), m_color.window_header, m_style.window_radius, corner_top );
		g_buffer->line( m_pos + vector2_t<int16_t>( 1, 20 ), m_pos + vector2_t<int16_t>( m_size.x - 2, 20 ), m_color.accent );

		auto text_size = g_buffer->get_text_size( &g_buffer->interface_font, m_title );
		g_buffer->text( &g_buffer->interface_font, m_title, m_pos + vector2_t<int16_t>( m_size.x * 0.5 - text_size.x * 0.5, 5 ), m_color.text_active );
	}

	if ( !m_flags & no_border )
		g_buffer->rectangle( m_pos, m_size, m_color.window_frame, m_style.window_radius );
}

void c_gui::end( ) {
	bool held = g_input->key_held( KEY_LMOUSE );
	bool inside_bounds = g_input->mouse_hovering_rect( m_pos, vector2_t<int16_t>( m_size.x, 20 ) );

	if ( held && !inside_bounds && !m_dragging && !m_drag_outside_bounds )
		m_drag_outside_bounds = true;
	else if ( m_drag_outside_bounds && !held )
		m_drag_outside_bounds = false;

	if ( !m_dragging && held && inside_bounds && !m_drag_outside_bounds ) {
		m_drag_advance = g_input->get_mouse_pos( ) - m_pos;
		m_dragging = true;
	}
	else if ( m_dragging && !held )
		m_dragging = false;

	if ( m_dragging )
		m_pos = g_input->get_mouse_pos( ) - m_drag_advance;

	inside_bounds = g_input->mouse_hovering_rect( m_pos + m_size - vector2_t<int16_t>( 5, 5 ), vector2_t<int16_t>( 5, 5 ) );

	if ( held && !inside_bounds && !m_resizing && !m_resize_outside_bounds )
		m_resize_outside_bounds = true;
	else if ( m_drag_outside_bounds && !held )
		m_resize_outside_bounds = false;

	if ( !m_resizing && held && inside_bounds && !m_resize_outside_bounds ) {
		m_resize_advance = g_input->get_mouse_pos( ) - ( m_pos + m_size );
		m_resizing = true;
	}
	else if ( m_resizing && !held )
		m_resizing = false;

	if ( m_resizing )
		m_size = g_input->get_mouse_pos( ) - ( m_pos + m_size ) - m_resize_advance;
}