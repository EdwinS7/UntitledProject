#include "buffer.hpp"

void c_buffer::create_objects( ) {
	RECT win32_size = RECT( 0, 0, g_win32->get_size( ).x, g_win32->get_size( ).y );

	push_clip( win32_size );
	push_texture( nullptr );

	g_font->create_font( &default_font, "Segoe UI", 16, 400, 4, true );
	g_font->create_font( &interface_font, "Verdana", 8, 400, 4, false );

#ifdef UNTITLED_USE_PRE_COMPUTED_SIN_COS
	m_pre_computed_sin.resize( RECTANGLE_SEGMENTS );
	m_pre_computed_cos.resize( RECTANGLE_SEGMENTS );

	for ( int16_t i = 0; i < RECTANGLE_SEGMENTS; i++ ) {
		m_pre_computed_cos[ i ] = cos( static_cast< double >( i ) / static_cast< double >( RECTANGLE_SEGMENTS ) );
		m_pre_computed_sin[ i ] = sin( static_cast< double >( i ) / static_cast< double >( RECTANGLE_SEGMENTS ) );
	}
#endif

	g_console->log( color_t(240, 240, 240, 255), "[ renderer ] created objects\n" );
}

void c_buffer::destroy_objects( ) {
	g_font->release_font( &default_font );
	g_font->release_font( &interface_font );

	m_command.textures.clear( );
	m_command.clips.clear( );

	g_console->log( color_t( 240, 240, 240, 255 ), "[ renderer ] destroyed and released objects\n" );
}

// __BUFFER__

void c_buffer::write_to_buffer( const int8_t primitive, const std::vector< vertex_t >* vertices, const std::vector<int32_t>* indices ) {
	int vertices_count = vertices->size( ),
		indices_count = indices == nullptr ? (vertices_count * 3) - 1 : indices->size();

	std::vector < int32_t > dynamic_indices( indices_count );

	if ( indices == nullptr )
		for ( int32_t i = 0; i < vertices_count; ++i )
			dynamic_indices[ i ] = i;

	m_vertices_count += vertices_count;
	m_indices_count += indices_count;

	m_draw_commands.push_back( draw_command_t( primitive, *vertices, indices == nullptr ? dynamic_indices : *indices, m_command, vertices_count, indices != nullptr ? indices->size( ) : indices_count ) );
}

