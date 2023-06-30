#include "../../.hpp/ctx/ctx.hpp"

int WINAPI WinMain( HINSTANCE instance, HINSTANCE prev_instance, PSTR cmd, int show_cmd ) {
    g_win32->create_window( "untitled - renderer", false, { 1280, 720 } );

    g_gfx->create_context( );

    g_gfx->create_device( g_win32->get_hwnd( ) );

    g_buffer->create_objects( );

    while ( g_win32->dispatch_messages( ) ) {
        g_ctx->update( );

        g_win32->set_window_title( std::vformat( "untitled - renderer ({} FPS)", std::make_format_args( g_ctx->get_framerate( ) ) ).c_str( ) );

        g_buffer->text( &g_buffer->default_font, "erin is a fucking nigger pedo", { 10, 10 }, color_t( 255, 255, 255, 255 ) );

        g_gfx->draw( );  
    }

    g_gfx->release( );
}

void c_ctx::update( ) {
    m_timepoint = std::chrono::high_resolution_clock::now( );

    m_real_time = static_cast< float >(
        std::chrono::duration_cast< std::chrono::duration<double> >(
            m_timepoint.time_since_epoch( )
        ).count( )
    );

    if ( m_real_time > when_to_update ) {
        when_to_update = m_real_time + 1.f;
        m_fps = m_frame_count;
        m_frame_count = 0;
    }
    else
        m_frame_count++;
}