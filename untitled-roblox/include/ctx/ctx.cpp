#include "ctx.hpp"

void c_ctx::init( ) {
    g_offsets->init( );

    script_context = g_rbx->get_script_context( );
    lua_state = g_rbx->get_global_state( );

    g_rbx->set_identity( lua_state, 8 );

    std::jthread{ [ ] { g_rbx->teleport_handler( ); } }.detach( );

    g_rbx->run_script( lua_state, "printidentity()" );
    g_rbx->run_script( lua_state, "print('Ready!')" );

    std::jthread{ [ this ] { this->listener( ); } }.detach( );
}

void c_ctx::listener( ) {
    while ( true ) {

    }
}

BOOL APIENTRY DllMain( HMODULE module, DWORD reason, LPVOID reserved ) {
    if ( reason != DLL_PROCESS_ATTACH )
        return 0;

    DisableThreadLibraryCalls( module );

    std::jthread{ [ ]( ) {
        g_ctx->init( );
    } }.detach( );

    return 1;
}