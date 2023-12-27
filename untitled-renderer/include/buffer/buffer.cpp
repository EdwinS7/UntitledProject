#include "buffer.hpp"

void cBuffer::CreateObjects( ) {
	RECT win32_size = RECT( 0, 0, gWin32->GetSize( ).x, gWin32->GetSize( ).y );

	PushClip( win32_size );
	PushTexture( nullptr );

	gFont->Create( &Fonts.Default, "Segoe UI", 16, 400, 4, true );
}

void cBuffer::DestroyObjects( ) {
	Fonts.Default.Release( );

	m_command.textures.clear( );
	m_command.clips.clear( );
}

// __BUFFER__

void cBuffer::WriteToBuffer( const int8_t primitive, const std::vector< Vertex >* vertices, const std::vector<int32_t>* indices ) {
	int vertices_count = vertices->size( ),
		indices_count = indices == nullptr ? (vertices_count * 3) - 1 : indices->size();

	std::vector < int32_t > dynamic_indices( indices_count );

	if ( indices == nullptr )
		for ( int32_t i = 0; i < vertices_count; ++i )
			dynamic_indices[ i ] = i;

	m_vertices_count += vertices_count;
	m_indices_count += indices_count;

	m_draw_commands.push_back( DrawCommand( primitive, *vertices, indices == nullptr ? dynamic_indices : *indices, m_command, vertices_count, indices != nullptr ? indices->size( ) : indices_count ) );
}

void cBuffer::BuildDrawCommands( const std::vector<DrawCommand>& draw_commands ) {
	for ( int i = 0; i < draw_commands.size( ); ++i ) {
		auto& draw_command = draw_commands[ i ];

		m_draw_command.vertices.insert( m_draw_command.vertices.end( ),
				std::make_move_iterator( draw_command.vertices.begin( ) ), std::make_move_iterator( draw_command.vertices.end( ) )
		);

		m_draw_command.indices.insert( m_draw_command.indices.end( ),
			std::make_move_iterator( draw_command.indices.begin( ) ), std::make_move_iterator( draw_command.indices.end( ) )
		);

		m_draw_command.vertices_count += draw_command.vertices_count;
		m_draw_command.indices_count += draw_command.indices_count;
	}
}

// __SHAPES__

void cBuffer::Line( const Vec2< int16_t > from, const Vec2< int16_t > to, const Color clr ) {
	std::vector<Vertex> vertices{
		Vertex( from.x, from.y, 0.f, 1.f, clr.hex ),
		Vertex( to.x, to.y, 0.f, 1.f, clr.hex )
	};

	rotate_object( &vertices );
	WriteToBuffer( LINE, &vertices, nullptr);
}

void cBuffer::Polyline( const std::vector< Vec2< int16_t > > points, const Color clr ) {
	std::vector<Vertex> vertices;
	vertices.reserve( points.size( ) );

	make_vertices( &vertices, &points, &clr );

	rotate_object( &vertices );
	WriteToBuffer( LINE, &vertices, nullptr );
}

void cBuffer::Polygon( const std::vector<Vec2<int16_t>> points, const Color clr ) {
	std::vector<Vertex> vertices;
	vertices.reserve( points.size( ) );

	make_vertices( &vertices, &points, &clr );

	std::vector<int32_t> indices;
	indices.reserve( vertices.size( ) * 3 );

	for ( int32_t i = 1; i < ( vertices.size( ) - 2 ) * 3; i++ ) {
		indices.push_back( 0 );
		indices.push_back( i );
		indices.push_back( i + 1 );
	}

	rotate_object( &vertices );
	WriteToBuffer( TRIANGLE, &vertices, &indices );
}

