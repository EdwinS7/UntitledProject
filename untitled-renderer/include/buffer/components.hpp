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

template <typename type_t = float>
class view_angles_t {
public:
	type_t pitch, yaw;

	constexpr view_angles_t( ) noexcept = default;
	constexpr view_angles_t( const type_t pitch, const type_t yaw ) noexcept : pitch( pitch ), yaw( yaw ) {}

	constexpr view_angles_t operator+( const view_angles_t& viewangles ) const noexcept { return { static_cast< type_t >( pitch + viewangles.pitch ), static_cast< type_t >( yaw + viewangles.yaw ) }; }
	constexpr view_angles_t operator-( const view_angles_t& viewangles ) const noexcept { return { static_cast< type_t >( pitch - viewangles.pitch ), static_cast< type_t >( yaw - viewangles.yaw ) }; }
	constexpr view_angles_t operator*( const view_angles_t& viewangles ) const noexcept { return { static_cast< type_t >( pitch * viewangles.pitch ), static_cast< type_t >( yaw * viewangles.yaw ) }; }
	constexpr view_angles_t operator/( const view_angles_t& viewangles ) const noexcept { return { static_cast< type_t >( pitch / viewangles.pitch ), static_cast< type_t >( yaw / viewangles.yaw ) }; }
	constexpr bool operator>( const view_angles_t& viewangles ) const noexcept { return pitch > viewangles.pitch && yaw > viewangles.yaw; }
	constexpr bool operator>=( const view_angles_t& viewangles ) const noexcept { return pitch >= viewangles.pitch && yaw >= viewangles.yaw; }
	constexpr bool operator<( const view_angles_t& viewangles ) const noexcept { return pitch < viewangles.pitch && yaw < viewangles.yaw; }
	constexpr bool operator<=( const view_angles_t& viewangles ) const noexcept { return pitch <= viewangles.pitch && yaw <= viewangles.yaw; }
	constexpr view_angles_t operator*=( const type_t& viewangles ) noexcept { pitch *= viewangles.pitch; yaw *= viewangles.yaw; return *this; }

#ifdef USE_CUSTOM_VIEW_ANGLES_OPERATORS
	USE_CUSTOM_VIEW_ANGLES_OPERATORS
#endif
};


template <typename type_t = int16_t>
class vector2_t {
public:
	type_t x, y;

	constexpr vector2_t( ) noexcept = default;
	constexpr vector2_t( const type_t x, const type_t y ) noexcept : x( x ), y( y ) {}

	constexpr vector2_t operator+( const vector2_t& vector ) const noexcept { return { static_cast< type_t >( x + vector.x ), static_cast< type_t >( y + vector.y ) }; }
	constexpr vector2_t operator-( const vector2_t& vector ) const noexcept { return { static_cast< type_t >( x - vector.x ), static_cast< type_t >( y - vector.y ) }; }
	constexpr vector2_t operator*( const vector2_t& vector ) const noexcept { return { static_cast< type_t >( x * vector.x ), static_cast< type_t >( y * vector.y ) }; }
	constexpr vector2_t operator/( const vector2_t& vector ) const noexcept { return { static_cast< type_t >( x / vector.x ), static_cast< type_t >( y / vector.y ) }; }
	constexpr bool operator>( const vector2_t& vector ) const noexcept { return x > vector.x && y > vector.y; }
	constexpr bool operator>=( const vector2_t& vector ) const noexcept { return x >= vector.x && y >= vector.y; }
	constexpr bool operator<( const vector2_t& vector ) const noexcept { return x < vector.x && y < vector.y; }
	constexpr bool operator<=( const vector2_t& vector ) const noexcept { return x <= vector.x && y <= vector.y; }
	constexpr bool operator==( const vector2_t& vector ) noexcept { return x == vector.x && y == vector.y; }
	constexpr vector2_t& operator*=( const type_t& val ) noexcept { return { x *= val, y *= val }; }

#ifdef USE_CUSTOM_VECTOR2_OPERATORS
	USE_CUSTOM_VECTOR2_OPERATORS
#endif
};

