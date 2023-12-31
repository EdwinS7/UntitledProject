#include "context.hpp"

void cContext::Update( ) {
    if ( GetFocus() == gWin32->GetHwnd( ) )
        gInput->PoolInput( );

    m_TimePoint = std::chrono::high_resolution_clock::now( );

    m_RealTime = static_cast< float >( std::chrono::duration_cast< std::chrono::duration<double> >( m_TimePoint.time_since_epoch( ) ).count( ) );
    m_DeltaTime = static_cast< float >( std::chrono::duration_cast< std::chrono::duration<double> >( m_TimePoint - m_FrameTime ).count( ) );
    m_FrameTime = m_TimePoint;

    if ( m_RealTime > m_WhenToUpdate ) {
        m_WhenToUpdate = m_RealTime + 1.f;
        m_Fps = m_FrameCount;
        m_FrameCount = 0;
    }
    else
        m_FrameCount++;
}