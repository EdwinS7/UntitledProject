#pragma once

#include "../../inc.hpp"

class c_mem {
public:

	std::uintptr_t scan( const std::string& image_name, const std::string& signature );
	std::uintptr_t scan( const std::uintptr_t image, const std::string& signature );

};

inline const auto g_mem = std::make_unique<c_mem>( );