template <typename type_t = float>
class vector3_t {
public:
	type_t x, y, z;

	constexpr vector3_t( ) noexcept = default;
	constexpr vector3_t( const type_t x, const type_t y, const type_t z ) noexcept : x( x ), y( y ), z( z ) {}

	constexpr vector3_t operator+( const vector3_t& vector ) const noexcept { return { static_cast< type_t >( x + vector.x ), static_cast< type_t >( y + vector.y ), static_cast< type_t >( z + vector.z ) }; }
	constexpr vector3_t operator-( const vector3_t& vector ) const noexcept { return { static_cast< type_t >( x - vector.x ), static_cast< type_t >( y - vector.y ), static_cast< type_t >( z - vector.z ) }; }
	constexpr vector3_t operator*( const vector3_t& vector ) const noexcept { return { static_cast< type_t >( x * vector.x ), static_cast< type_t >( y * vector.y ), static_cast< type_t >( z * vector.z ) }; }
	constexpr vector3_t operator/( const vector3_t& vector ) const noexcept { return { static_cast< type_t >( x / vector.x ), static_cast< type_t >( y / vector.y ), static_cast< type_t >( z / vector.z ) }; }
	constexpr bool operator>( const vector3_t& vector ) const noexcept { return x > vector.x && y > vector.y && z > vector.z; }
	constexpr bool operator>=( const vector3_t& vector ) const noexcept { return x >= vector.x && y >= vector.y && z >= vector.z; }
	constexpr bool operator<( const vector3_t& vector ) const noexcept { return x < vector.x && y < vector.y && z < vector.z; }
	constexpr bool operator<=( const vector3_t& vector ) const noexcept { return x <= vector.x && y <= vector.y && z <= vector.z; }
	constexpr vector3_t& operator*=( const type_t& val ) noexcept { return { static_cast< type_t >( x * val ), static_cast< type_t >( y * val ), static_cast< type_t >( z * val ) }; }

#ifdef USE_CUSTOM_VECTOR3_OPERATORS
	USE_CUSTOM_VECTOR3_OPERATORS
#endif
};

template <typename type_t = float>
class vector4_t {
public:
	type_t w, x, y, z;

	constexpr vector4_t( ) noexcept = default;
	constexpr vector4_t( const type_t w, const type_t x, const type_t y, const type_t z ) noexcept : w( w ), x( x ), y( y ), z( z ) {}

	constexpr vector4_t operator+( const vector4_t& vector ) const noexcept { return { static_cast< type_t >( w + vector.w ), static_cast< type_t >( x + vector.x ), static_cast< type_t >( y + vector.y ), static_cast< type_t >( z + vector.z ) }; }
	constexpr vector4_t operator-( const vector4_t& vector ) const noexcept { return { static_cast< type_t >( w - vector.w ), static_cast< type_t >( x - vector.x ), static_cast< type_t >( y - vector.y ), static_cast< type_t >( z - vector.z ) }; }
	constexpr vector4_t operator*( const vector4_t& vector ) const noexcept { return { static_cast< type_t >( w * vector.w ), static_cast< type_t >( x * vector.x ), static_cast< type_t >( y * vector.y ), static_cast< type_t >( z * vector.z ) }; }
	constexpr vector4_t operator/( const vector4_t& vector ) const noexcept { return { static_cast< type_t >( w / vector.w ), static_cast< type_t >( x / vector.x ), static_cast< type_t >( y / vector.y ), static_cast< type_t >( z / vector.z ) }; }
	constexpr bool operator>( const vector4_t& vector ) const noexcept { return w > vector.w && x > vector.x && y > vector.y && z > vector.z; }
	constexpr bool operator>=( const vector4_t& vector ) const noexcept { return w >= vector.w && x >= vector.x && y >= vector.y && z >= vector.z; }
	constexpr bool operator<( const vector4_t& vector ) const noexcept { return w < vector.w && x < vector.x && y < vector.y && z < vector.z; }
	constexpr bool operator<=( const vector4_t& vector ) const noexcept { return w <= vector.w && x <= vector.x && y <= vector.y && z <= vector.z; }
	constexpr vector4_t operator*=( const type_t& val ) noexcept { return { static_cast< type_t >( w * val ), static_cast< type_t >( x * val ), static_cast< type_t >( y * val ), static_cast< type_t >( z * val ) }; }

#ifdef USE_CUSTOM_VECTOR4_OPERATORS
	USE_CUSTOM_VECTOR4_OPERATORS
#endif
};

