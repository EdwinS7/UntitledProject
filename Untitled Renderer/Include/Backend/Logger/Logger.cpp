#include "Logger.hpp"

void cLogger::Print( LogLevel level, const std::string& message ) {
	std::string fmt_message = "";

	switch ( level ) {
	case LogLevel::Normal:
		fmt_message.append( "[+] " );
		break;
	case LogLevel::Information:
		fmt_message.append( "[?] " );
		break;
	case LogLevel::Warning:
		fmt_message.append( "[*] " );
		break;
	case LogLevel::Error:
		fmt_message.append( "[!] " );
		break;
	}

	fmt_message.append( message );
	std::cout << fmt_message << "\n";
}