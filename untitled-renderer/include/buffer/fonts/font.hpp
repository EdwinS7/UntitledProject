#pragma once

#include "../buffer.hpp"

class c_font {
public:
	void create_font( font_t* font, const char* font_name, const int16_t size, const int16_t weight, const int16_t padding, const bool anti_aliased );

	void release_font( font_t* font );

private:
	ALWAYS_INLINE std::string get_path( const char* font_name );

};

inline const auto g_font = std::make_unique<c_font>( );

#include "font.inl"