void c_buffer::build_draw_commands( const std::vector<draw_command_t>& draw_commands ) {
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

void c_buffer::line( const vector2_t< int16_t > from, const vector2_t< int16_t > to, const color_t clr ) {
	std::vector<vertex_t> vertices{
		vertex_t( from.x, from.y, 0.f, 1.f, clr.hex ),
		vertex_t( to.x, to.y, 0.f, 1.f, clr.hex )
	};

	write_to_buffer( LINE, &vertices, nullptr);
}

void c_buffer::polyline( const std::vector< vector2_t< int16_t > > points, const color_t clr ) {
	std::vector<vertex_t> vertices;
	vertices.reserve( points.size( ) );

	make_vertices( &vertices, &points, &clr );

	write_to_buffer( LINE, &vertices, nullptr );
}

void c_buffer::polygon( const std::vector<vector2_t<int16_t>> points, const color_t clr ) {
	std::vector<vertex_t> vertices;
	vertices.reserve( points.size( ) );

	make_vertices( &vertices, &points, &clr );

	std::vector<int32_t> indices;
	indices.reserve( vertices.size( ) * 3 );

	for ( int32_t i = 1; i < ( vertices.size( ) - 2 ) * 3; i++ ) {
		indices.push_back( 0 );
		indices.push_back( i );
		indices.push_back( i + 1 );
	}

	write_to_buffer( TRIANGLE, &vertices, &indices );
}

void c_buffer::rectangle( const vector2_t< int16_t > pos, const vector2_t< int16_t > size, const color_t clr, const int16_t rounding, const corner_flags flags ) {
	const bool round_top_left = ( flags & corner_top_left ) && rounding;
	const bool round_top_right = ( flags & corner_top_right ) && rounding;
	const bool round_bottom_left = ( flags & corner_bottom_left ) && rounding;
	const bool round_bottom_right = ( flags & corner_bottom_right ) && rounding;

	std::vector<vector2_t<int16_t>> points;

	std::initializer_list<std::tuple<vector2_t<int16_t>, vector2_t<int16_t>, int, bool>> gen_points = {
		std::tuple{vector2_t<int16_t>( pos.x, pos.y ), vector2_t<int16_t>( pos.x + rounding, pos.y + rounding ), 180, round_top_left},
		std::tuple{vector2_t<int16_t>( pos.x + size.x, pos.y ), vector2_t<int16_t>( pos.x + size.x - rounding, pos.y + rounding ), 270, round_top_right},
		std::tuple{vector2_t<int16_t>( pos.x + size.x, pos.y + size.y ), vector2_t<int16_t>( pos.x + size.x - rounding, pos.y + size.y - rounding ), 0, round_bottom_right},
		std::tuple{vector2_t<int16_t>( pos.x, pos.y + size.y ), vector2_t<int16_t>( pos.x + rounding, pos.y + size.y - rounding ), 90, round_bottom_left},
		std::tuple{vector2_t<int16_t>( pos.x, pos.y ), vector2_t<int16_t>( pos.x + rounding, pos.y + rounding ), 180, round_top_left}
	};

	for ( const std::tuple<vector2_t<int16_t>, vector2_t<int16_t>, int, bool>& corner_tuple : gen_points ) {
		bool should_round = std::get<3>( corner_tuple );

		if ( should_round ) {
			vector2_t<int16_t> corner_rounded = std::get<1>( corner_tuple );
			int angle = std::get<2>( corner_tuple );

			std::vector<vector2_t<int16_t>> corner_points;
			corner_points.reserve( RECTANGLE_SEGMENTS + 1 );

			generate_arc_points( &corner_points, &corner_rounded, rounding, 25, angle, RECTANGLE_SEGMENTS );

			points.insert( points.end( ), 
				std::make_move_iterator( corner_points.begin( ) ), std::make_move_iterator( corner_points.end( ) )
			);
		}
		else {
			vector2_t<int16_t> corner = std::get<0>( corner_tuple );

			points.push_back( corner );
		}
	}

	polyline( points, clr );
}

void c_buffer::filled_rectangle( const vector2_t< int16_t > pos, const vector2_t< int16_t > size, const color_t clr, const int16_t rounding, const corner_flags flags ) {
	const bool round_top_left = ( flags & corner_top_left ) && rounding;
	const bool round_top_right = ( flags & corner_top_right ) && rounding;
	const bool round_bottom_left = ( flags & corner_bottom_left ) && rounding;
	const bool round_bottom_right = ( flags & corner_bottom_right ) && rounding;

	std::vector<vector2_t<int16_t>> points;

	std::initializer_list<std::tuple<vector2_t<int16_t>, vector2_t<int16_t>, int, bool>> gen_points = {
		std::tuple{vector2_t<int16_t>( pos.x, pos.y ), vector2_t<int16_t>( pos.x + rounding, pos.y + rounding ), 180, round_top_left},
		std::tuple{vector2_t<int16_t>( pos.x + size.x, pos.y ), vector2_t<int16_t>( pos.x + size.x - rounding, pos.y + rounding ), 270, round_top_right},
		std::tuple{vector2_t<int16_t>( pos.x + size.x, pos.y + size.y ), vector2_t<int16_t>( pos.x + size.x - rounding, pos.y + size.y - rounding ), 0, round_bottom_right},
		std::tuple{vector2_t<int16_t>( pos.x, pos.y + size.y ), vector2_t<int16_t>( pos.x + rounding, pos.y + size.y - rounding ), 90, round_bottom_left},
		std::tuple{vector2_t<int16_t>( pos.x, pos.y ), vector2_t<int16_t>( pos.x + rounding, pos.y + rounding ), 180, round_top_left}
	};

	for ( const std::tuple<vector2_t<int16_t>, vector2_t<int16_t>, int, bool>& corner_tuple : gen_points ) {
		bool should_round = std::get<3>( corner_tuple );

		if ( should_round ) {
			vector2_t<int16_t> corner_rounded = std::get<1>( corner_tuple );

			int angle = std::get<2>( corner_tuple );

			std::vector<vector2_t<int16_t>> corner_points;
			corner_points.reserve( RECTANGLE_SEGMENTS + 1 );

			generate_arc_points( &corner_points, &corner_rounded, rounding, 25, angle, RECTANGLE_SEGMENTS );

			points.insert( points.end( ),
				std::make_move_iterator( corner_points.begin( ) ), std::make_move_iterator( corner_points.end( ) )
			);
		}
		else {
			vector2_t<int16_t> corner = std::get<0>( corner_tuple );

			points.push_back( corner );
		}
	}

	polygon( points, clr );
}

// @note: finish another time.
void c_buffer::textured_rectangle( texture* resource, const vector2_t<int16_t> pos, const vector2_t<int16_t> size, const color_t clr ) {
	std::vector<vertex_t> vertices{
		vertex_t( pos.x, pos.y, 0.f, 1.f, clr.hex ),
		vertex_t( pos.x + size.x, pos.y, 0.f, 1.f, clr.hex ),
		vertex_t( pos.x + size.x, pos.y + size.y, 0.f, 1.f, clr.hex ),
		vertex_t( pos.x, pos.y + size.y, 0.f, 1.f, clr.hex ),
		vertex_t( pos.x, pos.y, 0.f, 1.f, clr.hex )
	};

	push_texture( *resource );
	write_to_buffer( TRIANGLE, &vertices, nullptr );
	pop_texture( );
}

void c_buffer::gradient( const vector2_t< int16_t > pos, const vector2_t< int16_t > size, const color_t clr, const color_t clr2, const bool vertical ) {
	filled_gradient( pos, vector2_t< int16_t >( size.x, 1 ), clr, vertical ? clr : clr2, false );
	filled_gradient( vector2_t< int16_t >( pos.x + size.x, pos.y ), vector2_t< int16_t >( 1, size.y + 1 ), vertical ? clr : clr2, clr2, true );
	filled_gradient( vector2_t< int16_t >( pos.x, pos.y + size.y ), vector2_t< int16_t >( size.x, 1 ), clr, vertical ? clr : clr2, false );
	filled_gradient( pos, vector2_t< int16_t >( 1, size.y ), clr, vertical ? clr2 : clr, true );
}

void c_buffer::filled_gradient( const vector2_t< int16_t > pos, const vector2_t< int16_t > size, const color_t clr, const color_t clr2, const bool vertical ) {
	std::vector<vertex_t> vertices {
		vertex_t( pos.x, pos.y, 0.f, 1.f, clr.hex ),
		vertex_t( pos.x + size.x, pos.y, 0.f, 1.f, vertical ? clr.hex : clr2.hex ),
		vertex_t( pos.x + size.x, pos.y + size.y, 0.f, 1.f, clr2.hex ),
		vertex_t( pos.x, pos.y + size.y, 0.f, 1.f, vertical ? clr2.hex : clr.hex ),
		vertex_t( pos.x, pos.y, 0.f, 1.f, clr.hex )
	};

	write_to_buffer( TRIANGLE, &vertices, nullptr );
}

void c_buffer::triangle( const vector2_t< int16_t > p1, const vector2_t< int16_t > p2, const vector2_t< int16_t > p3, const color_t clr ) {
	std::vector<vertex_t> vertices{
		vertex_t( p1.x, p1.y, 0.f, 1.f, clr.hex ),
		vertex_t( p2.x, p2.y, 0.f, 1.f, clr.hex ),
		vertex_t( p3.x, p3.y, 0.f, 1.f, clr.hex )
	};

	write_to_buffer( LINE, &vertices, nullptr );
}

void c_buffer::filled_triangle( const vector2_t< int16_t > p1, const vector2_t< int16_t > p2, const vector2_t< int16_t > p3, const color_t clr ) {
	std::vector<vertex_t> vertices{
		vertex_t( p1.x, p1.y, 0.f, 1.f, clr.hex ),
		vertex_t( p2.x, p2.y, 0.f, 1.f, clr.hex ),
		vertex_t( p3.x, p3.y, 0.f, 1.f, clr.hex )
	};

	write_to_buffer( TRIANGLE, &vertices, nullptr );
}

void c_buffer::circle( const vector2_t< int16_t > pos, const int16_t radius, const color_t clr ) {
	std::vector<vector2_t<int16_t>> points;
	points.reserve( CIRCLE_SEGMENTS + 1 );

	generate_arc_points( &points, &pos, radius, 100, 0, CIRCLE_SEGMENTS );

	polyline( points, clr );
}

void c_buffer::filled_circle( const vector2_t< int16_t > pos, const int16_t radius, const color_t clr ) {
	std::vector<vector2_t<int16_t>> points;
	points.reserve( CIRCLE_SEGMENTS + 1 );

	generate_arc_points( &points, &pos, radius, 100, 0, CIRCLE_SEGMENTS );

	polygon( points, clr );
}

void c_buffer ::text( const font_t* font, const char* str, const vector2_t<int16_t> pos, const color_t clr ) {
	if ( !str || !font )
		return;

	vector2_t<int16_t> bounds = get_text_size( font, str );
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

		const glyph_t& glyph = font->char_set[ letter ];
		row_height = max( row_height, static_cast< int >( glyph.size.y ) );

		if ( letter == '\n' ) {
			y_advance += row_height + font->padding;
			advance = 0;
			continue;
		}

		float x = std::round( static_cast< float >( pos.x ) + advance + glyph.bearing.x ) + 0.5f;
		float y = std::round( y_offset + y_advance - glyph.bearing.y ) + 0.5f;

		const std::vector<vertex_t> vertices = {
			{x, y, 0.f, 1.f, clr.hex, 0.f, 0.f},
			{x + glyph.size.x, y, 0.f, 1.f, clr.hex, 1.f, 0.f},
			{x + glyph.size.x, y + glyph.size.y, 0.f, 1.f, clr.hex, 1.f, 1.f},
			{x, y + glyph.size.y, 0.f, 1.f, clr.hex, 0.f, 1.f}
		};

		push_texture( glyph.resource );
		write_to_buffer( TRIANGLE, &vertices, nullptr );
		pop_texture( );

		advance += glyph.advance / 64;
	}
}

