#pragma once

#include "../../include.hpp"

#define MAKE_FUNCTION( Name, Functions, Function ) Functions->at( Name ) = reinterpret_cast< std::uintptr_t* >( Utilities::Print );

namespace Untitled::Interpreter::Functions {
    int RegisterFunctions( std::map<const char*, std::uintptr_t*>* Functions ) {
        MAKE_FUNCTION( "Print", Functions, Utilities::Print );
        MAKE_FUNCTION( "PrintError", Functions, Utilities::PrintError );
        MAKE_FUNCTION( "PrintWarning", Functions, Utilities::PrintWarning );
        MAKE_FUNCTION( "PrintSuccess", Functions, Utilities::PrintSuccess );
        MAKE_FUNCTION( "ClearConsole", Functions, Utilities::ClearConsole );

        return 1;
    }
};