#include "Containers.hpp"

void Group::Begin( ) {
	m_HeaderSize = 18;

	gBuffer->FilledRectangle( m_Pos, m_Size, m_ColorPalate.Background, 5 ); // Background.

	gBuffer->FilledRectangle( m_Pos, Vec2<int16_t>( m_Size.x, m_HeaderSize ), m_ColorPalate.Header, 5, CornerTop ); // Header.
	gBuffer->Line( m_Pos + Vec2<int16_t>( 0, m_HeaderSize ), m_Pos + Vec2<int16_t>( m_Size.x, m_HeaderSize ), m_ColorPalate.Accent ); // Header accent bar.

	gBuffer->String( &gBuffer->Fonts.Interface, m_Title, m_Pos + Vec2<int16_t>( 3, 3 ), m_ColorPalate.Title ); // Header text.

	gBuffer->Rectangle( m_Pos, m_Size, m_ColorPalate.Shadow, 5 ); // Dark exterior outline.
	gBuffer->Rectangle( m_Pos + Vec2<int16_t>( 1, 1 ), m_Size - Vec2<int16_t>( 2, 2 ), m_ColorPalate.Outline, 5 ); // Light center outline.
	gBuffer->Rectangle( m_Pos + Vec2<int16_t>( 2, 2 ), m_Size - Vec2<int16_t>( 4, 4 ), m_ColorPalate.Shadow, 5 ); // Dark inner outline.
}

void Group::Destroy( ) {
	delete this;
}