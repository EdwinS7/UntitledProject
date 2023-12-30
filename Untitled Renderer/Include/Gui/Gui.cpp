#include "Gui.hpp"

void cGui::DemoWindow( ) {
    static Window* Window = gGui->NewWindow( "Untitled Gui", "A lightweight renderer & gui library", Vec2<int16_t>( 700, 500 ) );
    Window->HandleInput( );
    Window->Render( );
    {
        // Tabs.
        static Tab* GeneralTab = Window->AddTab( "General" );
        static Tab* OtherTab = Window->AddTab( "Other" );

        const auto Left = Vec2<int16_t>( 20, 20 );
        const auto Right = Vec2<int16_t>( Window->GetElementFrameSize( ).x / 2 + 40, 20 );

        const auto Size = Vec2<int16_t>(
            ( Window->GetElementFrameSize( ).x / 2 ) - 60,
            Window->GetElementFrameSize( ).y - 40
        );

        // Groups.
        static auto GeneralGroup = GeneralTab->NewGroup( "General", nullptr, Left );
        //GeneralGroup->Begin( Size );
        
        //static auto OtherGroup = GeneralTab->NewGroup( "Other", Window, Right );
        //OtherGroup->Begin( Size );
    }
}

Window* cGui::NewWindow( const char* Title, const char* Description, Vec2<int16_t> m_Size ) {
	auto Pos = Vec2<int16_t>( gWin32->GetSize( ).x / 2 - m_Size.x / 2, gWin32->GetSize( ).y / 2 - m_Size.y / 2 );
    m_Windows.push_back( Window( Title, Description, Pos, m_Size, Vec2<int16_t>( m_Size.x / 2, m_Size.y / 2 ), Vec2<int16_t>( m_Size.x * 1.5, m_Size.y * 1.5 ) ) );
	return &m_Windows.back( );
}