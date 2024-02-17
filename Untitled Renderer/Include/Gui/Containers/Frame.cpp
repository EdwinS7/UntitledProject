#include "Containers.hpp"

void Window::Render( ) {
	m_HeaderSize = 18;
	m_FooterSize = 18;

	gBuffer->FilledRectangle( m_LerpPos.As<int16_t>( ), m_LerpSize.As<int16_t>( ), m_ColorPalate.Background, 5 ); // Background.

	gBuffer->FilledRectangle( m_LerpPos.As<int16_t>( ), Vec2<int16_t>( m_LerpSize.As<int16_t>( ).x, m_HeaderSize ), m_ColorPalate.Header, 5, CornerTop ); // Header.
	gBuffer->Line( m_LerpPos.As<int16_t>( ) + Vec2<int16_t>( 0, m_HeaderSize ), m_LerpPos.As<int16_t>( ) + Vec2<int16_t>( m_LerpSize.As<int16_t>( ).x, m_HeaderSize ), m_ColorPalate.Accent ); // Header accent bar.

	gBuffer->String( &gBuffer->Fonts.Interface, m_Title, m_LerpPos.As<int16_t>( ) + Vec2<int16_t>( 3, 3 ), m_ColorPalate.Title ); // Header text.

	gBuffer->FilledRectangle( m_LerpPos.As<int16_t>( ) + Vec2<int16_t>( 0, m_LerpSize.As<int16_t>( ).y - m_FooterSize ), Vec2<int16_t>( m_LerpSize.As<int16_t>( ).x, m_FooterSize ), m_ColorPalate.Footer, 5, CornerRottom ); // Footer.
	gBuffer->Line( m_LerpPos.As<int16_t>( ) + Vec2<int16_t>( 0, m_LerpSize.As<int16_t>( ).y - m_FooterSize ), m_LerpPos.As<int16_t>( ) + Vec2<int16_t>( m_LerpSize.As<int16_t>( ).x, m_LerpSize.As<int16_t>( ).y - m_FooterSize ), m_ColorPalate.Accent ); // Footer accent bar.

	gBuffer->String( &gBuffer->Fonts.Interface, m_Description, m_LerpPos.As<int16_t>( ) + Vec2<int16_t>( 3, m_LerpSize.As<int16_t>( ).y - m_HeaderSize + 3 ), m_ColorPalate.Description ); // Footer text.

	gBuffer->Rectangle( m_LerpPos.As<int16_t>( ), m_LerpSize.As<int16_t>( ), m_ColorPalate.Shadow, 5 ); // Dark exterior outline.
	gBuffer->Rectangle( m_LerpPos.As<int16_t>( ) + Vec2<int16_t>( 1, 1 ), m_LerpSize.As<int16_t>( ) - Vec2<int16_t>( 2, 2 ), m_ColorPalate.Outline, 5 ); // Light center outline.
	gBuffer->Rectangle( m_LerpPos.As<int16_t>( ) + Vec2<int16_t>( 2, 2 ), m_LerpSize.As<int16_t>( ) - Vec2<int16_t>( 4, 4 ), m_ColorPalate.Shadow, 5 ); // Dark inner outline.

	Vec2<int16_t> TabTextSize = { 3, 0 };

	for ( int i = m_Tabs.size( ) - 1; i >= 0; i-- ) {
		TabTextSize.x += gBuffer->GetStringSize( &gBuffer->Fonts.Interface, m_Tabs[ i ].Title.c_str( ) ).x + 4;
		gBuffer->String( &gBuffer->Fonts.Interface, m_Tabs[ i ].Title, m_LerpPos.As<int16_t>( ) + Vec2<int16_t>( m_LerpSize.As<int16_t>( ).x - TabTextSize.x, 3 ), i == m_CurrentTab ? m_ColorPalate.TabTextActive : m_ColorPalate.TabTextInactive ); // Footer text.
	}
}

void Window::HandleInput( ) {
	//Dragging.
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

	m_LerpPos.Lerp( m_Pos.As<float>( ), gContext->GetDeltaTime( ) * 12.f );

	//Resizing.
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

	m_LerpSize.Lerp( m_Size.As<float>( ), gContext->GetDeltaTime( ) * 8.f );
}

Tab* Window::AddTab( const char* Title ) {
	m_Tabs.push_back( Tab( Title ) );
	return &m_Tabs.back( );
}

inline WindowColorPalate Window::GetColorPalate( ) {
	return m_ColorPalate;
}

void Window::SetColorPalate( WindowColorPalate Palate ) {
	m_ColorPalate = Palate;
}

Vec2<int16_t> Window::GetElementFramePos( ) {
	return Vec2<int16_t>( m_LerpPos.x, m_LerpPos.y + m_HeaderSize );
}

Vec2<int16_t> Window::GetElementFrameSize( ) {
	return Vec2<int16_t>( m_LerpSize.x, m_LerpSize.y - m_HeaderSize - m_FooterSize );
}

Vec2<int16_t> Window::GetLeftDock( ) {
	return Vec2<int16_t>( 20, 20 );
}

Vec2<int16_t> Window::GetRightDock( ) {
	return Vec2<int16_t>( GetElementFrameSize( ).x / 2 + 10, 20 );
}

void Window::Destroy( ) {
	delete this;
}