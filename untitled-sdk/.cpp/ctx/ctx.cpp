#include "../../.hpp/ctx/ctx.hpp"

template <typename type>
void safe_release( type*& obj ) {
	if ( obj ) {
		obj->Release( );
		obj = nullptr;
	}
}

void c_ctx::init( ) {
#ifdef _DEBUG
	if ( !AllocConsole( ) )
		return;

	FILE* file{};
	if ( freopen_s( &file, "CONOUT$", "w", stdout ) )
		return;

	SetConsoleTitle( xorstr_("untitled sdk") );

	std::printf( xorstr_("waiting for navsystem.dll...\n") );
#endif

	while ( !GetModuleHandle( xorstr_( "navsystem.dll" ) ) )
		std::this_thread::sleep_for( std::chrono::milliseconds{ 200 } );

#ifdef _DEBUG
	std::printf( xorstr_( "initializing minhook...\n" ) );
#endif

	if ( MH_Initialize( ) != MH_OK )
		return;

#ifdef _DEBUG
	std::printf( xorstr_("initializing hooks...\n") );
#endif

	m_hwnd = FindWindowA( xorstr_( "Counter-Strike 2" ), nullptr );

#ifdef _DEBUG
	std::printf( xorstr_( "initializing wndproc...\n" ) );
#endif

	hooks::original_wnd_proc = reinterpret_cast< WNDPROC >(
		SetWindowLongPtr( m_hwnd, GWLP_WNDPROC, reinterpret_cast< LONG_PTR >( hooks::wnd_proc ) )
	);

#ifdef _DEBUG
	std::printf( xorstr_( "initializing render...\n" ) );
#endif

	interfaces::m_render = reinterpret_cast<interfaces::c_render*>(
		( PVOID )g_util->absolute( g_mem->find_pattern( GetModuleHandle( xorstr_( "rendersystemdx11.dll" ) ), xorstr_( "66 0F 7F 05 ? ? ? ? 66 0F 7F 0D ? ? ? ? 48 89 35 ? ? ? ?" ) ) + 0x4 )
	);

#ifdef _DEBUG
	std::printf( xorstr_( "initializing swap chain...\n" ) );
#endif

	interfaces::m_swap_chain = interfaces::m_render->pSwapChain;

#ifdef _DEBUG
	std::printf( xorstr_( "initializing present...\n" ) );
#endif

	if ( MH_CreateHook( 
		g_util->get_method<void*>( interfaces::m_swap_chain, 8),
		reinterpret_cast< void* >( &hooks::present ),
		reinterpret_cast< void** >( &hooks::original_present )
	) != MH_OK )
		return;

#ifdef _DEBUG
	std::printf( xorstr_( "initializing resize buffers...\n" ) );
#endif

	if ( MH_CreateHook(
		g_util->get_method<void*>( interfaces::m_swap_chain, 13 ),
		reinterpret_cast< void* >( &hooks::resize_buffers ),
		reinterpret_cast< void** >( &hooks::original_resize_buffers )
	) != MH_OK )
		return;

#ifdef _DEBUG
	std::printf( xorstr_( "finishing up...\n" ) );
#endif

	if ( MH_EnableHook( MH_ALL_HOOKS ) != MH_OK )
		return;

#ifdef _DEBUG
	std::printf( xorstr_("done!\n") );
#endif
}

int __stdcall DllMain( _In_ HINSTANCE instance, _In_ DWORD reason, _In_ LPVOID reserved ) {
	if ( reason != DLL_PROCESS_ATTACH )
		return 0;

	DisableThreadLibraryCalls( instance );

	std::jthread{ [ ]( ) {
		g_ctx->init( );
	} }.detach( );

	return 1;
}