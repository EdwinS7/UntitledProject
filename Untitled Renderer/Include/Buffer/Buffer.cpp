#include "buffer.hpp"

void cBuffer::Init( ) {
	gGraphics->CreateFontFromName( &Fonts.Default, "Segoe UI", 16, 400, 4, true );
	gGraphics->CreateFontFromName( &Fonts.Interface, "Arial", 9, 100, 4, false );
	gGraphics->CreateFontFromName( &Fonts.SmallInterface, "Arial", 8, 100, 4, false );
	gGraphics->CreateTextureFromFile( &Textures.Default, "Default.png" );

	PushClip( gWin32->GetClipRect( ) );
	PushTexture( gBuffer->Textures.Default ); // @note: this is some ghetto bs, please fix lmao
}

void cBuffer::Release( ) {
	Fonts.Default.Release( );
	Fonts.Interface.Release( );
	Fonts.SmallInterface.Release( );

	Textures.Default->Release( );
	Textures.Default = nullptr;

	m_CommandResources.Textures.clear( );
	m_CommandResources.Clips.clear( );

	ClearCommands( );
}

void cBuffer::WriteToBuffer( const int8_t primitive, const std::vector< Vertex >* vertices, const std::vector<int32_t>* indices ) {
	int VerticesCount = vertices->size( ),
		IndicesCount = indices == nullptr ? ( VerticesCount * 3 ) - 1 : indices->size( );

	std::vector < int32_t > DynamicIndices( IndicesCount );

	if ( indices == nullptr )
		for ( int32_t i = 0; i < VerticesCount; ++i )
			DynamicIndices[ i ] = i;

	m_VerticesCount += VerticesCount;
	m_IndicesCount += IndicesCount;

	m_DrawCommands.push_back( DrawCommand( 
		primitive, *vertices, indices == nullptr ? DynamicIndices : *indices,
		m_CommandResources, VerticesCount, indices != nullptr ? indices->size( ) : IndicesCount )
	);
}

void cBuffer::BuildDrawCommands( const std::vector<DrawCommand>& draw_commands ) {
	for ( int i = 0; i < draw_commands.size( ); ++i ) {
		auto& DrawCommand = draw_commands[ i ];

		m_DrawCommand.Vertices.insert( m_DrawCommand.Vertices.end( ),
				std::make_move_iterator( DrawCommand.Vertices.begin( ) ), std::make_move_iterator( DrawCommand.Vertices.end( ) )
		);
		
		m_DrawCommand.Indices.insert( m_DrawCommand.Indices.end( ),
			std::make_move_iterator( DrawCommand.Indices.begin( ) ), std::make_move_iterator( DrawCommand.Indices.end( ) )
		);

		m_DrawCommand.VerticesCount += DrawCommand.VerticesCount;
		m_DrawCommand.IndicesCount += DrawCommand.IndicesCount;
	}
}

void cBuffer::Line( const Vec2< int16_t > from, const Vec2< int16_t > to, const Color color ) {
	std::vector<Vertex> Vertices = {
		Vertex( from.x, from.y, 0.f, 1.f, color.Hex ),
		Vertex( to.x, to.y, 0.f, 1.f, color.Hex )
	};

	RotateVertices( &Vertices );
	WriteToBuffer( LINE, &Vertices, nullptr );
}

void cBuffer::Polyline( const std::vector< Vec2< int16_t > > points, const Color color ) {
	std::vector<Vertex> Vertices;
	Vertices.reserve( points.size( ) );

	MakeVertices( &Vertices, &points, &color );
	RotateVertices( &Vertices );

	WriteToBuffer( LINE, &Vertices, nullptr );
}

void cBuffer::Polygon( const std::vector<Vec2<int16_t>> points, const Color color ) {
	std::vector<Vertex> Vertices;
	Vertices.reserve( points.size( ) );

	MakeVertices( &Vertices, &points, &color );

	std::vector<int32_t> Indices;
	Indices.reserve( Vertices.size( ) * 3 );

	for ( int32_t i = 1; i < ( Vertices.size( ) - 2 ) * 3; i++ ) {
		Indices.push_back( 0 );
		Indices.push_back( i );
		Indices.push_back( i + 1 );
	}

	RotateVertices( &Vertices );
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
			Points.push_back( std::get<0>( Corner ) );
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
			Points.push_back( std::get<0>( Corner ) );
	}

	Polygon( Points, color );
}

