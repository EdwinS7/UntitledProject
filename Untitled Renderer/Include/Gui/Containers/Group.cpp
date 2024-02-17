#include "Containers.hpp"

void Group::Begin( Vec2<int16_t> Padding, Vec2<int16_t> Size ) {
	m_Padding = Padding;
	m_HeaderSize = 18;

	m_ElementPadding = Vec2<int16_t>( 10, m_HeaderSize + 10 );

	auto Pos = m_Parent->GetElementFramePos( ) + m_Padding;

	gBuffer->FilledRectangle( Pos, Size, m_ColorPalate.Background, 5 ); // Background.

	gBuffer->FilledRectangle( Pos, Vec2<int16_t>( Size.x, m_HeaderSize ), m_ColorPalate.Header, 5, CornerTop ); // Header.
	gBuffer->Line( Pos + Vec2<int16_t>( 0, m_HeaderSize ), Pos + Vec2<int16_t>( Size.x, m_HeaderSize ), m_ColorPalate.Accent ); // Header accent bar.

	gBuffer->String( &gBuffer->Fonts.Interface, m_Title, Pos + Vec2<int16_t>( 3, 3 ), m_ColorPalate.Title ); // Header text.

	gBuffer->Rectangle( Pos, Size, m_ColorPalate.Shadow, 5 ); // Dark exterior outline.
	gBuffer->Rectangle( Pos + Vec2<int16_t>( 1, 1 ), Size - Vec2<int16_t>( 2, 2 ), m_ColorPalate.Outline, 5 ); // Light center outline.
	gBuffer->Rectangle( Pos + Vec2<int16_t>( 2, 2 ), Size - Vec2<int16_t>( 4, 4 ), m_ColorPalate.Shadow, 5 ); // Dark inner outline.
}

void Group::Destroy( ) {
	m_Parent = nullptr;
	delete this;
}

void Group::Checkbox( std::string Title, bool* State ) {
	auto Pos = m_Parent->GetElementFramePos( ) + m_Padding + m_ElementPadding;

	if ( gInput->KeyPressed( KEY_LMOUSE ) && gInput->MouseHoveringRect( Pos, Vec2<int16_t>( 10, 10 ) ) )
		*State = !(*State);

	gBuffer->FilledRectangle( Pos, Vec2<int16_t>( 10, 10 ), *State ? m_ColorPalate.Accent : m_ColorPalate.ElementBackground, 2 ); // Background.
	gBuffer->Rectangle( Pos, Vec2<int16_t>( 10, 10 ), m_ColorPalate.ElementOutline, 2 ); // Outline.
	gBuffer->String( &gBuffer->Fonts.SmallInterface, Title, Pos + Vec2<int16_t>( 15, -1 ), *State ? m_ColorPalate.ElementTextOn : m_ColorPalate.ElementTextOff ); //Title

	m_ElementPadding = Vec2<int16_t>( 10, m_ElementPadding.y + 10 );
}