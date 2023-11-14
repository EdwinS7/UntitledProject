#include "context.hpp"

void c_ctx::init( ) {
    try {
        // @note: Wait for last module loaded!
        while ( !GetModuleHandleA( "ntdll.dll" ) )
            std::this_thread::sleep_for( 1s );

        // @note: Allocate a console.
        if ( !AllocConsole( ) )
            return;

        // @note: fix in/out messages.
        FILE* fp = nullptr;
        freopen_s( &fp, "CONOUT$", "w", stdout );

        printf( "Allocating Console...\n" );
        printf( "Redirecting in/out put...\n" );

        // @note: run per frame ( run this under a function that is called 1x per frame )
        std::jthread{ [ ] {
            while ( true ) {
                g_memory->write_memory( g_memory->key_valid, 1 );
                g_memory->write_memory( , 0 );
            }
        } }.detach( );
    }
    catch ( const std::exception& error ) {
        printf( "Caught an exception: %s\n", error.what( ) );
    }
    
    FreeConsole( );
}

BOOL APIENTRY DllMain( HMODULE module, DWORD reason, LPVOID reserved ) {
    if ( reason != DLL_PROCESS_ATTACH )
        return 0;

    std::jthread{ [ ] {
        g_ctx->init( );
    } }.detach( );

    return 1;
}