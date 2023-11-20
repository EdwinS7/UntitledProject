#include "wrapper.hpp"

void Untitled::ImWrapper::CreateContext( ) {
	IMGUI_CHECKVERSION( );
    ImGui::CreateContext( );

    ImGuiIO& io = ImGui::GetIO( ); ( void ) io;
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;

    ImGui::StyleColorsDark( );

    ImGui_ImplWin32_Init( Untitled::Win32::GetHwnd( ) );
    ImGui_ImplDX11_Init( Untitled::Graphics::GetDevice( ), Untitled::Graphics::GetDeviceContext( ) );
}