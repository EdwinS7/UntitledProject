#include "Logger.hpp"

void cLogger::Log( LogLevel log_level, const std::string& message ) {
    std::string fmt_message;
    DWORD color = FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE;

    switch ( log_level ) {
    case LogLevel::Normal:
        fmt_message.append( "[+] " );
        break;
    case LogLevel::Information:
        color = FOREGROUND_GREEN | FOREGROUND_BLUE;
        fmt_message.append( "[?] " );
        break;
    case LogLevel::Warning:
        color = FOREGROUND_RED | FOREGROUND_GREEN;
        fmt_message.append( "[!] " );
        break;
    case LogLevel::Error:
        color = FOREGROUND_RED;
        fmt_message.append( "[x] " );
        break;
    case LogLevel::Success:
        color = FOREGROUND_GREEN;
        fmt_message.append( "[<3] " );
        break;
    }

    HANDLE hStdOut = GetStdHandle( STD_OUTPUT_HANDLE );
    SetConsoleTextAttribute( hStdOut, color );
    
    fmt_message.append( message );
    std::cout << fmt_message << "\n";
    m_Logs[ log_level ].push_back( fmt_message );
}

void cLogger::ClearLogs( LogLevel log_level ) {
    if ( log_level == LogLevel::END ) {
        m_Logs.clear( );
        return;
    }

    m_Logs[ log_level ].clear( );
}

std::vector<std::string> cLogger::GetLogs( LogLevel log_level ) const {
    auto it = m_Logs.find( log_level );

    if ( it != m_Logs.end( ) )
        return it->second;
    else
        return {};
}
