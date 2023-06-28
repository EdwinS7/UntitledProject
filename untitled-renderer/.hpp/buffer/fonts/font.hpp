#pragma once

#include "../buffer.hpp"

class c_font {
public:
	void create_objects( );

	bool create_font( const char* font_name, const uint16_t size, const uint16_t weight, const bool anti_aliased );

	void release( );

private:
	FT_Library m_lib;
	FT_Face	m_face;

	ALWAYS_INLINE const char* get_path( const char* font_name );
};

inline const auto g_font = std::make_unique<c_font>( );

#include "font.inl"