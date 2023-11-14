#pragma once

#include "../Include.hpp"

namespace Untitled::Interpreter {
    InterpreterResponse Interpret( const char* Environment, int Level, std::vector<Token> Tokens );
    int Register( std::map<const char*, std::uintptr_t>* Functions );
    int Hash( std::string Call );
};