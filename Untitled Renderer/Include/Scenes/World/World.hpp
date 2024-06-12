#pragma once
#ifndef WORLD_H
#define WORLD_H

#include <Common.hpp>
#include "../../Backend/LuaWrapper/LuaWrapper.hpp"

class cWorld {
public:
	void Init( );
	void Draw( );

private:

};

inline const auto gWorld = std::make_unique<cWorld>( );

#include "World.inl"
#endif