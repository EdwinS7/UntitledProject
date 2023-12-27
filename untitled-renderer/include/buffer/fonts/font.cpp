#include "font.hpp"

void cFont::Create( Font* font, const char* font_name, const int16_t size, const int16_t weight, const int16_t padding, const bool anti_aliased ) {
	FT_Library lib;
	FT_Face face;

	font->path = GetPath( font_name );
	font->padding = padding;
	font->size = size;

	if ( FT_Init_FreeType( &lib ) != FT_Err_Ok )
		std::printf( std::vformat( "[ Buffer ] FT_Init_FreeType failed ( {} )\n", std::make_format_args( font_name ) ).c_str( ) );

	if ( FT_New_Face( lib, font->path.c_str( ), 0, &face ) )
		std::printf( std::vformat( "[ Buffer ] FT_New_Face failed ( {} )\n", std::make_format_args( font_name ) ).c_str( ) );

	FT_Set_Char_Size( face, size * 64, 0, GetDpiForWindow( gWin32->GetHwnd( ) ), 0 );
	FT_Select_Charmap( face, FT_ENCODING_UNICODE );

	for ( unsigned char i = 0; i < 128; i++ ) {
		if ( FT_Load_Char( face, i, anti_aliased ? FT_LOAD_RENDER : FT_LOAD_RENDER | FT_LOAD_TARGET_MONO ) )
			std::printf( std::vformat( "[ Buffer ] FT_Load_Char failed, font most likely does not exist! ( {} )\n", std::make_format_args( font_name ) ).c_str( ) );

		int32_t width = face->glyph->bitmap.width ? face->glyph->bitmap.width : 16;
		int32_t height = face->glyph->bitmap.rows ? face->glyph->bitmap.rows : 16;

		if ( gGraphics->GetDevice( )->CreateTexture( width, height, 1, D3DUSAGE_DYNAMIC, D3DFMT_A8, D3DPOOL_DEFAULT, &font->char_set[ i ].resource, NULL ) )
			std::printf( std::vformat( "[ Buffer ] CreateTexture failed ( {} )\n", std::make_format_args( font_name ) ).c_str( ) );

		D3DLOCKED_RECT locked_rect;
		font->char_set[ i ].resource->LockRect( 0, &locked_rect, nullptr, D3DLOCK_DISCARD );

		UCHAR* source = face->glyph->bitmap.buffer;
		UCHAR* destination = reinterpret_cast< UCHAR* >( locked_rect.pBits );

		if ( source && destination ) {
			switch ( face->glyph->bitmap.pixel_mode ) {
			case FT_PIXEL_MODE_MONO:
				for ( int32_t y = 0; y < height; y++, source += face->glyph->bitmap.pitch, destination += locked_rect.Pitch ) {
					int8_t bits = 0;
					const uint8_t* bits_ptr = source;

					for ( int32_t x = 0; x < width; x++, bits <<= 1 ) {
						if ( ( x & 7 ) == 0 )
							bits = *bits_ptr++;

						destination[ x ] = ( bits & 0x80 ) ? 255 : 0;
					}
				}

				break;
			case FT_PIXEL_MODE_GRAY:
				for ( int32_t i = 0; i < height; ++i ) {
					memcpy( destination, source, width );

					source += face->glyph->bitmap.pitch;
					destination += locked_rect.Pitch;
				}

				break;
			}
		}

		font->char_set[ i ].resource->UnlockRect( 0 );

		D3DSURFACE_DESC description;
		font->char_set[ i ].resource->GetLevelDesc( 0, &description );

		font->char_set[ i ].size = { width, height };
		font->char_set[ i ].bearing = { static_cast< int32_t >( face->glyph->bitmap_left ), static_cast< int32_t >( face->glyph->bitmap_top ) };
		font->char_set[ i ].advance = face->glyph->advance.x;
	}

	FT_Done_Face( face );
	FT_Done_FreeType( lib );

	std::printf( std::vformat( "[ Buffer ] Created font ( name: {}, size: {}, weight: {}, antialiasing: {} )\n", std::make_format_args( font_name, size, weight, anti_aliased ) ).c_str( ) );
}