class vertex_t {
public:
	float x, y, z, rhw;
	DWORD clr;
	float u, v;

	constexpr vertex_t( ) noexcept = default;
	constexpr vertex_t( const float x, const float y, const float z, const float rhw, const DWORD clr, const float u = 0.f, const float v = 0.f ) noexcept
		: x( x ), y( y ), z( z ), rhw( rhw ), clr( clr ), u( u ), v( v ) {}

#ifdef USE_CUSTOM_VERTEX_OPERATORS
	USE_CUSTOM_VERTEX_OPERATORS
#endif
};

#define COLOR(r,g,b,a) ((DWORD)((((a)&0xff)<<24)|(((r)&0xff)<<16)|(((g)&0xff)<<8)|((b)&0xff)))

class color_t {
public:
	DWORD hex;

	constexpr color_t( ) noexcept = default;
	constexpr color_t( const uint8_t r, const uint8_t g, const uint8_t b, const uint8_t a ) noexcept
		: hex( COLOR( r, g, b, a ) ) {}

#ifdef USE_CUSTOM_COLOR_OPERATORS
	USE_CUSTOM_COLOR_OPERATORS
#endif
};

struct glyph_t {
	texture resource;
	vector2_t<int32_t> size;
	vector2_t<int32_t> bearing;
	int32_t advance;

#ifdef USE_CUSTOM_GLYPH_OPERATORS
	USE_CUSTOM_GLYPH_OPERATORS
#endif
};

class font_t {
public:
	std::string path;
	int16_t padding;
	int16_t size;

	std::vector<glyph_t> char_set{ 256 };

#ifdef USE_CUSTOM_FONT_OPERATORS
	USE_CUSTOM_FONT_OPERATORS
#endif
};

class command_t {
public:
	std::vector<RECT> clips;
	std::vector<texture> textures;

#ifdef USE_CUSTOM_COMMAND_OPERATORS
	USE_CUSTOM_COMMAND_OPERATORS
#endif
};

class draw_command_t {
public:
	int8_t primitive;
	std::vector<vertex_t> vertices;
	std::vector<std::int32_t> indices;
	command_t command;
	int vertices_count,
		indices_count;

	constexpr draw_command_t( ) noexcept = default;
	constexpr draw_command_t( const int8_t primitive, const std::vector<vertex_t> vertices, const std::vector<std::int32_t> indices, command_t command, const int vertices_count, const int indices_count ) noexcept
		: primitive( primitive ), vertices( vertices ), indices( indices ), command( command ), vertices_count( vertices_count ), indices_count( indices_count ) {}

#ifdef USE_CUSTOM_DRAW_DRAW_COMMAND_OPERATORS
	USE_CUSTOM_DRAW_DRAW_COMMAND_OPERATORS
#endif
};

class compiled_draw_command_t {
public:
	std::vector<vertex_t> vertices;
	std::vector<std::int32_t> indices;
	int vertices_count,
		indices_count;

	constexpr compiled_draw_command_t( ) noexcept = default;
	constexpr compiled_draw_command_t( const std::vector<vertex_t> vertices, const std::vector<std::int32_t> indices, const int vertices_count, const int indices_count ) noexcept
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