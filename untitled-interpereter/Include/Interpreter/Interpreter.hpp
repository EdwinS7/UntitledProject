#pragma once

#include "../Include.hpp"

namespace Untitled::Interpreter {
    int Register( std::map<const char*, std::uintptr_t*>* Functions );
    int Interpret( std::vector<Token> Tokens, int Level );
    int Hash( std::string Call );
};