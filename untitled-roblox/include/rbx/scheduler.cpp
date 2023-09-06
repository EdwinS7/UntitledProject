#include "rbx.hpp"

int c_rbx::get_job( const std::string& job_name ) {
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

uintptr_t c_rbx::get_script_context( ) {
    return *( uintptr_t* ) ( get_job( xorstr_( "WaitingHybridScriptsJob" ) ) + g_offsets->scheduler::job_context );
}

uintptr_t c_rbx::get_global_state( ) {
    int identity = 8, script = 0;
    return g_hooks->get_state( g_offsets->script_context, &identity, &script );
}