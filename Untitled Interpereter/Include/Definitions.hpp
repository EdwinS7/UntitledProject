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

enum class NodeType {
    Program,
    NumericLiteral,
    Identifier,
    BinaryExpr
};

class Statement {
public:
    NodeType Type;
};

class ParserResponse : public Statement {
public:
    std::vector<Statement*> Body;
};

class Expression : public Statement {};

class BinaryExpression : public Expression {
public:
    NodeType Type;
    Expression* Left;
    Expression* Right;
    std::string Operator; // Needs to be of type BinaryOperator.

    BinaryExpression( ) = default;

    BinaryExpression( NodeType Type, Expression* Left, Expression* Right, std::string Operator )
        : Type( Type ), Left( Left ), Right( Right ), Operator( Operator ) { }
};

class VariableExpression : public Expression {
public:
    NodeType Type;
    std::string Symbol;

    VariableExpression( ) = default;

    VariableExpression( NodeType Type, std::string Symbol )
        : Type( Type ), Symbol( Symbol ) { }
};

class NumericLiteral : public Expression {
public:
    NodeType Type;
    float Value;

    NumericLiteral( ) = default;

    NumericLiteral( NodeType Type, float Value )
        : Type( Type ), Value( Value ) { }
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