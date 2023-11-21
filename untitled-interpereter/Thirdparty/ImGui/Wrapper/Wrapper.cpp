#include "wrapper.hpp"

void Untitled::ImWrapper::CreateContext( ) {
	IMGUI_CHECKVERSION( );
    ImGui::CreateContext( );

    ImGuiIO& io = ImGui::GetIO( ); ( void ) io;
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;

    ImGui::StyleColorsDark( );

    ImFontConfig fontConfig;
    fontConfig.OversampleH = 3;     // Increase horizontal oversampling for better quality
    fontConfig.OversampleV = 3;     // Increase vertical oversampling for better quality
    fontConfig.PixelSnapH = true;   // Align every glyph to pixel boundary for a clean look

    ImFont* FontDefault = io.Fonts->AddFontFromFileTTF( "c:\\Windows\\Fonts\\CascadiaMono.ttf", 14.f, &fontConfig, io.Fonts->GetGlyphRangesDefault( ) );
    io.FontDefault = FontDefault;

    ImGui_ImplWin32_Init( Untitled::Win32::GetHwnd( ) );
    ImGui_ImplDX11_Init( Untitled::Graphics::GetDevice( ), Untitled::Graphics::GetDeviceContext( ) );
}