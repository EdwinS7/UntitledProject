#pragma once
#include "buffer.hpp"

enum corner_flags {
	corner_none = 0 << 0,
	corner_top_left = 1 << 0,
	corner_top_right = 1 << 1,
	corner_bottom_left = 1 << 2,
	corner_bottom_right = 1 << 3,

	corner_top = corner_top_left | corner_top_right,
	corner_right = corner_top_right | corner_bottom_right,
	corner_bottom = corner_bottom_left | corner_bottom_right,
	corner_left = corner_top_left | corner_bottom_left,

	corner_all = corner_top_left | corner_top_right |
	corner_bottom_left | corner_bottom_right
};


template <typename type_t = int16_t>
class Vec2 {
public:
	type_t x, y;

	constexpr Vec2( ) noexcept = default;
	constexpr Vec2( const type_t x, const type_t y ) noexcept : x( x ), y( y ) {}

	constexpr Vec2 operator+( const Vec2& vector ) const noexcept { return { static_cast< type_t >( x + vector.x ), static_cast< type_t >( y + vector.y ) }; }
	constexpr Vec2 operator-( const Vec2& vector ) const noexcept { return { static_cast< type_t >( x - vector.x ), static_cast< type_t >( y - vector.y ) }; }
	constexpr Vec2 operator*( const Vec2& vector ) const noexcept { return { static_cast< type_t >( x * vector.x ), static_cast< type_t >( y * vector.y ) }; }
	constexpr Vec2 operator/( const Vec2& vector ) const noexcept { return { static_cast< type_t >( x / vector.x ), static_cast< type_t >( y / vector.y ) }; }
	constexpr bool operator>( const Vec2& vector ) const noexcept { return x > vector.x && y > vector.y; }
	constexpr bool operator>=( const Vec2& vector ) const noexcept { return x >= vector.x && y >= vector.y; }
	constexpr bool operator<( const Vec2& vector ) const noexcept { return x < vector.x && y < vector.y; }
	constexpr bool operator<=( const Vec2& vector ) const noexcept { return x <= vector.x && y <= vector.y; }
	constexpr bool operator==( const Vec2& vector ) noexcept { return x == vector.x && y == vector.y; }
	constexpr Vec2& operator*=( const type_t& val ) noexcept { return { x *= val, y *= val }; }

#ifdef USE_CUSTOM_VECTOR2_OPERATORS
	USE_CUSTOM_VECTOR2_OPERATORS
#endif
};

template <typename type_t = float>
class Vec3 {
public:
	type_t x, y, z;

	constexpr Vec3( ) noexcept = default;
	constexpr Vec3( const type_t x, const type_t y, const type_t z ) noexcept : x( x ), y( y ), z( z ) {}

	constexpr Vec3 operator+( const Vec3& vector ) const noexcept { return { static_cast< type_t >( x + vector.x ), static_cast< type_t >( y + vector.y ), static_cast< type_t >( z + vector.z ) }; }
	constexpr Vec3 operator-( const Vec3& vector ) const noexcept { return { static_cast< type_t >( x - vector.x ), static_cast< type_t >( y - vector.y ), static_cast< type_t >( z - vector.z ) }; }
	constexpr Vec3 operator*( const Vec3& vector ) const noexcept { return { static_cast< type_t >( x * vector.x ), static_cast< type_t >( y * vector.y ), static_cast< type_t >( z * vector.z ) }; }
	constexpr Vec3 operator/( const Vec3& vector ) const noexcept { return { static_cast< type_t >( x / vector.x ), static_cast< type_t >( y / vector.y ), static_cast< type_t >( z / vector.z ) }; }
	constexpr bool operator>( const Vec3& vector ) const noexcept { return x > vector.x && y > vector.y && z > vector.z; }
	constexpr bool operator>=( const Vec3& vector ) const noexcept { return x >= vector.x && y >= vector.y && z >= vector.z; }
	constexpr bool operator<( const Vec3& vector ) const noexcept { return x < vector.x && y < vector.y && z < vector.z; }
	constexpr bool operator<=( const Vec3& vector ) const noexcept { return x <= vector.x && y <= vector.y && z <= vector.z; }
	constexpr Vec3& operator*=( const type_t& val ) noexcept { return { static_cast< type_t >( x * val ), static_cast< type_t >( y * val ), static_cast< type_t >( z * val ) }; }

#ifdef USE_CUSTOM_VECTOR3_OPERATORS
	USE_CUSTOM_VECTOR3_OPERATORS
#endif
};

template <typename type_t = float>
class Vec4 {
public:
	type_t w, x, y, z;

	constexpr Vec4( ) noexcept = default;
	constexpr Vec4( const type_t w, const type_t x, const type_t y, const type_t z ) noexcept : w( w ), x( x ), y( y ), z( z ) {}

