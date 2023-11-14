#pragma once

enum CallstackCode {
    VAR = 0,
    PRINT,
    CLEARCONSOLE,

    CUSTOM
};

enum ErrorCode {
    UNDEFINED = 1,

};

enum class CommandType {
    Clear,
    Reset,
    ListCommands
};

enum VariableTypes {
    VARIABLE_TYPE_INT = 0,
    VARIABLE_TYPE_BOOL,
    VARIABLE_TYPE_STRING,

    VARIABLE_TYPE_UNDEFINED
};

struct Variable {
    std::variant<int, std::string> value;

    Variable( ) = default;

    Variable( int v )
        : value( v ) { }

    Variable( const std::string& v )
        : value( v ) { }
};

struct Token {
    std::string Name;
    bool Used = false;

    Token( ) = default;

    Token( std::string Name )

        : Name( Name ) { }

    Token( std::string Name, bool Used )
        : Name( Name ), Used( Used ) { }
};

struct Task {
    const char* Environment;
    std::string Source;

    Task( ) = default;

    Task( std::string Source )
        : Source( Source ) { }

    Task( const char* Environment, std::string Source )
        : Environment( Environment ), Source( Source ) { }
};

struct InterpreterResponse {
    const char* Environment;
    int InstructionCount;
    int ExecutionLevel;

    std::vector<std::string> Variables, Errors;

    InterpreterResponse( ) = default;

    InterpreterResponse( const char* Environment, int InstructionCount, int ExecutionLevel, std::vector<std::string> Variables )
        : Environment( Environment ), InstructionCount( InstructionCount ), ExecutionLevel( ExecutionLevel ), Variables( Variables ) { }

    InterpreterResponse( const char* Environment, int InstructionCount, int ExecutionLevel, std::vector<std::string> Variables, std::vector<std::string> Errors )
        : Environment( Environment ), InstructionCount( InstructionCount ), ExecutionLevel( ExecutionLevel ), Variables( Variables ), Errors( Errors ) { }
};