void cBuffer::Rectangle( const Vec2< int16_t > pos, const Vec2< int16_t > size, const Color clr, const int16_t rounding, const corner_flags flags ) {
	const bool round_top_left = ( flags & corner_top_left ) && rounding;
	const bool round_top_right = ( flags & corner_top_right ) && rounding;
	const bool round_bottom_left = ( flags & corner_bottom_left ) && rounding;
	const bool round_bottom_right = ( flags & corner_bottom_right ) && rounding;

	std::vector<Vec2<int16_t>> points;

	std::initializer_list<std::tuple<Vec2<int16_t>, Vec2<int16_t>, int, bool>> gen_points = {
		std::tuple{Vec2<int16_t>( pos.x, pos.y ), Vec2<int16_t>( pos.x + rounding, pos.y + rounding ), 180, round_top_left},
		std::tuple{Vec2<int16_t>( pos.x + size.x, pos.y ), Vec2<int16_t>( pos.x + size.x - rounding, pos.y + rounding ), 270, round_top_right},
		std::tuple{Vec2<int16_t>( pos.x + size.x, pos.y + size.y ), Vec2<int16_t>( pos.x + size.x - rounding, pos.y + size.y - rounding ), 0, round_bottom_right},
		std::tuple{Vec2<int16_t>( pos.x, pos.y + size.y ), Vec2<int16_t>( pos.x + rounding, pos.y + size.y - rounding ), 90, round_bottom_left},
		std::tuple{Vec2<int16_t>( pos.x, pos.y ), Vec2<int16_t>( pos.x + rounding, pos.y + rounding ), 180, round_top_left}
	};

	for ( const std::tuple<Vec2<int16_t>, Vec2<int16_t>, int, bool>& corner_tuple : gen_points ) {
		bool should_round = std::get<3>( corner_tuple );

		if ( should_round ) {
			Vec2<int16_t> corner_rounded = std::get<1>( corner_tuple );
			int angle = std::get<2>( corner_tuple );

			std::vector<Vec2<int16_t>> corner_points;
			corner_points.reserve( RECTANGLE_SEGMENTS + 1 );

			generate_arc_points( &corner_points, &corner_rounded, rounding, 25, angle, RECTANGLE_SEGMENTS );

			points.insert( points.end( ), 
				std::make_move_iterator( corner_points.begin( ) ), std::make_move_iterator( corner_points.end( ) )
			);
		}
		else {
			Vec2<int16_t> corner = std::get<0>( corner_tuple );

			points.push_back( corner );
		}
	}

	Polyline( points, clr );
}

void cBuffer::FilledRectangle( const Vec2< int16_t > pos, const Vec2< int16_t > size, const Color clr, const int16_t rounding, const corner_flags flags ) {
	const bool round_top_left = ( flags & corner_top_left ) && rounding;
	const bool round_top_right = ( flags & corner_top_right ) && rounding;
	const bool round_bottom_left = ( flags & corner_bottom_left ) && rounding;
	const bool round_bottom_right = ( flags & corner_bottom_right ) && rounding;

	std::vector<Vec2<int16_t>> points;

	std::initializer_list<std::tuple<Vec2<int16_t>, Vec2<int16_t>, int, bool>> gen_points = {
		std::tuple{Vec2<int16_t>( pos.x, pos.y ), Vec2<int16_t>( pos.x + rounding, pos.y + rounding ), 180, round_top_left},
		std::tuple{Vec2<int16_t>( pos.x + size.x, pos.y ), Vec2<int16_t>( pos.x + size.x - rounding, pos.y + rounding ), 270, round_top_right},
		std::tuple{Vec2<int16_t>( pos.x + size.x, pos.y + size.y ), Vec2<int16_t>( pos.x + size.x - rounding, pos.y + size.y - rounding ), 0, round_bottom_right},
		std::tuple{Vec2<int16_t>( pos.x, pos.y + size.y ), Vec2<int16_t>( pos.x + rounding, pos.y + size.y - rounding ), 90, round_bottom_left},
		std::tuple{Vec2<int16_t>( pos.x, pos.y ), Vec2<int16_t>( pos.x + rounding, pos.y + rounding ), 180, round_top_left}
	};

	for ( const std::tuple<Vec2<int16_t>, Vec2<int16_t>, int, bool>& corner_tuple : gen_points ) {
		bool should_round = std::get<3>( corner_tuple );

		if ( should_round ) {
			Vec2<int16_t> corner_rounded = std::get<1>( corner_tuple );

			int angle = std::get<2>( corner_tuple );

			std::vector<Vec2<int16_t>> corner_points;
			corner_points.reserve( RECTANGLE_SEGMENTS + 1 );

			generate_arc_points( &corner_points, &corner_rounded, rounding, 25, angle, RECTANGLE_SEGMENTS );

			points.insert( points.end( ),
				std::make_move_iterator( corner_points.begin( ) ), std::make_move_iterator( corner_points.end( ) )
			);
		}
		else {
			Vec2<int16_t> corner = std::get<0>( corner_tuple );

			points.push_back( corner );
		}
	}

	Polygon( points, clr );
}

