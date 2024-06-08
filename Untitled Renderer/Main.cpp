#include "Common.hpp"

int WINAPI WinMain( HINSTANCE instance, HINSTANCE prev_instance, PSTR cmd, int show_cmd ) {
    gFileSystem->Init( );
    
    gWindow->Init( "Untitled Renderer", Vec2<int16_t>( 1920, 1080 ), true );
    gGraphics->Init( gWindow->GetHandle( ) );
    gBuffer->Init( );

    gWrapper->Init( );
    gAudio->Init( );
    
    std::string Script = "";
    std::cin >> Script;
    system( "cls" );

    gWrapper->LoadScriptFromFile( "Scripts/", Script );

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
    gAudio->Release( );
}