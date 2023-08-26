#pragma once

#include "../../inc.hpp"

class c_mem {
public:
	std::uintptr_t scan( const std::string& signature );

	std::string bytes_to_ida_pattern( byte* bytes, size_t size );

	std::vector<uintptr_t> get_xrefs_to( const std::string& signature, uintptr_t start, uintptr_t size );
	std::vector<uintptr_t> get_xrefs_to( uintptr_t address, uintptr_t start, uintptr_t size );

};

inline const auto g_mem = std::make_unique<c_mem>( );