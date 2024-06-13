#pragma once
#ifndef BUFFER_H
#define BUFFER_H

#include <Common.hpp>
#include "Components.hpp"
#include "../Graphics/DirectX9.hpp"
#include "../../Scripting/LuaWrapper.hpp"

class cBuffer {
public:
    void Init( );
    void Release( );

    void Line( Vec2<int16_t> from, Vec2<int16_t> to, const Color color );

    void Polyline( const std::vector<Vec2<int16_t>>& points, Color color );
    void Polygon( const std::vector<Vec2<int16_t>>& points, Color color );

    void Rectangle( Vec2<int16_t> position, Vec2<int16_t> size, Color Color, int16_t Rounding = 0, CornerFlags Flags = CornerAll );
    void FilledRectangle( Vec2<int16_t> position, Vec2<int16_t> size, Color Color, int16_t Rounding = 0, CornerFlags Flags = CornerAll );

    void TexturedRectangle( IDirect3DTexture9* texture, Vec2<int16_t> position, Vec2<int16_t> size, Color color_modulation );
    void Picture( Image* image, Vec2<int16_t> position, Color color_modulation );

    void Cube( Vec3<float> position, float size, Color color );

    void Gradient( Vec2<int16_t> position, Vec2<int16_t> size, Color color_from, Color color_to, bool vertical );
    void FilledGradient( Vec2<int16_t> position, Vec2<int16_t> size, Color color_from, Color color_to, bool vertical );
    void Gradient( Vec2<int16_t> position, Vec2<int16_t> size, Color color_top_left, Color color_top_right, Color color_bottom_right, Color color_bottom_left );
    void FilledGradient( Vec2<int16_t> position, Vec2<int16_t> size, Color color_top_left, Color color_top_right, Color color_bottom_right, Color color_bottom_left );

    void Triangle( Vec2<int16_t> point1, Vec2<int16_t> point2, Vec2<int16_t> point3, Color color );
    void FilledTriangle( Vec2<int16_t> point1, Vec2<int16_t> point2, Vec2<int16_t> point3, Color color );

    void Circle( Vec2<int16_t> position, int16_t radius, Color color );
    void FilledCircle( Vec2<int16_t> position, int16_t radius, Color color );

    void Text( Font* font, const std::string& string, Vec2<int16_t> Pos, Color color );
    Vec2<int16_t> GetTextSize( Font* font, const std::string& string );

    inline void BuildDrawCommands( std::vector<DrawCommand>* draw_commands );
    inline std::vector<DrawCommand>* GetDrawCommands( );

    inline CompiledDrawCommand* GetDrawCommand( );
    inline CommandResources* GetCommand( );

    inline void ClearCommands( );

    inline int GetCommandsCount( ) const;
    inline int GetVerticesCount( ) const;
    inline int GetIndicesCount( ) const;

    inline void PushClip( Rect<int16_t> Rect );
    inline void PopClip( );

    inline void PushTexture( IDirect3DTexture9* Texture );
    inline void PopTexture( );

    Font* GetDefaultFont( );

private:
    int m_VerticesCount{ 0 }, m_IndicesCount{ 0 },
        m_CircleSegments{ 64 }, m_RectangleSegments{ 16 };

	std::vector < DrawCommand > m_DrawCommands;
    CommandResources m_CommandResources;
	CompiledDrawCommand m_DrawCommand;

    Font DefaultFont;

    void WriteToBuffer( int8_t primitive, const std::vector<Vertex>* vertices, const std::vector<int32_t>* indices );
    inline void GenerateArcPoints( std::vector<Vec2<int16_t>>* points, const Vec2<int16_t>* position, int16_t radius, int16_t completion, int16_t rotation, int16_t segments );

    inline void MakeVertices( std::vector<Vertex>* vertices, const std::vector<Vec2<int16_t>>* points, const Color* color );
};

inline const auto gBuffer = std::make_unique<cBuffer>( );

#include "Buffer.inl"
#endif