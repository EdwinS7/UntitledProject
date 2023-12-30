#include "context.hpp"

void cContext::Init( ) {
    try {
        while ( !GetModuleHandleA( LAST_LOADED_MODULE ) )
            std::this_thread::sleep_for( 1ms );

        if ( !Utilities->AllocateConsole( std::format( "{} ( {} )", GAME_NAME, GAME_VERSION ) ) ) {
            LOG_ERROR( "Failed to allocate console!" );
            return;
        }

        if ( !Hooking::Init( ) )
            LOG_ERROR( Xor( "Failed to initiate hooks!" ) );

        LOG( Xor( "[End] You may close this console now!" ) );
    }
    catch ( const std::exception& error ) {
        LOG_ERROR( std::vformat( Xor( "Caught an exception: {}" ), std::make_format_args( error.what( ) ) ) );
    }
    
    //FreeConsole( );
}