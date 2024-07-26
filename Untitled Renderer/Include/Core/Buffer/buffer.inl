#include "Buffer.hpp"

inline void cBuffer::BuildDrawCommands( std::vector<DrawCommand>* draw_commands ) {
	for ( size_t i = 0; i < draw_commands->size( ); ++i ) {
		const DrawCommand* drawCommand = &( ( *draw_commands )[ i ] );

		m_DrawCommand.Vertices.insert( m_DrawCommand.Vertices.end( ),
			std::make_move_iterator( drawCommand->Vertices.begin( ) ),
			std::make_move_iterator( drawCommand->Vertices.end( ) )
		);

		m_DrawCommand.Indices.insert( m_DrawCommand.Indices.end( ),
			std::make_move_iterator( drawCommand->Indices.begin( ) ),
			std::make_move_iterator( drawCommand->Indices.end( ) )
		);

		m_DrawCommand.VerticesCount += drawCommand->VerticesCount;
		m_DrawCommand.IndicesCount += drawCommand->IndicesCount;
	}
}

// WIP, don't worry about messy code at the moment.
inline void cBuffer::GenerateArcVertices( std::vector<Vertex>* vertices, const Vec2<int16_t>* position, int16_t radius, int16_t completion, int16_t rotation, int16_t segments, Color center_color, Color color, bool filled ) {
	double Angle = ( static_cast< double >( rotation ) * M_PI ) / 180.0;
	double Conversion = static_cast< double >( completion ) * 0.01;

	double ThetaStep =  ((M_PI * 2.0) * Conversion) / static_cast< double >( segments );

	vertices->reserve( segments + 1 );

	auto GetPoint = [ & ] ( int16_t i ) {
		double Theta = Angle + static_cast< double >( i ) * ThetaStep;

		return Vec2<double>(
			static_cast< double >( position->x ) + radius * cos( Theta ),
			static_cast< double >( position->y ) + radius * sin( Theta )
		);
	};

	if ( filled ) {
		vertices->push_back( Vertex(
			static_cast< int16_t >( std::round( position->x ) ),
			static_cast< int16_t >( std::round( position->y ) ),
			0.f, 1.f, // z, rhw
			center_color.Hex,
			0.f, 0.f // u, v
		) );
	}

	for ( int i = 0; i <= segments; i++ ) {
		Vec2<double> point = GetPoint( i );

		vertices->push_back( Vertex(
			static_cast< int16_t >( std::round( point.x ) ),
			static_cast< int16_t >( std::round( point.y ) ),
			0.f, 1.f, // z, rhw
			color.Hex,
			0.f, 0.f // u, v
		) );
	}
}

inline void cBuffer::GenerateArcPoints( std::vector<Vec2<int16_t>>* Points, const Vec2<int16_t>* position, int16_t radius, int16_t completion, int16_t rotation, int16_t segments ) {
	double Angle = ( static_cast< double >( rotation ) * M_PI ) / 180.0;
	double Conversion = static_cast< double >( completion ) * 0.01;

	double ThetaStep = 2.0 * Conversion * M_PI / segments;

	Points->reserve( segments + 1 );

	auto GetPoint = [ & ] ( int16_t i ) {
		double Theta = Angle + static_cast< double >( i ) * ThetaStep;

		return Vec2<double>(
			static_cast< double >( position->x ) + radius * cos( Theta ),
			static_cast< double >( position->y ) + radius * sin( Theta )
		);
	};

	//Problem atm is were using line strips & triangle strips
	// We need to push back 3 points per triangle/line

	for ( int i = 0; i <= segments; i++ ) {
		Vec2<double> point = GetPoint( i );

		Points->push_back( Vec2<int16_t>(
			static_cast< int16_t >( std::round( point.x ) ),
			static_cast< int16_t >( std::round( point.y ) )
		) );
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

inline CommandResources* cBuffer::GetCommand( ) {
    return &m_CommandResources;
}

inline CompiledDrawCommand* cBuffer::GetDrawCommand( ) {
    return &m_DrawCommand;
}

inline std::vector<DrawCommand>* cBuffer::GetDrawCommands( ) {
    return &m_DrawCommands;
}

inline int cBuffer::GetCommandsCount( ) const {
    return m_DrawCommands.size();
}

inline int cBuffer::GetVerticesCount( ) const {
    return m_VerticesCount;
}

inline int cBuffer::GetIndicesCount( ) const {
    return m_IndicesCount;
}

inline void cBuffer::PushClip( Rect<int16_t> rect ) {
	m_CommandResources.ClipStack.push_back( RECT( rect.x, rect.y, rect.w, rect.h ) );
}

inline void cBuffer::PopClip( ) {
	m_CommandResources.ClipStack.pop_back( );
}

inline void cBuffer::PushTexture( IDirect3DTexture9* texture ) {
	m_CommandResources.TextureStack.push_back( texture );
}

inline void cBuffer::PopTexture( ) {
	m_CommandResources.TextureStack.pop_back( );
}

inline Font* cBuffer::GetDefaultFont( ) {
	return &DefaultFont;
}