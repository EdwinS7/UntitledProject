#include "Buffer.hpp"

void cBuffer::Init( ) {
	gGraphics->CreateFontFromName( &DefaultFont, "Arial", 12, 400, Vec2<int16_t>( 0, 0 ), false );

	PushClip( gWindow->GetClipRect( ) );
	PushTexture( nullptr );

	gWrapper->RunCallback( "OnObjectCreation" );
}

void cBuffer::Release( ) {
	m_CommandResources.TextureStack.clear( );
	m_CommandResources.ClipStack.clear( );

	ClearCommands( );
}

void cBuffer::Line( const Vec2< int16_t > from, const Vec2< int16_t > to, const Color color ) {
	std::vector<Vertex> Vertices = {
		Vertex( from.x, from.y, 0.f, 1.f, color.Hex ),
		Vertex( to.x, to.y, 0.f, 1.f, color.Hex )
	};

	WriteToBuffer( LINE, &Vertices, nullptr );
}

void cBuffer::Polyline( const std::vector<Vec2<int16_t>>& points, const Color color ) {
	std::vector<Vertex> Vertices;
	Vertices.reserve( points.size( ) );

	MakeVertices( &Vertices, &points, &color );
	WriteToBuffer( LINE, &Vertices, nullptr );
}

void cBuffer::Polygon( const std::vector<Vec2<int16_t>>& points, const Color color ) {
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

void cBuffer::Rectangle( const Vec2< int16_t > position, const Vec2< int16_t > size, const Color color, const int16_t rounding, const CornerFlags flags ) {
	const bool RoundTopLeft = ( flags & CornerTopLeft ) && rounding;
	const bool RoundTopRight = ( flags & CornerTopRight ) && rounding;
	const bool RoundBottomLeft = ( flags & CornerBottomLeft ) && rounding;
	const bool RoundBottomRight = ( flags & CornerBottomRight ) && rounding;

	std::vector<Vec2<int16_t>> Points;

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

			std::vector<Vec2<int16_t>> CornerPoints;
			CornerPoints.reserve( m_RectangleSegments + 1 );

			GenerateArcPoints( &CornerPoints, &CornerRounded, rounding, 25, Angle, m_RectangleSegments );

			Points.insert( Points.end( ),
				std::make_move_iterator( CornerPoints.begin( ) ), std::make_move_iterator( CornerPoints.end( ) )
			);
		}
		else
			Points.emplace_back( std::get<0>( Corner ) );
	}

	Polyline( Points, color );
}

void cBuffer::FilledRectangle( const Vec2< int16_t > position, const Vec2< int16_t > size, const Color color, const int16_t rounding, const CornerFlags flags ) {
	const bool RoundTopLeft = ( flags & CornerTopLeft ) && rounding;
	const bool RoundTopRight = ( flags & CornerTopRight ) && rounding;
	const bool RoundBottomLeft = ( flags & CornerBottomLeft ) && rounding;
	const bool RoundBottomRight = ( flags & CornerBottomRight ) && rounding;

	std::vector<Vec2<int16_t>> Points;

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

			std::vector<Vec2<int16_t>> CornerPoints;
			CornerPoints.reserve( m_RectangleSegments + 1 );

			GenerateArcPoints( &CornerPoints, &CornerRounded, rounding, 25, Angle, m_RectangleSegments );

			Points.insert( Points.end( ),
				std::make_move_iterator( CornerPoints.begin( ) ), std::make_move_iterator( CornerPoints.end( ) )
			);
		}
		else
			Points.emplace_back( std::get<0>( Corner ) );
	}

	Polygon( Points, color );
}

void cBuffer::TexturedRectangle( IDirect3DTexture9* texture, const Vec2<int16_t> position, const Vec2<int16_t> size, const Color color_modulation ) {
	std::vector<Vertex> Vertices = {
		Vertex( position.x, position.y, 0.f, 1.f, color_modulation.Hex ),
		Vertex( position.x + size.x, position.y, 0.f, 1.f, color_modulation.Hex ),
		Vertex( position.x + size.x, position.y + size.y, 0.f, 1.f, color_modulation.Hex ),
		Vertex( position.x + size.x, position.y + size.y, 0.f, 1.f, color_modulation.Hex ),
		Vertex( position.x, position.y + size.y, 0.f, 1.f, color_modulation.Hex ),
		Vertex( position.x, position.y, 0.f, 1.f, color_modulation.Hex )
	};

	PushTexture( texture );
	WriteToBuffer( TRIANGLE, &Vertices, nullptr );
	PopTexture( );
}

