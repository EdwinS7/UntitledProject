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

    if ( ImGui::Begin( Config::Gui::ListPanel ? "Tokens" : "Variables", &Config::Gui::Open, INACTIVE_WINDOW_FLAGS ) ) {
        if ( ImGui::BeginPopupContextItem( "List settings" ) ) {
            if ( Config::Gui::ListPanel != 0 && ImGui::MenuItem( "Switch to Variables View" ) )
                Config::Gui::ListPanel = 0;
            else if ( Config::Gui::ListPanel != 1 && ImGui::MenuItem( "Switch to Tokens View" ) )
                Config::Gui::ListPanel = 1;

            ImGui::EndPopup( );
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

    if ( ImGui::Begin( "Code editor", &Config::Gui::Open, WINDOW_FLAGS ) ) {
        Config::Gui::Editor.Render( "TextEditor", { ImGui::GetWindowSize( ).x - 17.f, ImGui::GetWindowSize( ).y - 62.f } );

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
    LexerResponse LexerResponse = Lexer::Tokenize( Source );

    if ( LexerResponse.Tokens.size( ) <= 3 ) {
        AddLog( 3, "Too few tokens to interpret!" );
        return 0;
    }

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