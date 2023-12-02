#pragma once

#include "../../Common.hpp"

class cUtilities {
public:
	int AllocateConsole( const std::string& Title );
};

inline const auto Utilities = std::make_unique<cUtilities>( );