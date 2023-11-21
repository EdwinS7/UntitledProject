#include "Parser.hpp"

LexerResponse Response;

ParserResponse Untitled::Parser::ProduceAST( const std::string& Source ) {
    Response = Lexer::Tokenize( Source );

    ParserResponse ParserResponse = { NodeType::Program, {} };

    for ( int i = 0; i < Response.Tokens.size( ); i++ )
        ParserResponse.Body.push_back( ParseStatement( i ) );

    return ParserResponse;
}

Token Untitled::Parser::Expect( int TokenID, int Type, const std::string& Error ) {
    Token Prev = Response.Tokens[ TokenID ];
    if ( Prev.Type != Type ) {
        Context::AddLog( 3, "Parser type mismatch" );
        return Token( );
    }

    Response.Tokens.erase( Response.Tokens.begin( ) );
    return Prev;
}

Token Untitled::Parser::Eat( int TokenID ) {
    Token Prev = Response.Tokens[ TokenID ];
    Response.Tokens.erase( Response.Tokens.begin( ) );
    return Prev;
}

Token Untitled::Parser::At( int TokenID ) {
    return Response.Tokens[ TokenID ];
}

bool Untitled::Parser::NotEof( Token Token ) {
	return false;
}

Statement* Untitled::Parser::ParseStatement( int TokenID ) {
    return ParseExpression( TokenID );
}

Expression* Untitled::Parser::ParseExpression( int TokenID ) {
    return ParseAdditiveExpression( TokenID );
}

Expression* Untitled::Parser::ParseAdditiveExpression( int TokenID ) {
    Expression* Left = ParseMultiplicativeExpression( TokenID );

    while ( At( TokenID ).Name == "+" || At( TokenID ).Name == "-" ) {
        Expression* Right = ParseMultiplicativeExpression( TokenID );

        Left = new BinaryExpression{ NodeType::BinaryExpr, Left, Right, Eat( TokenID ).Name.c_str( ) };
    }

    return Left;
}

Expression* Untitled::Parser::ParseMultiplicativeExpression( int TokenID ) {
    Expression* Left = ParsePrimaryExpression( TokenID );

    while ( At( TokenID ).Name == "/" || At( TokenID ).Name == "*" || At( TokenID ).Name == "%" ) {
        Expression* Right = ParsePrimaryExpression( TokenID );

        Left = new BinaryExpression{ NodeType::BinaryExpr, Left, Right, Eat( TokenID ).Name.c_str( ) };
    }

    return Left;
}

Expression* Untitled::Parser::ParsePrimaryExpression( int TokenID ) {
    auto Token = At( TokenID );

    switch ( Token.Type ) {
        case TokenType::Identifier:
            return new VariableExpression{ NodeType::Identifier, Token.Name };
        case TokenType::Number:
            return new NumericLiteral{ NodeType::NumericLiteral, std::stof( Token.Name ) };
        case TokenType::OpenParen:
        {
            Eat( TokenID ); // eat the opening paren
            Expression* Value = ParseExpression( TokenID );
            Expect( TokenID, TokenType::CloseParen, "Unexpected token found inside parenthesized expression. Expected closing parenthesis." );
            return Value;
        }
    }
}