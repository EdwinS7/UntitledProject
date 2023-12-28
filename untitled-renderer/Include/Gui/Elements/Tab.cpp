#include "Elements.hpp"

void Tab::Destroy( ) {
	delete this;
}

Group* Tab::NewGroup( const char* Title, Vec2<int16_t> Pos, Vec2<int16_t> Size ) {
	m_Groups.push_back( Group( Title, Pos, Size ) );
	return &m_Groups.back( );
}