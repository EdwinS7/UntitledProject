#include "Common.hpp"

int WINAPI WinMain( HINSTANCE instance, HINSTANCE prev_instance, PSTR cmd, int show_cmd ) {
    gWindow->InitWindow( "Untitled Renderer", { 1280, 720 } );
    //gConsole->InitConsole( "Developer Console" );
    gGraphics->Init( gWindow->GetHwnd( ) );
    gBuffer->Init( );

    while ( gWindow->DispatchMessages( ) ) {
        gContext->Update( );
        
        gGraphics->Draw( );
    }

    gGraphics->Release( );
    gBuffer->Release( );
}