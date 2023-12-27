#include "buffer.hpp"

inline void cBuffer::ClearCommands( ) {
    m_draw_commands.clear( );
    m_draw_command.reset( );

    m_vertices_count = 0;
    m_indices_count = 0;
}

inline Command cBuffer::GetCommand( ) {
    return m_command;
}

inline CompiledDrawCommand cBuffer::GetDrawCommand( ) {
    return m_draw_command;
}

inline std::vector<DrawCommand> cBuffer::GetDrawCommands( ) {
    return m_draw_commands;
}

inline int cBuffer::GetCommandsCount( ) {
    return m_draw_commands.size();
}

inline int cBuffer::GetVerticesCount( ) {
    return m_vertices_count;
}

inline int cBuffer::GetIndicesCount( ) {
    return m_indices_count;
}

inline void cBuffer::PushClip( RECT rect ) {
    m_command.clips.push_back( rect );
}

inline void cBuffer::PopClip( ) {
    m_command.clips.pop_back( );
}

inline void cBuffer::PushTexture( IDirect3DTexture9* resource ) {
    m_command.textures.push_back( resource );
}

inline void cBuffer::PopTexture( ) {
    m_command.textures.pop_back( );
}