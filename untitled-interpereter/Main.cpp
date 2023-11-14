#include "include/include.hpp"

int main( ) {
    const auto Scripts = Untitled::Utilities::GetFolderContents( "Scripts" );

    if ( Scripts.empty( ) ) {
        Untitled::Utilities::ClearConsole( "It seems your scripts folder is empty!\n" );
        Sleep( 3000 );
        return 1;
    }

    Untitled::Context::Init( );

    while ( true ) {
        SetConsoleTitle( "Untitled API (Commands inactive)" );

        Untitled::Utilities::Print( "Scripts:\n" );

        for ( int i = 0; i < Scripts.size( ); ++i ) {
            auto& [scriptName, scriptPath] = Scripts[ i ];
            Untitled::Utilities::PrintInfo( std::vformat( "{}: {}\n", std::make_format_args( std::to_string( i ), scriptName ) ).c_str( ) );
        }

        Untitled::Utilities::Print( "\nEnter Script ID: \n" );

        int ScriptSelection = 0;
        std::cin >> ScriptSelection;

        
        Untitled::Utilities::ClearConsole( );

        // Sandbox thread.
        std::jthread{ [ script = Scripts[ ScriptSelection ] ]( ) {
            Untitled::Context::RunScript( ENVIORNMENT, std::get<1>( script ), LEVEL );
        } }.detach( );

        // Command line.
        while ( true ) {
            SetConsoleTitle( "Untitled API (Commands active)" );

            std::string commandInput;
            std::cin >> commandInput;

            if ( !Untitled::Context::RunCommand( commandInput ) ) {
                Untitled::Utilities::ClearConsole( );
                break;
            }
        }
    }

    return 0;
}