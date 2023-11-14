#pragma once

#include "../../include.hpp"

namespace Untitled::Memory {
    int GetType( std::string ref );
    bool IsVariable( std::string ref );

    std::map<std::string, int> GetIntegers( );
    std::map<std::string, bool> GetBooleans( );
    std::map<std::string, std::string> GetStrings( );

    int GetInteger( std::string ref );
    bool GetBoolean( std::string ref );
    std::string GetString( std::string ref );
    
    void Set( std::string ref, int value );
    void Set( std::string ref, bool value );
    void Set( std::string ref, std::string value );
};