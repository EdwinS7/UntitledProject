#include "context.hpp"

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

int Untitled::Context::Execute( const char* Environment, const std::string& Source, int Level ) {
    auto Lexer = Lexer::Tokenize( Source );
    auto Interpreter = Interpreter::Interpret( Environment, Lexer );

    // No error handling right now!
    if ( Interpreter::Interpret( Environment, Lexer ) ) {
        /*for ( const auto& error : Interpreter.Errors ) {
            AddLog( 3, error.c_str( ) );
        }
        return 0;*/
    }

    //Config::Backend::SavedInterpreter = Interpreter;
    Config::Backend::SavedLexer = Lexer;

    AddLog( 4, "Script executed!" );

    return 1;
}

void Untitled::Context::AddLog( int Level, std::string Message ) {
    Config::Backend::Output.push_back( std::make_pair( Level, Message ) );
}

void Untitled::Context::ClearLog( ) {
    Config::Backend::Output.clear( );
}