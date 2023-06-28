#include "../../.hpp/buffer/buffer.hpp"

void c_buffer::create_objects( ) {
	vector2_t<uint16_t> win32_size = g_win32->get_size( );

	push_command( command_t( rect_t( 0, 0, win32_size.x, win32_size.y ), nullptr ) );
}

// __BUFFER__

void c_buffer::write_to_buffer( const uint8_t primitive, const std::vector< vertex_t >* vertices, const std::vector<uint32_t>* indices ) {
	int vertices_count = vertices->size( ),
		indices_count = indices == nullptr ? vertices_count * 3 : indices->size();

	std::vector < uint32_t > dynamic_indices( indices_count );

	if ( indices == nullptr )
		for ( uint32_t i = 0; i < vertices_count; ++i )
			dynamic_indices[ i ] = i;

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

void c_buffer::line( const vector2_t< uint16_t > from, const vector2_t< uint16_t > to, const color_t clr ) {
	std::vector<vertex_t> vertices{
		vertex_t( from.x, from.y, 0.f, 1.f, clr.hex ),
		vertex_t( to.x, to.y, 0.f, 1.f, clr.hex )
	};

	write_to_buffer( LINE, &vertices, nullptr);
}

void c_buffer::polyline( const std::vector< vector2_t< uint16_t > > points, const color_t clr ) {
	std::vector<vertex_t> vertices;

	make_vertices( &vertices, &points, &clr );

	write_to_buffer( LINE, &vertices, nullptr );
}

void c_buffer::polygon( const std::vector<vector2_t<uint16_t>> points, const color_t clr ) {
	std::vector<vertex_t> vertices;

	make_vertices( &vertices, &points, &clr );

	std::vector<uint32_t> indices;

	for ( uint32_t i = 1; i < ( vertices.size( ) - 2 ) * 3; i++ ) {
		indices.push_back( 0 );
		indices.push_back( i );
		indices.push_back( i + 1 );
	}

	write_to_buffer( TRIANGLE, &vertices, &indices );
}

void c_buffer::rectangle( const vector2_t< uint16_t > pos, const vector2_t< uint16_t > size, const color_t clr, const uint16_t rounding, const corner_flags flags ) {
	const bool round_top_left = ( flags & corner_top_left );
	const bool round_top_right = ( flags & corner_top_right );
	const bool round_bottom_left = ( flags & corner_bottom_left );
	const bool round_bottom_right = ( flags & corner_bottom_right );

	std::vector<vector2_t<uint16_t>> points;

	std::initializer_list<std::tuple<vector2_t<uint16_t>, vector2_t<uint16_t>, int, bool>> gen_points = {
		std::tuple{vector2_t<uint16_t>( pos.x, pos.y ), vector2_t<uint16_t>( pos.x + rounding, pos.y + rounding ), 180, round_top_left},
		std::tuple{vector2_t<uint16_t>( pos.x + size.x, pos.y ), vector2_t<uint16_t>( pos.x + size.x - rounding, pos.y + rounding ), 270, round_top_right},
		std::tuple{vector2_t<uint16_t>( pos.x + size.x, pos.y + size.y ), vector2_t<uint16_t>( pos.x + size.x - rounding, pos.y + size.y - rounding ), 0, round_bottom_right},
		std::tuple{vector2_t<uint16_t>( pos.x, pos.y + size.y ), vector2_t<uint16_t>( pos.x + rounding, pos.y + size.y - rounding ), 90, round_bottom_left}
	};

	for ( const std::tuple<vector2_t<uint16_t>, vector2_t<uint16_t>, int, bool>& corner_tuple : gen_points ) {
		bool should_round = std::get<3>( corner_tuple );

		if ( should_round ) {
			vector2_t<uint16_t> corner_rounded = std::get<1>( corner_tuple );
			int angle = std::get<2>( corner_tuple );

			std::vector<vector2_t<uint16_t>> corner_points;
			generate_arc_points( &corner_points, &corner_rounded, rounding, 25, angle, RECTANGLE_SEGMENTS );

			points.insert( points.end( ), corner_points.begin( ), corner_points.end( ) );
		}
		else {
			vector2_t<uint16_t> corner = std::get<0>( corner_tuple );

			points.push_back( corner );
		}
	}

	polyline( points, clr );
}

void c_buffer::filled_rectangle( const vector2_t< uint16_t > pos, const vector2_t< uint16_t > size, const color_t clr, const uint16_t rounding, const corner_flags flags ) {
	const bool round_top_left = ( flags & corner_top_left );
	const bool round_top_right = ( flags & corner_top_right );
	const bool round_bottom_left = ( flags & corner_bottom_left );
	const bool round_bottom_right = ( flags & corner_bottom_right );

	std::vector<vector2_t<uint16_t>> points;

	std::initializer_list<std::tuple<vector2_t<uint16_t>, vector2_t<uint16_t>, int, bool>> gen_points = {
		std::tuple{vector2_t<uint16_t>( pos.x, pos.y ), vector2_t<uint16_t>( pos.x + rounding, pos.y + rounding ), 180, round_top_left},
		std::tuple{vector2_t<uint16_t>( pos.x + size.x, pos.y ), vector2_t<uint16_t>( pos.x + size.x - rounding, pos.y + rounding ), 270, round_top_right},
		std::tuple{vector2_t<uint16_t>( pos.x + size.x, pos.y + size.y ), vector2_t<uint16_t>( pos.x + size.x - rounding, pos.y + size.y - rounding ), 0, round_bottom_right},
		std::tuple{vector2_t<uint16_t>( pos.x, pos.y + size.y ), vector2_t<uint16_t>( pos.x + rounding, pos.y + size.y - rounding ), 90, round_bottom_left},
		std::tuple{vector2_t<uint16_t>( pos.x, pos.y ), vector2_t<uint16_t>( pos.x + rounding, pos.y + rounding ), 180, round_top_left}
	};

	for ( const std::tuple<vector2_t<uint16_t>, vector2_t<uint16_t>, int, bool>& corner_tuple : gen_points ) {
		bool should_round = std::get<3>( corner_tuple );

		if ( should_round ) {
			vector2_t<uint16_t> corner_rounded = std::get<1>( corner_tuple );

			int angle = std::get<2>( corner_tuple );

			std::vector<vector2_t<uint16_t>> corner_points;
			generate_arc_points( &corner_points, &corner_rounded, rounding, 25, angle, RECTANGLE_SEGMENTS );

			points.insert( points.end( ), corner_points.begin( ), corner_points.end( ) );
		}
		else {
			vector2_t<uint16_t> corner = std::get<0>( corner_tuple );

			points.push_back( corner );
		}
	}

	polygon( points, clr );
}

void c_buffer::gradient( const vector2_t< uint16_t > pos, const vector2_t< uint16_t > size, const color_t clr, const color_t clr2, const bool vertical ) {
	filled_gradient( pos, vector2_t< uint16_t >( size.x, 1 ), clr, vertical ? clr : clr2, false );
	filled_gradient( vector2_t< uint16_t >( pos.x + size.x, pos.y ), vector2_t< uint16_t >( 1, size.y + 1 ), vertical ? clr : clr2, clr2, true );
	filled_gradient( vector2_t< uint16_t >( pos.x, pos.y + size.y ), vector2_t< uint16_t >( size.x, 1 ), clr, vertical ? clr : clr2, false );
	filled_gradient( pos, vector2_t< uint16_t >( 1, size.y ), clr, vertical ? clr2 : clr, true );
}

void c_buffer::filled_gradient( const vector2_t< uint16_t > pos, const vector2_t< uint16_t > size, const color_t clr, const color_t clr2, const bool vertical ) {
	std::vector<vertex_t> vertices {
		vertex_t( pos.x, pos.y, 0.f, 1.f, clr.hex ),
		vertex_t( pos.x + size.x, pos.y, 0.f, 1.f, vertical ? clr.hex : clr2.hex ),
		vertex_t( pos.x + size.x, pos.y + size.y, 0.f, 1.f, clr2.hex ),
		vertex_t( pos.x, pos.y + size.y, 0.f, 1.f, vertical ? clr2.hex : clr.hex ),
		vertex_t( pos.x, pos.y, 0.f, 1.f, clr.hex )
	};

	write_to_buffer( TRIANGLE, &vertices, nullptr );
}

void c_buffer::triangle( const vector2_t< uint16_t > p1, const vector2_t< uint16_t > p2, const vector2_t< uint16_t > p3, const color_t clr ) {
	std::vector<vertex_t> vertices{
		vertex_t( p1.x, p1.y, 0.f, 1.f, clr.hex ),
		vertex_t( p2.x, p2.y, 0.f, 1.f, clr.hex ),
		vertex_t( p3.x, p3.y, 0.f, 1.f, clr.hex )
	};

	write_to_buffer( LINE, &vertices, nullptr );
}

void c_buffer::filled_triangle( const vector2_t< uint16_t > p1, const vector2_t< uint16_t > p2, const vector2_t< uint16_t > p3, const color_t clr ) {
	std::vector<vertex_t> vertices{
		vertex_t( p1.x, p1.y, 0.f, 1.f, clr.hex ),
		vertex_t( p2.x, p2.y, 0.f, 1.f, clr.hex ),
		vertex_t( p3.x, p3.y, 0.f, 1.f, clr.hex )
	};

	write_to_buffer( TRIANGLE, &vertices, nullptr );
}

void c_buffer::circle( const vector2_t< uint16_t > pos, const uint16_t radius, const color_t clr ) {
	std::vector<vector2_t<uint16_t>> points( CIRCLE_SEGMENTS );

	generate_arc_points( &points, &pos, radius, 100, 0, CIRCLE_SEGMENTS );

	polyline( points, clr );
}

void c_buffer::filled_circle( const vector2_t< uint16_t > pos, const uint16_t radius, const color_t clr ) {
	std::vector<vector2_t<uint16_t>> points( CIRCLE_SEGMENTS );

	generate_arc_points( &points, &pos, radius, 100, 0, CIRCLE_SEGMENTS );

	polygon( points, clr );
}

// __UTILS__ / __PRIVATE__

void c_buffer::generate_arc_points( std::vector<vector2_t<uint16_t>>* points, const vector2_t<uint16_t>* pos, const uint16_t radius, const uint16_t completion, const uint16_t rotation, const uint16_t segments ) {
	double ang = static_cast< double >( rotation * M_PI ) / 180.0;
	double comp = ( completion * 0.01 );

	auto get_point = [ & ] ( uint16_t i ) {
		double theta = ang + 2.0 * comp * M_PI * static_cast< double >( i ) / static_cast< double >( segments );
		return vector2_t<double>( static_cast< double >( pos->x ) + radius * cos( theta ), static_cast< double >( pos->y ) + radius * sin( theta ) );
	};

	for ( int i = 0; i <= segments; ++i ) {
		vector2_t<double> point = get_point( i );

		points->push_back( vector2_t<uint16_t>( std::round( point.x ), std::round( point.y ) ) );
	}
}

void c_buffer::make_vertices( std::vector<vertex_t>* vertices, const std::vector<vector2_t<uint16_t>>* points, const color_t* clr ) {
	for ( const vector2_t<uint16_t>& point : *points )
		vertices->push_back( vertex_t( point.x, point.y, 0.f, 1.f, clr->hex ) );
}