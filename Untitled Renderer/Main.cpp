#include "Include/Untitled.hpp"

// I just have this for testing at the moment.
void RunScript( ) {
    while ( true ) {
        std::cout << "> ";

        std::string Input;
        std::vector<std::string> InputStorage;

        while ( std::getline( std::cin, Input ) ) {
            bool ShiftHeld = ( GetAsyncKeyState( VK_SHIFT ) & 0x8000 ) != 0;

            if ( Input.empty( ) && !ShiftHeld ) {
                break;
            }

            InputStorage.push_back( Input );

            if ( !ShiftHeld ) {
                break;
            }
        }

        Input.clear( );

        for ( const std::string& Line : InputStorage ) {
            Input.append( Line + "\n" );
        }
        
        if ( !Input.empty( ) && Input.back( ) == '\n' ) {
            Input.pop_back( );
        }

        if ( Input == "clear" || Input == "cls" ) {
            system( "cls" );
            continue;
        }

        // Creates new env for each script at the moment.
        static auto Environment = gLuaAPI->NewEnvironment( );

        if ( !Environment->LoadScriptFromFile( FS_SCRIPTS_FOLDER, Input ) ) {
            Environment->LoadScript( Input );
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