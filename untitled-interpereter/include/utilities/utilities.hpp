#pragma once

#include "../include.hpp"

namespace Untitled::Utilities {
    void Print( const char* str );
    void PrintError( const char* str );
    void PrintWarning( const char* str );
    void PrintSuccess( const char* str );

    std::string ToLower( std::string str );
    std::string ToUpper( std::string str );
};