void cBuffer::TexturedRectangle( LPDIRECT3DTEXTURE9* texture, const Vec2<int16_t> position, const Vec2<int16_t> size, const Color color ) {
	std::vector<Vertex> Vertices = {
		Vertex( position.x, position.y, 0.f, 1.f, color.Hex ),
		Vertex( position.x + size.x, position.y, 0.f, 1.f, color.Hex ),
		Vertex( position.x + size.x, position.y + size.y, 0.f, 1.f, color.Hex ),
		Vertex( position.x + size.x, position.y + size.y, 0.f, 1.f, color.Hex ),
		Vertex( position.x, position.y + size.y, 0.f, 1.f, color.Hex ),
		Vertex( position.x, position.y, 0.f, 1.f, color.Hex )
	};

	PushTexture( *texture );
	RotateVertices( &Vertices );
	WriteToBuffer( TRIANGLE, &Vertices, nullptr );
	PopTexture( );
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

	RotateVertices( &Vertices );
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

	RotateVertices( &Vertices );
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

	RotateVertices( &Vertices );
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

	RotateVertices( &Vertices );
	WriteToBuffer( TRIANGLE, &Vertices, nullptr );
}

void cBuffer::Triangle( const Vec2< int16_t > point1, const Vec2< int16_t > point2, const Vec2< int16_t > point3, const Color color ) {
	std::vector<Vertex> Vertices = {
		Vertex( point1.x, point1.y, 0.f, 1.f, color.Hex ),
		Vertex( point2.x, point2.y, 0.f, 1.f, color.Hex ),
		Vertex( point3.x, point3.y, 0.f, 1.f, color.Hex )
	};

	RotateVertices( &Vertices );
	WriteToBuffer( LINE, &Vertices, nullptr );
}

