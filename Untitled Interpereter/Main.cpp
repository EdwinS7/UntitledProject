#include "Common.hpp"

int WINAPI WinMain( HINSTANCE instance, HINSTANCE prev_instance, PSTR cmd, int show_cmd ) {
    Untitled::Win32::CreateRenderEnvironment( "Untitled Interpreter" );
    Untitled::Graphics::CreateDeviceAndSwapChain( );
    Untitled::ImWrapper::CreateContext( );
    Untitled::Context::Init( );

    while ( Untitled::Win32::Dispatch( ) ) {
        Untitled::Graphics::BeginScene( );
        Untitled::Gui::Render( );
        Untitled::Graphics::EndScene( );
    }
}