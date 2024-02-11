#pragma once
#include "buffer.hpp"

enum CornerFlags {
	CornerNone = 0 << 0,
	CornerTopLeft = 1 << 0,
	CornerTopRight = 1 << 1,
	CornerBottomLeft = 1 << 2,
	CornerBottomRight = 1 << 3,

	CornerTop = CornerTopLeft | CornerTopRight,
	CornerRight = CornerTopRight | CornerBottomRight,
	CornerRottom = CornerBottomLeft | CornerBottomRight,
	CornerLeft = CornerTopLeft | CornerBottomLeft,

	CornerAll = CornerTopLeft | CornerTopRight | CornerBottomLeft | CornerBottomRight
};

template <typename T = int16_t>
class Vec2 {
public:
	T x, y;

	constexpr Vec2() noexcept = default;
	constexpr Vec2(T x, T y) noexcept : x(x), y(y) {}

	constexpr Vec2 operator+(const Vec2& v) const noexcept { return { static_cast<T>(x + v.x), static_cast<T>(y + v.y) }; }
	constexpr Vec2 operator-(const Vec2& v) const noexcept { return { static_cast<T>(x - v.x), static_cast<T>(y - v.y) }; }
	constexpr Vec2 operator*(const Vec2& v) const noexcept { return { static_cast<T>(x * v.x), static_cast<T>(y * v.y) }; }
	constexpr Vec2 operator/(const Vec2& v) const noexcept { return { static_cast<T>(x / v.x), static_cast<T>(y / v.y) }; }

	constexpr bool operator>(const Vec2& v) const noexcept { return x > v.x && y > v.y; }
	constexpr bool operator>=(const Vec2& v) const noexcept { return x >= v.x && y >= v.y; }
	constexpr bool operator<(const Vec2& v) const noexcept { return x < v.x && y < v.y; }
	constexpr bool operator<=(const Vec2& v) const noexcept { return x <= v.x && y <= v.y; }

	constexpr bool operator==(const Vec2& v) const noexcept { return x == v.x && y == v.y; }

	constexpr Vec2& operator*=(const T& val) noexcept { x *= val; y *= val; return *this; }

	constexpr void Lerp( const Vec2& v, const float& t ) {
		x = static_cast< T >( x + ( v.x - x ) * t );
		y = static_cast< T >( y + ( v.y - y ) * t );
	}

	constexpr Vec2<int16_t> AsInt( ) {
		return Vec2<int16_t>( std::round( x ), std::round( y ) );
	}

	constexpr Vec2<float> AsFloat( ) {
		return Vec2<float>( x, y );
	}
};

template <typename T = int16_t>
class Vec3 {
public:
	T x, y, z;

	constexpr Vec3() noexcept = default;
	constexpr Vec3(T x, T y, T z) noexcept : x(x), y(y), z(z) {}

	constexpr Vec3 operator+(const Vec3& v) const noexcept { return { static_cast<T>(x + v.x), static_cast<T>(y + v.y), static_cast<T>(z + v.z) }; }
	constexpr Vec3 operator-(const Vec3& v) const noexcept { return { static_cast<T>(x - v.x), static_cast<T>(y - v.y), static_cast<T>(z - v.z) }; }
	constexpr Vec3 operator*(const Vec3& v) const noexcept { return { static_cast<T>(x * v.x), static_cast<T>(y * v.y), static_cast<T>(z * v.z) }; }
	constexpr Vec3 operator/(const Vec3& v) const noexcept { return { static_cast<T>(x / v.x), static_cast<T>(y / v.y), static_cast<T>(z / v.z) }; }

	constexpr bool operator>(const Vec3& v) const noexcept { return x > v.x && y > v.y && z > v.z; }
	constexpr bool operator>=(const Vec3& v) const noexcept { return x >= v.x && y >= v.y && z >= v.z; }
	constexpr bool operator<(const Vec3& v) const noexcept { return x < v.x && y < v.y && z < v.z; }
	constexpr bool operator<=(const Vec3& v) const noexcept { return x <= v.x && y <= v.y && z <= v.z; }

	constexpr bool operator==(const Vec3& v) const noexcept { return x == v.x && y == v.y && z == v.z; }

	constexpr Vec3& operator*=(const T& val) noexcept { x *= val; y *= val; z *= val; return *this; }
};

template <typename T = int16_t>
class Vec4 {
public:
	T w, x, y, z;

	constexpr Vec4() noexcept = default;
	constexpr Vec4(T w, T x, T y, T z) noexcept : w(w), x(x), y(y), z(z) {}

	constexpr Vec4 operator+(const Vec4& v) const noexcept { return { static_cast<T>(w + v.w), static_cast<T>(x + v.x), static_cast<T>(y + v.y), static_cast<T>(z + v.z) }; }
	constexpr Vec4 operator-(const Vec4& v) const noexcept { return { static_cast<T>(w - v.w), static_cast<T>(x - v.x), static_cast<T>(y - v.y), static_cast<T>(z - v.z) }; }
	constexpr Vec4 operator*(const Vec4& v) const noexcept { return { static_cast<T>(w * v.w), static_cast<T>(x * v.x), static_cast<T>(y * v.y), static_cast<T>(z * v.z) }; }
	constexpr Vec4 operator/(const Vec4& v) const noexcept { return { static_cast<T>(w / v.w), static_cast<T>(x / v.x), static_cast<T>(y / v.y), static_cast<T>(z / v.z) }; }

