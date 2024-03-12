#include "buffer.hpp"

inline void cBuffer::ClearCommands( ) {
    m_DrawCommands.clear( );
    m_DrawCommand.reset( );

    m_VerticesCount = 0;
    m_IndicesCount = 0;
}

inline CommandResources cBuffer::GetCommand( ) {
    return m_CommandResources;
}

inline CompiledDrawCommand cBuffer::GetDrawCommand( ) {
    return m_DrawCommand;
}

inline std::vector<DrawCommand> cBuffer::GetDrawCommands( ) {
    return m_DrawCommands;
}

inline int cBuffer::GetCommandsCount( ) {
    return m_DrawCommands.size();
}

inline int cBuffer::GetVerticesCount( ) {
    return m_VerticesCount;
}

inline int cBuffer::GetIndicesCount( ) {
    return m_IndicesCount;
}

inline void cBuffer::PushClip( Rect<int16_t> Rect ) {
	m_CommandResources.Clips.push_back( RECT( Rect.x, Rect.y, Rect.w, Rect.h ) );
}

inline void cBuffer::PopClip( ) {
	m_CommandResources.Clips.pop_back( );
}

inline void cBuffer::PushTexture( IDirect3DTexture9* Texture ) {
	m_CommandResources.Textures.push_back( Texture );
}

inline void cBuffer::PopTexture( ) {
	m_CommandResources.Textures.pop_back( );
}