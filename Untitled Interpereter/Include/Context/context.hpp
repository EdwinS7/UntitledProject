#pragma once

#include "../../Common.hpp"

enum ExecutionLevel {
    RESTRICTED = 0, // File system, Http system and Memory access are disabled along with command line.
    DEFAULT,        // Everything is allowed except command line access. 
    UNRESTRICTED    // (WARNING) You can do anything.
};

namespace Untitled::Context {
    void Init( );

    int Execute( const char* Environment, const std::string& Source, int Level );

    void AddLog( int Level, std::string Message );
    void ClearLog( );
};