// @note: finish another time.
void cBuffer::TexturedRectangle( IDirect3DTexture9* resource, const Vec2<int16_t> pos, const Vec2<int16_t> size, const Color clr ) {
	std::vector<Vertex> vertices{
		Vertex( pos.x, pos.y, 0.f, 1.f, clr.hex ),
		Vertex( pos.x + size.x, pos.y, 0.f, 1.f, clr.hex ),
		Vertex( pos.x + size.x, pos.y + size.y, 0.f, 1.f, clr.hex ),
		Vertex( pos.x + size.x, pos.y + size.y, 0.f, 1.f, clr.hex ),
		Vertex( pos.x, pos.y + size.y, 0.f, 1.f, clr.hex ),
		Vertex( pos.x, pos.y, 0.f, 1.f, clr.hex )
	};


	//push_texture( *resource );
	rotate_object( &vertices );
	WriteToBuffer( TRIANGLE, &vertices, nullptr );
	//pop_texture( );
}

void cBuffer::Gradient( const Vec2< int16_t > pos, const Vec2< int16_t > size, const Color clr, const Color clr2, const bool vertical ) {
	FilledGradient( pos, Vec2< int16_t >( size.x, 1 ), clr, vertical ? clr : clr2, false );
	FilledGradient( Vec2< int16_t >( pos.x + size.x, pos.y ), Vec2< int16_t >( 1, size.y + 1 ), vertical ? clr : clr2, clr2, true );
	FilledGradient( Vec2< int16_t >( pos.x, pos.y + size.y ), Vec2< int16_t >( size.x, 1 ), clr, vertical ? clr : clr2, false );
	FilledGradient( pos, Vec2< int16_t >( 1, size.y ), clr, vertical ? clr2 : clr, true );
}

void cBuffer::FilledGradient( const Vec2< int16_t > pos, const Vec2< int16_t > size, const Color clr, const Color clr2, const bool vertical ) {
	std::vector<Vertex> vertices {
		Vertex( pos.x, pos.y, 0.f, 1.f, clr.hex ),
		Vertex( pos.x + size.x, pos.y, 0.f, 1.f, vertical ? clr.hex : clr2.hex ),
		Vertex( pos.x + size.x, pos.y + size.y, 0.f, 1.f, clr2.hex ),
		Vertex( pos.x + size.x, pos.y + size.y, 0.f, 1.f, clr2.hex ),
		Vertex( pos.x, pos.y + size.y, 0.f, 1.f, vertical ? clr2.hex : clr.hex ),
		Vertex( pos.x, pos.y, 0.f, 1.f, clr.hex )
	};

	rotate_object( &vertices );
	WriteToBuffer( TRIANGLE, &vertices, nullptr );
}

void cBuffer::Gradient( const Vec2< int16_t > pos, const Vec2< int16_t > size, const Color clr, const Color clr2, const Color clr3, const Color clr4 ) {
	std::vector<Vertex> vertices{
		Vertex( pos.x, pos.y, 0.f, 1.f, clr.hex ),
		Vertex( pos.x + size.x, pos.y, 0.f, 1.f, clr2.hex ),
		Vertex( pos.x + size.x, pos.y + size.y, 0.f, 1.f, clr3.hex ),
		Vertex( pos.x + size.x, pos.y + size.y, 0.f, 1.f, clr3.hex ),
		Vertex( pos.x, pos.y + size.y, 0.f, 1.f, clr4.hex ),
		Vertex( pos.x, pos.y, 0.f, 1.f, clr.hex )
	};

	rotate_object( &vertices );
	WriteToBuffer( LINE, &vertices, nullptr );
}

