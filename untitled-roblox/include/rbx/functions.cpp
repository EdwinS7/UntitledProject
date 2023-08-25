#include "rbx.hpp"

void c_rbx::set_identity( const uintptr_t& state, const uint8_t& identity ) {
    *reinterpret_cast<int8_t*>( reinterpret_cast< int8_t* >( *( uintptr_t* ) ( state + 72 ) + 24 ) ) = identity;
}

void c_rbx::push_global( uintptr_t rl, int fn, const char* name ) {
    g_hooks->r_lua_pushcclosure( rl, fn, 0, 0, 0 );
    g_hooks->r_lua_setfield( rl, -10002, name );
}

void c_rbx::pop_stack( uintptr_t state, uint8_t amount ) {
    *reinterpret_cast< uintptr_t* >( state + g_offsets->top ) -= 0x10 * amount;
}

void c_rbx::teleport_handler( ) {
    static bool teleported = false;

    while ( true ) {
        if ( !teleported ) {
            std::this_thread::sleep_for( std::chrono::milliseconds( 100 ) );

            int identity = 8, script = 0;
            if ( g_ctx->lua_state != g_hooks->get_state( g_ctx->script_context, &identity, &script ) ) {
                if ( !teleported ) {
                    teleported = true;
                    std::this_thread::sleep_for( std::chrono::milliseconds( 6000 ) );

                    g_offsets->init( );
                    g_ctx->lua_state = g_hooks->get_state( g_ctx->script_context, &identity, &script );
                    g_rbx->set_identity( g_ctx->lua_state, 8 );

                    g_rbx->run_script( g_ctx->lua_state, "Teleported!" );

                    std::this_thread::sleep_for( std::chrono::milliseconds( 1000 ) );
                    teleported = false;
                }
            }
        }
        else
            std::this_thread::sleep_for( std::chrono::milliseconds( 5000 ) );
    }
}

int set_clipboard( lua_State* L ) {
    std::size_t Length;
    const char* Setting = lua_tolstring( L, 1, &Length );

    if ( OpenClipboard( NULL ) ) {
        if ( EmptyClipboard( ) ) {
            HGLOBAL hglbCopy = GlobalAlloc( GMEM_FIXED, Length + 1 );

            if ( hglbCopy == NULL )
                CloseClipboard( );

            memcpy( hglbCopy, Setting, Length + 1 );

            if ( !SetClipboardData( CF_TEXT, hglbCopy ) ) {
                CloseClipboard( );
                GlobalFree( hglbCopy );
            }

            CloseClipboard( );
        }
    }

    return 0;
}

int get_clipboard( lua_State* L ) {
    std::size_t Length;
    const char* Setting = lua_tolstring( L, 1, &Length );

    if ( OpenClipboard( NULL ) ) {
        HANDLE Got = GetClipboardData( CF_TEXT );

        if ( Got == nullptr )
            CloseClipboard( );

        char* pszText = static_cast< char* >( GlobalLock( Got ) );

        if ( pszText == nullptr )
            CloseClipboard( );

        std::string text( pszText );

        GlobalUnlock( Got );
        CloseClipboard( );

        lua_pushstring( L, text.c_str( ) );
        
        return 1;
    }

    return 0;
}

int set_readonly( uintptr_t rl ) {
    lua_setreadonly( ( lua_State* ) rl, 1, lua_toboolean( ( lua_State* ) rl, 2 ) );
    return 0;
}

void c_rbx::register_functions( uintptr_t rl ) {
    push_global( rl, convert_closure( ( uintptr_t ) set_readonly ), "set_readonly" );
    push_global( rl, convert_closure( ( uintptr_t ) set_clipboard ), "set_clipboard" );
    push_global( rl, convert_closure( ( uintptr_t ) get_clipboard ), "get_clipboard" );
}