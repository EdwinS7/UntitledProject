#include "buffer.hpp"

void cBuffer::Init( ) {
	gGraphics->CreateFontFromName( &DefaultFont, "Verdana", 10, 400, Vec2<int16_t>( 0, 0 ), false );

	PushClip( gWindow->GetClipRect( ) );
	PushTexture( nullptr );

	gLuaAPI->RunConnection( "OnObjectCreation" );
}

void cBuffer::Release( ) {
	m_CommandResources.TextureStack.clear( );
	m_CommandResources.ClipStack.clear( );

	ClearCommands( );
}

void cBuffer::Line( Vec2< int16_t > from, Vec2< int16_t > to, Color color ) {
	std::vector<Vertex> Vertices = {
		Vertex( from.x, from.y, 0.f, 1.f, color.Hex ),
		Vertex( to.x, to.y, 0.f, 1.f, color.Hex )
	};

	WriteToBuffer( LINE, &Vertices, nullptr );
}

void cBuffer::Polyline( const std::vector<Vec2<int16_t>>& points, Color color ) {
	std::vector<Vertex> Vertices;
	Vertices.reserve( points.size( ) );

	MakeVertices( &Vertices, &points, &color );
	WriteToBuffer( LINE, &Vertices, nullptr );
}

void cBuffer::Polygon( const std::vector<Vec2<int16_t>>& points, Color color ) {
	std::vector<Vertex> Vertices;
	Vertices.reserve( points.size( ) );

	MakeVertices( &Vertices, &points, &color );

	std::vector<int32_t> Indices;
	Indices.reserve( Vertices.size( ) * 3 );

	for ( size_t i = 1; i < ( Vertices.size( ) - 2 ) * 3; i++ ) {
		Indices.push_back( 0 );
		Indices.push_back( i );
		Indices.push_back( i + 1 );
	}

	WriteToBuffer( TRIANGLE, &Vertices, &Indices );
}

void cBuffer::Rectangle( Vec2< int16_t > position, Vec2< int16_t > size, Color color, int16_t rounding, CornerFlags flags ) {
	bool RoundTopLeft = ( flags & CornerTopLeft ) && rounding;
	bool RoundTopRight = ( flags & CornerTopRight ) && rounding;
	bool RoundBottomLeft = ( flags & CornerBottomLeft ) && rounding;
	bool RoundBottomRight = ( flags & CornerBottomRight ) && rounding;

	std::vector<Vertex> Vertices;

	std::initializer_list<std::tuple<Vec2<int16_t>, Vec2<int16_t>, int, bool>> Corners = {
		std::tuple{Vec2<int16_t>( position.x, position.y ), Vec2<int16_t>( position.x + rounding, position.y + rounding ), 180, RoundTopLeft},
		std::tuple{Vec2<int16_t>( position.x + size.x, position.y ), Vec2<int16_t>( position.x + size.x - rounding, position.y + rounding ), 270, RoundTopRight},
		std::tuple{Vec2<int16_t>( position.x + size.x, position.y + size.y ), Vec2<int16_t>( position.x + size.x - rounding, position.y + size.y - rounding ), 0, RoundBottomRight},
		std::tuple{Vec2<int16_t>( position.x, position.y + size.y ), Vec2<int16_t>( position.x + rounding, position.y + size.y - rounding ), 90, RoundBottomLeft},
		std::tuple{Vec2<int16_t>( position.x, position.y ), Vec2<int16_t>( position.x + rounding, position.y + rounding ), 180, RoundTopLeft}
	};

	for ( const std::tuple<Vec2<int16_t>, Vec2<int16_t>, int, bool>& Corner : Corners ) {
		bool should_round = std::get<3>( Corner );

		if ( should_round ) {
			Vec2<int16_t> CornerRounded = std::get<1>( Corner );
			int Angle = std::get<2>( Corner );

			std::vector<Vertex> CornerVertices;
			CornerVertices.reserve( m_RectangleSegments + 1 );

			GenerateArcVertices( &CornerVertices, &CornerRounded, rounding, 25, Angle, m_RectangleSegments, color, color, false );

			Vertices.insert( Vertices.end( ),
				std::make_move_iterator( CornerVertices.begin( ) ), std::make_move_iterator( CornerVertices.end( ) )
			);
		}
		else {
			auto CornerUnrounded = std::get<0>( Corner );
			Vertices.emplace_back( Vertex( static_cast< float >( CornerUnrounded.x ), static_cast< float >( CornerUnrounded.y ), 0.f, 1.f, color.Hex, 0.f, 0.f ) );
		}
	}

	WriteToBuffer( LINE, &Vertices, nullptr );
}

