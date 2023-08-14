#include "inc.hpp"

int WINAPI WinMain( HINSTANCE instance, HINSTANCE prev_instance, PSTR cmd, int show_cmd ) {
    g_win32->create_window( "untitled - renderer", { 1280, 720 } );
    g_gfx->create( g_win32->get_hwnd( ) );

    g_buffer->create_objects( );

    while ( g_win32->dispatch_messages( ) ) {
        g_ctx->update( );

        auto framerate = std::vformat( "{} FPS", std::make_format_args( g_ctx->get_framerate( ) ) );

        g_buffer->text(
            &g_buffer->default_font,
            framerate.c_str( ),
            vector2_t<uint16_t>( 5, 5 ),
            color_t( 255, 255, 255, 255 )
        );

        g_gfx->draw( );
    }

    g_gfx->release( );
}