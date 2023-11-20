#pragma once

#include "../../Common.hpp"

namespace Untitled::Interpreter {
    InterpreterResponse Interpret( const char* Environment, LexerResponse Tokens );
    int Register( std::map<const char*, std::uintptr_t>* Functions );

    void Release( );
};