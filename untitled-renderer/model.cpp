#include "inc.hpp"

int WINAPI WinMain( HINSTANCE instance, HINSTANCE prev_instance, PSTR cmd, int show_cmd ) {
    // Create Win32 window.
    g_win32->create_window( "untitled - renderer", { 1280, 720 } );

    // Initialize DirectX 9 pipeline.
    g_gfx->initialize( g_win32->get_hwnd( ) );

    // Create rendering resources.
    g_buffer->create_objects( );

    g_gfx->set_clear_color( color_t( 115, 140, 153, 255) );

    while ( g_win32->dispatch_messages( ) ) {
        g_ctx->update( );

#ifdef UNTITLED_SHOW_FPS
        auto framerate = std::vformat( "{} FPS", std::make_format_args( g_ctx->get_framerate( ) ) );

        g_buffer->text(
            &g_buffer->default_font, framerate.c_str( ), vector2_t<int16_t>( 5, 5 ), color_t( 160, 217, 255, 255 )
        );
#endif

        // gui demo
        static auto gui = std::make_unique<c_gui>( "untitled", vector2_t<int16_t>( 700, 670 ), window_flags::none );

        gui->start( );
        {

        }
        gui->end( );

        g_gfx->draw( );
    }

    g_gfx->release( );
}