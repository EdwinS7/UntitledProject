#pragma once

// includes
#include "../../Common.hpp"
#include "Components.hpp"

class cBuffer {
public:
    void Init( );
    void Release( );

    void WriteToBuffer( const int8_t primitive, const std::vector<Vertex>* vertices, const std::vector<int32_t>* indices );
    void BuildDrawCommands( const std::vector<DrawCommand>& draw_commands );

    void Line( const Vec2<int16_t> from, const Vec2<int16_t> to, const Color color );

    void Polyline( const std::vector<Vec2<int16_t>> points, const Color color );
    void Polygon( const std::vector<Vec2<int16_t>> points, const Color color );

    void Rectangle( const Vec2<int16_t> position, const Vec2<int16_t> size, const Color Color, const int16_t Rounding = 0, const CornerFlags Flags = CornerAll );
    void FilledRectangle( const Vec2<int16_t> position, const Vec2<int16_t> size, const Color Color, const int16_t Rounding = 0, const CornerFlags Flags = CornerAll );
    void TexturedRectangle( LPDIRECT3DTEXTURE9* texture, const Vec2<int16_t> position, const Vec2<int16_t> size, const Color Color );

    void Gradient( const Vec2<int16_t> position, const Vec2<int16_t> size, const Color color_from, const Color color_to, const bool vertical );
    void FilledGradient( const Vec2<int16_t> position, const Vec2<int16_t> size, const Color color_from, const Color color_to, const bool vertical );
    void Gradient( const Vec2<int16_t> position, const Vec2<int16_t> size, const Color color_top_left, const Color color_top_right, const Color color_bottom_right, const Color color_bottom_left );
    void FilledGradient( const Vec2<int16_t> position, const Vec2<int16_t> size, const Color color_top_left, const Color color_top_right, const Color color_bottom_right, const Color color_bottom_left );

    void Triangle( const Vec2<int16_t> point1, const Vec2<int16_t> point2, const Vec2<int16_t> point3, const Color color );
    void FilledTriangle( const Vec2<int16_t> point1, const Vec2<int16_t> point2, const Vec2<int16_t> point3, const Color color );

    void Circle( const Vec2<int16_t> position, const int16_t radius, const Color color );
    void FilledCircle( const Vec2<int16_t> position, const int16_t radius, const Color color );

    void String( const Font* font, const std::string& string, const Vec2<int16_t> Pos, const Color color );
    Vec2<int16_t> GetStringSize( const Font* font, const std::string& string );

    inline void ClearCommands( );
    inline CommandResources GetCommand( );
    inline CompiledDrawCommand GetDrawCommand( );
    inline std::vector<DrawCommand> GetDrawCommands( );

    inline int GetCommandsCount( );
    inline int GetVerticesCount( );
    inline int GetIndicesCount( );

    inline void PushClip( const Rect<int16_t> Rect );
    inline void PopClip( );

    inline void PushTexture( IDirect3DTexture9* Texture );
    inline void PopTexture( );
    
    struct cFonts {
        Font Default, Interface, SmallInterface;
    } Fonts;

    struct cTextures {

    } Textures;

private:
    int m_VerticesCount, m_IndicesCount;

    int m_CircleSegments{ 64 }, m_RectangleSegments{ 16 };
    int m_BezierCubicSegments{ 128 }, m_BezierQuadraticSegments{ 128 };

    bool m_DynamicArcSegments;

	std::vector < DrawCommand > m_DrawCommands;
    CommandResources m_CommandResources;
	CompiledDrawCommand m_DrawCommand;

    void GenerateArcPoints( std::vector<Vec2<int16_t>>* points, const Vec2<int16_t>* position, const int16_t radius, const int16_t completion, const int16_t rotation, int16_t segments );
    void GenerateQuadraticBezierPoints( std::vector<Vec2<int16_t>>* points, const Vec2<int16_t> point1, const Vec2<int16_t> point2, const Vec2<int16_t> point3 );

	void MakeVertices(  std::vector<Vertex>* vertices,  const std::vector<Vec2<int16_t>>* points, const Color* color );
};

inline const auto gBuffer = std::make_unique<cBuffer>( );

#include "buffer.inl"