void cBuffer::FilledRectangle( Vec2< int16_t > position, Vec2< int16_t > size, Color color, int16_t rounding, CornerFlags flags ) {
	bool RoundTopLeft = ( flags & CornerTopLeft ) && rounding;
	bool RoundTopRight = ( flags & CornerTopRight ) && rounding;
	bool RoundBottomLeft = ( flags & CornerBottomLeft ) && rounding;
	bool RoundBottomRight = ( flags & CornerBottomRight ) && rounding;

	std::vector<Vertex> Vertices;

	std::initializer_list<std::tuple<Vec2<int16_t>, Vec2<int16_t>, int, bool>> Corners = {
		std::tuple{Vec2<int16_t>( position.x, position.y ), Vec2<int16_t>( position.x + rounding, position.y + rounding ), 180, RoundTopLeft},
		std::tuple{Vec2<int16_t>( position.x + size.x, position.y ), Vec2<int16_t>( position.x + size.x - rounding, position.y + rounding ), 270, RoundTopRight},
		std::tuple{Vec2<int16_t>( position.x + size.x, position.y + size.y ), Vec2<int16_t>( position.x + size.x - rounding, position.y + size.y - rounding ), 0, RoundBottomRight},
		std::tuple{Vec2<int16_t>( position.x, position.y + size.y ), Vec2<int16_t>( position.x + rounding, position.y + size.y - rounding ), 90, RoundBottomLeft},
		std::tuple{Vec2<int16_t>( position.x, position.y ), Vec2<int16_t>( position.x + rounding, position.y + rounding ), 180, RoundTopLeft}
	};

	for ( const std::tuple<Vec2<int16_t>, Vec2<int16_t>, int, bool>& Corner : Corners ) {
		bool should_round = std::get<3>( Corner );

		if ( should_round ) {
			Vec2<int16_t> CornerRounded = std::get<1>( Corner );
			int Angle = std::get<2>( Corner );

			std::vector<Vertex> CornerVertices;
			CornerVertices.reserve( m_RectangleSegments + 1 );

			GenerateArcVertices( &CornerVertices, &CornerRounded, rounding, 25, Angle, m_RectangleSegments, color, color, false );

			Vertices.insert( Vertices.end( ),
				std::make_move_iterator( CornerVertices.begin( ) ), std::make_move_iterator( CornerVertices.end( ) )
			);
		}
		else {
			auto CornerUnrounded = std::get<0>( Corner );
			Vertices.emplace_back( Vertex( static_cast< float >( CornerUnrounded.x ), static_cast< float >( CornerUnrounded.y ), 0.f, 1.f, color.Hex, 0.f, 0.f ) );
		}
	}

	WriteToBuffer( TRIANGLE_FAN, &Vertices, nullptr );
}

