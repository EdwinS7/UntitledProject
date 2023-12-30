#include "Elements.hpp"

void Tab::Destroy( ) {
	delete this;
}

Group* Tab::NewGroup( const char* Title, Window* Window, Vec2<int16_t> Padding ) {
	//m_Groups.push_back( Group( Title, Window, Padding ) );
	//return &m_Groups.back( );
	return nullptr;
}