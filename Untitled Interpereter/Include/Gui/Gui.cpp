#include "Gui.hpp"

sConfig Cfg;

int Untitled::Gui::Render( ) {
    ImGui::SetNextWindowPos( { 0.f, 0.f } );
    ImGui::SetNextWindowSize( { 400.f, Win32::GetSize( ).y - 250.f } );

    if ( ImGui::Begin( Cfg.Gui.ListPanel ? "Tokens" : "Variables", &Cfg.Gui.Open, INACTIVE_WINDOW_FLAGS ) ) {
        if ( ImGui::BeginMenuBar( ) ) {
            if ( ImGui::BeginMenu( "Edit" ) ) {
                if ( ImGui::MenuItem( "Clear" ) )
                    Cfg.Gui.ListPanel == 0 ? Cfg.Backend.SavedInterpreter.Formatted.clear( ) : Cfg.Backend.SavedLexer.Formatted.clear( );

                ImGui::EndMenu( );
            }

            if ( ImGui::BeginMenu( "View" ) ) {
                if ( ImGui::MenuItem( "Variables" ) )
                    Cfg.Gui.ListPanel = 0;
                if ( ImGui::MenuItem( "Tokens" ) )
                    Cfg.Gui.ListPanel = 1;

                ImGui::EndMenu( );
            }

            ImGui::EndMenuBar( );
        }

        const auto& formattedData = ( Cfg.Gui.ListPanel == 0 ) ? Cfg.Backend.SavedInterpreter.Formatted : Cfg.Backend.SavedLexer.Formatted;
        for ( const auto& formatted : formattedData )
            ImGui::Text( formatted.c_str( ) );

        ImGui::End( );
    }

    ImGui::SetNextWindowPos( { 0.f, Win32::GetSize( ).y - 250.f } );
    ImGui::SetNextWindowSize( { Win32::GetSize( ).x, 250.f } );

    if ( ImGui::Begin( "Output", &Cfg.Gui.Open, INACTIVE_WINDOW_FLAGS ) ) {
        if ( ImGui::BeginMenuBar( ) ) {
            if ( ImGui::BeginMenu( "Edit" ) ) {

                ImGui::EndMenu( );
            }

            if ( ImGui::BeginMenu( "View" ) ) {
                if ( ImGui::MenuItem( "Output" ) )
                    Cfg.Gui.OutputPanel = 0;

                if ( ImGui::MenuItem( "Errors" ) )
                    Cfg.Gui.OutputPanel = 1;

                if ( ImGui::MenuItem( "Warnings" ) )
                    Cfg.Gui.OutputPanel = 1;

                ImGui::EndMenu( );
            }

            ImGui::EndMenuBar( );
        }

        std::vector<ImColor> colors = {
            ImColor( 245, 245, 245, 255 ),
            ImColor( 100, 100, 255, 255 ),
            ImColor( 219, 205, 75, 255 ),
            ImColor( 217, 20, 20, 255 ),
            ImColor( 27, 209, 106, 255 )
        };

        for ( const auto& Log : Cfg.Backend.Output )
            ImGui::TextColored( colors[ Log.first ], Log.second.c_str( ) );

        ImGui::End( );
    }

    ImGui::SetNextWindowPos( { 400.f, 0.f } );
    ImGui::SetNextWindowSize( { Win32::GetSize( ).x - 400.f, Win32::GetSize( ).y - 250.f } );

    if ( ImGui::Begin( "Code editor", &Cfg.Gui.Open, WINDOW_FLAGS ) ) {
        if ( ImGui::BeginMenuBar( ) ) {
            if ( ImGui::BeginMenu( "Edit" ) ) {
                bool ro = Cfg.Gui.Editor.IsReadOnly( );
                if ( ImGui::MenuItem( "Read-only mode", nullptr, &ro ) )
                    Cfg.Gui.Editor.SetReadOnly( ro );
                ImGui::Separator( );

                if ( ImGui::MenuItem( "Undo", "Ctrl-Z", nullptr, !ro && Cfg.Gui.Editor.CanUndo( ) ) )
                    Cfg.Gui.Editor.Undo( );
                if ( ImGui::MenuItem( "Redo", "Ctrl-Y", nullptr, !ro && Cfg.Gui.Editor.CanRedo( ) ) )
                    Cfg.Gui.Editor.Redo( );

                ImGui::Separator( );

                if ( ImGui::MenuItem( "Copy", "Ctrl-C", nullptr, Cfg.Gui.Editor.HasSelection( ) ) )
                    Cfg.Gui.Editor.Copy( );
                if ( ImGui::MenuItem( "Cut", "Ctrl-X", nullptr, !ro && Cfg.Gui.Editor.HasSelection( ) ) )
                    Cfg.Gui.Editor.Cut( );
                if ( ImGui::MenuItem( "Delete", "Del", nullptr, !ro && Cfg.Gui.Editor.HasSelection( ) ) )
                    Cfg.Gui.Editor.Delete( );
                if ( ImGui::MenuItem( "Paste", "Ctrl-V", nullptr, !ro && ImGui::GetClipboardText( ) != nullptr ) )
                    Cfg.Gui.Editor.Paste( );

                ImGui::Separator( );

                if ( ImGui::MenuItem( "Select all", nullptr, nullptr ) )
                    Cfg.Gui.Editor.SetSelection( TextEditor::Coordinates( ), TextEditor::Coordinates( Cfg.Gui.Editor.GetTotalLines( ), 0 ) );

                ImGui::EndMenu( );
            }

            if ( ImGui::BeginMenu( "View" ) ) {
                if ( ImGui::MenuItem( "Dark palette" ) ) {
                    Cfg.Gui.Editor.SetPalette( TextEditor::GetDarkPalette( ) );
                    ImGui::StyleColorsDark( );
                }
                if ( ImGui::MenuItem( "Light palette" ) ) {
                    Cfg.Gui.Editor.SetPalette( TextEditor::GetLightPalette( ) );
                    ImGui::StyleColorsLight( );
                }
                if ( ImGui::MenuItem( "Cherry blossom palette" ) ) {
                    Cfg.Gui.Editor.SetPalette( TextEditor::GetCherryBlossomPalette( ) );
                    ImGui::StyleColorsCherryBlossom( );
                }

                ImGui::EndMenu( );
            }

            ImGui::EndMenuBar( );
        }

        Cfg.Gui.Editor.Render( "TextEditor", { ImGui::GetWindowSize( ).x - 17.f, ImGui::GetWindowSize( ).y - 80.f } );

        ImGui::Dummy( { 0.f, 3.f } );

        if ( ImGui::Button( "Execute" ) ) {
            std::jthread{ [ &editor = Cfg.Gui.Editor ] {
                Context::Execute( ENVIORNMENT, editor.GetText( ), UNRESTRICTED );
            } }.detach( );
        }

        ImGui::SameLine( );

        if ( ImGui::Button( "Clear" ) )
            Cfg.Gui.Editor.SetText( "" );

        ImGui::End( );
    }

    return 1;
}