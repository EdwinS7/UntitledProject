#pragma once

enum CallstackCode {
    LET = 0,
    PRINT,
    CLEARCONSOLE
};

enum ErrorCode {
    UNDEFINED = 1,

};

struct Variable {
    std::variant<int, std::string> value;

    Variable( ) = default;
    Variable( int v ) : value( v ) { }
    Variable( const std::string& v ) : value( v ) { }
};

struct Token {
    std::string name;
    int error;

    Token( ) = default;
    Token( std::string name ) : name( name ) { }
    Token( std::string name, int error ) : name( name ), error( error ) { }
};