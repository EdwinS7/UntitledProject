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

    m_Logs[ log_level ].push_back( fmt_message );
}

std::vector<std::string> cLogger::GetLogs( LogLevel log_level ) const {
    auto it = m_Logs.find( log_level );

    if ( it != m_Logs.end( ) )
        return it->second;
    else
        return {};
}

void cLogger::ClearLogs( LogLevel log_level ) {
    if ( log_level == LogLevel::END ) {
        m_Logs.clear( );
        return;
    }

    m_Logs[ log_level ].clear( );
}