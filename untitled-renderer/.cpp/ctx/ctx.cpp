#include "../../.hpp/ctx/ctx.hpp"

int WINAPI WinMain( HINSTANCE instance, HINSTANCE prev_instance, PSTR cmd, int show_cmd ) {
    g_win32->create_window( "untitled - renderer", false, { 1280, 720 } );

    g_gfx->create_context( );

    g_gfx->create_device( g_win32->get_hwnd( ) );

    g_buffer->create_objects( );

    while ( g_win32->dispatch_messages( ) ) {
        g_gfx->begin_scene( );
        g_ctx->update( );

        g_win32->set_window_title( std::vformat( "untitled - renderer ({} FPS)", std::make_format_args( g_ctx->get_framerate( ) ) ).c_str( ) );

        g_buffer->circle( { 60, 60 }, 50, color_t( 255, 255, 255, 255 ) );

        g_gfx->render_draw_data( );
        g_gfx->end_scene( );
    }

    g_gfx->release( );
}

// __DEMO__
void c_ctx::update( ) {
    static int fps = 60;

    std::chrono::steady_clock::time_point m_high_resolution_clock = std::chrono::high_resolution_clock::now( );

    float m_real_time = static_cast< float >(
        std::chrono::duration_cast< std::chrono::duration<double> >(
            m_high_resolution_clock.time_since_epoch( )
        ).count( )
    );

    static float m_update = m_real_time + 1.f;

    if ( m_real_time > m_update ) {
        m_update = m_real_time + 1.f;
        m_fps = fps;
        fps = 0;
    }
    else
        fps++;
}

int c_ctx::get_framerate( ) {
    return m_fps;
}