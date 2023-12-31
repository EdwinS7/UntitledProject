#include "buffer.hpp"

void cBuffer::Init( bool ApplyDefaults ) {
	PushClip( gWin32->GetClipRect( ) );
	PushTexture( nullptr );

	// Create font objects.
	CreateFontFromName( &Fonts.Default, "Segoe UI", 16, 400, 4, true );
	CreateFontFromName( &Fonts.Interface, "Arial", 9, 100, 4, false );

	//Apply default settings.
	if ( ApplyDefaults ) {
		m_CircleSegments = 64, m_RectangleSegments = 16;
		m_BezierCubicSegments = 128, m_BezierQuadraticSegments = 128;
	}
}

void cBuffer::Destroy( ) {
	m_CommandResources.Textures.clear( );
	m_CommandResources.Clips.clear( );

	// Release Fonts.
	Fonts.Default.Release( );
	Fonts.Interface.Release( );
}

void cBuffer::WriteToBuffer( const int8_t Primitive, const std::vector< Vertex >* Vertices, const std::vector<int32_t>* Indices ) {
	int VerticesCount = Vertices->size( ),
		IndicesCount = Indices == nullptr ? ( VerticesCount * 3 ) - 1 : Indices->size( );

	std::vector < int32_t > DynamicIndices( IndicesCount );

	if ( Indices == nullptr )
		for ( int32_t i = 0; i < VerticesCount; ++i )
			DynamicIndices[ i ] = i;

	m_VerticesCount += VerticesCount;
	m_IndicesCount += IndicesCount;

	m_DrawCommands.push_back( DrawCommand( Primitive, *Vertices, Indices == nullptr ? DynamicIndices : *Indices, m_CommandResources, VerticesCount, Indices != nullptr ? Indices->size( ) : IndicesCount ) );
}

