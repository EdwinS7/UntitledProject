#include "Hooks.hpp"

int Hooking::Init( ) {
    LOG( Xor( "[MinHook] Creating Instance..." ) );

    if ( !MH_Init( ) )
        return 0;

    LOG( Xor( "[MinHook] Instance created!" ) );

    MH_Hook(
        reinterpret_cast< LPVOID >( Memory->Scan( Xor( "41 56 41 55 41 54 55 57 56 53 48 81 EC 90 00 00 00 48" ) ) ),
        reinterpret_cast< LPVOID >( &Hooks::IsKeyValid ),
        reinterpret_cast< LPVOID* >( &Hooks::IsKeyValidOriginal )
    );

    LOG( Xor( "[MinHook] Enabling hooks..." ) );

    if ( !MH_Enable( ) )
        return 0;

    LOG( Xor( "[MinHook] Hooks enabled!" ) );

    return 1;
}

int Hooking::MH_Init( ) {
    if ( MH_Initialize( ) != MH_OK ) {
        LOG( Xor( "[MinHook] Failed to initialize?" ) );
        return 0;
    }

    return 1;
}

int Hooking::MH_Hook( void* Address, void* Detour, void** Original ) {
    if ( MH_CreateHook( Address, Detour, Original ) != MH_OK ) {
        LOG( Xor( "[MinHook] Failed to create hook?" ) );
        return 0;
    }

    return 1;
}

int Hooking::MH_Enable( ) {
    if ( MH_EnableHook( MH_ALL_HOOKS ) != MH_OK ) {
        LOG( Xor( "[MinHook] Failed to enable hooks?" ) );
        return 0;
    }

    return 1;
}

template <typename hook_type>
void Hooking::CreateHook( hook_type* Hook, const std::string& Signature ) {
    *Hook = reinterpret_cast< hook_type >(
         Memory->Scan( Signature )
    );
}

template <typename hook_type>
void Hooking::CreateHook( hook_type* Hook, const uintptr_t& Offset ) {
    *Hook = reinterpret_cast< hook_type >(
        Memory->Rebase( Offset )
    );
}