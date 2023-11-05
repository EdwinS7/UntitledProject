#include "context.hpp"

void Untitled::Context::Init( ) {
    std::map<const char*, std::uintptr_t*> Functions;

    Interpreter::Functions::RegisterFunctions( &Functions );
    Interpreter::Register( &Functions );
}

int Untitled::Context::RunScript( const char* env, std::string src, int level ) {
    std::vector<Token> Tokens = Tokenizer::Tokenize( src );

    // Debug logs from tokenizer
    Utilities::PrintWarning( "[DEBUG]\n" );

    if ( !PrintTokenTable( Tokens ) ) {
        Utilities::PrintError( "Tokens are empty!\n" );
        return 0;
    }

    // Output
    Utilities::PrintWarning( "\n[OUTPUT]\n" );

    if ( !Interpreter::Interpret( Tokens, level ) ) {
        Utilities::PrintError( "Failed to Interpret tokens!\n" );
        return 0;
    }

    return 1;
}

int Untitled::Context::PrintTokenTable( std::vector<Token> Tokens ) {
    printf( "Tokens = {\n" );
    for ( size_t i = 0; i < Tokens.size( ); ++i ) {
        if ( i > 0 )
            printf( ",\n" );

        printf( "   %s", Tokens[ i ].name.c_str( ) );
    }
    printf( "\n}\n" );

    return 1;
}

int Untitled::Context::IsFirstLaunch( ) {
    HKEY HKey;

    if ( RegOpenKeyEx( HKEY_CURRENT_USER, "Software//UntitledAPI", 0, KEY_READ, &HKey ) != ERROR_SUCCESS ) {
        if ( RegCreateKey( HKEY_CURRENT_USER, "Software//UntitledAPI", &HKey ) == ERROR_SUCCESS ) {
            DWORD value = 1;
            RegSetValueEx( HKey, "IsFirstLaunch", 0, REG_DWORD, ( BYTE* ) &value, sizeof( value ) );
            RegCloseKey( HKey );
        }

        return 1;
    }
    else {
        RegCloseKey( HKey );
        return 0;
    }
}