#include <iostream>
#include <windows.h>
#include <shlobj.h>
#include <cstdio>
#include <string>

inline std::string GetFontPath( const char* FontName ) {
    HKEY key;

    if ( RegOpenKeyExA( HKEY_LOCAL_MACHINE, "Software\\Microsoft\\Windows NT\\CurrentVersion\\Fonts", 0, KEY_READ, &key ) != ERROR_SUCCESS ) {
        std::printf( "[ Buffer ] get_path failed ( failed to open registry )\n" );
        return "";
    }

    std::string str_path;
    char str_buffer[ MAX_PATH ];

    for ( auto i = 0;; i++ ) {
        DWORD buf_size = MAX_PATH;
        memset( str_buffer, 0, MAX_PATH );

        if ( RegEnumValueA( key, i, str_buffer, &buf_size, nullptr, nullptr, nullptr, nullptr ) != ERROR_SUCCESS ) {
            break; // End of registry values
        }

        // Extract font name from registry entry
        std::string fontEntry( str_buffer );
        size_t separatorPos = fontEntry.find( " (" );
        if ( separatorPos != std::string::npos )
            fontEntry = fontEntry.substr( 0, separatorPos );

        // Print font names for debugging
        printf( "Font name from registry: %s\n", fontEntry.c_str( ) );

        // Compare font names
        if ( _stricmp( fontEntry.c_str( ), FontName ) == 0 ) {
            buf_size = MAX_PATH;
            RegQueryValueExA( key, str_buffer, nullptr, nullptr, reinterpret_cast< LPBYTE >( str_buffer ), &buf_size );
            str_path = str_buffer;
            break;
        }
    }

    RegCloseKey( key );

    if ( str_path.empty( ) ) {
        std::printf( "[ Buffer ] get_path failed ( font not found )\n" );
        return "";
    }

    memset( str_buffer, 0, MAX_PATH );
    SHGetFolderPathA( nullptr, CSIDL_FONTS, nullptr, 0, str_buffer );

    return std::string( str_buffer ) + "\\" + str_path;
}

int main( ) {
    std::string fontPath = GetFontPath( "HalfLife2 Regular" );
    if ( !fontPath.empty( ) ) {
        printf( "Font path: %s\n", fontPath.c_str( ) );
    }
    return 0;
}