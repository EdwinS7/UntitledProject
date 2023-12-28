#include "buffer.hpp"

void cBuffer::Init( bool ApplyDefaults ) {
	PushClip( gWin32->GetCanvasRect( ) );
	PushTexture( nullptr );

	// Create font objects.
	CreateFontFromName( &Fonts.Default, "Segoe UI", 16, 400, 4, true );
	CreateFontFromName( &Fonts.Interface, "Arial", 9, 100, 4, false );

	//Apply default settings.
	if ( ApplyDefaults ) {
		m_CircleSegments = 64, m_RectangleSegments = 16;
		m_BezierCubicSegments, m_BezierQuadraticSegments = 128;
	}
}

void cBuffer::Destroy( ) {
	m_command.textures.clear( );
	m_command.clips.clear( );

	// Release Fonts.
	Fonts.Default.Release( );
	Fonts.Interface.Release( );
}

void cBuffer::WriteToBuffer( const int8_t primitive, const std::vector< Vertex >* vertices, const std::vector<int32_t>* indices ) {
	int vertices_count = vertices->size( ),
		indices_count = indices == nullptr ? ( vertices_count * 3 ) - 1 : indices->size( );

	std::vector < int32_t > dynamic_indices( indices_count );

	if ( indices == nullptr )
		for ( int32_t i = 0; i < vertices_count; ++i )
			dynamic_indices[ i ] = i;

	m_VerticesCount += vertices_count;
	m_IndicesCount += indices_count;

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

void cBuffer::Line( const Vec2< int16_t > from, const Vec2< int16_t > to, const Color clr ) {
	std::vector<Vertex> vertices{
		Vertex( from.x, from.y, 0.f, 1.f, clr.hex ),
		Vertex( to.x, to.y, 0.f, 1.f, clr.hex )
	};

	RotateVertices( &vertices );
	WriteToBuffer( LINE, &vertices, nullptr );
}

void cBuffer::Polyline( const std::vector< Vec2< int16_t > > points, const Color clr ) {
	std::vector<Vertex> vertices;
	vertices.reserve( points.size( ) );

	MakeVertices( &vertices, &points, &clr );
	RotateVertices( &vertices );

	WriteToBuffer( LINE, &vertices, nullptr );
}

void cBuffer::Polygon( const std::vector<Vec2<int16_t>> points, const Color clr ) {
	std::vector<Vertex> vertices;
	vertices.reserve( points.size( ) );

	MakeVertices( &vertices, &points, &clr );

	std::vector<int32_t> indices;
	indices.reserve( vertices.size( ) * 3 );

	for ( int32_t i = 1; i < ( vertices.size( ) - 2 ) * 3; i++ ) {
		indices.push_back( 0 );
		indices.push_back( i );
		indices.push_back( i + 1 );
	}

	RotateVertices( &vertices );
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
			corner_points.reserve( ( m_RectangleSegments ) +1 );

			GenerateArcPoints( &corner_points, &corner_rounded, rounding, 25, angle, 64 );

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
			corner_points.reserve( ( m_RectangleSegments ) +1 );

			GenerateArcPoints( &corner_points, &corner_rounded, rounding, 25, angle, m_RectangleSegments );

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
	RotateVertices( &vertices );
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
	std::vector<Vertex> vertices{
		Vertex( pos.x, pos.y, 0.f, 1.f, clr.hex ),
		Vertex( pos.x + size.x, pos.y, 0.f, 1.f, vertical ? clr.hex : clr2.hex ),
		Vertex( pos.x + size.x, pos.y + size.y, 0.f, 1.f, clr2.hex ),
		Vertex( pos.x + size.x, pos.y + size.y, 0.f, 1.f, clr2.hex ),
		Vertex( pos.x, pos.y + size.y, 0.f, 1.f, vertical ? clr2.hex : clr.hex ),
		Vertex( pos.x, pos.y, 0.f, 1.f, clr.hex )
	};

	RotateVertices( &vertices );
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

	RotateVertices( &vertices );
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

	RotateVertices( &vertices );
	WriteToBuffer( TRIANGLE, &vertices, nullptr );
}

void cBuffer::Triangle( const Vec2< int16_t > p1, const Vec2< int16_t > p2, const Vec2< int16_t > p3, const Color clr ) {
	std::vector<Vertex> vertices{
		Vertex( p1.x, p1.y, 0.f, 1.f, clr.hex ),
		Vertex( p2.x, p2.y, 0.f, 1.f, clr.hex ),
		Vertex( p3.x, p3.y, 0.f, 1.f, clr.hex )
	};

	RotateVertices( &vertices );
	WriteToBuffer( LINE, &vertices, nullptr );
}

void cBuffer::FilledTriangle( const Vec2< int16_t > p1, const Vec2< int16_t > p2, const Vec2< int16_t > p3, const Color clr ) {
	std::vector<Vertex> vertices{
		Vertex( p1.x, p1.y, 0.f, 1.f, clr.hex ),
		Vertex( p2.x, p2.y, 0.f, 1.f, clr.hex ),
		Vertex( p3.x, p3.y, 0.f, 1.f, clr.hex )
	};

	RotateVertices( &vertices );
	WriteToBuffer( TRIANGLE, &vertices, nullptr );
}

void cBuffer::Circle( const Vec2< int16_t > pos, const int16_t radius, const Color clr ) {
	std::vector<Vec2<int16_t>> points;
	points.reserve( m_CircleSegments + 1 );

	GenerateArcPoints( &points, &pos, radius, 100, 0, m_CircleSegments );

	Polyline( points, clr );
}

void cBuffer::FilledCircle( const Vec2< int16_t > pos, const int16_t radius, const Color clr ) {
	std::vector<Vec2<int16_t>> points;
	points.reserve( m_CircleSegments + 1 );

	GenerateArcPoints( &points, &pos, radius, 100, 0, m_CircleSegments );

	Polygon( points, clr );
}

void cBuffer::String( const Font* font, const char* str, const Vec2<int16_t> pos, const Color clr ) {
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

void cBuffer::GenerateArcPoints( std::vector<Vec2<int16_t>>* Points, const Vec2<int16_t>* Pos, const int16_t Radius, const int16_t Completion, const int16_t Rotation, const int16_t Segments ) {
	double Angle = static_cast< double >( Rotation * M_PI ) / 180.0;
	double Coversion = ( Completion * 0.01 );

	auto get_point = [ & ] ( int16_t i ) {
		double theta = Angle + 2.0 * Coversion * M_PI * static_cast< double >( i ) / static_cast< double >( Segments );
		return Vec2<double>( static_cast< double >( Pos->x ) + Radius * cos( theta ), static_cast< double >( Pos->y ) + Radius * sin( theta ) );
	};

	for ( int i = 0; i <= Segments; ++i ) {
		Vec2<double> point = get_point( i );

		Points->push_back( Vec2<int16_t>( std::round( point.x ), std::round( point.y ) ) );
	}
}

void cBuffer::GenerateQuadraticBezierPoints( std::vector<Vec2<int16_t>>* Points, const Vec2<int16_t> Point1, const Vec2<int16_t> Point2, const Vec2<int16_t> Point3 ) {
	for ( int i = 0; i < m_BezierQuadraticSegments; i++ ) {
		double val = static_cast< double >( i ) / static_cast< double >( ( m_BezierQuadraticSegments ) -1 );

		Points->push_back( {
			static_cast< int16_t >( std::round( std::lerp( std::lerp( Point1.x, Point3.x, val ), std::lerp( Point3.x, Point2.x, val ), val ) ) ),
			static_cast< int16_t >( std::round( std::lerp( std::lerp( Point1.y, Point3.y, val ), std::lerp( Point3.y, Point2.y, val ), val ) ) )
		} );
	}
}

void cBuffer::MakeVertices( std::vector<Vertex>* vertices, const std::vector<Vec2<int16_t>>* points, const Color* clr ) {
	for ( const Vec2<int16_t>& point : *points )
		vertices->push_back( Vertex( point.x, point.y, 0.f, 1.f, clr->hex ) );
}

void cBuffer::RotateObject( float val ) {
	m_Rotation = val;
}

void cBuffer::RotateVertices( std::vector<Vertex>* vertices, Vec2<int16_t> center_manual ) {
	if ( vertices->empty( ) )
		return;

	Vec2<int16_t> center( 0, 0 );

	for ( const auto& vertex : *vertices ) {
		center.x += vertex.x;
		center.y += vertex.y;
	}
	center.x /= static_cast< float >( vertices->size( ) );
	center.y /= static_cast< float >( vertices->size( ) );

SKIP_CENTER:

	float radians = m_Rotation * M_PI / 180.0f;
	float cos_value = cosf( radians );
	float sin_value = sinf( radians );

	for ( auto& vertex : *vertices ) {
		float translatedX = vertex.x - center.x;
		float translatedY = vertex.y - center.y;

		float new_x = translatedX * cos_value - translatedY * sin_value;
		float new_y = translatedX * sin_value + translatedY * cos_value;

		vertex.x = new_x + center.x;
		vertex.y = new_y + center.y;
	}
}

void cBuffer::CreateFontFromName( Font* Font, const char* FontName, const int16_t Size, const int16_t Weight, const int16_t Padding, const bool Antialiased ) {
	FT_Library lib;
	FT_Face face;

	Font->path = GetFontPath( FontName );
	Font->padding = Padding;
	Font->size = Size;

	if ( FT_Init_FreeType( &lib ) != FT_Err_Ok )
		std::printf( std::vformat( "[ Buffer ] FT_Init_FreeType failed ( {} )\n", std::make_format_args( FontName ) ).c_str( ) );

	if ( FT_New_Face( lib, Font->path.c_str( ), 0, &face ) )
		std::printf( std::vformat( "[ Buffer ] FT_New_Face failed ( {} )\n", std::make_format_args( FontName ) ).c_str( ) );

	FT_Set_Char_Size( face, Size * 64, 0, GetDpiForWindow( gWin32->GetHwnd( ) ), 0 );
	FT_Select_Charmap( face, FT_ENCODING_UNICODE );

	for ( unsigned char i = 0; i < 128; i++ ) {
		if ( FT_Load_Char( face, i, Antialiased ? FT_LOAD_RENDER : FT_LOAD_RENDER | FT_LOAD_TARGET_MONO ) )
			std::printf( std::vformat( "[ Buffer ] FT_Load_Char failed, font most likely does not exist! ( {} )\n", std::make_format_args( FontName ) ).c_str( ) );

		int32_t width = face->glyph->bitmap.width ? face->glyph->bitmap.width : 16;
		int32_t height = face->glyph->bitmap.rows ? face->glyph->bitmap.rows : 16;

		if ( gGraphics->GetDevice( )->CreateTexture( width, height, 1, D3DUSAGE_DYNAMIC, D3DFMT_A8, D3DPOOL_DEFAULT, &Font->char_set[ i ].resource, NULL ) )
			std::printf( std::vformat( "[ Buffer ] CreateTexture failed ( {} )\n", std::make_format_args( FontName ) ).c_str( ) );

		D3DLOCKED_RECT locked_rect;
		Font->char_set[ i ].resource->LockRect( 0, &locked_rect, nullptr, D3DLOCK_DISCARD );

		UCHAR* source = face->glyph->bitmap.buffer;
		UCHAR* destination = reinterpret_cast< UCHAR* >( locked_rect.pBits );

		if ( source && destination ) {
			switch ( face->glyph->bitmap.pixel_mode ) {
			case FT_PIXEL_MODE_MONO:
				for ( int32_t y = 0; y < height; y++, source += face->glyph->bitmap.pitch, destination += locked_rect.Pitch ) {
					int8_t bits = 0;
					const uint8_t* bits_ptr = source;

					for ( int32_t x = 0; x < width; x++, bits <<= 1 ) {
						if ( ( x & 7 ) == 0 )
							bits = *bits_ptr++;

						destination[ x ] = ( bits & 0x80 ) ? 255 : 0;
					}
				}

				break;
			case FT_PIXEL_MODE_GRAY:
				for ( int32_t i = 0; i < height; ++i ) {
					memcpy( destination, source, width );

					source += face->glyph->bitmap.pitch;
					destination += locked_rect.Pitch;
				}

				break;
			}
		}

		Font->char_set[ i ].resource->UnlockRect( 0 );

		D3DSURFACE_DESC description;
		Font->char_set[ i ].resource->GetLevelDesc( 0, &description );

		Font->char_set[ i ].size = { width, height };
		Font->char_set[ i ].bearing = { static_cast< int32_t >( face->glyph->bitmap_left ), static_cast< int32_t >( face->glyph->bitmap_top ) };
		Font->char_set[ i ].advance = face->glyph->advance.x;
	}

	FT_Done_Face( face );
	FT_Done_FreeType( lib );

	std::printf( std::vformat( "[ Buffer ] Created font ( name: {}, size: {}, weight: {}, antialiasing: {} )\n", std::make_format_args( FontName, Size, Weight, Antialiased ) ).c_str( ) );
}