#include "Hooks.hpp"

template<typename type>
int MAKE_HOOK( const std::string& pattern, type detour, type* original ) {
    if ( MH_CreateHook( reinterpret_cast< LPVOID >( Memory::Scan( pattern ) ), reinterpret_cast< LPVOID >( detour ), reinterpret_cast< LPVOID* >( original ) ) != MH_OK ) {
        LOG( std::vformat( Xor( "[Hooks] Failed to hook function [ {} ]..." ), std::make_format_args( pattern ) ) );
        return 0;
    }
}

int Hooks::Initialize( ) {
    if ( MH_Initialize( ) != MH_OK )
        return 0;

    MAKE_HOOK( Xor( "41 56 41 55 41 54 55 57 56 53 48 81 EC 90 00 00 00 48" ), &IsLicenseKeyValid, &IsLicenseKeyValidFn );

    if ( MH_EnableHook( MH_ALL_HOOKS ) != MH_OK )
        return 0;

    LOG( Xor( "[Hooks] Done!" ) );

    return 1;
}

void Hooks::Shutdown( ) {
    MH_DisableHook( MH_ALL_HOOKS );
    MH_RemoveHook( MH_ALL_HOOKS );
}