void cBuffer::BuildDrawCommands( const std::vector<DrawCommand>& DrawCommands ) {
	for ( int i = 0; i < DrawCommands.size( ); ++i ) {
		auto& DrawCommand = DrawCommands[ i ];

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

void cBuffer::Line( const Vec2< int16_t > From, const Vec2< int16_t > To, const Color Color ) {
	std::vector<Vertex> Vertices = {
		Vertex( From.x, From.y, 0.f, 1.f, Color.hex ),
		Vertex( To.x, To.y, 0.f, 1.f, Color.hex )
	};

	RotateVertices( &Vertices );
	WriteToBuffer( LINE, &Vertices, nullptr );
}

void cBuffer::Polyline( const std::vector< Vec2< int16_t > > Points, const Color Color ) {
	std::vector<Vertex> Vertices;
	Vertices.reserve( Points.size( ) );

	MakeVertices( &Vertices, &Points, &Color );
	RotateVertices( &Vertices );

	WriteToBuffer( LINE, &Vertices, nullptr );
}

void cBuffer::Polygon( const std::vector<Vec2<int16_t>> Points, const Color Color ) {
	std::vector<Vertex> Vertices;
	Vertices.reserve( Points.size( ) );

	MakeVertices( &Vertices, &Points, &Color );

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

void cBuffer::Rectangle( const Vec2< int16_t > Pos, const Vec2< int16_t > Size, const Color Color, const int16_t Rounding, const CornerFlags Flags ) {
	const bool RoundTopLeft = ( Flags & CornerTopLeft ) && Rounding;
	const bool RoundTop_right = ( Flags & CornerTopRight ) && Rounding;
	const bool RoundBottomLeft = ( Flags & CornerBottomLeft ) && Rounding;
	const bool RoundBottomRight = ( Flags & CornerBottomRight ) && Rounding;

	std::vector<Vec2<int16_t>> Points;

	std::initializer_list<std::tuple<Vec2<int16_t>, Vec2<int16_t>, int, bool>> Corners = {
		std::tuple{Vec2<int16_t>( Pos.x, Pos.y ), Vec2<int16_t>( Pos.x + Rounding, Pos.y + Rounding ), 180, RoundTopLeft},
		std::tuple{Vec2<int16_t>( Pos.x + Size.x, Pos.y ), Vec2<int16_t>( Pos.x + Size.x - Rounding, Pos.y + Rounding ), 270, RoundTop_right},
		std::tuple{Vec2<int16_t>( Pos.x + Size.x, Pos.y + Size.y ), Vec2<int16_t>( Pos.x + Size.x - Rounding, Pos.y + Size.y - Rounding ), 0, RoundBottomRight},
		std::tuple{Vec2<int16_t>( Pos.x, Pos.y + Size.y ), Vec2<int16_t>( Pos.x + Rounding, Pos.y + Size.y - Rounding ), 90, RoundBottomLeft},
		std::tuple{Vec2<int16_t>( Pos.x, Pos.y ), Vec2<int16_t>( Pos.x + Rounding, Pos.y + Rounding ), 180, RoundTopLeft}
	};

	for ( const std::tuple<Vec2<int16_t>, Vec2<int16_t>, int, bool>& Corner : Corners ) {
		bool should_round = std::get<3>( Corner );

		if ( should_round ) {
			Vec2<int16_t> CornerRounded = std::get<1>( Corner );
			int Angle = std::get<2>( Corner );

			std::vector<Vec2<int16_t>> CornerPoints;
			CornerPoints.reserve( m_RectangleSegments + 1 );

			GenerateArcPoints( &CornerPoints, &CornerRounded, Rounding, 25, Angle, m_RectangleSegments );

			Points.insert( Points.end( ), 
				std::make_move_iterator( CornerPoints.begin( ) ), std::make_move_iterator( CornerPoints.end( ) )
			);
		}
		else
			Points.push_back( std::get<0>( Corner ) );
	}

	Polyline( Points, Color );
}

void cBuffer::FilledRectangle( const Vec2< int16_t > Pos, const Vec2< int16_t > Size, const Color Color, const int16_t Rounding, const CornerFlags Flags ) {
	const bool RoundTopLeft = ( Flags & CornerTopLeft ) && Rounding;
	const bool RoundTop_right = ( Flags & CornerTopRight ) && Rounding;
	const bool RoundBottomLeft = ( Flags & CornerBottomLeft ) && Rounding;
	const bool RoundBottomRight = ( Flags & CornerBottomRight ) && Rounding;

	std::vector<Vec2<int16_t>> Points;

	std::initializer_list<std::tuple<Vec2<int16_t>, Vec2<int16_t>, int, bool>> Corners = {
		std::tuple{Vec2<int16_t>( Pos.x, Pos.y ), Vec2<int16_t>( Pos.x + Rounding, Pos.y + Rounding ), 180, RoundTopLeft},
		std::tuple{Vec2<int16_t>( Pos.x + Size.x, Pos.y ), Vec2<int16_t>( Pos.x + Size.x - Rounding, Pos.y + Rounding ), 270, RoundTop_right},
		std::tuple{Vec2<int16_t>( Pos.x + Size.x, Pos.y + Size.y ), Vec2<int16_t>( Pos.x + Size.x - Rounding, Pos.y + Size.y - Rounding ), 0, RoundBottomRight},
		std::tuple{Vec2<int16_t>( Pos.x, Pos.y + Size.y ), Vec2<int16_t>( Pos.x + Rounding, Pos.y + Size.y - Rounding ), 90, RoundBottomLeft},
		std::tuple{Vec2<int16_t>( Pos.x, Pos.y ), Vec2<int16_t>( Pos.x + Rounding, Pos.y + Rounding ), 180, RoundTopLeft}
	};

	for ( const std::tuple<Vec2<int16_t>, Vec2<int16_t>, int, bool>& Corner : Corners ) {
		bool should_round = std::get<3>( Corner );

		if ( should_round ) {
			Vec2<int16_t> CornerRounded = std::get<1>( Corner );
			int Angle = std::get<2>( Corner );

			std::vector<Vec2<int16_t>> CornerPoints;
			CornerPoints.reserve( m_RectangleSegments + 1 );

			GenerateArcPoints( &CornerPoints, &CornerRounded, Rounding, 25, Angle, m_RectangleSegments );

			Points.insert( Points.end( ),
				std::make_move_iterator( CornerPoints.begin( ) ), std::make_move_iterator( CornerPoints.end( ) )
			);
		}
		else
			Points.push_back( std::get<0>( Corner ) );
	}

	Polygon( Points, Color );
}

// @note: finish another time.
void cBuffer::TexturedRectangle( IDirect3DTexture9* Resource, const Vec2<int16_t> Pos, const Vec2<int16_t> Size, const Color Color ) {
	std::vector<Vertex> Vertices = {
		Vertex( Pos.x, Pos.y, 0.f, 1.f, Color.hex ),
		Vertex( Pos.x + Size.x, Pos.y, 0.f, 1.f, Color.hex ),
		Vertex( Pos.x + Size.x, Pos.y + Size.y, 0.f, 1.f, Color.hex ),
		Vertex( Pos.x + Size.x, Pos.y + Size.y, 0.f, 1.f, Color.hex ),
		Vertex( Pos.x, Pos.y + Size.y, 0.f, 1.f, Color.hex ),
		Vertex( Pos.x, Pos.y, 0.f, 1.f, Color.hex )
	};


	PushTexture( Resource );
	RotateVertices( &Vertices );
	WriteToBuffer( TRIANGLE, &Vertices, nullptr );
	PopTexture( );
}

void cBuffer::Gradient( const Vec2< int16_t > Pos, const Vec2< int16_t > Size, const Color Color1, const Color Color2, const bool Vertical ) {
	std::vector<Vertex> Vertices = {
		Vertex( Pos.x, Pos.y, 0.f, 1.f, Color1.hex ),
		Vertex( Pos.x + Size.x, Pos.y, 0.f, 1.f, Vertical ? Color1.hex : Color2.hex ),
		Vertex( Pos.x + Size.x, Pos.y + Size.y, 0.f, 1.f, Color2.hex ),
		Vertex( Pos.x + Size.x, Pos.y + Size.y, 0.f, 1.f, Color2.hex ),
		Vertex( Pos.x, Pos.y + Size.y, 0.f, 1.f, Vertical ? Color2.hex : Color1.hex ),
		Vertex( Pos.x, Pos.y, 0.f, 1.f, Color1.hex )
	};

	RotateVertices( &Vertices );
	WriteToBuffer( LINE, &Vertices, nullptr );
}

void cBuffer::FilledGradient( const Vec2< int16_t > Pos, const Vec2< int16_t > Size, const Color Color1, const Color Color2, const bool Vertical ) {
	std::vector<Vertex> Vertices = {
		Vertex( Pos.x, Pos.y, 0.f, 1.f, Color1.hex ),
		Vertex( Pos.x + Size.x, Pos.y, 0.f, 1.f, Vertical ? Color1.hex : Color2.hex ),
		Vertex( Pos.x + Size.x, Pos.y + Size.y, 0.f, 1.f, Color2.hex ),
		Vertex( Pos.x + Size.x, Pos.y + Size.y, 0.f, 1.f, Color2.hex ),
		Vertex( Pos.x, Pos.y + Size.y, 0.f, 1.f, Vertical ? Color2.hex : Color1.hex ),
		Vertex( Pos.x, Pos.y, 0.f, 1.f, Color1.hex )
	};

	RotateVertices( &Vertices );
	WriteToBuffer( TRIANGLE, &Vertices, nullptr );
}

void cBuffer::Gradient( const Vec2< int16_t > Pos, const Vec2< int16_t > Size, const Color ColorTL, const Color ColorTR, const Color ColorBR, const Color ColorBL ) {
	std::vector<Vertex> Vertices = {
		Vertex( Pos.x, Pos.y, 0.f, 1.f, ColorTL.hex ),
		Vertex( Pos.x + Size.x, Pos.y, 0.f, 1.f, ColorTR.hex ),
		Vertex( Pos.x + Size.x, Pos.y + Size.y, 0.f, 1.f, ColorBR.hex ),
		Vertex( Pos.x + Size.x, Pos.y + Size.y, 0.f, 1.f, ColorBR.hex ),
		Vertex( Pos.x, Pos.y + Size.y, 0.f, 1.f, ColorBL.hex ),
		Vertex( Pos.x, Pos.y, 0.f, 1.f, ColorTL.hex )
	};

	RotateVertices( &Vertices );
	WriteToBuffer( LINE, &Vertices, nullptr );
}

void cBuffer::FilledGradient( const Vec2< int16_t > Pos, const Vec2< int16_t > Size, const Color ColorTL, const Color ColorTR, const Color ColorBR, const Color ColorBL ) {
	std::vector<Vertex> Vertices = {
		Vertex( Pos.x, Pos.y, 0.f, 1.f, ColorTL.hex ),
		Vertex( Pos.x + Size.x, Pos.y, 0.f, 1.f, ColorTR.hex ),
		Vertex( Pos.x + Size.x, Pos.y + Size.y, 0.f, 1.f, ColorBR.hex ),
		Vertex( Pos.x + Size.x, Pos.y + Size.y, 0.f, 1.f, ColorBR.hex ),
		Vertex( Pos.x, Pos.y + Size.y, 0.f, 1.f, ColorBL.hex ),
		Vertex( Pos.x, Pos.y, 0.f, 1.f, ColorTL.hex )
	};

	RotateVertices( &Vertices );
	WriteToBuffer( TRIANGLE, &Vertices, nullptr );
}

void cBuffer::Triangle( const Vec2< int16_t > Point1, const Vec2< int16_t > Point2, const Vec2< int16_t > Point3, const Color Color ) {
	std::vector<Vertex> Vertices = {
		Vertex( Point1.x, Point1.y, 0.f, 1.f, Color.hex ),
		Vertex( Point2.x, Point2.y, 0.f, 1.f, Color.hex ),
		Vertex( Point3.x, Point3.y, 0.f, 1.f, Color.hex )
	};

	RotateVertices( &Vertices );
	WriteToBuffer( LINE, &Vertices, nullptr );
}

void cBuffer::FilledTriangle( const Vec2< int16_t > Point1, const Vec2< int16_t > Point2, const Vec2< int16_t > Point3, const Color Color ) {
	std::vector<Vertex> Vertices = {
		Vertex( Point1.x, Point1.y, 0.f, 1.f, Color.hex ),
		Vertex( Point2.x, Point2.y, 0.f, 1.f, Color.hex ),
		Vertex( Point3.x, Point3.y, 0.f, 1.f, Color.hex )
	};

	RotateVertices( &Vertices );
	WriteToBuffer( TRIANGLE, &Vertices, nullptr );
}

void cBuffer::Circle( const Vec2< int16_t > Pos, const int16_t Radius, const Color Color ) {
	std::vector<Vec2<int16_t>> points;
	points.reserve( m_CircleSegments + 1 );

	GenerateArcPoints( &points, &Pos, Radius, 100, 0, m_CircleSegments );
	Polyline( points, Color );
}

void cBuffer::FilledCircle( const Vec2< int16_t > Pos, const int16_t Radius, const Color Color ) {
	std::vector<Vec2<int16_t>> points;
	points.reserve( m_CircleSegments + 1 );

	GenerateArcPoints( &points, &Pos, Radius, 100, 0, m_CircleSegments );
	Polygon( points, Color );
}

void cBuffer::String( const Font* Font, const std::string& String, const Vec2<int16_t> Pos, const Color Color ) {
	if ( !Font || String.empty( ) )
		return;

	Vec2<int16_t> TextSize = GetStringSize( Font, String ), Advance = { 0, 0 };
	int RowHeight = 0;

	for ( auto& Letter : String ) {
		if ( !std::isprint( Letter ) && Letter != '\n' || Letter == ' ' ) {
			Advance.x += Font->CharSet[ Letter ].Advance / 64;
			continue;
		}

		const Glyph& Glyph = Font->CharSet[ Letter ];
		RowHeight = max( RowHeight, static_cast< int >( Glyph.Size.y ) );

		if ( Letter == '\n' ) {
			Advance.y += RowHeight + Font->Padding;
			Advance.x = 0;
			continue;
		}

		Vec2<float> LetterPos = {
			std::round( static_cast< float >( Pos.x ) + Advance.x + Glyph.Bearing.x ) + 0.5f,
			std::round( static_cast< float >( Pos.y ) + ( TextSize.y * 0.75f ) + Advance.y - Glyph.Bearing.y ) + 0.5f
		};

		std::vector<Vertex> Vertices = {
			{LetterPos.x, LetterPos.y, 0.f, 1.f, Color.hex, 0.f, 0.f},
			{LetterPos.x + Glyph.Size.x, LetterPos.y, 0.f, 1.f, Color.hex, 1.f, 0.f},
			{LetterPos.x + Glyph.Size.x, LetterPos.y + Glyph.Size.y, 0.f, 1.f, Color.hex, 1.f, 1.f},
			{LetterPos.x, LetterPos.y + Glyph.Size.y, 0.f, 1.f, Color.hex, 0.f, 1.f}
		};

		PushTexture( Glyph.Resource );
		WriteToBuffer( TRIANGLE, &Vertices, nullptr );
		PopTexture( );

		Advance.x += Glyph.Advance / 64;
	}
}

Vec2<int16_t> cBuffer::GetStringSize( const Font* Font, const std::string& String ) {
	if ( !Font || String.empty( ) )
		return Vec2<int16_t>( );

	Vec2<int16_t> Size{ 0, static_cast< int16_t >( Font->Size * 1.5f ) };

	for ( char Letter : String )
		Size.x += Font->CharSet.at( Letter ).Advance / 64;

	return Size;
}

void cBuffer::GenerateArcPoints( std::vector<Vec2<int16_t>>* Points, const Vec2<int16_t>* Pos, const int16_t Radius, const int16_t Completion, const int16_t Rotation, const int16_t Segments ) {
	double Angle = ( static_cast< double >( Rotation ) * M_PI ) / 180.0;
	double Conversion = Completion * 0.01;

	int16_t SegmentCount = m_DynamicArcSegments ? Segments > Radius ? Radius : max( Segments, 8 ) : Segments;
	Points->reserve( SegmentCount + 1 );

	auto GetPoint = [ & ] ( int16_t i ) {
		double Theta = Angle + 2.0 * Conversion * M_PI * static_cast< double >( i ) / static_cast< double >( SegmentCount );
		return Vec2<double>( static_cast< double >( Pos->x ) + Radius * cos( Theta ), static_cast< double >( Pos->y ) + Radius * sin( Theta ) );
	};

	for ( int i = 0; i < SegmentCount; i++ ) {
		Vec2<double> point = GetPoint( i );

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

void cBuffer::MakeVertices( std::vector<Vertex>* Vertices, const std::vector<Vec2<int16_t>>* points, const Color* clr ) {
	for ( const Vec2<int16_t>& point : *points )
		Vertices->push_back( Vertex( point.x, point.y, 0.f, 1.f, clr->hex ) );
}

void cBuffer::RotateObject( float val ) {
	m_Rotation = val;
}

void cBuffer::RotateVertices( std::vector<Vertex>* Vertices, Vec2<int16_t> CenterPoint ) {
	if ( Vertices->empty( ) )
		return;

	Vec2<int16_t> Center( 0, 0 );

	for ( const auto& Vertex : *Vertices ) {
		Center.x += Vertex.x;
		Center.y += Vertex.y;
	}
	Center.x /= static_cast< float >( Vertices->size( ) );
	Center.y /= static_cast< float >( Vertices->size( ) );

SKIP_CENTER:

	float Radians = m_Rotation * M_PI / 180.0f;
	float CosValue = cosf( Radians );
	float SinValue = sinf( Radians );

	for ( auto& vertex : *Vertices ) {
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

void cBuffer::CreateFontFromName( Font* Font, const char* FontName, const int16_t Size, const int16_t Weight, const int16_t Padding, const bool Antialiased ) {
	FT_Library lib;
	FT_Face face;

	Font->Path = GetFontPath( FontName );
	Font->Padding = Padding;
	Font->Size = Size;

	if ( FT_Init_FreeType( &lib ) != FT_Err_Ok )
		std::printf( std::vformat( "[ Buffer ] FT_Init_FreeType failed ( {} )\n", std::make_format_args( FontName ) ).c_str( ) );

	if ( FT_New_Face( lib, Font->Path.c_str( ), 0, &face ) )
		std::printf( std::vformat( "[ Buffer ] FT_New_Face failed ( {} )\n", std::make_format_args( FontName ) ).c_str( ) );

	FT_Set_Char_Size( face, Size * 64, 0, GetDpiForWindow( gWin32->GetHwnd( ) ), 0 );
	FT_Select_Charmap( face, FT_ENCODING_UNICODE );

	for ( unsigned char i = 0; i < 128; i++ ) {
		if ( FT_Load_Char( face, i, Antialiased ? FT_LOAD_RENDER : FT_LOAD_RENDER | FT_LOAD_TARGET_MONO ) )
			std::printf( std::vformat( "[ Buffer ] FT_Load_Char failed, font most likely does not exist! ( {} )\n", std::make_format_args( FontName ) ).c_str( ) );

		int32_t width = face->glyph->bitmap.width ? face->glyph->bitmap.width : 16;
		int32_t height = face->glyph->bitmap.rows ? face->glyph->bitmap.rows : 16;

		if ( gGraphics->GetDevice( )->CreateTexture( width, height, 1, D3DUSAGE_DYNAMIC, D3DFMT_A8, D3DPOOL_DEFAULT, &Font->CharSet[ i ].Resource, NULL ) )
			std::printf( std::vformat( "[ Buffer ] CreateTexture failed ( {} )\n", std::make_format_args( FontName ) ).c_str( ) );

		D3DLOCKED_RECT locked_rect;
		Font->CharSet[ i ].Resource->LockRect( 0, &locked_rect, nullptr, D3DLOCK_DISCARD );

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

		Font->CharSet[ i ].Resource->UnlockRect( 0 );

		D3DSURFACE_DESC description;
		Font->CharSet[ i ].Resource->GetLevelDesc( 0, &description );

		Font->CharSet[ i ].Size = { width, height };
		Font->CharSet[ i ].Bearing = { static_cast< int32_t >( face->glyph->bitmap_left ), static_cast< int32_t >( face->glyph->bitmap_top ) };
		Font->CharSet[ i ].Advance = face->glyph->advance.x;
	}

	FT_Done_Face( face );
	FT_Done_FreeType( lib );

	std::printf( std::vformat( "[ Buffer ] Created font ( name: {}, size: {}, weight: {}, antialiasing: {} )\n", std::make_format_args( FontName, Size, Weight, Antialiased ) ).c_str( ) );
}