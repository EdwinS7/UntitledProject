#pragma once

#include "../../Common.hpp"


class cInterface {
public:
	virtual void Init( );
	virtual void Draw( );

private:
	
};

inline const auto gInterface = std::make_unique<cInterface>( );

#include "Interface.inl"