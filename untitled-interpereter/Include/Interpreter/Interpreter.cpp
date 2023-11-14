#include "Interpreter.hpp"
#include <typeindex>

std::uintptr_t CustomFunctionPointer;
std::map<const char*, std::uintptr_t> GlobalFunctions;

#define VAR_THIS 0
#define VAR_NAME 1
#define VAR_INT 2
#define VAR_BOOL 2
#define VAR_STRING 3

InterpreterResponse Untitled::Interpreter::Interpret( const char* Environment, int Level, std::vector<Token> Tokens ) {
    std::vector<std::string> Variables, Errors;
    bool ShouldContinue = false;

    for ( int i = 0; i < Tokens.size( ); i++ ) {
        auto Get = [ &Tokens, i ] ( int parameter ) -> std::string {
            Tokens[ i + parameter ].Used = true;
            return Tokens[ i + parameter ].Name;
        };

        switch ( Hash( Tokens[ i ].Name ) ) {
        case VAR:
            /*if ( i + 3 > Tokens.size( ) ) {
                Errors.push_back( std::vformat( "Error in VAR instruction: \033[91mNot enough arguments.\033[0m\n", std::make_format_args( Tokens[ i ].Name ) ) );
                continue;
            }*/

            if ( Memory::GetType( Get( VAR_INT ) ) == VARIABLE_TYPE_INT )
                Memory::Set( Get( VAR_NAME ), std::stoi( Get( VAR_INT ) ) );

            else if ( Memory::GetType( Get( VAR_BOOL ) ) == VARIABLE_TYPE_BOOL ) {
                if ( Utilities::ToLower( Get( 2 ) ) == "true" || Utilities::ToLower( Get( 2 ) ) == "false" )
                    Memory::Set( Get( VAR_NAME ), true );
                else
                    Memory::Set( Get( VAR_NAME ), false );
            }
            else if ( Memory::GetType( Get( VAR_STRING ) ) == VARIABLE_TYPE_STRING )
                Memory::Set( Get( VAR_NAME ), Get( VAR_STRING ) );

            break;
        case PRINT:
            if ( i + 1 > Tokens.size( ) ) {
                continue;
            }

            try {
                Utilities::Print( Memory::GetString( Get( VAR_NAME ) ).c_str( ) );
            }
            catch ( const std::invalid_argument& err ) {
                try {
                    Utilities::Print( std::to_string( Memory::GetInteger( Get( VAR_NAME ) ) ).c_str( ) );
                }
                catch ( const std::invalid_argument& err ) {
                    try {
                        Utilities::Print( std::to_string( Memory::GetBoolean( Get( VAR_NAME ) ) ).c_str( ) );
                    }
                    catch ( const std::invalid_argument& err ) {}
                }
            }

            break;
        case CLEARCONSOLE:
            Untitled::Utilities::ClearConsole( );
            break;
        case CUSTOM:
            CALL_FUNCTION( CustomFunctionPointer );
            break;
        default:
            std::vector<std::string> WhitelistedChars = { "\"", "-", "+", "/", "*", "=", "<", ">", "?" };

            for ( auto& Char : WhitelistedChars ) {
                if ( Utilities::ToLower( Tokens[ i ].Name ) == Char ) {
                    ShouldContinue = true;
                    break;
                }
            }

            if ( ShouldContinue ) {
                ShouldContinue = false;
                continue;
            }

            // Make sure its not a string or boolean.
            if ( Tokens[ i - 1 ].Name == "\"" && Tokens[ i + 1 ].Name == "\"" || Utilities::ToLower( Tokens[ i ].Name ) == "true" || Utilities::ToLower( Tokens[ i ].Name ) == "false" || Utilities::IsNumber( Tokens[ i ].Name ) )
                continue;

            if ( !Tokens[ i ].Used )
                Errors.push_back( std::vformat( "Unused instruction \033[91m{}\033[0m was found!\n", std::make_format_args( Tokens[ i ].Name ) ) );
            else if ( !Memory::IsVariable( Get( VAR_THIS ) ) )
                Errors.push_back( std::vformat( "Unknown instruction \033[91m{}\033[0m was called! Perhaps it's undefined!\n", std::make_format_args( Tokens[ i ].Name ) ) );

            break;
        }
    }

    for ( auto& Variable : Memory::GetIntegers( ) )
        Variables.push_back( std::vformat( "( Integer ) {} = {}", std::make_format_args( Variable.first, Variable.second ) ) );

    for ( auto& Variable : Memory::GetBooleans( ) )
        Variables.push_back( std::vformat( "( Boolean ) {} = {}", std::make_format_args( Variable.first, Variable.second ) ) );

    for ( auto& Variable : Memory::GetStrings( ) )
        Variables.push_back( std::vformat( "( String ) {} = '{}'", std::make_format_args( Variable.first, Variable.second ) ) );

    return InterpreterResponse( Environment, Tokens.size( ), Level, Variables, Errors );
}

int Untitled::Interpreter::Register( std::map<const char*, std::uintptr_t>* Functions ) {
    //Utilities::PrintInfo( "Registered Functions: {\n" );

    for ( auto& Function : *Functions ) {
        //Utilities::Print( std::vformat( "   {}\n", std::make_format_args( Function.first ) ).c_str( ) );
        GlobalFunctions[ Function.first ] = Function.second;
    }

    //Utilities::Print( "}\n\n" );

    return 1;
}

int Untitled::Interpreter::Hash( std::string call ) {
	if ( call == "Var" ) return CallstackCode::VAR;

    if ( call == "Print" ) return CallstackCode::PRINT;
    if ( call == "ClearConsole" ) return CallstackCode::CLEARCONSOLE;

    // Custom Functions.
    for ( auto& Function : GlobalFunctions ) {
        if ( call == Function.first ) {
            CustomFunctionPointer = Function.second;
            return CallstackCode::CUSTOM;
        }
    }
}