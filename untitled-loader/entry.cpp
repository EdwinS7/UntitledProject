#include "common.hpp"

int WINAPI WinMain( HINSTANCE instance, HINSTANCE prev_instance, PSTR cmd, int show_cmd ) {
	Win32->CreateRenderEnvironment( "Untitled Loader" );
    Win32->CreateLoggingEnvironment( "Untitled Loader" );
    Graphics->CreateDeviceAndSwapChain( );
    ImWrapper->CreateContext( );

	while ( Win32->Dispatch( ) ) {
        Graphics->BeginScene( );
        Context->RenderWindow( );
        Graphics->EndScene( );
	}
}