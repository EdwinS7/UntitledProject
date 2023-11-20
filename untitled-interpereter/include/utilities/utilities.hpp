#pragma once

#include "../../Common.hpp"

namespace Untitled::Utilities {
    void PrintVersion( );

    std::string ToLower( std::string str );
    std::string ToUpper( std::string str );

    FOLDER_CONTENT GetFolderContents( const std::string& Folder );

    int IsFirstLaunch( );
};