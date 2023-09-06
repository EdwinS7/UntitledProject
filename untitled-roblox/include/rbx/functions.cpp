#include "rbx.hpp"

void c_rbx::set_identity( const uint8_t& identity ) {
    *reinterpret_cast<int8_t*>( reinterpret_cast< int8_t* >( *( uintptr_t* ) ( g_offsets->lua_state + g_offsets->lua::extra_space ) + g_offsets->lua::identity ) ) = identity;
}

int c_rbx::get_identity( ) {
    return *reinterpret_cast< int8_t* >( reinterpret_cast< int8_t* >( *( uintptr_t* ) ( g_offsets->lua_state + g_offsets->lua::extra_space ) + g_offsets->lua::identity ) );
}

void c_rbx::pop_stack( uint8_t amount ) {
    *reinterpret_cast< uintptr_t* >( g_offsets->lua_state + g_offsets->lua::top ) -= 0x10 * amount;
}

int demo_func( uintptr_t rl ) {
    g_hooks->print( 0, "print('crazy shit right?')" );
    return 0;
}

void c_rbx::register_functions( uintptr_t rl ) {
    // @note: 
    g_hooks->auxopen( rl, "demo_func", convert_closure( ( uintptr_t ) demo_func ), 0 );
}