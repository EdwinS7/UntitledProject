#include "Containers.hpp"

void Group::Begin( Vec2<int16_t> Size ) {
	m_HeaderSize = 18;

	auto Pos = m_Parent->GetElementFramePos( );

	gBuffer->FilledRectangle( Pos + m_Padding, Size, m_ColorPalate.Background, 5 ); // Background.

	gBuffer->FilledRectangle( Pos + m_Padding, Vec2<int16_t>( Size.x, m_HeaderSize ), m_ColorPalate.Header, 5, CornerTop ); // Header.
	gBuffer->Line( Pos + m_Padding + Vec2<int16_t>( 0, m_HeaderSize ), Pos + m_Padding + Vec2<int16_t>( Size.x, m_HeaderSize ), m_ColorPalate.Accent ); // Header accent bar.

	gBuffer->String( &gBuffer->Fonts.Interface, m_Title, Pos + m_Padding + Vec2<int16_t>( 3, 3 ), m_ColorPalate.Title ); // Header text.

	gBuffer->Rectangle( Pos + m_Padding, Size, m_ColorPalate.Shadow, 5 ); // Dark exterior outline.
	gBuffer->Rectangle( Pos + m_Padding + Vec2<int16_t>( 1, 1 ), Size - Vec2<int16_t>( 2, 2 ), m_ColorPalate.Outline, 5 ); // Light center outline.
	gBuffer->Rectangle( Pos + m_Padding + Vec2<int16_t>( 2, 2 ), Size - Vec2<int16_t>( 4, 4 ), m_ColorPalate.Shadow, 5 ); // Dark inner outline.
}

void Group::Destroy( ) {
	m_Parent = nullptr;
	delete this;
}