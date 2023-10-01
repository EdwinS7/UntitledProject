#include "context.hpp"

void c_ctx::update( ) {
    g_input->pool_input( );

    m_timepoint = std::chrono::high_resolution_clock::now( );

    m_real_time = static_cast< float >(
        std::chrono::duration_cast< std::chrono::duration<double> >(
            m_timepoint.time_since_epoch( )
        ).count( )
    );

    if ( m_real_time > m_when_to_update ) {
        m_when_to_update = m_real_time + 1.f;
        m_fps = m_frame_count;
        m_frame_count = 0;
    }
    else
        m_frame_count++;
}