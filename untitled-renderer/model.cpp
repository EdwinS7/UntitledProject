#include "inc.hpp"

int WINAPI WinMain( HINSTANCE instance, HINSTANCE prev_instance, PSTR cmd, int show_cmd ) {
    // create win32 window.
    g_win32->create_window( "untitled - renderer", { 1280, 720 } );

    // initialize dx9 pipeline.
    g_gfx->initialize( g_win32->get_hwnd( ) );

    // create render resources.
    g_buffer->create_objects( );

    g_gfx->set_clear_color( color_t(25, 25, 25, 255) );
    g_gfx->set_vsync( false );

    while ( g_win32->dispatch_messages( ) ) {
        g_ctx->update( );

        g_buffer->filled_gradient( vector2_t<uint16_t>( 0, 0 ), g_win32->get_size( ), color_t( 8, 26, 59, 255 ), color_t( 0, 10, 37, 255 ), false );

#ifdef UNTITLED_USE_CONSOLE
        g_console->draw( );
#endif

#ifdef UNTITLED_SHOW_FPS
        auto framerate = std::vformat( "{} FPS", std::make_format_args( g_ctx->get_framerate( ) ) );

        g_buffer->text(
            &g_buffer->default_font, framerate.c_str( ), vector2_t<uint16_t>( 5, 5 ), color_t( 160, 217, 255, 255 )
        );
#endif

        g_gfx->draw( );
    }

    g_gfx->release( );
}