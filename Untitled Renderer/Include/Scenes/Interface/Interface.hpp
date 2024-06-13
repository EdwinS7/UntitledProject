#pragma once
#ifndef INTERFACE_H
#define INTERFACE_H

#include <Common.hpp>
#include "../../Scripting/LuaWrapper.hpp"

class cInterface {
public:
	void Init( );
	void Draw( );

private:
	bool m_DebugConsole{ true };
};

inline const auto gInterface = std::make_unique<cInterface>( );

#include "Interface.inl"
#endif