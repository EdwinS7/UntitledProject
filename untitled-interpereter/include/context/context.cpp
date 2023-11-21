#include "context.hpp"

namespace Config {
    namespace Gui {
        TextEditor Editor;
        bool Open = true;
        int ListPanel = 1;
    }

    namespace Backend {
        LexerResponse SavedLexer;
        InterpreterResponse SavedInterpreter;
        std::vector<std::pair<int, std::string>> Output;
    }
}

void Untitled::Context::Init( ) {
    if ( Untitled::Utilities::IsFirstLaunch( ) )
        AddLog( 1, "We've detected this is your first time using untitled!" );
    else
        AddLog( 1, "Welcome back!" );

    if ( !std::filesystem::is_directory( "Scripts" ) && !Untitled::Utilities::IsFirstLaunch( ) ) {
        std::filesystem::create_directory( "Scripts" );
        AddLog( 2, "It seems your scripts folder has been deleted!" );
    }
}

void Untitled::Context::RenderWindow( ) {
    ImGui::SetNextWindowPos( { 0.f, 0.f } );
    ImGui::SetNextWindowSize( { 400.f, Win32::GetSize( ).y - 250.f } );

    if ( ImGui::Begin( Config::Gui::ListPanel ? "Tokens" : "Variables", &Config::Gui::Open, INACTIVE_WINDOW_FLAGS | ImGuiWindowFlags_MenuBar ) ) {
        if ( ImGui::BeginMenuBar( ) ) {
            if ( ImGui::BeginMenu( "Edit" ) ) {
                if ( ImGui::MenuItem( "Clear", nullptr, nullptr ) )
                    ( Config::Gui::ListPanel == 0 ) ? Config::Backend::SavedInterpreter.Formatted.clear( ) : Config::Backend::SavedLexer.Formatted.clear( );

                ImGui::EndMenu( );
            }

            if ( ImGui::BeginMenu( "View" ) ) {
                if ( ImGui::MenuItem( "Variables" ) )
                    Config::Gui::ListPanel = 0;
                if ( ImGui::MenuItem( "Tokens" ) )
                    Config::Gui::ListPanel = 1;

                ImGui::EndMenu( );
            }

            ImGui::EndMenuBar( );
        }

        const auto& formattedData = ( Config::Gui::ListPanel == 0 ) ? Config::Backend::SavedInterpreter.Formatted : Config::Backend::SavedLexer.Formatted;
        for ( const auto& formatted : formattedData )
            ImGui::Text( formatted.c_str( ) );

        ImGui::End( );
    }

    ImGui::SetNextWindowPos( { 0.f, Win32::GetSize( ).y - 250.f } );
    ImGui::SetNextWindowSize( { Win32::GetSize( ).x, 250.f } );

    if ( ImGui::Begin( "Output", &Config::Gui::Open, INACTIVE_WINDOW_FLAGS ) ) {
        std::vector<ImColor> colors = {
            ImColor( 245, 245, 245, 255 ),
            ImColor( 100, 100, 255, 255 ),
            ImColor( 219, 205, 75, 255 ),
            ImColor( 217, 20, 20, 255 ),
            ImColor( 27, 209, 106, 255 )
        };

        for ( const auto& log : Config::Backend::Output )
            ImGui::TextColored( colors[ log.first ], log.second.c_str( ) );

        ImGui::End( );
    }

    ImGui::SetNextWindowPos( { 400.f, 0.f } );
    ImGui::SetNextWindowSize( { Win32::GetSize( ).x - 400.f, Win32::GetSize( ).y - 250.f } );

    if ( ImGui::Begin( "Code editor", &Config::Gui::Open, WINDOW_FLAGS | ImGuiWindowFlags_MenuBar ) ) {
        if ( ImGui::BeginMenuBar( ) ) {
            if ( ImGui::BeginMenu( "Edit" ) ) {
                bool ro = Config::Gui::Editor.IsReadOnly( );
                if ( ImGui::MenuItem( "Read-only mode", nullptr, &ro ) )
                    Config::Gui::Editor.SetReadOnly( ro );
                ImGui::Separator( );

                if ( ImGui::MenuItem( "Undo", "Ctrl-Z", nullptr, !ro && Config::Gui::Editor.CanUndo( ) ) )
                    Config::Gui::Editor.Undo( );
                if ( ImGui::MenuItem( "Redo", "Ctrl-Y", nullptr, !ro && Config::Gui::Editor.CanRedo( ) ) )
                    Config::Gui::Editor.Redo( );

                ImGui::Separator( );

                if ( ImGui::MenuItem( "Copy", "Ctrl-C", nullptr, Config::Gui::Editor.HasSelection( ) ) )
                    Config::Gui::Editor.Copy( );
                if ( ImGui::MenuItem( "Cut", "Ctrl-X", nullptr, !ro && Config::Gui::Editor.HasSelection( ) ) )
                    Config::Gui::Editor.Cut( );
                if ( ImGui::MenuItem( "Delete", "Del", nullptr, !ro && Config::Gui::Editor.HasSelection( ) ) )
                    Config::Gui::Editor.Delete( );
                if ( ImGui::MenuItem( "Paste", "Ctrl-V", nullptr, !ro && ImGui::GetClipboardText( ) != nullptr ) )
                    Config::Gui::Editor.Paste( );

                ImGui::Separator( );

                if ( ImGui::MenuItem( "Select all", nullptr, nullptr ) )
                    Config::Gui::Editor.SetSelection( TextEditor::Coordinates( ), TextEditor::Coordinates( Config::Gui::Editor.GetTotalLines( ), 0 ) );

                ImGui::EndMenu( );
            }

            if ( ImGui::BeginMenu( "View" ) ) {
                if ( ImGui::MenuItem( "Dark palette" ) ) {
                    Config::Gui::Editor.SetPalette( TextEditor::GetDarkPalette( ) );
                    ImGui::StyleColorsDark( );
                }   
                if ( ImGui::MenuItem( "Light palette" ) ) {
                    Config::Gui::Editor.SetPalette( TextEditor::GetLightPalette( ) );
                    ImGui::StyleColorsLight( );
                }
                if ( ImGui::MenuItem( "Cherry blossom palette" ) ) {
                    Config::Gui::Editor.SetPalette( TextEditor::GetCherryBlossomPalette( ) );
                    ImGui::StyleColorsCherryBlossom( );
                }

                ImGui::EndMenu( );
            }

            ImGui::EndMenuBar( );
        }

        Config::Gui::Editor.Render( "TextEditor", { ImGui::GetWindowSize( ).x - 17.f, ImGui::GetWindowSize( ).y - 80.f } );

        ImGui::Dummy( { 0.f, 3.f } );

        if ( ImGui::Button( "Execute" ) ) {
            std::jthread{ [ &editor = Config::Gui::Editor ] {
                RunScript( ENVIORNMENT, editor.GetText( ), UNRESTRICTED );
            } }.detach( );
        }

        ImGui::SameLine( );

        if ( ImGui::Button( "Clear" ) )
            Config::Gui::Editor.SetText( "" );

        ImGui::End( );
    }
}

