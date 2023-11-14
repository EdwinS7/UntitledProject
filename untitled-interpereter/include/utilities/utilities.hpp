#pragma once

#include "../include.hpp"

namespace Untitled::Utilities {
    void Print( const char* str );
    void PrintInfo( const char* str );
    void PrintError( const char* str );
    void PrintWarning( const char* str );
    void PrintSuccess( const char* str );

    void ClearConsole( const char* reason = nullptr );

    void PrintVersion( );

    void CommandExecute( const char* command );

    std::string ToLower( std::string str );
    std::string ToUpper( std::string str );
    bool IsNumber( std::string str );

    FOLDER_CONTENT GetFolderContents( std::string folder );

    int IsFirstLaunch( );
};