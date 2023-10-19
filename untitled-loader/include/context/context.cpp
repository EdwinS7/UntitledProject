#include "context.hpp"

void CContext::RenderWindow( ) {
	ImGui::Begin( REGION_NAME );
	{
		m_ActiveRegion = 1;

		switch ( m_ActiveRegion ) {
		case 0:
			ImGui::InputText( "Username", &m_Username );
			ImGui::InputText( "Password", &m_Password );

			if ( ImGui::Button( "Login" ) ) {
#if DEVELOPER_BUILD
				if ( m_Username == MANUAL_USER && m_Password == MANUAL_PASSWORD ) {
					LOG( "logged in successfully!" );
					m_ActiveRegion = 1;
				}
				else
					LOG( "failed to log in, incorrect credentials!" );
#else

#endif
			}
			break;
		case 1:
			ImGui::Combo( "Software", &m_CurrentSelection, m_Software );

			if ( ImGui::Button( "Inject" ) ) {
				switch ( m_CurrentSelection ) {
				case 0:
					Utilities->InjectDLL( "mx sim.exe", Modules->MXS_Binary );
					break;
				}
			}

			break;
		}
	}
	ImGui::End( );
}