void cBuffer::TexturedRectangle( IDirect3DTexture9* texture, Vec2<int16_t> position, Vec2<int16_t> size, Color color_modulation ) {
	std::vector<Vertex> Vertices = {
		Vertex( static_cast< float >( position.x ), static_cast< float >( position.y ), 0.f, 1.f, color_modulation.Hex ),
		Vertex( static_cast< float >( position.x + size.x ), static_cast< float >( position.y ), 0.f, 1.f, color_modulation.Hex ),
		Vertex( static_cast< float >( position.x + size.x ), static_cast< float >( position.y + size.y ), 0.f, 1.f, color_modulation.Hex ),
		Vertex( static_cast< float >( position.x + size.x ), static_cast< float >( position.y + size.y ), 0.f, 1.f, color_modulation.Hex ),
		Vertex( static_cast< float >( position.x ), static_cast< float >( position.y + size.y ), 0.f, 1.f, color_modulation.Hex ),
		Vertex( static_cast< float >( position.x ), static_cast< float >( position.y ), 0.f, 1.f, color_modulation.Hex )
	};

	PushTexture( texture );
	WriteToBuffer( TRIANGLE, &Vertices, nullptr );
	PopTexture( );
}

void cBuffer::Picture( Image* image, Vec2<int16_t> position, Color color_modulation ) {
	std::vector<Vertex> Vertices = {
		Vertex( static_cast< float >( position.x ), static_cast< float >( position.y ), 0.f, 1.f, color_modulation.Hex ),
		Vertex( static_cast< float >( position.x + image->Size.x ), static_cast< float >( position.y ), 0.f, 1.f, color_modulation.Hex ),
		Vertex( static_cast< float >( position.x + image->Size.x ), static_cast< float >( position.y + image->Size.y ), 0.f, 1.f, color_modulation.Hex ),
		Vertex( static_cast< float >( position.x + image->Size.x ), static_cast< float >( position.y + image->Size.y ), 0.f, 1.f, color_modulation.Hex ),
		Vertex( static_cast< float >( position.x ), static_cast< float >( position.y + image->Size.y ), 0.f, 1.f, color_modulation.Hex ),
		Vertex( static_cast< float >( position.x ), static_cast< float >( position.y ), 0.f, 1.f, color_modulation.Hex )
	};

	PushTexture( image->Texture );
	WriteToBuffer( TRIANGLE, &Vertices, nullptr );
	PopTexture( );
}

void cBuffer::Gradient( Vec2<int16_t> position, Vec2<int16_t> size, Color color1, Color color2, bool vertical ) {
	std::vector<Vertex> Vertices = {
		Vertex( static_cast< float >( position.x ), static_cast< float >( position.y ), 0.f, 1.f, color1.Hex ),
		Vertex( static_cast< float >( position.x + size.x ), static_cast< float >( position.y ), 0.f, 1.f, vertical ? color1.Hex : color2.Hex ),
		Vertex( static_cast< float >( position.x + size.x ), static_cast< float >( position.y + size.y ), 0.f, 1.f, color2.Hex ),
		Vertex( static_cast< float >( position.x + size.x ), static_cast< float >( position.y + size.y ), 0.f, 1.f, color2.Hex ),
		Vertex( static_cast< float >( position.x ), static_cast< float >( position.y + size.y ), 0.f, 1.f, vertical ? color2.Hex : color1.Hex ),
		Vertex( static_cast< float >( position.x ), static_cast< float >( position.y ), 0.f, 1.f, color1.Hex )
	};

	WriteToBuffer( LINE, &Vertices, nullptr );
}

void cBuffer::FilledGradient( Vec2<int16_t> position, Vec2<int16_t> size, Color color1, Color color2, bool vertical ) {
	std::vector<Vertex> Vertices = {
		Vertex( static_cast< float >( position.x ), static_cast< float >( position.y ), 0.f, 1.f, color1.Hex ),
		Vertex( static_cast< float >( position.x + size.x ), static_cast< float >( position.y ), 0.f, 1.f, vertical ? color1.Hex : color2.Hex ),
		Vertex( static_cast< float >( position.x + size.x ), static_cast< float >( position.y + size.y ), 0.f, 1.f, color2.Hex ),
		Vertex( static_cast< float >( position.x + size.x ), static_cast< float >( position.y + size.y ), 0.f, 1.f, color2.Hex ),
		Vertex( static_cast< float >( position.x ), static_cast< float >( position.y + size.y ), 0.f, 1.f, vertical ? color2.Hex : color1.Hex ),
		Vertex( static_cast< float >( position.x ), static_cast< float >( position.y ), 0.f, 1.f, color1.Hex )
	};

	WriteToBuffer( TRIANGLE, &Vertices, nullptr );
}

