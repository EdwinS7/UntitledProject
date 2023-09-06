#include "utilities.hpp"

void c_utilities::open_console( const std::string& title ) {
    if ( !AllocConsole( ) )
        return;

    FILE* file{};
    if ( freopen_s( &file, xorstr_( "CONOUT$" ), xorstr_( "w" ), stdout ) )
        return;

    SetConsoleTitle( title.c_str( ) );
}