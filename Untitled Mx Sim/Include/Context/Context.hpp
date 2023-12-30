#pragma once

#include "../../common.hpp"

class cContext {
public:
	void Init( );

protected:
	
};

inline const auto Context = std::make_unique<cContext>( );