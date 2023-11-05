#include "include/include.hpp"

int main( ) {
    SetConsoleTitle( "Untitled API" );

    if ( Untitled::Context::IsFirstLaunch( ) )
        Untitled::Utilities::Print( "We've detected this is your first time using untitled!\n\n" );
    else
        Untitled::Utilities::Print( "Welcome back!\n\n" );

    if ( !std::filesystem::is_directory( "Scripts" ) && !Untitled::Context::IsFirstLaunch( ) ) {
        std::filesystem::create_directory( "Scripts" );
        Untitled::Utilities::PrintWarning( "It seems your scripts folder has been deleted!\n\n" );
    }

Start:

    // ID, User selection.
    int ScriptID = 0, ScriptSelection = 0;
    
    // Name, Source code.
    std::vector<std::tuple<std::string, std::string>> Scripts
        = Untitled::Utilities::GetFolderContents( "Scripts" );

    // Prevent vector crashes due to trying to access no existent memory.
    if ( Scripts.empty( ) ) {
        Untitled::Utilities::ClearConsole( "It seems your scripts folder is empty!\n" );
        Sleep( 3000 );
        return 1;
    }

    Untitled::Utilities::Print( "Scripts:\n" );

    for ( auto Script : Scripts ) {
        Untitled::Utilities::PrintWarning( std::vformat( "{}: {}\n", std::make_format_args( std::to_string( ScriptID ), std::get<0>( Script ) ) ).c_str( ) );
        ScriptID++;
    }

    Untitled::Utilities::Print( "\nEnter Script ID: \n" );

    std::cin >> ScriptSelection;

    Untitled::Utilities::ClearConsole( );

    //Sandbox thread.
    {
        std::jthread{ [ Scripts, ScriptSelection ]( ) {
            Untitled::Context::RunScript( "UntitledAPI", std::get<1>( Scripts[ ScriptSelection ] ), LEVEL );
        } }.detach( );
    }

    // Command line
COMMAND_LINE:

    std::string CommandInput;
    std::cin >> CommandInput;

    if ( Untitled::Utilities::ToLower( CommandInput ) == "/clear" )
        Untitled::Utilities::ClearConsole( );
    else if ( Untitled::Utilities::ToLower( CommandInput ) == "/r" || Untitled::Utilities::ToLower( CommandInput ) == "/reset" ) {
        Untitled::Utilities::ClearConsole( );
        goto Start;
    }
    else if ( Untitled::Utilities::ToLower( CommandInput ) == "/listcmd" ) {
        Untitled::Utilities::Print( "\nCommands:\n/clear (Clears console output)\n/r OR /reset (Reset's the applications state to start)\n" );
    }
        
    goto COMMAND_LINE;
    
    return 0;
}