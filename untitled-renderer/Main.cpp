#include "common.hpp"

int WINAPI WinMain( HINSTANCE instance, HINSTANCE prev_instance, PSTR cmd, int show_cmd ) {
    gWin32->CreateRenderEnvironment( "Untitled Renderer", { 1280, 720 } );
    gGraphics->Init( gWin32->GetHwnd( ) );
    gBuffer->CreateObjects( );

    while ( gWin32->Dispatch( ) ) {
        gContext->Update( );

        gBuffer->FilledRectangle( { 100, 100 }, { 100, 100 }, { 15, 15, 15, 255 }, 10, corner_all );
        gBuffer->Rectangle( { 100, 100 }, { 100, 100 }, { 255, 120, 255, 255 }, 10, corner_all );

        gGraphics->Draw( );
    }

    gGraphics->Release( );
}