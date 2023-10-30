#pragma once

#include "../include.hpp"

enum CallstackCode {
    LET = 0,
    PRINT
};

struct Variable {
    std::variant<int, std::string> value;

    Variable( ) = default;
    Variable( int v ) : value( v ) {}
    Variable( const std::string& v ) : value( v ) {}
};

namespace Untitled::Interpereter {
    std::vector<std::string> Tokenize( std::string& src );
    int RunScript( std::string src );
    int Hash( std::string call );
    
};