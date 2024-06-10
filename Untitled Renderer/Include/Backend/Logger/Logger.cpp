#include "Logger.hpp"

void cLogger::Log( LogLevel log_level, const std::string& message ) {
    std::string fmt_message;

    switch ( log_level ) {
    case LogLevel::Normal:
        fmt_message = "[+] ";
        break;
    case LogLevel::Information:
        fmt_message = "[i] ";
        break;
    case LogLevel::Warning:
        fmt_message = "[!] ";
        break;
    case LogLevel::Error:
        fmt_message = "[x] ";
        break;
    case LogLevel::Success:
        fmt_message = "[*] ";
        break;
    case LogLevel::Unknown:
        fmt_message = "[?] ";
        break;
    }

    fmt_message.append( message );
    std::cout << fmt_message << "\n";

    m_Logs[ log_level ].emplace_back( fmt_message );
}

std::vector<std::string> cLogger::GetLogs( LogLevel log_level ) const {
    if ( log_level == LogLevel::END ) {
        std::vector<std::string> AllLogs;

        for ( int i = static_cast< int >( LogLevel::Normal ); i < static_cast< int >( LogLevel::Unknown ); ++i ) {
            LogLevel currentLevel = static_cast< LogLevel >( i );
            auto it = m_Logs.find( currentLevel );
            if ( it != m_Logs.end( ) ) {
                AllLogs.insert( AllLogs.end( ), it->second.begin( ), it->second.end( ) );
            }
        }

        return AllLogs;
    }
    else {
        return m_Logs[ log_level ];
    }
}

void cLogger::ClearLogs( LogLevel log_level ) {
    if ( log_level == LogLevel::END ) {
        m_Logs.clear( );
    }
    else {
        m_Logs.erase( log_level );
    }
}