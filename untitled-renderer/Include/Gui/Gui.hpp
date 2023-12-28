#pragma once

// includes
#include "../../Common.hpp"

#include "Containers/Containers.hpp"
#include "Elements/Elements.hpp"

extern class Window;

class cGui {
public:
	Window* NewWindow( const char* Title, const char* Description, Vec2<int16_t> Size );

private:
	std::vector<Window> m_Windows;
};

inline const auto gGui = std::make_unique<cGui>( );

#include "Gui.inl"