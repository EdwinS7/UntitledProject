#include "buffer.hpp"

ALWAYS_INLINE void c_buffer::clear_commands( ) {
    m_draw_commands.clear( );
    m_draw_command.reset( );

    m_indices_count = 0;
    m_vertices_count = 0;
}

ALWAYS_INLINE command_t c_buffer::get_command( ) {
    return m_command;
}

ALWAYS_INLINE compiled_draw_command_t c_buffer::get_draw_command( ) {
    return m_draw_command;
}

ALWAYS_INLINE std::vector<draw_command_t> c_buffer::get_draw_commands( ) {
    return m_draw_commands;
}

ALWAYS_INLINE int c_buffer::get_num_of_commands( ) {
    return m_draw_commands.size();
}

ALWAYS_INLINE int c_buffer::get_num_of_vertices( ) {
    return m_vertices_count;
}

ALWAYS_INLINE int c_buffer::get_num_of_indices( ) {
    return m_indices_count;
}

ALWAYS_INLINE void c_buffer::push_clip( RECT rect ) {
    m_command.clips.push_back( rect );
}

ALWAYS_INLINE void c_buffer::pop_clip( ) {
    m_command.clips.pop_back( );
}

ALWAYS_INLINE void c_buffer::push_texture( texture resource ) {
    m_command.textures.push_back( resource );
}

ALWAYS_INLINE void c_buffer::pop_texture( ) {
    m_command.textures.pop_back( );
}