void cBuffer::Gradient( Vec2<int16_t> position, Vec2<int16_t> size, Color color_top_left, Color color_top_right, Color color_bottom_right, Color color_bottom_left ) {
	std::vector<Vertex> Vertices = {
		Vertex( static_cast< float >( position.x ), static_cast< float >( position.y ), 0.f, 1.f, color_top_left.Hex ),
		Vertex( static_cast< float >( position.x + size.x ), static_cast< float >( position.y ), 0.f, 1.f, color_top_right.Hex ),
		Vertex( static_cast< float >( position.x + size.x ), static_cast< float >( position.y + size.y ), 0.f, 1.f, color_bottom_right.Hex ),
		Vertex( static_cast< float >( position.x + size.x ), static_cast< float >( position.y + size.y ), 0.f, 1.f, color_bottom_right.Hex ),
		Vertex( static_cast< float >( position.x ), static_cast< float >( position.y + size.y ), 0.f, 1.f, color_bottom_left.Hex ),
		Vertex( static_cast< float >( position.x ), static_cast< float >( position.y ), 0.f, 1.f, color_top_left.Hex )
	};

	WriteToBuffer( LINE, &Vertices, nullptr );
}

void cBuffer::FilledGradient( Vec2<int16_t> position, Vec2<int16_t> size, Color color_top_left, Color color_top_right, Color color_bottom_right, Color color_bottom_left ) {
	std::vector<Vertex> Vertices = {
		Vertex( static_cast< float >( position.x ), static_cast< float >( position.y ), 0.f, 1.f, color_top_left.Hex ),
		Vertex( static_cast< float >( position.x + size.x ), static_cast< float >( position.y ), 0.f, 1.f, color_top_right.Hex ),
		Vertex( static_cast< float >( position.x + size.x ), static_cast< float >( position.y + size.y ), 0.f, 1.f, color_bottom_right.Hex ),
		Vertex( static_cast< float >( position.x + size.x ), static_cast< float >( position.y + size.y ), 0.f, 1.f, color_bottom_right.Hex ),
		Vertex( static_cast< float >( position.x ), static_cast< float >( position.y + size.y ), 0.f, 1.f, color_bottom_left.Hex ),
		Vertex( static_cast< float >( position.x ), static_cast< float >( position.y ), 0.f, 1.f, color_top_left.Hex )
	};

	WriteToBuffer( TRIANGLE, &Vertices, nullptr );
}

void cBuffer::Triangle( Vec2<int16_t> point1, Vec2<int16_t> point2, Vec2<int16_t> point3, Color color ) {
	std::vector<Vertex> Vertices = {
		Vertex( static_cast< float >( point1.x ), static_cast< float >( point1.y ), 0.f, 1.f, color.Hex ),
		Vertex( static_cast< float >( point2.x ), static_cast< float >( point2.y ), 0.f, 1.f, color.Hex ),
		Vertex( static_cast< float >( point3.x ), static_cast< float >( point3.y ), 0.f, 1.f, color.Hex )
	};

	WriteToBuffer( LINE, &Vertices, nullptr );
}

void cBuffer::FilledTriangle( Vec2<int16_t> point1, Vec2<int16_t> point2, Vec2<int16_t> point3, Color color ) {
	std::vector<Vertex> Vertices = {
		Vertex( static_cast< float >( point1.x ), static_cast< float >( point1.y ), 0.f, 1.f, color.Hex ),
		Vertex( static_cast< float >( point2.x ), static_cast< float >( point2.y ), 0.f, 1.f, color.Hex ),
		Vertex( static_cast< float >( point3.x ), static_cast< float >( point3.y ), 0.f, 1.f, color.Hex )
	};

	WriteToBuffer( TRIANGLE, &Vertices, nullptr );
}

