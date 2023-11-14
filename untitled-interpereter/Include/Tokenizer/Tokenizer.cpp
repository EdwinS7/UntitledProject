#include "Tokenizer.hpp"

std::vector<Token> Untitled::Tokenizer::Tokenize( std::string& src ) {
    std::vector<Token> Tokens;
    std::size_t Start;

    for ( std::size_t i = 0; i < src.size( ); ) {
        if ( src.compare( i, 2, "(!)" ) == 0 ) { // Skip Comments
            while ( i < src.size( ) && src[ i ] != '\n' )
                i++;

            continue;
        }

        if ( std::isspace( src[ i ] ) ) // Skip whitespace
            i++;
        else if ( src[ i ] == '"' ) { // Handle Double-Quoted Strings
            Start = i++;

            Tokens.push_back( Token( src.substr( Start, i - Start ) ) );

            while ( i < src.size( ) && src[ i ] != '"' )
                i++;

            if ( i < src.size( ) ) {
                Tokens.push_back( Token( src.substr( Start + 1, i - Start - 1 ) ) );
                i++;
            }

            Start = i++;

            Tokens.push_back( Token( src.substr( Start - 1, i - Start ) ) );
        }
        else if ( std::isdigit( src[ i ] ) || src[ i ] == '-' || ( src[ i ] == '.' && std::isdigit( src[ i + 1 ] ) ) ) { // Handle Numbers
            Start = i;

            while ( i < src.size( ) && ( std::isdigit( src[ i ] ) || src[ i ] == '.' ) )
                i++;

            Tokens.push_back( Token( src.substr( Start, i - Start ) ) );
        }
        else if ( std::isalpha( src[ i ] ) || src[ i ] == '_' ) { // Handle Identifiers
            Start = i;

            while ( i < src.size( ) && ( std::isalnum( src[ i ] ) || src[ i ] == '_' ) )
                i++;

            Tokens.push_back( Token( src.substr( Start, i - Start ) ) );
        }
        else
            i++;
    }

    return Tokens;
}