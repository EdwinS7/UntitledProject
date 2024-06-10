#include "Common.hpp"

int WINAPI WinMain( HINSTANCE instance, HINSTANCE prev_instance, PSTR cmd, int show_cmd ) {
    gFileSystem->Init( );
    gLuaWrapper->Init( );

    gWindow->Init( "Untitled Renderer", Vec2<int16_t>( 1280, 720 ), false );
    gGraphics->Init( gWindow->GetHandle( ) );
    gBuffer->Init( );
    

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