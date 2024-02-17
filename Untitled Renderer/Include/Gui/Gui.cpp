#include "Gui.hpp"

void cGui::DemoWindow( ) {
    static Window* Window = gGui->NewWindow( "Untitled Gui Demo Window", "A lightweight renderer & gui library", Vec2<int16_t>( 700, 500 ) );
    {
        static Tab* GeneralTab = Window->AddTab( "General" );
        static Tab* OtherTab = Window->AddTab( "Other" );

        Window->HandleInput( );
        Window->Render( );
        {
            static Group* GeneralGroup = GeneralTab->NewGroup( "General", &Window );
            static Group* OtherGroup = OtherTab->NewGroup( "Other", &Window );

            const auto GroupSize = Vec2<int16_t>(
                Window->GetElementFrameSize( ).x / 2 - 30,
                Window->GetElementFrameSize( ).y - 40
            );

            GeneralGroup->Begin( Window->GetLeftDock( ), GroupSize );
            {
                static bool TestBoolean = true;
                GeneralGroup->Checkbox( "Demo Checkbox", &TestBoolean );
            }

            /*OtherGroup->Begin( Window->GetRightDock( ), GroupSize );
            {
                
            }*/
        }
    }
}

Window* cGui::NewWindow( const char* Title, const char* Description, Vec2<int16_t> Size ) {
	auto Pos = Vec2<int16_t>( gWin32->GetSize( ).x / 2 - Size.x / 2,  gWin32->GetSize( ).y / 2 - Size.y / 2 );

    m_Windows.push_back( Window( 
            Title, Description,
            Pos, Size,
            Vec2<int16_t>( Size.x / 2, Size.y / 2 ),
            Vec2<int16_t>( Size.x * 1.5, Size.y * 1.5 )
        )
    );

	return &m_Windows.back( );
}