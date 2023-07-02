 #include "../../../.hpp/buffer/fonts/font.hpp"

void c_font::create_font( font_t* font, const char* font_name, const uint16_t size, const uint16_t weight, const bool anti_aliased ) {
	FT_Library lib;
	FT_Face face;

	font->path = get_path( font_name );
	font->size = size;

	std::cout << "[ renderer ] received font file from ( " + font->path + " )" << std::endl;

	if ( FT_Init_FreeType( &lib ) != FT_Err_Ok )
		throw std::runtime_error( "create_objects failed ( FT_Init_FreeType )" );

	if ( FT_New_Face( lib, font->path.c_str(), 0, &face ) )
		throw std::runtime_error( "create_font failed ( FT_New_Face )" );

	FT_Set_Char_Size( face, size * 64, 0, GetDpiForWindow( g_win32->get_hwnd( ) ), 0 );
	FT_Select_Charmap( face, FT_ENCODING_UNICODE );

	for ( uchar_t i = 0; i < 128; i++ ) {
		if ( FT_Load_Char( face, i, anti_aliased ? FT_LOAD_RENDER : FT_LOAD_RENDER | FT_LOAD_TARGET_MONO ) )
			throw std::runtime_error( "create_font failed ( FT_Load_Char, font most likely does not exist! )" );

		uint32_t width = face->glyph->bitmap.width ? face->glyph->bitmap.width : 16;
		uint32_t height = face->glyph->bitmap.rows ? face->glyph->bitmap.rows : 16;

		if ( g_gfx->get_device()->CreateTexture( width, height, 1, D3DUSAGE_DYNAMIC, D3DFMT_A8, D3DPOOL_DEFAULT, &font->char_set[i].resource, NULL ) )
			throw std::runtime_error{ "create_font failed ( create_texture failed )" };

		D3DLOCKED_RECT locked_rect;
		font->char_set[ i ].resource->LockRect( 0, &locked_rect, nullptr, D3DLOCK_DISCARD );

		UCHAR* source = face->glyph->bitmap.buffer;
		UCHAR* destination = reinterpret_cast< UCHAR* >( locked_rect.pBits );

		if ( source && destination ) {
			switch ( face->glyph->bitmap.pixel_mode ) {
			case FT_PIXEL_MODE_MONO:
				for ( uint32_t y = 0; y < height; y++, source += face->glyph->bitmap.pitch, destination += locked_rect.Pitch ) {
					uint8_t bits = 0;
					const uint8_t* bits_ptr = source;

					for ( uint32_t x = 0; x < width; x++, bits <<= 1 ) {
						if ( ( x & 7 ) == 0 )
							bits = *bits_ptr++;

						destination[ x ] = ( bits & 0x80 ) ? 255 : 0;
					}
				}

				break;
			case FT_PIXEL_MODE_GRAY:
				for ( uint32_t i = 0; i < height; ++i ) {
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
		font->char_set[ i ].bearing = { static_cast< uint32_t >( face->glyph->bitmap_left ), static_cast< uint32_t >( face->glyph->bitmap_top ) };
		font->char_set[ i ].advance = face->glyph->advance.x;
	}

	FT_Done_Face( face );
	FT_Done_FreeType( lib );
}

void c_font::release_font( font_t* font ) {
	for ( int i = 0; i < font->char_set.size( ); i++ ) {
		font->char_set[ i ].resource->Release( );
		font->char_set[ i ].resource = nullptr;
	}
}