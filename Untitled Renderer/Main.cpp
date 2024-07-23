#include "Include/Untitled.hpp"

// I just have this for testing at the moment.
void RunScript( ) {
    gLogger->Log( LogLevel::Information, "Welcome to Untitled Renderer. Use 'help' for a list of commands." );
    gLogger->Log( LogLevel::Information, "To run a script, enter the file name with the .lua extension." );

    while ( true ) {
        std::cout << "> ";

        std::string Input;
        std::cin >> Input;

        if ( Input == "help" || Input == "h" || Input == "?" ) {
            gLogger->Log( LogLevel::Information, "{'clear', 'cls', 'c'} - Clears the console" );
            gLogger->Log( LogLevel::Information, "{'information', 'info', 'i'} - Clears the console" );
            continue;
        }
        else if ( Input == "clear" || Input == "cls" || Input == "c" ) {
			system( "cls" );
            continue;
        }
        else if ( Input == "information" || Input == "info" || Input == "i" ) {
            gLogger->Log( LogLevel::Information, "Developed by @fuckuneedthisfor" );
            continue;
        }

        static auto Environment = gLuaAPI->NewEnvironment( );

        if ( !Environment->LoadScriptFromFile( FS_SCRIPTS_FOLDER, Input ) ) {
			gLogger->Log( LogLevel::Error, std::format("Failed to load '{}'", Input) );
        }
    }
}

int WINAPI WinMain( HINSTANCE hInstance, HINSTANCE hPrevInstance, PSTR lpCmdLine, int nCmdShow ) {
    gWindow->Init( "Untitled Renderer", Vec2<int16_t>( 1280, 720 ), false );
    gGraphics->Init( gWindow->GetHandle( ) );
    gFileSystem->Init( );
    gBuffer->Init( );

    gLuaAPI->Init( );

    std::thread LuaThread( RunScript );

    while ( gWindow->DispatchMessages( ) ) {
        gContext->Update( );
       
        if ( !gWindow->IsMinimized( ) ) {
            gInterface->Draw( );
            gWorld->Draw( );
        }

        gGraphics->DrawScene( );
    }

    gGraphics->Release( );
    gBuffer->Release( );
}