#pragma once

#include "../buffer.hpp"
#include FT_FREETYPE_H
#include FT_BITMAP_H

class cFont {
public:
	void Create( Font* font, const char* font_name, const int16_t size, const int16_t weight, const int16_t padding, const bool anti_aliased );

private:
	inline std::string GetPath( const char* font_name );

};

inline const auto gFont = std::make_unique<cFont>( );

#include "font.inl"