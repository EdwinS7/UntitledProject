#include "Tokenizer.hpp"

std::vector<Token> Untitled::Tokenizer::Tokenize( std::string& src ) {
    //std::vector<Token> Tokens; // Chached tokens
    //std::size_t Start; // Used for data points

    //for ( int i = 0; i < src.size( ); i++ ) {
    //    if ( std::isspace( src[ i ] ) || !std::isalpha( src[ i ] ) )
    //        continue; // Skip whitespace & non alphabetical letters.

    //    switch ( src[ i ] ) {
    //    case '"': // QUOTE / STRINGS
    //        Start = i;

    //        while ( i < src.size( ) && src[ i ] != '"' )
    //            i++;

    //        if ( i < src.size( ) ) {
    //            Tokens.push_back( Token( src.substr( Start + 1, i - Start - 1 ) ) );
    //            i++;
    //        }

    //        break;
    //    case '_':
    //        Start = i;

    //        while ( i < src.size( ) && ( std::isalnum( src[ i ] ) || src[ i ] == '_' ) )
    //            i++;

    //        Tokens.push_back( Token( src.substr( Start, i - Start ) ) );

    //        break;
    //    }
    //}

    //return Tokens;

    std::vector<Token> Tokens;
    std::size_t i = 0;

    while ( i < src.size( ) ) {
        if ( std::isspace( src[ i ] ) )
            i++; // Skip whitespace
        else if ( src[ i ] == '"' ) {
            // Handle double-quoted strings
            std::size_t start = i;
            i++;

            while ( i < src.size( ) && src[ i ] != '"' )
                i++;

            if ( i < src.size( ) ) {
                Tokens.push_back( Token( src.substr( start + 1, i - start - 1 ) ) );
                i++; // Skip the closing quote
            }
        }
        else if ( std::isdigit( src[ i ] ) || src[ i ] == '-' || ( src[ i ] == '.' && std::isdigit( src[ i + 1 ] ) ) ) {
            // Handle numbers
            std::size_t start = i;

            while ( i < src.size( ) && ( std::isdigit( src[ i ] ) || src[ i ] == '.' ) )
                i++;

            Tokens.push_back( Token( src.substr( start, i - start ) ) );
        }
        else if ( std::isalpha( src[ i ] ) || src[ i ] == '_' ) {
            // Handle identifiers
            std::size_t start = i;

            while ( i < src.size( ) && ( std::isalnum( src[ i ] ) || src[ i ] == '_' ) )
                i++;

            Tokens.push_back( Token( src.substr( start, i - start ) ) );
        }
        else {
            // Handle other characters or unknown tokens
            i++;
        }
    }

    return Tokens;
}