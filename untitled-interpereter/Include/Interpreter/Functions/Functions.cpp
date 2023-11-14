#include "Functions.hpp"

#define MAKE_FUNCTION(Name, Functions, Function) \
    do { \
        (*(Functions))[Name] = reinterpret_cast<uintptr_t>(Function); \
    } while (false)

int Untitled::Interpreter::Functions::RegisterFunctions( std::map<const char*, std::uintptr_t>* Functions ) {
    MAKE_FUNCTION( "Print", Functions, Utilities::Print );
    MAKE_FUNCTION( "PrintError", Functions, Utilities::PrintError );
    MAKE_FUNCTION( "PrintInfo", Functions, Utilities::PrintInfo );
    MAKE_FUNCTION( "PrintSuccess", Functions, Utilities::PrintSuccess );
    MAKE_FUNCTION( "ClearConsole", Functions, Utilities::ClearConsole );
    MAKE_FUNCTION( "PrintVersion", Functions, Utilities::PrintVersion );

    return 1;
}