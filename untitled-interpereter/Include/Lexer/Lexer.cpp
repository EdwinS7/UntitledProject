#include "Lexer.hpp"

std::unordered_map<std::string, int> Keywords = {
    { "Let", TokenType::Let }
};

LexerResponse Untitled::Lexer::Tokenize( const std::string& Source ) {
    auto Lexer = LexerResponse( );

    int Start = 0;
    for ( int i = 0; i < Source.size( ); i++ ) {
        if ( Source[ i ] == '(' )
            Lexer.Tokens.push_back( Token( std::string( 1, Source[ i ] ), TokenType::OpenParen ) );
        else if ( Source[ i ] == ')' )
            Lexer.Tokens.push_back( Token( std::string( 1, Source[ i ] ), TokenType::CloseParen ) );
        else if ( Source[ i ] == '+' || Source[ i ] == '-' || Source[ i ] == '*' || Source[ i ] == '/' )
            Lexer.Tokens.push_back( Token( std::string( 1, Source[ i ] ), TokenType::BinaryOperator ) );
        else if ( Source[ i ] == '=' )
            Lexer.Tokens.push_back( Token( std::string( 1, Source[ i ] ), TokenType::Equals ) );
        else {
            // Handle multichar tokens
            Start = i;

            if ( IsWhitespace( std::string( 1, Source[ i ] ) ) )
                continue;
            else if ( IsInteger( std::string( 1, Source[ i ] ) ) ) {
                while ( i < Source.size( ) && ( std::isdigit( Source[ i ] ) || Source[ i ] == '.' ) )
                    i++;

                Lexer.Tokens.push_back( Token( Source.substr( Start, i - Start ), TokenType::Number ) );
            }
            else if ( IsAlpha( std::string( 1, Source[ i ] ) ) ) {
                while ( i < Source.size( ) && ( std::isalnum( Source[ i ] ) || Source[ i ] == '_' ) )
                    i++;

                auto sToken = Source.substr( Start, i - Start );

                auto keywordIt = Keywords.find( sToken );
                if ( keywordIt != Keywords.end( ) )
                    Lexer.Tokens.push_back( Token( sToken, keywordIt->second ) );
                else
                    Lexer.Tokens.push_back( Token( sToken, TokenType::Identifier ) );
            }
            else
                Context::AddLog( 3, ( "Unrecognized character found in source: ", std::string( 1, Source[ i ] ) ) );
        }
    }

    Lexer.Tokens.push_back( Token("EndOfFile", TokenType::Eof ) );

    for ( const auto& Token : Lexer.Tokens )
        Lexer.Formatted.push_back( "{ name: \"" + Token.Name + "\", type: " + std::to_string( Token.Type ) + " }" );

    return Lexer;
}

int Untitled::Lexer::IsWhitespace( const std::string& Token ) {
    return std::all_of( Token.begin( ), Token.end( ), [ ] ( unsigned char c ) { return std::isspace( c ); } );
}

int Untitled::Lexer::IsInteger( const std::string& Token ) {
    return !Token.empty( ) && std::all_of( Token.begin( ), Token.end( ), [ ] ( unsigned char c ) { return std::isdigit( c ); } );
}

int Untitled::Lexer::IsAlpha( const std::string& Token ) {
    return !Token.empty( ) && std::all_of( Token.begin( ), Token.end( ), [ ] ( unsigned char c ) { return std::isalpha( c ); } );
}