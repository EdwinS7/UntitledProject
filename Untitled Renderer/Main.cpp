#include "Common.hpp"

int WINAPI WinMain( HINSTANCE instance, HINSTANCE prev_instance, PSTR cmd, int show_cmd ) {
    gWin32->CreateRenderEnvironment( "Untitled Renderer", { 1280, 720 } );
    gGraphics->Init( gWin32->GetHwnd( ) );
    gBuffer->Init( true );

    while ( gWin32->Dispatch( ) ) {
        gWin32->SetRenderEnvironmentTitle( std::vformat("Delta time: {}", std::make_format_args( gContext->GetDeltaTime( ) ) ).c_str( ) );
        gContext->Update( );
        gGui->DemoWindow( );
        gGraphics->Draw( );
    }

    gGraphics->Release( );
}