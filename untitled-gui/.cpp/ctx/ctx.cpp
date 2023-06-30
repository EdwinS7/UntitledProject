#include "../../.hpp/ctx/ctx.hpp"

int WINAPI WinMain( HINSTANCE instance, HINSTANCE prev_instance, PSTR cmd, int show_cmd ) {
    g_win32->create_window( "untitled - renderer", false, { 1280, 720 } );

    g_gfx->create_context( );

    g_gfx->create_device( g_win32->get_hwnd( ) );

    g_buffer->create_objects( );

    while ( g_win32->dispatch_messages( ) ) {
        g_ctx->update( );

        g_win32->set_window_title( std::vformat( "untitled - renderer ({} FPS)", std::make_format_args( g_ctx->get_framerate( ) ) ).c_str( ) );

        g_buffer->filled_rectangle( { 60, 60 }, { 50, 50 }, color_t( 0, 0, 0, 255 ), 10, corner_all );
        g_buffer->rectangle( { 60, 60 }, { 50, 50 }, color_t( 255, 255, 255, 255 ), 10, corner_all );

        g_gfx->draw( );
    }

    g_gfx->release( );
}