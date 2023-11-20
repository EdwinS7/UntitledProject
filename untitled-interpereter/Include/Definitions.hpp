#pragma once

enum TokenType {
    // Literal Types
    Number,
    Identifier,

    // Grouping & Operators
    OpenParen,
    CloseParen,
    BinaryOperator,
    Equals,

    // Keywords
    Let,
    Eof
};

struct Token {
    std::string Name;
    int Type;

    Token( ) = default;

    Token( std::string Name, int Type )
        : Name( Name ), Type( Type ) { }
};

struct LexerResponse {
    std::vector<Token> Tokens;
    std::vector<std::string> Formatted;

    LexerResponse( ) = default;

    LexerResponse( std::vector<Token> Tokens, std::vector<std::string> Formatted )
        : Tokens( Tokens ), Formatted( Formatted ) { }
};

struct InterpreterResponse {
    const char* Environment;
    int InstructionCount;
    int ExecutionLevel;

    std::vector<std::string> Formatted, Errors;

    InterpreterResponse( ) = default;

    InterpreterResponse( const char* Environment, int InstructionCount, int ExecutionLevel, std::vector<std::string> Formatted )
        : Environment( Environment ), InstructionCount( InstructionCount ), ExecutionLevel( ExecutionLevel ), Formatted( Formatted ) { }

    InterpreterResponse( const char* Environment, int InstructionCount, int ExecutionLevel, std::vector<std::string> Formatted, std::vector<std::string> Errors )
        : Environment( Environment ), InstructionCount( InstructionCount ), ExecutionLevel( ExecutionLevel ), Formatted( Formatted ), Errors( Errors ) { }
};