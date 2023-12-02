#include "Console.hpp"

HWND ConsoleHwnd;
HMENU ConsoleHMenu;

int Console::Create( const std::string& Title ) {
    if ( !AllocConsole( ) )
        return 0;

    SetConsoleTitle( Title.c_str( ) );

    freopen_s( reinterpret_cast< FILE** >stdin, Xor( "CONIN$" ), Xor( "r" ), stdin );
    freopen_s( reinterpret_cast< FILE** >stdout, Xor( "CONOUT$" ), Xor( "w" ), stdout );
    freopen_s( reinterpret_cast< FILE** >stderr, Xor( "CONOUT$" ), Xor( "w" ), stderr );

    ConsoleHwnd = GetConsoleWindow( );
    ConsoleHMenu = GetSystemMenu( ConsoleHwnd, FALSE );

    EnableMenuItem( ConsoleHMenu, SC_CLOSE, MF_DISABLED );
    EnableMenuItem( ConsoleHMenu, SC_KEYMENU, MF_DISABLED );
    EnableMenuItem( ConsoleHMenu, SC_MOUSEMENU, MF_DISABLED );

    LOG( Xor( "[Console] Created logging environment!" ) );

	return 1;
}

int Console::Shutdown( ) {
    EnableMenuItem( ConsoleHMenu, SC_CLOSE, MF_ENABLED );

    return ( int )FreeConsole( );
}