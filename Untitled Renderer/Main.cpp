#include "Include/Untitled.hpp"

int WINAPI WinMain( HINSTANCE hInstance, HINSTANCE hPrevInstance, PSTR lpCmdLine, int nCmdShow ) {
    gWindow->Init( "Untitled Renderer", Vec2<int16_t>( 1280, 720 ), false );
    gGraphics->Init( gWindow->GetHandle( ) );

    gFileSystem->Init( );
    gBuffer->Init( );
    
    gLuaWrapper->Init( );

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