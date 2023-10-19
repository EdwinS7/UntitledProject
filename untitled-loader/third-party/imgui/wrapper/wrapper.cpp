#include "wrapper.hpp"

void CWrapper::CreateContext( ) {
	 IMGUI_CHECKVERSION( );
    ImGui::CreateContext( );

    ImGuiIO& io = ImGui::GetIO( ); ( void ) io;
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;

    ImGui::StyleColorsDark( );

    ImGui_ImplWin32_Init( Win32->GetHwnd( ) );
    ImGui_ImplDX11_Init( Graphics->GetDevice( ), Graphics->GetDeviceContext( ) );
}