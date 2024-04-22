#pragma once

#include "../../Common.hpp"


class cInterface {
public:
	void Draw( );

private:
	
};

inline const auto gInterface = std::make_unique<cInterface>( );

#include "Interface.inl"