void cBuffer::FilledGradient( const Vec2< int16_t > pos, const Vec2< int16_t > size, const Color clr, const Color clr2, const Color clr3, const Color clr4 ) {
	std::vector<Vertex> vertices{
		Vertex( pos.x, pos.y, 0.f, 1.f, clr.hex ),
		Vertex( pos.x + size.x, pos.y, 0.f, 1.f, clr2.hex ),
		Vertex( pos.x + size.x, pos.y + size.y, 0.f, 1.f, clr3.hex ),
		Vertex( pos.x + size.x, pos.y + size.y, 0.f, 1.f, clr3.hex ),
		Vertex( pos.x, pos.y + size.y, 0.f, 1.f, clr4.hex ),
		Vertex( pos.x, pos.y, 0.f, 1.f, clr.hex )
	};

	rotate_object( &vertices );
	WriteToBuffer( TRIANGLE, &vertices, nullptr );
}

void cBuffer::Triangle( const Vec2< int16_t > p1, const Vec2< int16_t > p2, const Vec2< int16_t > p3, const Color clr ) {
	std::vector<Vertex> vertices{
		Vertex( p1.x, p1.y, 0.f, 1.f, clr.hex ),
		Vertex( p2.x, p2.y, 0.f, 1.f, clr.hex ),
		Vertex( p3.x, p3.y, 0.f, 1.f, clr.hex )
	};

	rotate_object( &vertices );
	WriteToBuffer( LINE, &vertices, nullptr );
}

void cBuffer::FilledTriangle( const Vec2< int16_t > p1, const Vec2< int16_t > p2, const Vec2< int16_t > p3, const Color clr ) {
	std::vector<Vertex> vertices{
		Vertex( p1.x, p1.y, 0.f, 1.f, clr.hex ),
		Vertex( p2.x, p2.y, 0.f, 1.f, clr.hex ),
		Vertex( p3.x, p3.y, 0.f, 1.f, clr.hex )
	};

	rotate_object( &vertices );
	WriteToBuffer( TRIANGLE, &vertices, nullptr );
}

void cBuffer::Circle( const Vec2< int16_t > pos, const int16_t radius, const Color clr ) {
	std::vector<Vec2<int16_t>> points;
	points.reserve( CIRCLE_SEGMENTS + 1 );

	generate_arc_points( &points, &pos, radius, 100, 0, CIRCLE_SEGMENTS );

	Polyline( points, clr );
}

void cBuffer::FilledCircle( const Vec2< int16_t > pos, const int16_t radius, const Color clr ) {
	std::vector<Vec2<int16_t>> points;
	points.reserve( CIRCLE_SEGMENTS + 1 );

	generate_arc_points( &points, &pos, radius, 100, 0, CIRCLE_SEGMENTS );

	Polygon( points, clr );
}

void cBuffer ::String( const Font* font, const char* str, const Vec2<int16_t> pos, const Color clr ) {
	if ( !str || !font )
		return;

	Vec2<int16_t> bounds = GetStringSize( font, str );
	float y_offset = static_cast< float >( pos.y ) + ( bounds.y * 0.75f );

	int y_advance = 0;
	int row_height = 0;
	int advance = 0;

	for ( const char* ptr = str; *ptr; ++ptr ) {
		char letter = *ptr;

		if ( !std::isprint( letter ) && letter != '\n' || letter == ' ' ) {
			advance += font->char_set[ letter ].advance / 64;
			continue;
		}

		const Glyph& glyph = font->char_set[ letter ];
		row_height = max( row_height, static_cast< int >( glyph.size.y ) );

		if ( letter == '\n' ) {
			y_advance += row_height + font->padding;
			advance = 0;
			continue;
		}

		float x = std::round( static_cast< float >( pos.x ) + advance + glyph.bearing.x ) + 0.5f;
		float y = std::round( y_offset + y_advance - glyph.bearing.y ) + 0.5f;

		std::vector<Vertex> vertices = {
			{x, y, 0.f, 1.f, clr.hex, 0.f, 0.f},
			{x + glyph.size.x, y, 0.f, 1.f, clr.hex, 1.f, 0.f},
			{x + glyph.size.x, y + glyph.size.y, 0.f, 1.f, clr.hex, 1.f, 1.f},
			{x, y + glyph.size.y, 0.f, 1.f, clr.hex, 0.f, 1.f}
		};

		PushTexture( glyph.resource );
		WriteToBuffer( TRIANGLE, &vertices, nullptr );
		PopTexture( );

		advance += glyph.advance / 64;
	}
}

