#pragma once

#include "../include.hpp"

namespace Untitled::Context {
    void Init( );

    int RunScript( const char* env, std::string src, int level );
    int RunCommand( std::string Cmd );
};