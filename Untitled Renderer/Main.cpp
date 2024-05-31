#include "Common.hpp"

int WINAPI WinMain( HINSTANCE instance, HINSTANCE prev_instance, PSTR cmd, int show_cmd ) {
    gWindow->Init( "Untitled Renderer", Vec2<int16_t>( 1280, 720 ), true );
    gGraphics->Init( gWindow->GetHandle( ) );
    gWrapper->Init( );
    gBuffer->Init( );

    RETRY:
    std::string ScriptFileName;
    std::cin >> ScriptFileName;
    system( "cls" );

    if ( !gWrapper->LoadScriptFromFile( ScriptFileName ) )
        goto RETRY;

    while ( gWindow->DispatchMessages( ) ) {
        gContext->Update( );

        if ( gWindow->IsFocused( ) ) {
            gInterface->Draw( );
            gWorld->Draw( );
        }

        gGraphics->Draw( );
    }

    gGraphics->Release( );
    gBuffer->Release( );
}