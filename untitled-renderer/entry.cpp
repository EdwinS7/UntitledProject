#include "common.hpp"

void draw_buffer( ) {
    g_buffer->textured_rectangle( &g_buffer->default_texture, vector2_t<int16_t>( 5, 5 ), vector2_t<int16_t>( 288, 288 ), color_t( 255, 100, 255, 100 ) );
}

int WINAPI WinMain( HINSTANCE instance, HINSTANCE prev_instance, PSTR cmd, int show_cmd ) {
    // Create Win32 window.
    g_win32->create_window( "untitled - renderer", { 1280, 720 } );

    // Initialize DirectX 9 pipeline.
    g_gfx->initialize( g_win32->get_hwnd( ) );

    // Create rendering resources.
    g_buffer->create_objects( );

    g_gfx->set_clear_color( color_t( 0, 0, 0, 255 ) );

    while ( g_win32->dispatch_messages( ) ) {
        g_ctx->update( );
        draw_buffer( );

#ifdef UNTITLED_SHOW_STATS
        auto display_info = std::vformat(
            "{} FPS\n{} COMMANDS\n{} VERTICES\n{} INDICES",

            std::make_format_args(
                g_ctx->get_framerate( ),
                g_buffer->get_num_of_commands( ),
                g_buffer->get_num_of_vertices( ),
                g_buffer->get_num_of_indices( )
            )
        );

        g_buffer->text(
            &g_buffer->default_font, display_info.c_str( ), vector2_t<int16_t>( 5, 5 ), color_t( 160, 217, 255, 255 )
        );
#endif

        g_gfx->draw( );
    }

    g_gfx->release( );
}