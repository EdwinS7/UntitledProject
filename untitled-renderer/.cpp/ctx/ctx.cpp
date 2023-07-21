#include "../../.hpp/ctx/ctx.hpp"

int WINAPI WinMain( HINSTANCE instance, HINSTANCE prev_instance, PSTR cmd, int show_cmd ) {
    g_win32->create_window( "untitled - renderer", { 1280, 720 } );

    g_gfx->create_context( );

    g_gfx->create_device( g_win32->get_hwnd( ) );

    g_buffer->create_objects( );

    while ( g_win32->dispatch_messages( ) ) {
        g_ctx->update( );

        auto framerate = std::vformat( "{} FPS", std::make_format_args( g_ctx->get_framerate( ) ) );

        g_buffer->text( 
            &g_buffer->default_font,
           framerate.c_str(),
            vector2_t<uint16_t>(5, 5),
            color_t( 255, 255, 255, 255 )
        );

        static bool test = false;

        if ( g_input->key_pressed( VK_LBUTTON ) ) {
            test = !test;
        }

        g_buffer->filled_circle( g_input->get_mouse_pos( ), 6, g_input->key_held( VK_LBUTTON ) ? color_t( 255, 0, 0, 255 ) : color_t( 255, 255, 255, 255 ) );

        g_buffer->filled_rectangle( { 100, 100 }, { 50, 50 }, test ? color_t( 255, 0, 0, 255 ) : color_t( 255, 255, 255, 255 ), 10, corner_all );

        g_gfx->draw( );
    }

    g_gfx->release( );
}

void c_ctx::update( ) {
    g_input->pool_input( );

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