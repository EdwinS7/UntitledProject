#include "Logger.hpp"

int cConsole::InitConsole( const char* title ) {
	if ( !title )
		return 0;

	m_Open = true;
	AllocConsole( );
	SetConsoleTitleA( title );

	FILE* fp = nullptr;
	freopen_s( &fp, "CONIN$", "r", stdin );
	freopen_s( &fp, "CONOUT$", "w", stdout );
	freopen_s( &fp, "CONOUT$", "w", stderr );

	Print( 3, "Console", std::vformat( "Created Console ({})", std::make_format_args( title ) ).c_str( ) );

	return 1;
}

std::vector<const char*> LogLevel = {
	"INFO",      // Green
	"WARNING",   // Yellow
	"ERROR",     // Red
	"SUCCESS"    // Cyan
};

bool cConsole::Print( int level, const char* parent, const char* message ) {
	if ( m_Open ) {
		SetConsoleTextAttribute( GetStdHandle( STD_OUTPUT_HANDLE ), FOREGROUND_RED );
		std::printf( std::vformat( "[{}][{}] {}\n", std::make_format_args( LogLevel[ level ], parent, message ) ).c_str( ) );
		SetConsoleTextAttribute( GetStdHandle( STD_OUTPUT_HANDLE ), FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE ); // Reset to default color
		return true;
	}

	return false;
}

void cConsole::ExitConsole( ) {
	ShowWindow( GetConsoleWindow( ), SW_HIDE );
	FreeConsole( );
	m_Open = false;
}