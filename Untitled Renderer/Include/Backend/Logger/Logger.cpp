#include "Logger.hpp"

void cLogger::Log( LogLevel log_level, const std::string& message ) {
    std::string fmt_message;

    static const std::unordered_map<LogLevel, DWORD> ColorMap = {
        {LogLevel::Normal, FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE},
        {LogLevel::Information, FOREGROUND_GREEN | FOREGROUND_BLUE},
        {LogLevel::Warning, FOREGROUND_RED | FOREGROUND_GREEN},
        {LogLevel::Error, FOREGROUND_RED},
        {LogLevel::Success, FOREGROUND_GREEN},
        {LogLevel::Unknown, FOREGROUND_INTENSITY}
    };

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

    HANDLE hStdOut = GetStdHandle( STD_OUTPUT_HANDLE );
    SetConsoleTextAttribute( hStdOut, ColorMap.at( log_level ) );
    
    fmt_message.append( message );
    std::cout << fmt_message << "\n";

    SetConsoleTextAttribute( hStdOut, FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE );

    m_Logs[ log_level ].emplace_back( ColorMap.at( log_level ), fmt_message );
}

std::vector<std::pair<DWORD, std::string>> cLogger::GetLogs( LogLevel level ) const {
    if ( level == LogLevel::END ) {
        std::vector<std::pair<DWORD, std::string>> all_logs;
        for ( const auto& log_pair : m_Logs ) {
            all_logs.insert( all_logs.end( ), log_pair.second.begin( ), log_pair.second.end( ) );
        }
        return all_logs;
    }
    auto it = m_Logs.find( level );
    return it != m_Logs.end( ) ? it->second : std::vector<std::pair<DWORD, std::string>>{};
}

void cLogger::ClearLogs( LogLevel level ) {
    if ( level == LogLevel::END ) {
        m_Logs.clear( );
    }
    else {
        m_Logs.erase( level );
    }
}