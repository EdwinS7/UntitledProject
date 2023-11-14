#include "utilities.hpp"

void Untitled::Utilities::Print(const char* str ) {
    std::printf( str );
}

void Untitled::Utilities::PrintError(const char* str ) {
    std::printf( "\x1b[31m" );
    std::printf( str );
    std::printf( "\x1b[0m" );
}

void Untitled::Utilities::PrintInfo( const char* str ) {
    std::printf( "\x1b[94m" );
    std::printf( str );
    std::printf( "\x1b[0m" );
}

void Untitled::Utilities::PrintWarning( const char* str ) {
    std::printf( "\x1b[33m" );
    std::printf( str );
    std::printf( "\x1b[0m" );
}

void Untitled::Utilities::PrintSuccess( const char* str ) {
    std::printf( "\x1b[92m" );
    std::printf( str );
    std::printf( "\x1b[0m" );
}

void Untitled::Utilities::ClearConsole( const char* reason ) {
    system( "cls" );

    if ( reason )
        std::printf( reason );
}

void Untitled::Utilities::PrintVersion( ) {
    std::printf( "1.0\n" );
}

void Untitled::Utilities::CommandExecute( const char* command ) {
    if ( !std::strcmp( command, "Clear" ) )
        ClearConsole( );
}

std::string Untitled::Utilities::ToLower( std::string str ) {
    std::string result = str;

    for ( char& c : result )
        c = std::tolower( c );

    return result;
}

std::string Untitled::Utilities::ToUpper( std::string str ) {
    std::string result = str;

    for ( char& c : result )
        c = std::toupper( c );

    return result;
}

bool Untitled::Utilities::IsNumber( std::string str ) {
    return !str.empty( ) && std::find_if( str.begin( ), str.end( ), [ ] ( unsigned char c ) { return !std::isdigit( c ); } ) == str.end( );
}

FOLDER_CONTENT Untitled::Utilities::GetFolderContents( std::string folder ) {
    std::vector<std::tuple<std::string, std::string>> Temp;

    for ( const auto& entry : std::filesystem::directory_iterator( "Scripts" ) ) {
        if ( entry.is_regular_file( ) ) {
            std::string Name, Content;

            Name = entry.path( ).filename( ).string( );

            std::ifstream File( entry.path( ) );

            if ( File.is_open( ) )
                Content = std::string( ( std::istreambuf_iterator<char>( File ) ), std::istreambuf_iterator<char>( ) );
            else {
                Untitled::Utilities::PrintError( "Failed to open file!\n" );
                continue;
            }

            Temp.push_back( std::make_tuple( Name, Content ) );
        }
    }

    return Temp;
}

int Untitled::Utilities::IsFirstLaunch( ) {
    HKEY HKey;

    if ( RegOpenKeyEx( HKEY_CURRENT_USER, "Software//UntitledAPI", 0, KEY_READ, &HKey ) != ERROR_SUCCESS ) {
        if ( RegCreateKey( HKEY_CURRENT_USER, "Software//UntitledAPI", &HKey ) == ERROR_SUCCESS ) {
            DWORD value = 1;
            RegSetValueEx( HKey, "IsFirstLaunch", 0, REG_DWORD, ( BYTE* ) &value, sizeof( value ) );
            RegCloseKey( HKey );
        }

        return 1;
    }
    else {
        RegCloseKey( HKey );
        return 0;
    }
}