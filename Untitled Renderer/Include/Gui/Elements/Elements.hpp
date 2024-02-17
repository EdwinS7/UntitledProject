#pragma once

#include "../Gui.hpp"

extern class Group;
extern class Window;

class Tab {
public:
	Tab( ) = default;

	Tab( const char* Title )
		: Title( Title ) {}

	void Destroy( );

	Group* NewGroup( const char* Title, Window** Window );

	std::string Title;

private:
	std::vector<Group> m_Groups;
	
};