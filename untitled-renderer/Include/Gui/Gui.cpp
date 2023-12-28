#include "Gui.hpp"

Window* cGui::NewWindow( const char* Title, const char* Description, Vec2<int16_t> m_Size ) {
	auto m_Pos = Vec2<int16_t>( gWin32->GetSize( ).x / 2 - m_Size.x / 2, gWin32->GetSize( ).y / 2 - m_Size.y / 2 );
	m_Windows.push_back( Window( Title, Description, m_Pos, m_Size, Vec2<int16_t>( m_Size.x / 2, m_Size.y / 2 ), Vec2<int16_t>( m_Size.x * 1.5, m_Size.y * 1.5 ) ) );
	return &m_Windows.back( );
}