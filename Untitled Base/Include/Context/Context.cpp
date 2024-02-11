#include "context.hpp"

void Context::Init( void* Reserved ) {
    Console::Create( std::format( "{} ( {} )", GAME_NAME, GAME_VERSION ).c_str( ) );

    LOG( std::format( "[Context] Waiting for {}", LAST_LOADED_MODULE ) );

    while ( !GetModuleHandleA( LAST_LOADED_MODULE ) )
        std::this_thread::sleep_for( std::chrono::milliseconds( 1 ) );

    if ( Hooks::Initialize( ) )
        LOG( Xor( "[Hooks] Initialized!" ) );

    if ( !Interface::Init( ) )
        LOG( Xor( "[Interface] Failed!" ) );
    

    while ( !GetAsyncKeyState( VK_DELETE ) && !GetAsyncKeyState( VK_END ) )
        std::this_thread::sleep_for( std::chrono::milliseconds( 1 ) );

    Shutdown( );

    FreeLibraryAndExitThread( reinterpret_cast< HMODULE >( Reserved ), 0 );
}

void Context::Shutdown( ) {
    LOG( Xor( "[End] You may close this console now!" ) );

    Console::Shutdown( );
    Hooks::Shutdown( );
}