#pragma once

#include "../../Common.hpp"

namespace Untitled::Parser {
	ParserResponse ProduceAST( const std::string& Source );

	Token Expect( int TokenID, int Type, const std::string& Error );
	Token Eat( int TokenID );
	Token At( int TokenID );

	bool NotEof( Token Token );

    Statement* ParseStatement( int TokenID );
    Expression* ParseExpression( int TokenID );
    Expression* ParseAdditiveExpression( int TokenID );
    Expression* ParseMultiplicativeExpression( int TokenID );
    Expression* ParsePrimaryExpression( int TokenID );
};