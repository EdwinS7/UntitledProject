#include "../../.hpp/buffer/buffer.hpp"

void c_buffer::create_objects( ) {
	vector2_t<uint16_t> win32_size = g_win32->get_size( );

	push_command( command_t( rect_t( 0, 0, win32_size.x, win32_size.y ), nullptr ) );
}

// __BUFFER__

void c_buffer::write_to_buffer( const std::vector< vertex_t >* vertices, const std::vector < uint32_t >* indices ) {
	if ( vertices->empty( ) )
		throw std::runtime_error{ "write_idx failed (vertices invalid)" };

	int vertices_count = vertices->size( ),
		indices_count = indices == nullptr ? vertices_count * 3 : indices->size();

	std::vector < uint32_t > dynamic_indices( indices_count );

	if ( indices == nullptr ) {
		for ( uint32_t i = 0; i < vertices_count; ++i )
			dynamic_indices[ i ] = i;
	}

	m_draw_commands.push_back( draw_command_t( *vertices, indices == nullptr ? dynamic_indices : *indices, m_command, vertices_count, indices_count ) );
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

void c_buffer::line( const vector2_t< uint16_t > from, const vector2_t< uint16_t > to, const color_t clr, const uint8_t thickness ) {
	const auto& thick = thickness * 0.5f;
	std::vector<vertex_t> vertices;

	make_points_between( &vertices, &from, &to, &clr );

	std::vector<vertex_t> pixel;

	for ( const vertex_t& vertex : vertices ) {
		pixel = {
			vertex_t( vertex.x - thick, vertex.y - thick, 0.f, 1.f, vertex.clr ),
			vertex_t( vertex.x + thick, vertex.y - thick, 0.f, 1.f, vertex.clr ),
			vertex_t( vertex.x + thick, vertex.y + thick, 0.f, 1.f, vertex.clr ),
			vertex_t( vertex.x + thick, vertex.y + thick, 0.f, 1.f, vertex.clr ),
			vertex_t( vertex.x - thick, vertex.y + thick, 0.f, 1.f, vertex.clr ),
			vertex_t( vertex.x - thick, vertex.y - thick, 0.f, 1.f, vertex.clr )
		};

		write_to_buffer( &pixel, nullptr );
	}
}

void c_buffer::polyline( const std::vector< vector2_t< uint16_t > > points, const color_t clr, const uint8_t thickness ) {
	for ( uint16_t i = 0; i < points.size( ) - 1; i++ )
		line( points[ i ], points[ i + 1 ], clr, thickness );
}

void c_buffer::polygon( const std::vector<vector2_t<uint16_t>> points, const color_t clr ) {
	std::vector<vertex_t> vertices;

	make_vertices( &vertices, &points, &clr );

	write_to_buffer( &vertices, nullptr );
}

void c_buffer::rectangle( const vector2_t< uint16_t > pos, const vector2_t< uint16_t > size, const color_t clr ) {
	filled_rectangle( pos, vector2_t< uint16_t >( size.x, 1 ), clr );
	filled_rectangle( vector2_t< uint16_t >( pos.x + size.x, pos.y ), vector2_t< uint16_t >( 1, size.y + 1 ), clr );
	filled_rectangle( vector2_t< uint16_t >( pos.x, pos.y + size.y ), vector2_t< uint16_t >( size.x, 1 ), clr );
	filled_rectangle( pos, vector2_t< uint16_t >( 1, size.y ), clr );
}

void c_buffer::filled_rectangle( const vector2_t< uint16_t > pos, const vector2_t< uint16_t > size, const color_t clr ) {
	std::vector<vertex_t> vertices{
		vertex_t( pos.x, pos.y, 0.f, 1.f, clr.hex ),
		vertex_t( pos.x + size.x, pos.y, 0.f, 1.f, clr.hex ),
		vertex_t( pos.x + size.x, pos.y + size.y, 0.f, 1.f, clr.hex ),
		vertex_t( pos.x + size.x, pos.y + size.y, 0.f, 1.f, clr.hex ),
		vertex_t( pos.x, pos.y + size.y, 0.f, 1.f, clr.hex ),
		vertex_t( pos.x, pos.y, 0.f, 1.f, clr.hex )
	};

	write_to_buffer( &vertices, nullptr );
}

void c_buffer::gradient( const vector2_t< uint16_t > pos, const vector2_t< uint16_t > size, const color_t clr, const color_t clr2, const bool vertical ) {
	filled_gradient( pos, vector2_t< uint16_t >( size.x, 1 ), clr, vertical ? clr : clr2, false );
	filled_gradient( vector2_t< uint16_t >( pos.x + size.x, pos.y ), vector2_t< uint16_t >( 1, size.y + 1 ), vertical ? clr : clr2, clr2, true );
	filled_gradient( vector2_t< uint16_t >( pos.x, pos.y + size.y ), vector2_t< uint16_t >( size.x, 1 ), clr, vertical ? clr : clr2, false );
	filled_gradient( pos, vector2_t< uint16_t >( 1, size.y ), clr, vertical ? clr2 : clr, true );
}

void c_buffer::filled_gradient( const vector2_t< uint16_t > pos, const vector2_t< uint16_t > size, const color_t clr, const color_t clr2, const bool vertical ) {
	std::vector<vertex_t> vertices{
		vertex_t( pos.x, pos.y, 0.f, 1.f, clr.hex ),
		vertex_t( pos.x + size.x, pos.y, 0.f, 1.f, vertical ? clr.hex : clr2.hex ),
		vertex_t( pos.x + size.x, pos.y + size.y, 0.f, 1.f, clr2.hex ),
		vertex_t( pos.x + size.x, pos.y + size.y, 0.f, 1.f, clr2.hex ),
		vertex_t( pos.x, pos.y + size.y, 0.f, 1.f, vertical ? clr2.hex : clr.hex ),
		vertex_t( pos.x, pos.y, 0.f, 1.f, clr.hex )
	};

	write_to_buffer( &vertices, nullptr );
}

void c_buffer::circle( const vector2_t< uint16_t > pos, const uint16_t radius, const color_t clr ) {
	std::vector<vector2_t<uint16_t>> points;
	generate_arc_points( &points, &pos, radius, 100, 0, 64, false );

	polyline( points, clr, 1.f );
}

void c_buffer::filled_circle( const vector2_t< uint16_t > pos, const uint16_t radius, const color_t clr ) {
	std::vector<vector2_t<uint16_t>> points;
	generate_arc_points( &points, &pos, radius, 100, 0, 64, true );

	polygon( points, clr );
}

void c_buffer::filled_circle_multi_color( const vector2_t< uint16_t > pos, const uint16_t radius, const color_t outer_clr, const color_t inner_clr ) {
	
}

// __COMMANDS__

void c_buffer::clear_commands( ) {
	m_draw_commands.clear( );
	m_draw_command.reset( );
}

command_t c_buffer::get_command( ) {
	return m_command;
}

compiled_draw_command_t c_buffer::get_draw_command( ) {
	return m_draw_command;
}

std::vector<draw_command_t> c_buffer::get_draw_commands( ) {
	return m_draw_commands;
}

void c_buffer::push_command( const command_t command ) {
	m_command += command;
}

// __UTILS__ / __PRIVATE__

void c_buffer::generate_arc_points( std::vector<vector2_t<uint16_t>>* points, const vector2_t<uint16_t>* pos, const uint16_t radius, const uint16_t completion, const uint16_t rotation, const uint16_t segments, const bool filled ) {
	double ang = static_cast< double >( rotation * M_PI ) / 180.0;
	double comp = ( completion * 0.01 ) * M_PI;
	double diameter = static_cast< double >( radius );

	auto get_point = [ & ] ( uint16_t i ) {
		double theta = ang + 2.0 * comp * static_cast< double >( i ) / static_cast< double >( segments );
		return vector2_t<double>( static_cast< double >( pos->x ) + diameter * cos( theta ), static_cast< double >( pos->y ) + diameter * sin( theta ) );
	};

	for ( int i = 0; i < segments; ++i ) {
		vector2_t<double> point = get_point( i );
		vector2_t<double> next_point = get_point( i + 1 );

		if ( filled )
			points->push_back( vector2_t<uint16_t>( pos->x, pos->y ) );

		points->push_back( vector2_t<uint16_t>( point.x, point.y ) );
		points->push_back( vector2_t<uint16_t>( next_point.x, next_point.y ) );
	}
}

void c_buffer::make_points_between( std::vector<vertex_t>* vertices, const vector2_t<uint16_t>* from, const vector2_t<uint16_t>* to, const color_t* clr ) {
	int steps = max( std::abs( from->x - to->x ), std::abs( from->y - to->y ) );
	float x_increment = static_cast< float >( to->x - from->x ) / steps;
	float y_increment = static_cast< float >( to->y - from->y ) / steps;

	vector2_t<uint16_t> point = *from;

	for ( int i = 0; i <= steps; ++i ) {
		vertices->push_back( vertex_t( point.x, point.y, 0.f, 1.f, clr->hex ) );

		point.x = static_cast< uint16_t >( std::round( point.x + x_increment ) );
		point.y = static_cast< uint16_t >( std::round( point.y + y_increment ) );
	}
}

void c_buffer::make_vertices( std::vector<vertex_t>* vertices, const std::vector<vector2_t<uint16_t>>* points, const color_t* clr ) {
	for ( const vector2_t<uint16_t>& point : *points )
		vertices->push_back( vertex_t( point.x, point.y, 0.f, 1.f, clr->hex ) );
}