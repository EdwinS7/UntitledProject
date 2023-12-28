#include "Common.hpp"

int WINAPI WinMain( HINSTANCE instance, HINSTANCE prev_instance, PSTR cmd, int show_cmd ) {
    gWin32->CreateRenderEnvironment( "Untitled Renderer", { 1280, 720 } );
    
    gGraphics->Init( gWin32->GetHwnd( ) );
    gGraphics->SetClearColor( { 120, 120, 120, 255 } );

    gBuffer->Init( true );

    auto Window = gGui->NewWindow( "Redacted", "A Simple rendering library for your needs!", Vec2<int16_t>( 700, 500 ) );

    while ( gWin32->Dispatch( ) ) {
        gContext->Update( );
        Window->Render( );
        gGraphics->Draw( );
    }

    gGraphics->Release( );
    Window->Destroy( );
}