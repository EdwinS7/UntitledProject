#include "Interface.hpp"

int Interface::Init( ) {
	return ImGuiHook::Load( Draw );
}

int Interface::Draw( ) {
	// Init.
	if ( !ImGui::Begin( SOFTWARE ) )
		return 0;

	// Elements.
	ImGui::Text( "Hello world!" );

	// End.
	ImGui::End( );

	return 1;
}