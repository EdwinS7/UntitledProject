#include "../../../.hpp/buffer/fonts/font.hpp"

void c_font::create_objects( ) {
	if ( FT_Init_FreeType( &m_lib ) != FT_Err_Ok )
		throw std::runtime_error( "create_font failed( FT_Init_FreeType )" );
}

bool c_font::create_font( const char* font_name, const uint16_t size, const uint16_t weight, const bool anti_aliased ) {
	if ( FT_New_Face( m_lib, get_path( font_name ), 0, &m_face ) != FT_Err_Ok )
		throw std::runtime_error( "create_font failed( FT_New_Face )" );

	FT_Set_Char_Size( m_face, size * 64, 0, GetDpiForWindow( g_win32->get_hwnd( ) ), 0 );
	FT_Select_Charmap( m_face, FT_ENCODING_UNICODE );



	return true;
}

void c_font::release( ) {

}