void cBuffer::FilledTriangle( const Vec2< int16_t > point1, const Vec2< int16_t > point2, const Vec2< int16_t > point3, const Color color ) {
	std::vector<Vertex> Vertices = {
		Vertex( point1.x, point1.y, 0.f, 1.f, color.Hex ),
		Vertex( point2.x, point2.y, 0.f, 1.f, color.Hex ),
		Vertex( point3.x, point3.y, 0.f, 1.f, color.Hex )
	};

	RotateVertices( &Vertices );
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

void cBuffer::String( const Font* font, const std::string& str, const Vec2<int16_t> pos, const Color color ) {
	if ( !font || str.empty( ) )
		return;

	Vec2<int16_t> advance = { 0, 0 };
	int rowHeight = 0;

	for ( const auto& character : str ) {
		if ( !std::isprint( character ) && character != '\n' || character == ' ' ) {
			advance.x += font->CharSet[ character ].Advance / 64;
			continue;
		}

		const auto* glyph = &font->CharSet[ character ];
		rowHeight = max( rowHeight, static_cast< int >( glyph->Size.y ) );

		if ( character == '\n' ) {
			advance.y += rowHeight + font->Padding;
			advance.x = 0;
			continue;
		}

		Vec2<float> letterPos = {
			std::round( static_cast< float >( pos.x ) + advance.x + glyph->Bearing.x ) + 0.5f,
			std::round( static_cast< float >( pos.y ) + ( GetStringSize( font, str ).y * 0.75f ) + advance.y - glyph->Bearing.y ) + 0.5f
		};

		std::vector<Vertex> vertices = {
			{letterPos.x, letterPos.y, 0.f, 1.f, color.Hex, 0.f, 0.f},
			{letterPos.x + glyph->Size.x, letterPos.y, 0.f, 1.f, color.Hex, 1.f, 0.f},
			{letterPos.x + glyph->Size.x, letterPos.y + glyph->Size.y, 0.f, 1.f, color.Hex, 1.f, 1.f},
			{letterPos.x, letterPos.y + glyph->Size.y, 0.f, 1.f, color.Hex, 0.f, 1.f}
		};

		PushTexture( glyph->Texture );
		WriteToBuffer( TRIANGLE, &vertices, nullptr );
		PopTexture( );

		advance.x += glyph->Advance / 64;
	}
}

Vec2<int16_t> cBuffer::GetStringSize( const Font* font, const std::string& string ) {
	if ( !font || string.empty( ) )
		return Vec2<int16_t>( );

	Vec2<int16_t> Size{ 0, static_cast< int16_t >( font->Size * 1.5f ) };

	for ( char Letter : string )
		Size.x += font->CharSet.at( Letter ).Advance / 64;

	return Size;
}

void cBuffer::GenerateArcPoints( std::vector<Vec2<int16_t>>* Points, const Vec2<int16_t>* position, const int16_t radius, const int16_t completion, const int16_t rotation, const int16_t segments ) {
	double Angle = ( static_cast< double >( rotation ) * M_PI ) / 180.0;
	double Conversion = completion * 0.01;

	int16_t SegmentCount = m_DynamicArcSegments ? segments > radius ? radius : max( segments, 8 ) : segments;
	if ( SegmentCount )

	Points->reserve( SegmentCount + 1 );

	auto GetPoint = [ & ] ( int16_t i ) {
		double Theta = Angle + 2.0 * Conversion * M_PI * static_cast< double >( i ) / static_cast< double >( SegmentCount );
		return Vec2<double>( static_cast< double >( position->x ) + radius * cos( Theta ), static_cast< double >( position->y ) + radius * sin( Theta ) );
	};

	for ( int i = 0; i < SegmentCount; i++ ) {
		Vec2<double> point = GetPoint( i );

		Points->push_back( Vec2<int16_t>( std::round( point.x ), std::round( point.y ) ) );
	}
}

void cBuffer::GenerateQuadraticBezierPoints( std::vector<Vec2<int16_t>>* points, const Vec2<int16_t> point1, const Vec2<int16_t> point2, const Vec2<int16_t> point3 ) {
	for ( int i = 0; i < m_BezierQuadraticSegments; i++ ) {
		int CompletionFactor = static_cast< double >( i ) / static_cast< double >( m_BezierQuadraticSegments );
		
		points->push_back( {
			static_cast< int16_t >( std::round( std::lerp( std::lerp( point1.x, point3.x, CompletionFactor ), std::lerp( point3.x, point2.x, CompletionFactor ), CompletionFactor ) ) ),
			static_cast< int16_t >( std::round( std::lerp( std::lerp( point1.y, point3.y, CompletionFactor ), std::lerp( point3.y, point2.y, CompletionFactor ), CompletionFactor ) ) )
		} );
	}
}

void cBuffer::MakeVertices( std::vector<Vertex>* vertices, const std::vector<Vec2<int16_t>>* points, const Color* color ) {
	for ( const Vec2<int16_t>& point : *points )
		vertices->push_back( Vertex( point.x, point.y, 0.f, 1.f, color->Hex ) );
}

void cBuffer::RotateObject( float val ) {
	m_Rotation = val;
}

void cBuffer::RotateVertices( std::vector<Vertex>* vertices, Vec2<int16_t> center ) {
	if ( vertices->empty( ) )
		return;

	Vec2<int16_t> Center( 0, 0 );

	for ( const auto& Vertex : *vertices ) {
		Center.x += Vertex.x;
		Center.y += Vertex.y;
	}
	Center.x /= static_cast< float >( vertices->size( ) );
	Center.y /= static_cast< float >( vertices->size( ) );

SKIP_CENTER:

	float Radians = m_Rotation * M_PI / 180.0f;
	float CosValue = cosf( Radians );
	float SinValue = sinf( Radians );

	for ( auto& vertex : *vertices ) {
		float translatedX = vertex.x - Center.x;
		float translatedY = vertex.y - Center.y;

		Vec2<float> NewPos = {
			translatedX * CosValue - translatedY * SinValue,
			translatedX * SinValue + translatedY * CosValue
		};

		vertex.x = NewPos.x + Center.x;
		vertex.y = NewPos.y + Center.y;
	}
}