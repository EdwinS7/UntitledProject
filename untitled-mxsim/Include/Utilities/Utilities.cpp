#include "Utilities.hpp"

int cUtilities::AllocateConsole( const std::string& Title ) {
    if ( !AllocConsole( ) )
        return 0;

    SetConsoleTitle( std::vformat( Xor( "{} ( {} )" ), std::make_format_args( GAME_NAME, GAME_VERSION ) ).c_str( ) );

    FILE* file_pointer = nullptr;
    freopen_s( &file_pointer, Xor( "CONIN$" ), Xor( "r" ), stdin );
    freopen_s( &file_pointer, Xor( "CONOUT$" ), Xor( "w" ), stdout );
    freopen_s( &file_pointer, Xor( "CONOUT$" ), Xor( "w" ), stderr );

    return 1;
}