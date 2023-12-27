#pragma once

// includes
#include "../../common.hpp"
#include "components.hpp"
#include "fonts/font.hpp"

class cBuffer {
public:
    void CreateObjects( );
    void DestroyObjects( );

    void WriteToBuffer( const int8_t Primitive, const std::vector<Vertex>* Vertices, const std::vector<int32_t>* Indices );
    void BuildDrawCommands( const std::vector<DrawCommand>& DrawCommands );

    void Line( const Vec2<int16_t> From, const Vec2<int16_t> To, const Color Color );
    void Polyline( const std::vector<Vec2<int16_t>> Points, const Color Color );
    void Polygon( const std::vector<Vec2<int16_t>> Points, const Color Color );
    void Rectangle( const Vec2<int16_t> Pos, const Vec2<int16_t> Size, const Color Color, const int16_t Rounding = 0, const corner_flags Flags = corner_all );
    void FilledRectangle( const Vec2<int16_t> Pos, const Vec2<int16_t> Size, const Color Color, const int16_t Rounding = 0, const corner_flags Flags = corner_all );
    void TexturedRectangle( IDirect3DTexture9* Resource, const Vec2<int16_t> Pos, const Vec2<int16_t> Size, const Color Color );
    void Gradient( const Vec2<int16_t> Pos, const Vec2<int16_t> Size, const Color ColorFrom, const Color ColorTo, const bool Vertical );
    void FilledGradient( const Vec2<int16_t> Pos, const Vec2<int16_t> Size, const Color ColorFrom, const Color ColorTo, const bool Vertical );
    void Gradient( const Vec2<int16_t> Pos, const Vec2<int16_t> Size, const Color Color1, const Color Color2, const Color Color3, const Color Color4 );
    void FilledGradient( const Vec2<int16_t> Pos, const Vec2<int16_t> Size, const Color Color1, const Color Color2, const Color Color3, const Color Color4 );
    void Triangle( const Vec2<int16_t> Point1, const Vec2<int16_t> Point2, const Vec2<int16_t> Point3, const Color Color );
    void FilledTriangle( const Vec2<int16_t> Point1, const Vec2<int16_t> Point2, const Vec2<int16_t> Point3, const Color Color );
    void Circle( const Vec2<int16_t> Pos, const int16_t Radius, const Color Color );
    void FilledCircle( const Vec2<int16_t> Pos, const int16_t Radius, const Color Color );

    void String( const Font* Font, const char* String, const Vec2<int16_t> Pos, const Color Color );
    Vec2<int16_t> GetStringSize( const Font* Font, const char* String );

    void RotateObject( float Degrees );

    inline void ClearCommands( );
    inline Command GetCommand( );
    inline CompiledDrawCommand GetDrawCommand( );
    inline std::vector<DrawCommand> GetDrawCommands( );

    inline int GetCommandsCount( );
    inline int GetVerticesCount( );
    inline int GetIndicesCount( );

    inline void PushClip( RECT rect );
    inline void PopClip( );

    inline void PushTexture( IDirect3DTexture9* Resource );
    inline void PopTexture( );

    void GenerateQuadraticBezierPoints( std::vector<Vec2<int16_t>>* Points, const Vec2<int16_t> Point1, const Vec2<int16_t> Point2, const Vec2<int16_t> Point3 );

    struct cFonts {
        Font Default;
    } Fonts;

    struct cTextures {
        IDirect3DTexture9* Default;
    } Textures;

private:
    float m_rotation;

    int m_vertices_count;
    int m_indices_count;

	std::vector < DrawCommand > m_draw_commands;
	CompiledDrawCommand m_draw_command;
	Command m_command;

	void generate_arc_points( 
		std::vector<Vec2<int16_t>>* points,
		const Vec2<int16_t>* pos, 
		const int16_t radius,
		const int16_t completion,
		const int16_t rotation,
		const int16_t segments
	);

	void make_vertices( 
		std::vector<Vertex>* vertices, 
		const std::vector<Vec2<int16_t>>* points, 
		const Color* color
	);
    void rotate_object( std::vector<Vertex>* vertices, Vec2<int16_t> center_manual = { -1, -1 } );
};

inline const auto gBuffer = std::make_unique<cBuffer>( );

/* lower = performance, higher = quality */
#define CIRCLE_SEGMENTS 64
#define RECTANGLE_SEGMENTS 16
#define BEZIER_CUBIC_SEGMENTS 128
#define BEZIER_QUADRATIC_SEGMENTS 128

#include "buffer.inl"