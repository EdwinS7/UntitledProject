#include "console.hpp"

void c_console::draw( ) {
	g_buffer->filled_rectangle( vector2_t<int16_t>( 20, 20 ), vector2_t<int16_t>( 350, g_win32->get_size( ).y - 40 ), color_t( 25, 25, 25, 255 ), 8 );

	g_buffer->line( vector2_t<int16_t>( 20, 40 ), vector2_t<int16_t>( 370, 40 ), color_t( 0, 0, 0, 255 ) );
	g_buffer->rectangle( vector2_t<int16_t>( 20, 20 ), vector2_t<int16_t>( 350, g_win32->get_size( ).y - 40 ), color_t( 0, 0, 0, 255 ), 8 );
}

void c_console::handle( ) {
	for ( int i = 0; i < history.size( ); i++ ) {
		if ( g_ctx->get_real_time( ) >= std::get<2>( history[ i ] ) + 1.f )
			history.erase( history.begin( ) + i );
	}
}

void c_console::log( color_t clr, const char* msg ) {
#ifdef UNTITLED_USE_CONSOLE
	history.push_back( std::make_tuple( msg, clr, g_ctx->get_real_time( ) ) );
#endif
}