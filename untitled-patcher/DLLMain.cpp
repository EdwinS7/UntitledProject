#include "Include/Common.hpp"

BOOL APIENTRY DllMain( HMODULE Module, DWORD Reason, LPVOID Reserved ) {
    if ( Reason != DLL_PROCESS_ATTACH )
        return 0;

    std::jthread{ [ ]( ) {
        Context->Init( );
    } }.detach( );

    return 1;
}