#include "scheduler.hpp"

int c_scheduler::get_job( const std::string& job_name ) {
    uintptr_t scheduler = g_hooks->get_scheduler( );

    uintptr_t job_start = *( uintptr_t* ) ( scheduler + g_offsets->scheduler::job_start );
    uintptr_t job_end = *( uintptr_t* ) ( scheduler + g_offsets->scheduler::job_end );

    while ( job_start != job_end ) {
        uintptr_t job = *( uintptr_t* ) ( job_start );

        if ( *reinterpret_cast< std::string* >( job + g_offsets->scheduler::job_name ) == job_name )
            return job;

        job_start += 8;
    }

    return NULL;
}

uintptr_t c_scheduler::get_script_context( ) {
    return *( uintptr_t* ) ( get_job( xorstr_( "WaitingHybridScriptsJob" ) ) + g_offsets->scheduler::job_context );
}

uintptr_t c_scheduler::get_global_state( ) {
    int identity = 8, script = 0;
    return g_hooks->get_state( g_offsets->script_context, &identity, &script );
}

void c_scheduler::run_script( const std::string& source ) {
    if ( source.length( ) <= 8 )
        return;

    std::string bytecode = g_serializer->serialize( source );

    // @note: rebuild Luau_Load, it's what you should be using!
    if ( g_hooks->lua_vm_load( g_offsets->lua_state, &bytecode, g_util->random_string( 16 ).c_str( ), 0 ) ) {
        g_hooks->print( print_level::error, xorstr_( "Unexpected error during execution." ) );
        return;
    }

    g_hooks->task_defer( g_offsets->lua_state );
    pop_stack( 1 );
}