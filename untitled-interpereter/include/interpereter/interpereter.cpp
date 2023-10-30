#include "interpereter.hpp"

std::vector<std::string> Untitled::Interpereter::Tokenize( std::string& src ) {
    std::vector<std::string> Tokens;
    std::size_t i = 0;

    while ( i < src.size( ) ) {
        if ( std::isspace( src[ i ] ) )
            i++;

        else if ( src[ i ] == '"' ) {
            std::size_t start = i;
            i++;

            while ( i < src.size( ) && ( src[ i ] != '"' ) )
                i++;

            if ( i < src.size( ) ) {
                i++;
                Tokens.push_back( src.substr( start + 1, i - start - 2 ) );
            }
            else {
                Tokens.push_back( "Unbalanced double quotes" );
                break;
            }
        }
        else if ( std::isdigit( src[ i ] ) || src[ i ] == '-' || ( src[ i ] == '.' && std::isdigit( src[ i + 1 ] ) ) ) {
            std::size_t start = i;

            while ( i < src.size( ) && ( std::isdigit( src[ i ] ) || src[ i ] == '.' ) )
                i++;

            Tokens.push_back( src.substr( start, i - start ) );
        }
        else if ( std::isalpha( src[ i ] ) || src[ i ] == '_' ) {
            std::size_t start = i;

            while ( i < src.size( ) && ( std::isalnum( src[ i ] ) || src[ i ] == '_' ) )
                i++;

            Tokens.push_back( src.substr( start, i - start ) );
        }
        else
            i++;
    }

    return Tokens;
}

int Untitled::Interpereter::RunScript( std::string src ) {
    std::vector<std::pair<std::string, bool>> BoolVars;
    std::vector<std::pair<std::string, int>> IntegerVars;
    std::vector<std::pair<std::string, std::string>> StringVars;

    std::vector<std::string> Tokens = Tokenize( src );

    for ( int i = 0; i < Tokens.size( ); i++ ) {
        auto GetFunctionParamater = [ &Tokens, i ] ( int parameter ) -> std::string {
            return Tokens[ i + parameter ];
        };

        switch ( Hash( Tokens[ i ] ) ) {
        case LET:
            try {
                bool isVariable = false;

                for ( auto String : StringVars ) {
                    if ( GetFunctionParamater( 2 ) == std::get<0>( String ) ) {
                        IntegerVars.push_back( std::make_pair( GetFunctionParamater( 0 ), std::stoi( std::get<1>( String ) ) ) );
                        isVariable = true;
                        break;
                    }
                }

                if ( !isVariable ) {
                    IntegerVars.push_back( std::make_pair( GetFunctionParamater( 0 ), std::stoi( GetFunctionParamater( 1 ) ) ) );
                }
            }
            catch ( const std::invalid_argument& err ) {
                try {
                    StringVars.push_back( std::make_pair( GetFunctionParamater( 1 ), GetFunctionParamater( 2 ) ) );
                }
                catch ( const std::invalid_argument& err ) {
                    Untitled::Utilities::Print( err.what( ) );
                }
            }

            break;
        case PRINT:
            try {
                bool IsVariable = false;
                for ( auto String : StringVars ) {
                    if ( GetFunctionParamater( 1 ) == std::get<0>( String ) ) {
                        Untitled::Utilities::Print( std::vformat( "{}\n", std::make_format_args( std::get<1>( String ) ) ).c_str( ) );
                        IsVariable = true;
                        break;
                    }
                }

                if ( !IsVariable )
                    Untitled::Utilities::Print( std::vformat( "{}\n", std::make_format_args( GetFunctionParamater( 1 ) ) ).c_str( ) );
            }
            catch ( const std::invalid_argument& err ) {
                try {
                    Untitled::Utilities::Print( std::vformat( "{}\n", std::make_format_args( std::stoi( GetFunctionParamater( 1 ) ) ) ).c_str( ) );
                }
                catch ( const std::invalid_argument& err ) {
                    Untitled::Utilities::Print( err.what( ) );
                }
            }

            break;
        }
    }

    /*for ( auto& var : BoolVars )
        Untitled::Utilities::Print( std::vformat( "{}: {}\n", std::make_format_args( std::get<0>( var ), std::get<1>( var ) ) ).c_str( ) );

    for ( auto& var : IntegerVars )
        Untitled::Utilities::Print( std::vformat( "{}: {}\n", std::make_format_args( std::get<0>( var ), std::get<1>( var ) ) ).c_str( ) );

    for ( auto& var : StringVars )
        Untitled::Utilities::Print( std::vformat( "{}: {}\n", std::make_format_args( std::get<0>( var ), std::get<1>( var ) ) ).c_str( ) );*/

    /*for ( auto& Token : Tokens )
        Untitled::Utilities::Print( std::vformat( "{}\n", std::make_format_args( Token.c_str( ) ) ).c_str( ) );*/

    return 1;
}

int Untitled::Interpereter::Hash( std::string call ) {
	if ( Untitled::Utilities::ToUpper( call ) == "LET" ) return LET;
    if ( Untitled::Utilities::ToUpper( call ) == "PRINT" ) return PRINT;
}