#include "Elements.hpp"

void Tab::Destroy( ) {
	delete this;
}

int GroupID = 0;
Group* Tab::NewGroup( const char* Title, Window** Window ) {
    m_Groups.push_back( Group( Title, *Window ) );
    GroupID++;

    return &m_Groups[ GroupID - 1 ];
}