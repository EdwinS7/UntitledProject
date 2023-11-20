#include "utilities.hpp"

void Untitled::Utilities::PrintVersion( ) {
    Context::AddLog( 1, "Untitled 1.0 (pre-pre alpha)" );
}

std::string Untitled::Utilities::ToLower( std::string String ) {
    for ( char& c : String )
        c = std::tolower( c );

    return String;
}

std::string Untitled::Utilities::ToUpper( std::string String ) {
    for ( char& c : String )
        c = std::toupper( c );

    return String;
}

FOLDER_CONTENT Untitled::Utilities::GetFolderContents( const std::string& Folder ) {
    FOLDER_CONTENT FolderContent;

    for ( const auto& entry : std::filesystem::directory_iterator( Folder ) ) {
        if ( entry.is_regular_file( ) ) {
            std::ifstream file( entry.path( ) );

            if ( file.is_open( ) ) {
                std::string name = entry.path( ).filename( ).string( );
                std::string content( ( std::istreambuf_iterator<char>( file ) ), std::istreambuf_iterator<char>( ) );
                FolderContent.push_back( std::make_tuple( name, content ) );
            }
            else
                continue;
        }
    }

    return FolderContent;
}

int Untitled::Utilities::IsFirstLaunch( ) {
    HKEY hKey;

    if ( RegOpenKeyEx( HKEY_CURRENT_USER, "Software\\UntitledAPI", 0, KEY_READ, &hKey ) != ERROR_SUCCESS ) {
        if ( RegCreateKey( HKEY_CURRENT_USER, "Software\\UntitledAPI", &hKey ) == ERROR_SUCCESS ) {
            DWORD value = 1;
            RegSetValueEx( hKey, "IsFirstLaunch", 0, REG_DWORD, reinterpret_cast< const BYTE* >( &value ), sizeof( value ) );
            RegCloseKey( hKey );
        }

        return 1;
    }

    RegCloseKey( hKey );
    return 0;
}