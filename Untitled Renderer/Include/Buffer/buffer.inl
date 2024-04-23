#include "Buffer.hpp"

inline void cBuffer::WriteToBuffer( const int8_t primitive, const std::vector< Vertex >* vertices, const std::vector<int32_t>* indices ) {
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

inline void cBuffer::BuildDrawCommands( const std::vector<DrawCommand>& draw_commands ) {
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

inline Vec2<int16_t> cBuffer::GetTextSize( const Font* font, const std::string& string ) {
	if ( !font || string.empty( ) )
		return Vec2<int16_t>( );

	Vec2<int16_t> Size{ 0, static_cast< int16_t >( font->Size * 1.5f ) };

	for ( char Letter : string )
		Size.x += font->CharSet.at( Letter ).Advance / 64;

	return Size;
}

inline void cBuffer::GenerateArcPoints( std::vector<Vec2<int16_t>>* Points, const Vec2<int16_t>* position, const int16_t radius, const int16_t completion, const int16_t rotation, const int16_t segments ) {
	double Angle = ( static_cast< double >( rotation ) * M_PI ) / 180.0;
	double Conversion = completion * 0.01;

	int16_t SegmentCount = m_DynamicArcSegments ? segments > radius ? radius : max( segments, 8 ) : segments;
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

inline void cBuffer::GenerateQuadraticBezierPoints( std::vector<Vec2<int16_t>>* points, const Vec2<int16_t> point1, const Vec2<int16_t> point2, const Vec2<int16_t> point3 ) {
	for ( int i = 0; i < m_BezierQuadraticSegments; i++ ) {
		int CompletionFactor = static_cast< double >( i ) / static_cast< double >( m_BezierQuadraticSegments );

		points->push_back( {
			static_cast< int16_t >( std::round( std::lerp( std::lerp( point1.x, point3.x, CompletionFactor ), std::lerp( point3.x, point2.x, CompletionFactor ), CompletionFactor ) ) ),
			static_cast< int16_t >( std::round( std::lerp( std::lerp( point1.y, point3.y, CompletionFactor ), std::lerp( point3.y, point2.y, CompletionFactor ), CompletionFactor ) ) )
		} );
	}
}

inline void cBuffer::MakeVertices( std::vector<Vertex>* vertices, const std::vector<Vec2<int16_t>>* points, const Color* color ) {
	for ( const Vec2<int16_t>& point : *points )
		vertices->push_back( Vertex( point.x, point.y, 0.f, 1.f, color->Hex ) );
}

inline void cBuffer::ClearCommands( ) {
    m_DrawCommands.clear( );
    m_DrawCommand.reset( );

    m_VerticesCount = 0;
    m_IndicesCount = 0;
}

inline CommandResources cBuffer::GetCommand( ) {
    return m_CommandResources;
}

inline CompiledDrawCommand cBuffer::GetDrawCommand( ) {
    return m_DrawCommand;
}

inline std::vector<DrawCommand> cBuffer::GetDrawCommands( ) {
    return m_DrawCommands;
}

inline int cBuffer::GetCommandsCount( ) {
    return m_DrawCommands.size();
}

inline int cBuffer::GetVerticesCount( ) {
    return m_VerticesCount;
}

inline int cBuffer::GetIndicesCount( ) {
    return m_IndicesCount;
}

inline void cBuffer::PushClip( Rect<int16_t> Rect ) {
	m_CommandResources.ClipStack.push_back( RECT( Rect.x, Rect.y, Rect.w, Rect.h ) );
}

inline void cBuffer::PopClip( ) {
	m_CommandResources.ClipStack.pop_back( );
}

inline void cBuffer::PushTexture( IDirect3DTexture9* Texture ) {
	m_CommandResources.TextureStack.push_back( Texture );
}

inline void cBuffer::PopTexture( ) {
	m_CommandResources.TextureStack.pop_back( );
}

inline Font* cBuffer::GetDefaultFont( ) {
	return &DefaultFont;
}