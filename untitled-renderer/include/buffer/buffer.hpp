#pragma once

// includes
#include "../../Common.hpp"
#include "Components.hpp"

class cBuffer {
public:
    void Init( bool ApplyDefaults );
    void Destroy( );

    void WriteToBuffer( const int8_t Primitive, const std::vector<Vertex>* Vertices, const std::vector<int32_t>* Indices );
    void BuildDrawCommands( const std::vector<DrawCommand>& DrawCommands );

    void Line( const Vec2<int16_t> From, const Vec2<int16_t> To, const Color Color );
    void Polyline( const std::vector<Vec2<int16_t>> Points, const Color Color );
    void Polygon( const std::vector<Vec2<int16_t>> Points, const Color Color );
    void Rectangle( const Vec2<int16_t> Pos, const Vec2<int16_t> Size, const Color Color, const int16_t Rounding = 0, const CornerFlags Flags = CornerAll );
    void FilledRectangle( const Vec2<int16_t> Pos, const Vec2<int16_t> Size, const Color Color, const int16_t Rounding = 0, const CornerFlags Flags = CornerAll );
    void TexturedRectangle( IDirect3DTexture9* Resource, const Vec2<int16_t> Pos, const Vec2<int16_t> Size, const Color Color );
    void Gradient( const Vec2<int16_t> Pos, const Vec2<int16_t> Size, const Color ColorFrom, const Color ColorTo, const bool Vertical );
    void FilledGradient( const Vec2<int16_t> Pos, const Vec2<int16_t> Size, const Color ColorFrom, const Color ColorTo, const bool Vertical );
    void Gradient( const Vec2<int16_t> Pos, const Vec2<int16_t> Size, const Color Color1, const Color Color2, const Color Color3, const Color Color4 );
    void FilledGradient( const Vec2<int16_t> Pos, const Vec2<int16_t> Size, const Color Color1, const Color Color2, const Color Color3, const Color Color4 );
    void Triangle( const Vec2<int16_t> Point1, const Vec2<int16_t> Point2, const Vec2<int16_t> Point3, const Color Color );
    void FilledTriangle( const Vec2<int16_t> Point1, const Vec2<int16_t> Point2, const Vec2<int16_t> Point3, const Color Color );
    void Circle( const Vec2<int16_t> Pos, const int16_t Radius, const Color Color );
    void FilledCircle( const Vec2<int16_t> Pos, const int16_t Radius, const Color Color );

    void String( const Font* Font, const std::string& String, const Vec2<int16_t> Pos, const Color Color );
    Vec2<int16_t> GetStringSize( const Font* Font, const std::string& String );

    void RotateObject( float Degrees );

    inline void ClearCommands( );
    inline Command GetCommand( );
    inline CompiledDrawCommand GetDrawCommand( );
    inline std::vector<DrawCommand> GetDrawCommands( );

    inline int GetCommandsCount( );
    inline int GetVerticesCount( );
    inline int GetIndicesCount( );

    inline void PushClip( RECT Rect );
    inline void PopClip( );

    inline void PushTexture( IDirect3DTexture9* Resource );
    inline void PopTexture( );
    
    struct cFonts {
        Font Default, Interface;
    } Fonts;

    struct cTextures {
        IDirect3DTexture9* Default;
    } Textures;

private:
    float m_Rotation;

    int m_VerticesCount, m_IndicesCount;

    int m_CircleSegments, m_RectangleSegments;
    int m_BezierCubicSegments, m_BezierQuadraticSegments;

	std::vector < DrawCommand > m_draw_commands;
	CompiledDrawCommand m_DrawCommand;
	Command m_Command;

    void GenerateArcPoints( std::vector<Vec2<int16_t>>* Points, const Vec2<int16_t>* Pos, const int16_t Radius, const int16_t Completion, const int16_t Rotation, int16_t Segments );
    void GenerateQuadraticBezierPoints( std::vector<Vec2<int16_t>>* Points, const Vec2<int16_t> Point1, const Vec2<int16_t> Point2, const Vec2<int16_t> Point3 );

	void MakeVertices(  std::vector<Vertex>* Vertices,  const std::vector<Vec2<int16_t>>* points,  const Color* color );
    void RotateVertices( std::vector<Vertex>* Vertices, Vec2<int16_t> center_manual = { -1, -1 } );

    void CreateFontFromName( Font* Font, const char* FontName, const int16_t Size, const int16_t Weight, const int16_t Padding, const bool Antialiased );
    inline std::string GetFontPath( const char* FontName );
};

inline const auto gBuffer = std::make_unique<cBuffer>( );

#include "buffer.inl"