	constexpr Vec4 operator+( const Vec4& vector ) const noexcept { return { static_cast< type_t >( w + vector.w ), static_cast< type_t >( x + vector.x ), static_cast< type_t >( y + vector.y ), static_cast< type_t >( z + vector.z ) }; }
	constexpr Vec4 operator-( const Vec4& vector ) const noexcept { return { static_cast< type_t >( w - vector.w ), static_cast< type_t >( x - vector.x ), static_cast< type_t >( y - vector.y ), static_cast< type_t >( z - vector.z ) }; }
	constexpr Vec4 operator*( const Vec4& vector ) const noexcept { return { static_cast< type_t >( w * vector.w ), static_cast< type_t >( x * vector.x ), static_cast< type_t >( y * vector.y ), static_cast< type_t >( z * vector.z ) }; }
	constexpr Vec4 operator/( const Vec4& vector ) const noexcept { return { static_cast< type_t >( w / vector.w ), static_cast< type_t >( x / vector.x ), static_cast< type_t >( y / vector.y ), static_cast< type_t >( z / vector.z ) }; }
	constexpr bool operator>( const Vec4& vector ) const noexcept { return w > vector.w && x > vector.x && y > vector.y && z > vector.z; }
	constexpr bool operator>=( const Vec4& vector ) const noexcept { return w >= vector.w && x >= vector.x && y >= vector.y && z >= vector.z; }
	constexpr bool operator<( const Vec4& vector ) const noexcept { return w < vector.w && x < vector.x && y < vector.y && z < vector.z; }
	constexpr bool operator<=( const Vec4& vector ) const noexcept { return w <= vector.w && x <= vector.x && y <= vector.y && z <= vector.z; }
	constexpr Vec4 operator*=( const type_t& val ) noexcept { return { static_cast< type_t >( w * val ), static_cast< type_t >( x * val ), static_cast< type_t >( y * val ), static_cast< type_t >( z * val ) }; }

#ifdef USE_CUSTOM_VECTOR4_OPERATORS
	USE_CUSTOM_VECTOR4_OPERATORS
#endif
};

class Vertex {
public:
	float x, y, z, rhw;
	DWORD clr;
	float u, v;

	constexpr Vertex( ) noexcept = default;
	constexpr Vertex( const float x, const float y, const float z, const float rhw, const DWORD clr, const float u = 0.f, const float v = 0.f ) noexcept
		: x( x ), y( y ), z( z ), rhw( rhw ), clr( clr ), u( u ), v( v ) {}

#ifdef USE_CUSTOM_VERTEX_OPERATORS
	USE_CUSTOM_VERTEX_OPERATORS
#endif
};

#define COLOR(r,g,b,a) ((DWORD)((((a)&0xff)<<24)|(((r)&0xff)<<16)|(((g)&0xff)<<8)|((b)&0xff)))

class Color {
public:
	DWORD hex;

	constexpr Color( ) noexcept = default;
	constexpr Color( const uint8_t r, const uint8_t g, const uint8_t b, const uint8_t a ) noexcept
		: hex( COLOR( r, g, b, a ) ) {}

#ifdef USE_CUSTOM_COLOR_OPERATORS
	USE_CUSTOM_COLOR_OPERATORS
#endif
};

struct Glyph {
	IDirect3DTexture9* resource;
	Vec2<int32_t> size;
	Vec2<int32_t> bearing;
	int32_t advance;

#ifdef USE_CUSTOM_GLYPH_OPERATORS
	USE_CUSTOM_GLYPH_OPERATORS
#endif
};

class Font {
public:
	std::string path;
	int16_t padding;
	int16_t size;

	std::vector<Glyph> char_set{ 256 };

	void Release( ) {
		for ( auto& cur_char : char_set ) {
			cur_char.resource->Release( );
			cur_char.resource = nullptr;
		}
	}

#ifdef USE_CUSTOM_FONT_OPERATORS
	USE_CUSTOM_FONT_OPERATORS
#endif
};

class Command {
public:
	std::vector<RECT> clips;
	std::vector<IDirect3DTexture9*> textures;

#ifdef USE_CUSTOM_COMMAND_OPERATORS
	USE_CUSTOM_COMMAND_OPERATORS
#endif
};

class DrawCommand {
public:
	int8_t primitive;
	std::vector<Vertex> vertices;
	std::vector<std::int32_t> indices;
	Command command;
	int vertices_count,
		indices_count;

	constexpr DrawCommand( ) noexcept = default;
	constexpr DrawCommand( const int8_t primitive, const std::vector<Vertex> vertices, const std::vector<std::int32_t> indices, Command command, const int vertices_count, const int indices_count ) noexcept
		: primitive( primitive ), vertices( vertices ), indices( indices ), command( command ), vertices_count( vertices_count ), indices_count( indices_count ) {}

#ifdef USE_CUSTOM_DRAW_DRAW_COMMAND_OPERATORS
	USE_CUSTOM_DRAW_DRAW_COMMAND_OPERATORS
#endif
};

class CompiledDrawCommand {
public:
	std::vector<Vertex> vertices;
	std::vector<std::int32_t> indices;
	int vertices_count,
		indices_count;

	constexpr CompiledDrawCommand( ) noexcept = default;
	constexpr CompiledDrawCommand( const std::vector<Vertex> vertices, const std::vector<std::int32_t> indices, const int vertices_count, const int indices_count ) noexcept
		: vertices( vertices ), indices( indices ), vertices_count( vertices_count ), indices_count( indices_count ) {}

	void reset( ) {
		vertices.clear( );
		indices.clear( );
		vertices_count = 0;
		indices_count = 0;
	}

#ifdef USE_CUSTOM_DRAW_COMPILED_DRAW_COMMAND_OPERATORS
	USE_CUSTOM_DRAW_COMPILED_DRAW_COMMAND_OPERATORS
#endif
};