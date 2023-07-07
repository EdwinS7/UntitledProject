#include "../../.hpp/gui/gui.hpp"

void c_gui::draw( ) {
	ImGui::SetNextWindowPos( ImGui::GetIO( ).DisplaySize / 2.f - ImGui::GetStyle( ).WindowMinSize / 2.f, ImGuiCond_Once );

	ImGui::Begin( xorstr_( "untitled" ), &is_open );



	ImGui::End( );
}