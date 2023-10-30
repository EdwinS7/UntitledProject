#include "utilities.hpp"

void Untitled::Utilities::Print(const char* str ) {
    std::printf( str );
}

void Untitled::Utilities::PrintError(const char* str ) {
    std::printf( "\x1b[31m" );
    std::printf( str );
    std::printf( "\x1b[0m" );
}

void Untitled::Utilities::PrintWarning( const char* str ) {
    std::printf( "\x1b[94m" );
    std::printf( str );
    std::printf( "\x1b[0m" );
}

void Untitled::Utilities::PrintSuccess( const char* str ) {
    std::printf( "\x1b[92m" );
    std::printf( str );
    std::printf( "\x1b[0m" );
}

std::string Untitled::Utilities::ToLower( std::string str ) {
    std::string result = str;

    for ( char& c : result )
        c = std::tolower( c );

    return result;
}

std::string Untitled::Utilities::ToUpper( std::string str ) {
    std::string result = str;

    for ( char& c : result )
        c = std::toupper( c );

    return result;
}