#include "context.hpp"

void c_ctx::init( ) {
    try {
        g_utilities->open_console( xorstr_( "rust-internal" ) );

        g_hooks->init( );

        printf( xorstr_( "done!\n" ) );
    }
    catch ( const std::exception& error ) {
        printf( xorstr_( "Caught an exception: %s\n" ), error.what( ) );
    }
}

BOOL APIENTRY DllMain( HMODULE module, DWORD reason, LPVOID reserved ) {
    if ( reason != DLL_PROCESS_ATTACH )
        return 0;

    std::jthread{ [ ]( ) {
        g_ctx->init( );
    } }.detach( );

    return 1;
}