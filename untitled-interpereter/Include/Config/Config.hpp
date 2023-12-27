#pragma once

#include "../../Common.hpp"

class cConfig {
    struct Gui {
        TextEditor Editor;
        bool Open = true;
        int ListPanel = 1;
    };

    struct Backend {
        LexerResponse SavedLexer;
        InterpreterResponse SavedInterpreter;
        std::vector<std::pair<int, std::string>> Output;
    };
};

inline const auto Cfg = std::make_unique<cConfig>( );

namespace Untitled::Config {
    void Save( const char* Config );
    void Load( const char* Config );
    void Create( const char* Config );
    void Remove( const char* Config );

    void Init( );
};