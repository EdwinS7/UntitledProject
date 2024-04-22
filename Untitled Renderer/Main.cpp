#include "Common.hpp"

int WINAPI WinMain( HINSTANCE instance, HINSTANCE prev_instance, PSTR cmd, int show_cmd ) {
    gWindow->InitWindow( "Untitled Renderer", { 1280, 720 } );
    //gConsole->InitConsole( "Developer Console" );
    gGraphics->Init( gWindow->GetHwnd( ) );
    gBuffer->Init( );

    while ( gWindow->DispatchMessages( ) ) {
        gContext->Update( );
        gBuffer->FilledRectangle( { 200, 200 }, { 100, 100 }, { 255, 0, 0, 255 }, 15 );
        gGraphics->Draw( );
    }

    gGraphics->Release( );
    gBuffer->Release( );
}