void cBuffer::Picture( Image* image, const Vec2<int16_t> position, const Color color_modulation ) {
	std::vector<Vertex> Vertices = {
		Vertex( position.x, position.y, 0.f, 1.f, color_modulation.Hex ),
		Vertex( position.x + image->Size.x, position.y, 0.f, 1.f, color_modulation.Hex ),
		Vertex( position.x + image->Size.x, position.y + image->Size.y, 0.f, 1.f, color_modulation.Hex ),
		Vertex( position.x + image->Size.x, position.y + image->Size.y, 0.f, 1.f, color_modulation.Hex ),
		Vertex( position.x, position.y + image->Size.y, 0.f, 1.f, color_modulation.Hex ),
		Vertex( position.x, position.y, 0.f, 1.f, color_modulation.Hex )
	};

	PushTexture( image->Texture );
	WriteToBuffer( TRIANGLE, &Vertices, nullptr );
	PopTexture( );
}

void cBuffer::Cube( const Vec3<float> position, float size, const Color color ) {
	float halfSize = size / 2.0f;

	std::vector<Vertex> Vertices = {
		// Front face
		Vertex( position.x - halfSize, position.y - halfSize, position.z + halfSize, 1.f, color.Hex ),
		Vertex( position.x + halfSize, position.y - halfSize, position.z + halfSize, 1.f, color.Hex ),
		Vertex( position.x + halfSize, position.y + halfSize, position.z + halfSize, 1.f, color.Hex ),
		Vertex( position.x - halfSize, position.y + halfSize, position.z + halfSize, 1.f, color.Hex ),
		// Back face
		Vertex( position.x - halfSize, position.y - halfSize, position.z - halfSize, 1.f, color.Hex ),
		Vertex( position.x + halfSize, position.y - halfSize, position.z - halfSize, 1.f, color.Hex ),
		Vertex( position.x + halfSize, position.y + halfSize, position.z - halfSize, 1.f, color.Hex ),
		Vertex( position.x - halfSize, position.y + halfSize, position.z - halfSize, 1.f, color.Hex )
	};

	// Define the indices to create triangles (or quads) from the vertices
	std::vector<int32_t> Indices = {
		// Front face
		0, 1, 2, 2, 3, 0,
		// Right face
		1, 5, 6, 6, 2, 1,
		// Back face
		5, 4, 7, 7, 6, 5,
		// Left face
		4, 0, 3, 3, 7, 4,
		// Top face
		3, 2, 6, 6, 7, 3,
		// Bottom face
		4, 5, 1, 1, 0, 4
	};

	WriteToBuffer( LINE, &Vertices, &Indices );
}


void cBuffer::Gradient( const Vec2< int16_t > position, const Vec2< int16_t > size, const Color color1, const Color color2, const bool vertical ) {
	std::vector<Vertex> Vertices = {
		Vertex( position.x, position.y, 0.f, 1.f, color1.Hex ),
		Vertex( position.x + size.x, position.y, 0.f, 1.f, vertical ? color1.Hex : color2.Hex ),
		Vertex( position.x + size.x, position.y + size.y, 0.f, 1.f, color2.Hex ),
		Vertex( position.x + size.x, position.y + size.y, 0.f, 1.f, color2.Hex ),
		Vertex( position.x, position.y + size.y, 0.f, 1.f, vertical ? color2.Hex : color1.Hex ),
		Vertex( position.x, position.y, 0.f, 1.f, color1.Hex )
	};

	WriteToBuffer( LINE, &Vertices, nullptr );
}

void cBuffer::FilledGradient( const Vec2< int16_t > position, const Vec2< int16_t > size, const Color color1, const Color color2, const bool vertical ) {
	std::vector<Vertex> Vertices = {
		Vertex( position.x, position.y, 0.f, 1.f, color1.Hex ),
		Vertex( position.x + size.x, position.y, 0.f, 1.f, vertical ? color1.Hex : color2.Hex ),
		Vertex( position.x + size.x, position.y + size.y, 0.f, 1.f, color2.Hex ),
		Vertex( position.x + size.x, position.y + size.y, 0.f, 1.f, color2.Hex ),
		Vertex( position.x, position.y + size.y, 0.f, 1.f, vertical ? color2.Hex : color1.Hex ),
		Vertex( position.x, position.y, 0.f, 1.f, color1.Hex )
	};

	WriteToBuffer( TRIANGLE, &Vertices, nullptr );
}

void cBuffer::Gradient( const Vec2<int16_t> position, const Vec2<int16_t> size, const Color color_top_left, const Color color_top_right, const Color color_bottom_right, const Color color_bottom_left ) {
	std::vector<Vertex> Vertices = {
		Vertex( position.x, position.y, 0.f, 1.f, color_top_left.Hex ),
		Vertex( position.x + size.x, position.y, 0.f, 1.f, color_top_right.Hex ),
		Vertex( position.x + size.x, position.y + size.y, 0.f, 1.f, color_bottom_right.Hex ),
		Vertex( position.x + size.x, position.y + size.y, 0.f, 1.f, color_bottom_right.Hex ),
		Vertex( position.x, position.y + size.y, 0.f, 1.f, color_bottom_left.Hex ),
		Vertex( position.x, position.y, 0.f, 1.f, color_top_left.Hex )
	};

	WriteToBuffer( LINE, &Vertices, nullptr );
}

