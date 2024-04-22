#include "Common.hpp"

int WINAPI WinMain( HINSTANCE instance, HINSTANCE prev_instance, PSTR cmd, int show_cmd ) {
    gGraphics->Init( gWindow->InitWindow( "Untitled Renderer" ) );
    gBuffer->Init( );

    while ( gWindow->DispatchMessages( ) ) {
        gContext->Update( );
        gInterface->Draw( );
        gGraphics->Draw( );
    }

    gGraphics->Release( );
    gBuffer->Release( );
}