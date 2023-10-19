#include "common.hpp"
#include <algorithm>

double func_linear( float x, float to, float speed_multiplier ) {
    return x + ( 1000.f * speed_multiplier * g_ctx->get_delta_time( ) );
}

int WINAPI WinMain( HINSTANCE instance, HINSTANCE prev_instance, PSTR cmd, int show_cmd ) {
    // Create Win32 window.
    g_win32->create_window( "untitled - renderer", { 1280, 720 } );

    // Initialize DirectX 9 pipeline.
    g_gfx->initialize( g_win32->get_hwnd( ) );

    // Create rendering resources.
    g_buffer->create_objects( );

    while ( g_win32->dispatch_messages( ) ) {
        g_ctx->update( );

        static float test = 0.f;

        if ( test > 359.f )
            test = 0;
        else
            test = func_linear( test, 360.f, 0.1f );

        g_buffer->set_rotation( test );
        g_buffer->filled_rectangle( { 100, 100 }, { 100, 100 }, { 15, 15, 15, 255 }, 10, corner_all );
        g_buffer->rectangle( { 100, 100 }, { 100, 100 }, { 255, 120, 255, 255 }, 10, corner_all );

        g_gfx->draw( );
    }

    g_gfx->release( );
}