// __UTILS__ / __PRIVATE__

Vec2<int16_t> cBuffer::GetStringSize( const Font* font, const char* str ) {
	if ( !str || !font )
		return Vec2<int16_t>( 0, 0 );

	Vec2<int16_t> size{ 0, static_cast< int16_t >( font->size * 1.5f ) };

	for ( const char* ptr = str; *ptr; ++ptr ) {
		char letter = *ptr;
		size.x += font->char_set.at( letter ).advance / 64;
	}

	return size;
}

void cBuffer::generate_arc_points( std::vector<Vec2<int16_t>>* points, const Vec2<int16_t>* pos, const int16_t radius, const int16_t completion, const int16_t rotation, const int16_t segments ) {
	double ang = static_cast< double >( rotation * M_PI ) / 180.0;
	double comp = ( completion * 0.01 );

	auto get_point = [ & ] ( int16_t i ) {
		double theta = ang + 2.0 * comp * M_PI * static_cast< double >( i ) / static_cast< double >( segments );
		return Vec2<double>( static_cast< double >( pos->x ) + radius * cos( theta ), static_cast< double >( pos->y ) + radius * sin( theta ) );
	};

	for ( int i = 0; i <= segments; ++i ) {
		Vec2<double> point = get_point( i );

		points->push_back( Vec2<int16_t>( std::round( point.x ), std::round( point.y ) ) );
	}
}

void cBuffer::GenerateQuadraticBezierPoints( std::vector<Vec2<int16_t>>* points, const Vec2<int16_t> p0, const Vec2<int16_t> p1, const Vec2<int16_t> p2 ) {
	for ( int i = 0; i < BEZIER_QUADRATIC_SEGMENTS; i++ ) {
		double val = static_cast< double >( i ) / static_cast< double >( BEZIER_QUADRATIC_SEGMENTS - 1 );

		points->push_back( {
			static_cast< int16_t >( std::round( std::lerp( std::lerp( p0.x, p2.x, val ), std::lerp( p2.x, p1.x, val ), val ) ) ),
			static_cast< int16_t >( std::round( std::lerp( std::lerp( p0.y, p2.y, val ), std::lerp( p2.y, p1.y, val ), val ) ) )
		} );
	}
}

void cBuffer::make_vertices( std::vector<Vertex>* vertices, const std::vector<Vec2<int16_t>>* points, const Color* clr ) {
	for ( const Vec2<int16_t>& point : *points )
		vertices->push_back( Vertex( point.x, point.y, 0.f, 1.f, clr->hex ) );
}

void cBuffer::RotateObject( float val ) {
	m_rotation = val;
}

void cBuffer::rotate_object( std::vector<Vertex>* vertices, Vec2<int16_t> center_manual ) {
	if ( vertices->empty( ) )
		return;

	Vec2<int16_t> center( 0, 0 );

	/*if ( center_manual != vector2_t<int16_t>( -1, -1 ) ) {
		center = center_manual;
		goto SKIP_CENTER;
	}*/

	for ( const auto& vertex : *vertices ) {
		center.x += vertex.x;
		center.y += vertex.y;
	}
	center.x /= static_cast< float >( vertices->size( ) );
	center.y /= static_cast< float >( vertices->size( ) );

	SKIP_CENTER:

	float radians = m_rotation * M_PI / 180.0f;
	float cos_value = cosf( radians );
	float sin_value = sinf( radians );

	for ( auto& vertex : *vertices ) {
		// Translate the vertex relative to the center
		float translatedX = vertex.x - center.x;
		float translatedY = vertex.y - center.y;

		// Perform rotation around the center
		float new_x = translatedX * cos_value - translatedY * sin_value;
		float new_y = translatedX * sin_value + translatedY * cos_value;

		// Translate the vertex back to its original position
		vertex.x = new_x + center.x;
		vertex.y = new_y + center.y;
	}
}