#include "wrapper.hpp"

void Untitled::ImWrapper::CreateContext( ) {
	IMGUI_CHECKVERSION( );
    ImGui::CreateContext( );

    ImGuiIO& io = ImGui::GetIO( ); ( void ) io;
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;

    ImGui::StyleColorsDark( );

    ImFontConfig fontConfig;
    fontConfig.SizePixels = 16.0f;  // Adjust the font size as needed
    fontConfig.OversampleH = 3;     // Increase horizontal oversampling for better quality
    fontConfig.OversampleV = 3;     // Increase vertical oversampling for better quality
    fontConfig.PixelSnapH = true;   // Align every glyph to pixel boundary for a clean look

    ImFont* FontDefault = io.Fonts->AddFontFromFileTTF( "c:\\Windows\\Fonts\\CascadiaMono.ttf", 14.0f, &fontConfig, io.Fonts->GetGlyphRangesDefault( ) );
    io.FontDefault = FontDefault;

    ImGui_ImplWin32_Init( Untitled::Win32::GetHwnd( ) );
    ImGui_ImplDX11_Init( Untitled::Graphics::GetDevice( ), Untitled::Graphics::GetDeviceContext( ) );
}