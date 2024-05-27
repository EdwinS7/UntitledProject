#include "Common.hpp"

int WINAPI WinMain( HINSTANCE instance, HINSTANCE prev_instance, PSTR cmd, int show_cmd ) {
    gWindow->InitWindow( "Untitled Renderer" );
    gGraphics->Init( );
    gBuffer->Init( );

    while ( gWindow->DispatchMessages( ) ) {
        gContext->Update( );

        if ( gInput->AnyKeyPressed( ) ) {
            gInterface->Init( );
            gWorld->Init( );
        }

        gGraphics->Draw( );
    }

    gGraphics->Release( );
    gBuffer->Release( );
}