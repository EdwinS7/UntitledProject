#include "Logger.hpp"

void cLogger::Log( LogLevel log_level, const std::string& message ) {
    std::string fmt_message;
    WORD Color = FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE; // Default color

    switch ( log_level ) {
    case LogLevel::Normal:
        fmt_message = "[+] ";
        Color = FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE; // White
        break;
    case LogLevel::Information:
        fmt_message = "[i] ";
        Color = FOREGROUND_BLUE | FOREGROUND_INTENSITY; // Light Blue
        break;
    case LogLevel::Warning:
        fmt_message = "[!] ";
        Color = FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_INTENSITY; // Orange-ish Yellow
        break;
    case LogLevel::Error:
        fmt_message = "[x] ";
        Color = FOREGROUND_RED; // Red
        break;
    case LogLevel::Success:
        fmt_message = "[*] ";
        Color = FOREGROUND_GREEN | FOREGROUND_INTENSITY; // Light Green-ish
        break;
    case LogLevel::Unknown:
        fmt_message = "[?] ";
        Color = FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE | FOREGROUND_INTENSITY; // Grey
        break;
    }

    fmt_message.append( message );
    m_Logs[ log_level ].emplace_back( fmt_message );

    SetConsoleTextAttribute( GetStdHandle( STD_OUTPUT_HANDLE ), Color );
    std::cout << fmt_message << "\n";
    SetConsoleTextAttribute( GetStdHandle( STD_OUTPUT_HANDLE ), FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE );
}

std::vector<std::string> cLogger::GetLogs( LogLevel log_level ) const {
	if ( log_level == LogLevel::END ) {
		std::vector<std::string> AllLogs;

		for ( const auto& [level, logs] : m_Logs ) {
			AllLogs.insert( AllLogs.end( ), logs.begin( ), logs.end( ) );
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