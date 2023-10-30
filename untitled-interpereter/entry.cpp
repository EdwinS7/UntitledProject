#include "include/include.hpp"

int main( ) {
    HKEY HKey;

    if ( Untitled::Context::IsFirstLaunch( ) )
        Untitled::Utilities::Print( "We've detected this is your first time using untitled!\n\n" );
    else
        Untitled::Utilities::Print( "Welcome back!\n\n" );

    if ( !std::filesystem::is_directory( "Scripts" ) && !Untitled::Context::IsFirstLaunch( ) ) {
        std::filesystem::create_directory( "Scripts" );
        Untitled::Utilities::PrintWarning( "It seems your scripts folder has been deleted!\n\n" );
    }

Start:

    int ScriptID = 0, ScriptSelection = 0;
    
    // ID, Name, Source code.
    std::vector<std::tuple<int, std::string, std::string>> Scripts;

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

            Scripts.push_back( std::make_tuple( ScriptID, Name, Content ) );
        }
    }

    // Prevent vector crashes due to trying to access no existent memory.
    if ( Scripts.empty( ) ) {
        system( "cls" );
        Untitled::Utilities::PrintWarning( "It seems your scripts folder is empty!\n" );
        Sleep( 3000 );
        return 1;
    }
        

    Untitled::Utilities::Print( "Scripts:\n" );

    for ( auto Script : Scripts ) {
        Untitled::Utilities::PrintWarning( std::vformat( "{}: {}\n", std::make_format_args( std::to_string( ScriptID ), std::get<1>( Script ) ) ).c_str( ) );
        ScriptID++;
    }

    Untitled::Utilities::Print( "\nEnter Script ID: \n" );

    std::cin >> ScriptSelection;

    system( "cls" );

    if ( Untitled::Interpereter::RunScript( std::get<2>( Scripts[ ScriptSelection ] ) ) ) {
        Untitled::Utilities::PrintSuccess( "Succeeded!" );
        Sleep( 500 );
    }
    else {
        Untitled::Utilities::PrintError( "Failed!" );
        Sleep( 5000 );
    }

    return 0;
}