void cBuffer::FilledGradient( const Vec2<int16_t> position, const Vec2<int16_t> size, const Color color_top_left, const Color color_top_right, const Color color_bottom_right, const Color color_bottom_left ) {
	std::vector<Vertex> Vertices = {
		Vertex( position.x, position.y, 0.f, 1.f, color_top_left.Hex ),
		Vertex( position.x + size.x, position.y, 0.f, 1.f, color_top_right.Hex ),
		Vertex( position.x + size.x, position.y + size.y, 0.f, 1.f, color_bottom_right.Hex ),
		Vertex( position.x + size.x, position.y + size.y, 0.f, 1.f, color_bottom_right.Hex ),
		Vertex( position.x, position.y + size.y, 0.f, 1.f, color_bottom_left.Hex ),
		Vertex( position.x, position.y, 0.f, 1.f, color_top_left.Hex )
	};

	WriteToBuffer( TRIANGLE, &Vertices, nullptr );
}

void cBuffer::Triangle( const Vec2< int16_t > point1, const Vec2< int16_t > point2, const Vec2< int16_t > point3, const Color color ) {
	std::vector<Vertex> Vertices = {
		Vertex( point1.x, point1.y, 0.f, 1.f, color.Hex ),
		Vertex( point2.x, point2.y, 0.f, 1.f, color.Hex ),
		Vertex( point3.x, point3.y, 0.f, 1.f, color.Hex )
	};

	WriteToBuffer( LINE, &Vertices, nullptr );
}

void cBuffer::FilledTriangle( const Vec2< int16_t > point1, const Vec2< int16_t > point2, const Vec2< int16_t > point3, const Color color ) {
	std::vector<Vertex> Vertices = {
		Vertex( point1.x, point1.y, 0.f, 1.f, color.Hex ),
		Vertex( point2.x, point2.y, 0.f, 1.f, color.Hex ),
		Vertex( point3.x, point3.y, 0.f, 1.f, color.Hex )
	};

	WriteToBuffer( TRIANGLE, &Vertices, nullptr );
}

void cBuffer::Circle( const Vec2< int16_t > pos, const int16_t radius, const Color color ) {
	std::vector<Vec2<int16_t>> points;
	points.reserve( m_CircleSegments + 1 );

	GenerateArcPoints( &points, &pos, radius, 100, 0, m_CircleSegments );
	Polyline( points, color );
}

void cBuffer::FilledCircle( const Vec2< int16_t > pos, const int16_t radius, const Color color ) {
	std::vector<Vec2<int16_t>> points;
	points.reserve( m_CircleSegments + 1 );

	GenerateArcPoints( &points, &pos, radius, 100, 0, m_CircleSegments );
	Polygon( points, color );
}

void cBuffer::Text( const Font* font, const std::string& str, const Vec2<int16_t> pos, const Color color ) {
	if ( !font->Valid || str.empty( ) || ( ( ( color.Hex >> 24 ) & 0xFF ) ) < 1 )
		return;

	Vec2<int16_t> Advance = { 0, 0 }, TextSize = GetTextSize( font, str );
	int RowHeight = 0;

	for ( const auto& Char : str ) {
		if ( !std::isprint( Char ) && Char != '\n' || Char == ' ' ) {
			Advance.x += font->CharSet[ Char ].Advance / 64;
			continue;
		}

		const auto* glyph = &font->CharSet[ Char ];
		RowHeight = max( RowHeight, static_cast< int >( glyph->Size.y ) );

		if ( Char == '\n' ) {
			Advance = Vec2<int16_t>( 0, Advance.y + RowHeight + font->Padding.y );
			continue;
		}

		Vec2<float> Pos = {
			std::round( static_cast< float >( pos.x ) + Advance.x + glyph->Bearing.x ) + 0.5f,
			std::round( static_cast< float >( pos.y ) + ( TextSize.y * 0.75f ) + Advance.y - glyph->Bearing.y ) + 0.5f
		};

		std::vector<Vertex> Vertices = {
			{Pos.x, Pos.y, 0.f, 1.f, color.Hex, 0.f, 0.f},
			{Pos.x + glyph->Size.x, Pos.y, 0.f, 1.f, color.Hex, 1.f, 0.f},
			{Pos.x + glyph->Size.x, Pos.y + glyph->Size.y, 0.f, 1.f, color.Hex, 1.f, 1.f},
			{Pos.x, Pos.y + glyph->Size.y, 0.f, 1.f, color.Hex, 0.f, 1.f}
		};

		PushTexture( glyph->Texture );
		WriteToBuffer( TRIANGLE, &Vertices, nullptr );
		PopTexture( );

		Advance.x += glyph->Advance / 64 + font->Padding.x;
	}
}