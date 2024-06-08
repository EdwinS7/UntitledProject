#include "Common.hpp"

int WINAPI WinMain( HINSTANCE instance, HINSTANCE prev_instance, PSTR cmd, int show_cmd ) {
    gWindow->Init( "Untitled Renderer", Vec2<int16_t>( 1280, 720 ), true );
    gGraphics->Init( gWindow->GetHandle( ) );
    gWrapper->Init( );
    gBuffer->Init( );

    std::string Script = "";
    std::cin >> Script;
    system( "cls" );

    gWrapper->LoadScriptFromFile( Script );

    while ( gWindow->DispatchMessages( ) ) {
        gContext->Update( );
        gCamera->Update( );

        if ( gWindow->IsFocused( ) ) {
            gInterface->Draw( );
            gWorld->Draw( );
        }

        gGraphics->DrawScene( );
    }

    gGraphics->Release( );
    gBuffer->Release( );
}