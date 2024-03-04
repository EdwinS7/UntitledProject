#include "Common.hpp"

int WINAPI WinMain( HINSTANCE instance, HINSTANCE prev_instance, PSTR cmd, int show_cmd ) {
    gWin32->CreateRenderEnvironment( "Untitled Renderer", { 1280, 720 } );
    //gWin32->CreateLoggingEnvironment( "Untitled Logs" );
    gGraphics->Init( gWin32->GetHwnd( ) );
    gBuffer->Init( );

    while ( gWin32->Dispatch( ) ) {
        gContext->Update( );
        //gBuffer->TexturedRectangle( &gBuffer->Textures.Demo, { 200, 200 }, { 300, 300 }, { 255, 255, 255, 255 } );
        gGraphics->Draw( );
    }

    gGraphics->Release( );
    gBuffer->Release( );
}