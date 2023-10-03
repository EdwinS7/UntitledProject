#include "common.hpp"

int WINAPI WinMain( HINSTANCE instance, HINSTANCE prev_instance, PSTR cmd, int show_cmd ) {
    // Create Win32 window.
    g_win32->create_window( "untitled - renderer", { 1280, 720 } );

    // Initialize DirectX 9 pipeline.
    g_gfx->initialize( g_win32->get_hwnd( ) );

    // Create rendering resources.
    g_buffer->create_objects( );

    while ( g_win32->dispatch_messages( ) ) {
        g_ctx->update( );

        // bullshit experimental shit I wanna fuck with
        g_buffer->filled_gradient_circle( { 100, 100 }, 25, color_t( 255, 255, 255, 255 ), color_t( 0, 0, 0, 0 ) );

        g_gfx->draw( );
    }

    g_gfx->release( );
}