void cBuffer::Circle( Vec2<int16_t> pos, float radius, float completion, float rotation, int16_t segments, Color color ) {
	std::vector<Vertex> Vertices;
	Vertices.reserve( segments );

	GenerateArcVertices( &Vertices, &pos, radius, completion, rotation, segments, color, color, false );

	WriteToBuffer( LINE, &Vertices, nullptr );
}

void cBuffer::FilledCircle( Vec2<int16_t> pos, float radius, float completion, float rotation, int16_t segments, Color center_color, Color color ) {
	std::vector<Vertex> Vertices;
	Vertices.reserve( segments );
	
	GenerateArcVertices( &Vertices, &pos, radius, completion, rotation, segments, center_color, color, true );

	WriteToBuffer( TRIANGLE_FAN, &Vertices, nullptr );
}

void cBuffer::Text( Font* font, const std::string& str, Vec2<int16_t> pos, Color color ) {
	if ( !font->Valid || str.empty( ) )
		return;

	Vec2<int16_t> Advance = { 0, 0 }, TextSize = GetTextSize( font, str );
	Vec2<float> GlyphPos = { 0.f, 0.f };
	int RowHeight = 0;

	for ( const auto& Char : str ) {
		if ( !std::isprint( Char ) && Char != '\n' || Char == ' ' ) {
			Advance.x += font->CharSet[ Char ].Advance / 64;
			continue;
		}

		auto* Glyph = &font->CharSet[ Char ];
		RowHeight = max( RowHeight, static_cast< int >( Glyph->Size.y ) );

		if ( Char == '\n' ) {
			Advance = Vec2<int16_t>( 0, Advance.y + RowHeight + font->Padding.y );
			continue;
		}

		GlyphPos = {
			std::round( static_cast< float >( pos.x ) + Advance.x + Glyph->Bearing.x ) + 0.5f,
			std::round( static_cast< float >( pos.y ) + ( TextSize.y * 0.75f ) + Advance.y - Glyph->Bearing.y ) + 0.5f
		};

		std::vector<Vertex> Vertices = {
			{GlyphPos.x, GlyphPos.y, 0.f, 1.f, color.Hex, 0.f, 0.f},
			{GlyphPos.x + Glyph->Size.x, GlyphPos.y, 0.f, 1.f, color.Hex, 1.f, 0.f},
			{GlyphPos.x + Glyph->Size.x, GlyphPos.y + Glyph->Size.y, 0.f, 1.f, color.Hex, 1.f, 1.f},
			{GlyphPos.x, GlyphPos.y + Glyph->Size.y, 0.f, 1.f, color.Hex, 0.f, 1.f}
		};

		PushTexture( Glyph->Texture );
		WriteToBuffer( TRIANGLE, &Vertices, nullptr );
		PopTexture( );

		Advance.x += Glyph->Advance / 64 + font->Padding.x;
	}
}

Vec2<int16_t> cBuffer::GetTextSize( Font* font, const std::string& text ) {
	if ( !font->Valid || text.empty( ) )
		return Vec2<int16_t>( );

	Vec2<int16_t> Size{ 0, static_cast< int16_t >( font->Size * 1.5f ) };

	for ( char c : text )
		Size.x += font->CharSet.at( c ).Advance / 64;

	return Size;
}

void cBuffer::WriteToBuffer( int8_t primitive, const std::vector<Vertex>* vertices, const std::vector<int32_t>* indices ) {
	int VerticesCount = vertices->size( );
	int IndicesCount = indices ? indices->size( ) : VerticesCount * 3 - 1;

	std::vector<int32_t> DynamicIndices( IndicesCount, 0 );

	if ( !indices )
		for ( int i = 0; i < VerticesCount; ++i )
			DynamicIndices[ i ] = i;

	m_VerticesCount += VerticesCount;
	m_IndicesCount += IndicesCount;

	m_DrawCommands.emplace_back( primitive, *vertices, indices ? *indices : std::move( DynamicIndices ),
		m_CommandResources, VerticesCount, IndicesCount );
}