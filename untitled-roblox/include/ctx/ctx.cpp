#include "ctx.hpp"

void c_ctx::init( ) {
    script_context = g_rbx->get_script_context( );

    int identity = 8, script = 0;
    lua_state = g_hooks->get_state( script_context, &identity, &script );

    g_rbx->set_identity( lua_state, 8 );

    std::jthread{ [ ]( ) {
        g_rbx->teleport_handler( );
    } }.detach( );

    g_rbx->run_script( lua_state, ( lua_State* ) luaL_newstate( ), "printidentity()" );
    g_rbx->run_script( lua_state, ( lua_State* ) luaL_newstate( ), "print('Ready!')" );

    std::thread listenerThread( [ this ] ( ) {
        this->listener( );
    } );

    listenerThread.detach( );
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