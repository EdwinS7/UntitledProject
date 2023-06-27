#include "buffer.hpp"

ALWAYS_INLINE void c_buffer::clear_commands( ) {
    m_draw_commands.clear( );
    m_draw_command.reset( );
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

ALWAYS_INLINE void c_buffer::push_command( const command_t command ) {
    m_command += command;
}