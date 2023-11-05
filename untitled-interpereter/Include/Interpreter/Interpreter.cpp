#include "Interpreter.hpp"

std::map<const char*, std::uintptr_t*> GlobalFunctions;

int Untitled::Interpreter::Register( std::map<const char*, std::uintptr_t*>* Functions ) {
    for ( auto& Func : *Functions )
        GlobalFunctions[ Func.first ] = Func.second;

    return 1;
}

int Untitled::Interpreter::Interpret( std::vector<Token> Tokens, int level ) {
    for ( int i = 0; i < Tokens.size( ); i++ ) {
        auto GetFunctionParamater = [ &Tokens, i ] ( int parameter ) -> std::string {
            return Tokens[ i + parameter ].name;
        };

        switch ( Hash( Tokens[ i ].name ) ) {
        case LET:

            break;
        case PRINT:
            
            break;
        case CLEARCONSOLE:
            Untitled::Utilities::ClearConsole( );
            break;
        default:
            Tokens[ i ].error = ErrorCode::UNDEFINED;
            break;
        }
    }

    return 1;
}

int Untitled::Interpreter::Hash( std::string call ) {
	if ( call == "Var" ) return LET;

    if ( call == "Print" ) return PRINT;
    if ( call == "ClearConsole" ) return CLEARCONSOLE;
}