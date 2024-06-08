#include "Logger.hpp"

void cLogger::Print( LogLevel level, const std::string& message ) {
    std::string fmt_message;
    DWORD color = FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE;

    switch ( level ) {
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
    }

    HANDLE hStdOut = GetStdHandle( STD_OUTPUT_HANDLE );
    SetConsoleTextAttribute( hStdOut, color );
    
    fmt_message.append( message );
    std::cout << fmt_message << "\n";
}