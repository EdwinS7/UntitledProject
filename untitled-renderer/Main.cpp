#include "Common.hpp"

void GuiRender( ) {
    static auto Window = gGui->NewWindow( "Redacted", "A Simple rendering library for your needs!", Vec2<int16_t>( 700, 500 ) );

    Window->Begin( );
    {
        static auto General = Window->AddTab( "General" );
        static auto Other = Window->AddTab( "Other" );

        int Padding = 20;

        Vec2<int16_t> Size =  Vec2<int16_t>( ( Window->GetElementFrameSize( ).x / 2 ) - Padding * 3, Window->GetElementFrameSize( ).y - Padding * 2 );

        Vec2<int16_t> TopLeft = Vec2<int16_t>( Window->GetElementFramePos( ).x + Padding, Window->GetElementFramePos( ).y + Padding );
        Vec2<int16_t> TopRight = Vec2<int16_t>( Window->GetElementFramePos( ).x + Padding * 2 + Window->GetElementFrameSize( ).x / 2, Window->GetElementFramePos( ).y + Padding );
        
        static auto MainGroup = General->NewGroup( "Main", TopLeft, Size );
        //static auto OtherGroup = General->NewGroup( "Other", TopRight, Size );

        MainGroup->Begin( );
        //OtherGroup->Begin( );
    }
}

int WINAPI WinMain( HINSTANCE instance, HINSTANCE prev_instance, PSTR cmd, int show_cmd ) {
    gWin32->CreateRenderEnvironment( "Untitled Renderer", { 1280, 720 } );
    gGraphics->Init( gWin32->GetHwnd( ) );
    gBuffer->Init( true );

    while ( gWin32->Dispatch( ) ) {
        gContext->Update( );
        GuiRender( );
        gGraphics->Draw( );
    }

    gGraphics->Release( );
}