#include "Common.hpp"

BOOL APIENTRY DllMain( HMODULE module, DWORD reason, LPVOID reserved ) {
    if ( reason != DLL_PROCESS_ATTACH )
        return 0;

    std::jthread{ [ ] {
        Context->Init( );
    } }.detach( );

    return 1;
}