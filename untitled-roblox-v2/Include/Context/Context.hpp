#pragma once

#include "../Common.hpp"

class CContext {
public:
	void Init( );

private:
	
};

inline const auto Context = std::make_unique<CContext>( );