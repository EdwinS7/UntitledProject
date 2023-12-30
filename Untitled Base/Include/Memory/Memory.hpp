#pragma once

#include "../../Common.hpp"

namespace Memory {
	std::uintptr_t Scan( const std::string& Signature );
	std::uintptr_t Rebase( std::uintptr_t Address );
};