	constexpr bool operator>(const Vec4& v) const noexcept { return w > v.w && x > v.x && y > v.y && z > v.z; }
	constexpr bool operator>=(const Vec4& v) const noexcept { return w >= v.w && x >= v.x && y >= v.y && z >= v.z; }
	constexpr bool operator<(const Vec4& v) const noexcept { return w < v.w && x < v.x && y < v.y && z < v.z; }
	constexpr bool operator<=(const Vec4& v) const noexcept { return w <= v.w && x <= v.x && y <= v.y && z <= v.z; }

	constexpr bool operator==(const Vec4& v) const noexcept { return w == v.w && x == v.x && y == v.y && z == v.z; }

	constexpr Vec4& operator*=(const T& val) noexcept { w *= val; x *= val; y *= val; z *= val; return *this; }
};

template <typename T = int16_t>
class Rect {
public:
	T x, y, w, h;

	constexpr Rect( ) noexcept = default;
	constexpr Rect( T x, T y, T w, T h ) noexcept : x( x ), y( y ), w( w ), h( h ) {}

	constexpr Rect operator+( const Rect& v ) const noexcept { return { static_cast< T >( x + v.x ), static_cast< T >( y + v.y ), static_cast< T >( w + v.w ), static_cast< T >( h + v.h ) }; }
	constexpr Rect operator-( const Rect& v ) const noexcept { return { static_cast< T >( x - v.x ), static_cast< T >( y - v.y ), static_cast< T >( w - v.w ), static_cast< T >( h - v.h ) }; }
	constexpr Rect operator*( const Rect& v ) const noexcept { return { static_cast< T >( x * v.x ), static_cast< T >( y * v.y ), static_cast< T >( w * v.w ), static_cast< T >( h * v.h ) }; }
	constexpr Rect operator/( const Rect& v ) const noexcept { return { static_cast< T >( x / v.x ), static_cast< T >( y / v.y ), static_cast< T >( w / v.w ), static_cast< T >( h / v.h ) }; }

	constexpr bool operator>( const Rect& v ) const noexcept { return x > v.x && y > v.y && w > v.w && h > v.h; }
	constexpr bool operator>=( const Rect& v ) const noexcept { return x >= v.x && y >= v.y && w >= v.w && h >= v.h; }
	constexpr bool operator<( const Rect& v ) const noexcept { return x < v.x && y < v.y && w < v.w && h < v.h; }
	constexpr bool operator<=( const Rect& v ) const noexcept { return x <= v.x && y <= v.y && w <= v.w && h <= v.h; }

	constexpr bool operator==( const Rect& v ) const noexcept { return x == v.x && y == v.y && w == v.w && h == v.h; }

	constexpr Rect& operator*=( const T& val ) noexcept { x *= val; y *= val; w *= val; h *= val; return *this; }
};

class Vertex {
public:
	float x, y, z, rhw;
	uint32_t Color;
	float u, v;

	constexpr Vertex() noexcept = default;
	constexpr Vertex(float x, float y, float z, float rhw, uint32_t Color, float u = 0.0f, float v = 0.0f) noexcept
		: x(x), y(y), z(z), rhw(rhw), Color(Color), u(u), v(v) {}
};

#define COLOR(r,g,b,a) ((DWORD)((((a)&0xff)<<24)|(((r)&0xff)<<16)|(((g)&0xff)<<8)|((b)&0xff)))

class Color {
public:
	DWORD hex;

	constexpr Color() noexcept = default;
	constexpr Color(uint8_t r, uint8_t g, uint8_t b, uint8_t a) noexcept
		: hex(COLOR(r, g, b, a)) {}
};

struct Glyph {
	IDirect3DTexture9* Resource = nullptr;
	Vec2<int32_t> Size, Bearing;
	int32_t Advance;
};

class Font {
public:
	std::string Path;
	int16_t Padding, Size;

	std::vector<Glyph> CharSet{ 256 };

	void Release() {
		for (auto& glyph : CharSet) {
			if (glyph.Resource)
				glyph.Resource->Release();

			glyph.Resource = nullptr;
		}
	}
};

class CommandResources {
public:
	std::vector<IDirect3DTexture9*> Textures;
	std::vector<RECT> Clips;
};

class DrawCommand {
public:
	int8_t Primitive;
	CommandResources Resources;
	std::vector<Vertex> Vertices;
	std::vector<std::int32_t> Indices;
	int VerticesCount, IndicesCount;

	constexpr DrawCommand() noexcept = default;
	constexpr DrawCommand( int8_t Primitive, std::vector<Vertex> Vertices, std::vector<std::int32_t> Indices, CommandResources Resources, int VerticesCount, int IndicesCount ) noexcept
		: Primitive( Primitive ), Vertices( Vertices ), Indices( Indices ), Resources( Resources ), VerticesCount( VerticesCount ), IndicesCount( IndicesCount ) {}
};

class CompiledDrawCommand {
public:
	std::vector<Vertex> Vertices;
	std::vector<std::int32_t> Indices;
	int VerticesCount, IndicesCount;

	constexpr CompiledDrawCommand() noexcept = default;
	constexpr CompiledDrawCommand(std::vector<Vertex> Vertices, std::vector<std::int32_t> Indices, int VerticesCount, int IndicesCount) noexcept
		: Vertices(Vertices), Indices(Indices), VerticesCount(VerticesCount), IndicesCount(IndicesCount) {}

	void reset() {
		Vertices.clear();
		Indices.clear();
		VerticesCount = 0;
		IndicesCount = 0;
	}
};