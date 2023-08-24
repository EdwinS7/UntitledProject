#pragma once

#include "../../inc.hpp"

class c_util {
public:
	std::string bytes_to_ida_pattern( byte* bytes, size_t size );
	uintptr_t pattern_scan( uintptr_t mod, const char* sig, size_t size );
	std::vector<uintptr_t> get_xrefs_to( uintptr_t address, uintptr_t start, uintptr_t size );

	std::uintptr_t scan( const std::string& image_name, const std::string& signature );
	std::uintptr_t scan( const std::uintptr_t image, const std::string& signature );

private:
	
};

inline const auto g_util = std::make_unique<c_util>( );