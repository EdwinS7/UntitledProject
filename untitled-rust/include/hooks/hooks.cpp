#include "hooks.hpp"

void c_hooks::init( ) {
	if ( MH_Initialize( ) != MH_OK )
		printf( "[MinHook] MH_Initialize failed!\n" );


}

HRESULT __stdcall c_hooks::present( IDXGISwapChain* pSwapChain, UINT SyncInterval, UINT Flags ) {

}

HRESULT __stdcall c_hooks::resize_buffers( IDXGISwapChain* Swap, UINT a2, UINT a3, UINT a4, DXGI_FORMAT a5, UINT a6 ) {

}