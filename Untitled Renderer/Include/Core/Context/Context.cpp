#include "Context.hpp"

void cContext::Update( ) {
    if ( gInput->IsActive( ) ) {
        gInput->PoolInput( );

        gLuaAPI->RunConnection( "OnInputUpdate" );
    }

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

// Lmfao idek
std::string cContext::GetHwid( ) const {
    std::string Hwid = "Unknown";

    DWORD VolumeSerialNumber = 0;
    if ( GetVolumeInformationA( "C:\\", NULL, 0, &VolumeSerialNumber, NULL, NULL, NULL, 0 ) ) {
        int CPUInfo[ 4 ] = { 0 };
        __cpuid( CPUInfo, 0 );
        DWORD CPUIdPart1 = CPUInfo[ 0 ] ^ CPUInfo[ 1 ];
        DWORD CPUIdPart2 = CPUInfo[ 2 ] ^ CPUInfo[ 3 ];
        Hwid = std::to_string( VolumeSerialNumber ) + ":" + std::to_string( CPUIdPart1 ) + ":" + std::to_string( CPUIdPart2 );
    }

    return Hwid;
}

std::string cContext::GetIPAddress( ) const {
    std::string IpAddress = "Unknown";

    WSADATA wsaData;
    if ( WSAStartup( MAKEWORD( 2, 2 ), &wsaData ) != 0 ) {
        return IpAddress;
    }

    char HostBuffer[ 256 ];
    if ( gethostname( HostBuffer, sizeof( HostBuffer ) ) == SOCKET_ERROR ) {
        WSACleanup( );
        return IpAddress;
    }

    addrinfo Hints = { 0 }, *Info = nullptr;
    Hints.ai_family = AF_INET;
    Hints.ai_socktype = SOCK_STREAM;
    Hints.ai_protocol = IPPROTO_TCP;

    if ( getaddrinfo( HostBuffer, nullptr, &Hints, &Info ) != 0 ) {
        WSACleanup( );
        return IpAddress;
    }

    sockaddr_in* ipv4 = ( sockaddr_in* ) Info->ai_addr;
    char IPbuffer[ INET_ADDRSTRLEN ];
    inet_ntop( AF_INET, &( ipv4->sin_addr ), IPbuffer, INET_ADDRSTRLEN );

    IpAddress = std::string( IPbuffer );

    freeaddrinfo( Info );
    WSACleanup( );

    return IpAddress;
}