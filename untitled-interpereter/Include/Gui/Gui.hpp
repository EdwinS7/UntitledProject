#pragma once

#include "../../Common.hpp"

#define WINDOW_FLAGS ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoSavedSettings | ImGuiWindowFlags_MenuBar
#define INACTIVE_WINDOW_FLAGS ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoSavedSettings | ImGuiWindowFlags_NoNav | ImGuiWindowFlags_MenuBar

struct sConfig {
    struct sGui {
        TextEditor Editor;
        bool Open = true;
        int ListPanel = 1;
        int OutputPanel = 0;
    } Gui;

    struct sBackend {
        LexerResponse SavedLexer;
        InterpreterResponse SavedInterpreter;
        std::vector<std::pair<int, std::string>> Output;
    } Backend;
};

extern sConfig Cfg;

namespace Untitled::Gui {
    int Render( );
};