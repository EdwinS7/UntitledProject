#pragma once

#include "../Gui.hpp"

extern class Group;

class Tab {
public:
	Tab( ) = default;

	Tab( const char* Title )
		: Title( Title ) {}

	void Destroy( );

	Group* NewGroup( const char* Title, Vec2<int16_t> Pos, Vec2<int16_t> Size );

	std::string Title;
private:
	std::vector<Group> m_Groups;
	int m_Padding;

};