#include "rbx.hpp"

int c_rbx::get_job( const std::string& job ) {
    uintptr_t scheduler_address = g_hooks->get_scheduler( );

    uintptr_t job_start = *( uintptr_t* ) ( scheduler_address + 308 );
    uintptr_t job_end = *( uintptr_t* ) ( scheduler_address + 312 );

    while ( job_start != job_end ) {
        uintptr_t job_address = *( uintptr_t* ) ( job_start );
        std::string* job_string = reinterpret_cast< std::string* >( job_address + 16 );

        if ( *job_string == job )
            return job_address;

        job_start += 8;
    }

    return NULL;
}

uintptr_t c_rbx::get_script_context( ) {
    uintptr_t job_address = get_job( "WaitingHybridScriptsJob" );
    uintptr_t context_address = *( uintptr_t* ) ( job_address + 312 );

    return context_address;
}

uintptr_t c_rbx::get_lua_state( ) {
    int identity = 8, script = 0;
    return g_hooks->get_state( g_ctx->script_context, &identity, &script );
}