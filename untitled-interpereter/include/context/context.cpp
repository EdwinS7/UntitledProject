#include "context.hpp"

void Untitled::Context::Init( ) {
    std::map<const char*, std::uintptr_t> Functions;

    Interpreter::Functions::RegisterFunctions( &Functions );
    Interpreter::Register( &Functions );

    if ( Untitled::Utilities::IsFirstLaunch( ) )
        Untitled::Utilities::Print( "We've detected this is your first time using untitled!\n\n" );
    else
        Untitled::Utilities::Print( "Welcome back!\n\n" );

    if ( !std::filesystem::is_directory( "Scripts" ) && !Untitled::Utilities::IsFirstLaunch( ) ) {
        std::filesystem::create_directory( "Scripts" );
        Untitled::Utilities::PrintInfo( "It seems your scripts folder has been deleted!\n\n" );
    }
}

int Untitled::Context::RunScript( const char* Environment, std::string Source, int Level ) {
    std::vector<Token> Tokens = Tokenizer::Tokenize( Source );
    InterpreterResponse Interpreter = Interpreter::Interpret( Environment, Level, Tokens );

    if ( Interpreter.Errors.size( ) > 0 ) {
        for ( auto& Error : Interpreter.Errors )
            Utilities::Print( Error.c_str( ) );

        return 0;
    }

    Utilities::PrintInfo( "Tokens = {\n" );
    for ( size_t i = 0; i < Tokens.size( ); ++i ) {
        if ( i > 0 )
            Utilities::Print( ",\n" );

        Utilities::Print( std::vformat( "   {}", std::make_format_args( Tokens[ i ].Name ) ).c_str( ) );
    }
    Utilities::PrintInfo( "\n}\n" );

    Utilities::PrintInfo( "\nVariables = {\n" );
    for ( size_t i = 0; i < Interpreter.Variables.size( ); ++i ) {
        if ( i > 0 )
            Utilities::Print( ",\n" );

        Utilities::Print( std::vformat( "   {}", std::make_format_args( Interpreter.Variables[ i ] ) ).c_str( ) );
    }
    Utilities::PrintInfo( "\n}\n" );

    return 1;
}

std::map<std::string, uintptr_t> Commands;
int Untitled::Context::RunCommand( std::string cmd ) {
    std::map<std::string, uintptr_t> Commands;
    Commands[ "/clear" ] = reinterpret_cast< uintptr_t >( Utilities::ClearConsole );

    auto lowerCmd = Untitled::Utilities::ToLower( cmd );
    auto it = Commands.find( lowerCmd );

    if ( it != Commands.end( ) ) {
        CALL_FUNCTION( it->second );
        return 1;
    }

    return 1;
}