int Untitled::Context::RunScript( const char* Environment, const std::string& Source, int Level ) {
    auto LexerResponse = Lexer::Tokenize( Source );

    if ( LexerResponse.Tokens.size( ) <= 3 ) {
        AddLog( 3, "Too few tokens to interpret!" );
        return 0;
    }

    auto ParserResponse = Parser::ProduceAST( Source );

    InterpreterResponse InterpreterResponse = Interpreter::Interpret( Environment, LexerResponse );

    if ( !InterpreterResponse.Errors.empty( ) ) {
        for ( const auto& error : InterpreterResponse.Errors ) {
            AddLog( 3, error.c_str( ) );
        }
        return 0;
    }

    UpdateLists( InterpreterResponse, LexerResponse );
    AddLog( 4, "Script executed!" );

    Interpreter::Release( );

    return 1;
}

void Untitled::Context::UpdateLists( InterpreterResponse Interpreter, LexerResponse Lexer ) {
    Config::Backend::SavedInterpreter = Interpreter;
    Config::Backend::SavedLexer = Lexer;
}

void Untitled::Context::AddLog( int Level, std::string Message ) {
    Config::Backend::Output.push_back( std::make_pair( Level, Message ) );
}

void Untitled::Context::ClearLog( ) {
    Config::Backend::Output.clear( );
}