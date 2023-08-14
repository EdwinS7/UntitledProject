#include "console.hpp"

#ifdef UNTITLED_USE_CONSOLE

void c_console::create( const char* title, vector2_t<uint16_t> size ) {
    AllocConsole( );
    
    freopen_s( &file_pointer, "CONIN$", "r", stdin );
    freopen_s( &file_pointer, "CONOUT$", "w", stdout );
    freopen_s( &file_pointer, "CONOUT$", "w", stderr );

    log( LOG_INFO, "[ win32 ] console created\n" );
}

void c_console::log( int level, const char* msg ) {
    static const int colorMap[] = {
        FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE,
        FOREGROUND_BLUE | FOREGROUND_GREEN,
        FOREGROUND_RED | FOREGROUND_GREEN,
        FOREGROUND_RED,
        FOREGROUND_GREEN
    };
    
    SetConsoleTextAttribute( file_pointer, colorMap[ level ] );
    printf( msg );
}

#endif