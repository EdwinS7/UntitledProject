#pragma once

#include "../include.hpp"

namespace Untitled::Context {
    void Init( );

    int RunScript( const char* env, std::string src, int level );
    int PrintTokenTable( std::vector<Token> Tokens );
    int IsFirstLaunch( );
};