#pragma once

#include "../../Common.hpp"

enum ExecutionLevel {
    RESTRICTED = 0, // File system, Http system and Memory access are disabled along with command line.
    DEFAULT,        // Everything is allowed except command line access. 
    UNRESTRICTED    // (WARNING) You can do anything.
};

#define WINDOW_FLAGS ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoSavedSettings
#define INACTIVE_WINDOW_FLAGS ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoSavedSettings | ImGuiWindowFlags_NoNav

namespace Untitled::Context {
    void Init( );
    void RenderWindow( );

    int RunScript( const char* Environment, const std::string& Source, int Level );
    void UpdateLists( InterpreterResponse Interpreter, LexerResponse Lexer );

    void AddLog( int Level, std::string Message );
    void ClearLog( );
};