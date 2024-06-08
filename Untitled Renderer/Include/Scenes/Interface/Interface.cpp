#include "Interface.hpp"

void cInterface::Init( ) {
    for ( auto& Callback : gWrapper->GetCallbacks( "OnInterfaceSpawn" ) ) {
        auto Result = Callback.Function( );

        if ( !Result.valid( ) )
            std::cout << static_cast< std::string >( Result.get<sol::error>( ).what( ) ) << "\n";
    }
}

void cInterface::Draw( ) {
    for ( auto& Callback : gWrapper->GetCallbacks( "OnInterfacePaint" ) ) {
        auto Result = Callback.Function( );

        if ( !Result.valid( ) )
            std::cout << static_cast< std::string >( Result.get<sol::error>( ).what( ) ) << "\n";
    }
    
    gBuffer->Picture( &gBuffer->TestImage, Vec2<int16_t>( 50, 50 ), Color( 255, 255, 255, 255 ) );

    /*if ( gInput->KeyPressed( KEY_F1 ) )
        m_DebugConsole = !m_DebugConsole;

    if ( m_DebugConsole ) {
        auto ScreenSize = gWindow->GetSize( );
        auto TextSize = gBuffer->GetTextSize( gBuffer->GetDefaultFont( ), "Execute: " + m_DebugConsoleInput );
        
        gBuffer->FilledRectangle( Vec2<int16_t>( 0, 0 ), Vec2<int16_t>( ScreenSize.x, TextSize.y + 4 ), Color( 0, 0, 0, 50 ) );
        gBuffer->Text( gBuffer->GetDefaultFont( ), "Execute: " + m_DebugConsoleInput, Vec2<int16_t>( 2, 2 ), Color( 255, 255, 255 ) );
    }*/
}