// __UTILS__ / __PRIVATE__

vector2_t<int16_t> c_buffer::get_text_size( const font_t* font, const char* str ) {
	if ( !str || !font )
		return vector2_t<int16_t>( 0, 0 );

	vector2_t<int16_t> size{ 0, static_cast< int16_t >( font->size * 1.5f ) };

	for ( const char* ptr = str; *ptr; ++ptr ) {
		char letter = *ptr;
		size.x += font->char_set.at( letter ).advance / 64;
	}

	return size;
}

void c_buffer::generate_arc_points( std::vector<vector2_t<int16_t>>* points, const vector2_t<int16_t>* pos, const int16_t radius, const int16_t completion, const int16_t rotation, const int16_t segments ) {
	double ang = static_cast< double >( rotation * M_PI ) / 180.0;
	double comp = ( completion * 0.01 );

	// @TODO: UNTITLED_USE_PRE_COMPUTED_SIN_COS is incomplete nor correct at all.
	auto get_point = [ & ] ( int16_t i ) {
#ifdef UNTITLED_USE_PRE_COMPUTED_SIN_COS
		double cos_theta = m_pre_computed_cos[ i % RECTANGLE_SEGMENTS ];
		double sin_theta = m_pre_computed_sin[ i % RECTANGLE_SEGMENTS ];
		return vector2_t<double>( static_cast< double >( pos->x ) + radius * cos_theta, static_cast< double >( pos->y ) + radius * sin_theta );
#else
		double theta = ang + 2.0 * comp * M_PI * static_cast< double >( i ) / static_cast< double >( segments );
		return vector2_t<double>( static_cast< double >( pos->x ) + radius * cos( theta ), static_cast< double >( pos->y ) + radius * sin( theta ) );
#endif
	};

	for ( int i = 0; i <= segments; ++i ) {
		vector2_t<double> point = get_point( i );

		points->push_back( vector2_t<int16_t>( std::round( point.x ), std::round( point.y ) ) );
	}
}

void c_buffer::generate_quadratic_bezier_points( std::vector<vector2_t<int16_t>>* points, const vector2_t<int16_t> p0, const vector2_t<int16_t> p1, const vector2_t<int16_t> p2 ) {
	for ( int i = 0; i < BEZIER_QUADRATIC_SEGMENTS; i++ ) {
		double val = static_cast< double >( i ) / static_cast< double >( BEZIER_QUADRATIC_SEGMENTS - 1 );

		points->push_back( {
			static_cast< int16_t >( std::round( std::lerp( std::lerp( p0.x, p2.x, val ), std::lerp( p2.x, p1.x, val ), val ) ) ),
			static_cast< int16_t >( std::round( std::lerp( std::lerp( p0.y, p2.y, val ), std::lerp( p2.y, p1.y, val ), val ) ) )
		} );
	}
}

void c_buffer::make_vertices( std::vector<vertex_t>* vertices, const std::vector<vector2_t<int16_t>>* points, const color_t* clr ) {
	for ( const vector2_t<int16_t>& point : *points )
		vertices->push_back( vertex_t( point.x, point.y, 0.f, 1.f, clr->hex ) );
}