#pragma once

#include "../../../common.hpp"

class CWrapper {
public:
	void CreateContext( );

private:

};

inline const auto ImWrapper = std::make_unique<CWrapper>( );