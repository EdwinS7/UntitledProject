#pragma once

#include "../../Common.hpp"

namespace Untitled::Lexer {
    LexerResponse Tokenize( const std::string& Source );

    int IsWhitespace( const std::string& Token );
    int IsInteger( const std::string& Token );
    int IsAlpha( const std::string& Token );
};