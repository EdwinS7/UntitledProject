#include "Gui.hpp"

Window* cGui::NewWindow( const char* Title, const char* Description, Vec2<int16_t> m_Size ) {
	auto m_Pos = Vec2<int16_t>( gWin32->GetSize( ).x / 2 - m_Size.x / 2, gWin32->GetSize( ).y / 2 - m_Size.y / 2 );
	m_Windows.push_back( Window( Title, Description, m_Pos, m_Size, Vec2<int16_t>( m_Size.x / 2, m_Size.y / 2 ), Vec2<int16_t>( m_Size.x * 1.5, m_Size.y * 1.5 ) ) );
	return &m_Windows.back( );
}

void Window::Render( ) {
	HandleInput( );

	m_HeaderSize = 18;
	m_FooterSize = 18;

	gBuffer->FilledRectangle( m_Pos, m_Size, m_ColorPalate.Background, 5 ); // Background.

	gBuffer->FilledRectangle( m_Pos, Vec2<int16_t>( m_Size.x, m_HeaderSize ), m_ColorPalate.Header, 5, corner_top ); // Header.
	gBuffer->Line( m_Pos + Vec2<int16_t>( 0, m_HeaderSize ), m_Pos + Vec2<int16_t>( m_Size.x, m_HeaderSize ), m_ColorPalate.Accent ); // Header accent bar.

	gBuffer->FilledRectangle( m_Pos + Vec2<int16_t>( 0, m_Size.y - m_FooterSize ), Vec2<int16_t>( m_Size.x, m_FooterSize ), m_ColorPalate.Footer, 5, corner_bottom ); // Footer.
	gBuffer->Line( m_Pos + Vec2<int16_t>( 0, m_Size.y - m_FooterSize ), m_Pos + Vec2<int16_t>( m_Size.x, m_Size.y - m_FooterSize ), m_ColorPalate.Accent ); // Footer accent bar.

	gBuffer->Rectangle( m_Pos, m_Size, m_ColorPalate.Shadow, 5 ); // Dark exterior outline.
	gBuffer->Rectangle( m_Pos + Vec2<int16_t>(1, 1), m_Size - Vec2<int16_t>( 2, 2 ), m_ColorPalate.Outline, 5 ); // Light center outline.
	gBuffer->Rectangle( m_Pos + Vec2<int16_t>( 2, 2 ), m_Size - Vec2<int16_t>( 4, 4 ), m_ColorPalate.Shadow, 5 ); // Dark inner outline.

	gBuffer->String( &gBuffer->Fonts.Interface, m_Title.c_str( ), m_Pos + Vec2<int16_t>( 3, 3 ), m_ColorPalate.Title );
	gBuffer->String( &gBuffer->Fonts.Interface, m_Description.c_str( ), m_Pos + Vec2<int16_t>( 3, m_Size.y - m_HeaderSize + 3 ), m_ColorPalate.Description );
}

void Window::HandleInput( ) {
	if ( gInput->KeyHeld( KEY_LMOUSE ) && !m_Dragging.StartedOutside ) {
		if ( gInput->MouseHoveringRect( m_Pos, Vec2<int16_t>( m_Size.x, m_HeaderSize ) ) && !m_Dragging.Active ) {
			m_Dragging.Difference = m_Pos - gInput->GetMousePos( );
			m_Dragging.Active = true;
		}
		else
			m_Dragging.StartedOutside = true;
	}
	else if ( !gInput->KeyHeld( KEY_LMOUSE ) && m_Dragging.StartedOutside )
		m_Dragging.StartedOutside = false;

	if ( m_Dragging.Active && !gInput->KeyHeld( KEY_LMOUSE ) )
		m_Dragging.Active = false;

	if ( m_Dragging.Active )
		m_Pos = gInput->GetMousePos( ) + m_Dragging.Difference;

	if ( gInput->KeyHeld( KEY_LMOUSE ) && !m_Resizing.StartedOutside ) {
		if ( gInput->MouseHoveringRect( m_Pos + m_Size - Vec2<int16_t>( 10, 10 ), Vec2<int16_t>( 10, 10 ) ) && !m_Resizing.Active ) {
			m_Resizing.Difference = ( m_Pos + m_Size ) - gInput->GetMousePos( );
			m_Resizing.Active = true;
		}
		else
			m_Resizing.StartedOutside = true;
	}
	else if ( !gInput->KeyHeld( KEY_LMOUSE ) && m_Resizing.StartedOutside )
		m_Resizing.StartedOutside = false;

	if ( m_Resizing.Active && !gInput->KeyHeld( KEY_LMOUSE ) )
		m_Resizing.Active = false;

	if ( m_Resizing.Active )
		m_Size = ( gInput->GetMousePos( ) - m_Pos ) + m_Resizing.Difference;

	if ( m_Resizing.Active || gInput->MouseHoveringRect( m_Pos + m_Size - Vec2<int16_t>( 10, 10 ), Vec2<int16_t>( 10, 10 ) ) )
		gInput->SetCursorStyle( IDC_SIZENWSE );

	m_Size.x = std::clamp( m_Size.x, m_MinSize.x, m_MaxSize.x );
	m_Size.y = std::clamp( m_Size.y, m_MinSize.y, m_MaxSize.y );
}

void Window::Destroy( ) {

}

void Window::SetColorPalate( WindowColorPalate Palate ) {
	m_ColorPalate = Palate;
}