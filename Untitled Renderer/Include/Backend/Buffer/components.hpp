#pragma once

// Enum for corner flags
enum CornerFlags {
	CornerNone = 0 << 0,
	CornerTopLeft = 1 << 0,
	CornerTopRight = 1 << 1,
	CornerBottomLeft = 1 << 2,
	CornerBottomRight = 1 << 3,

	CornerTop = CornerTopLeft | CornerTopRight,
	CornerRight = CornerTopRight | CornerBottomRight,
	CornerBottom = CornerBottomLeft | CornerBottomRight,
	CornerLeft = CornerTopLeft | CornerBottomLeft,

	CornerAll = CornerTopLeft | CornerTopRight | CornerBottomLeft | CornerBottomRight
};

// Template class for 2D vector
template <typename T = int16_t>
class Vec2 {
public:
	T x{}, y{};

	constexpr Vec2( ) noexcept = default;
	constexpr Vec2( T x, T y ) noexcept : x( x ), y( y ) {}

	constexpr Vec2 operator+( const Vec2& v ) const noexcept { return { static_cast< T >( x + v.x ), static_cast< T >( y + v.y ) }; }
	constexpr Vec2 operator-( const Vec2& v ) const noexcept { return { static_cast< T >( x - v.x ), static_cast< T >( y - v.y ) }; }
	constexpr Vec2 operator*( const Vec2& v ) const noexcept { return { static_cast< T >( x * v.x ), static_cast< T >( y * v.y ) }; }
	constexpr Vec2 operator/( const Vec2& v ) const noexcept { return { static_cast< T >( x / v.x ), static_cast< T >( y / v.y ) }; }

	constexpr Vec2& operator*=( const Vec2& v ) noexcept { x *= v.x; y *= v.y; return *this; }
	constexpr Vec2& operator+=( const Vec2& v ) noexcept { x += v.x; y += v.y; return *this; }
	constexpr Vec2& operator-=( const Vec2& v ) noexcept { x -= v.x; y -= v.y; return *this; }
	constexpr Vec2& operator/=( const Vec2& v ) noexcept { x /= v.x; y /= v.y; return *this; }

	constexpr bool operator>( const Vec2& v ) const noexcept { return x > v.x && y > v.y; }
	constexpr bool operator<( const Vec2& v ) const noexcept { return x < v.x && y < v.y; }

	constexpr bool operator>=( const Vec2& v ) const noexcept { return x >= v.x && y >= v.y; }
	constexpr bool operator<=( const Vec2& v ) const noexcept { return x <= v.x && y <= v.y; }

	constexpr bool operator==( const Vec2& v ) const noexcept { return x == v.x && y == v.y; }

	constexpr Vec2 Min( const Vec2& v ) const noexcept {
		return { x < v.x ? x : v.x, y < v.y ? y : v.y };
	}

	constexpr Vec2 Max( const Vec2& v ) const noexcept {
		return { x > v.x ? x : v.x, y > v.y ? y : v.y };
	}

	constexpr void Lerp( const Vec2& v, const float& t ) noexcept {
		x += static_cast< T >( ( v.x - x ) * t );
		y += static_cast< T >( ( v.y - y ) * t );
	}

	constexpr Vec2 DistanceTo( const Vec2& v ) const noexcept {
		return { static_cast< int16_t >( v.x - x ), static_cast< int16_t >( v.y - y ) };
	}

	template <typename U = T>
	constexpr Vec2<U> As( ) const noexcept {
		return { static_cast< U >( x ), static_cast< U >( y ) };
	}
};

// Template class for 3D vector
template <typename T = int32_t>
class Vec3 {
public:
	T x{}, y{}, z{};

	constexpr Vec3( ) noexcept = default;
	constexpr Vec3( T x, T y, T z ) noexcept : x( x ), y( y ), z( z ) {}

	constexpr Vec3 operator+( const Vec3& v ) const noexcept { return { static_cast< T >( x + v.x ), static_cast< T >( y + v.y ), static_cast< T >( z + v.z ) }; }
	constexpr Vec3 operator-( const Vec3& v ) const noexcept { return { static_cast< T >( x - v.x ), static_cast< T >( y - v.y ), static_cast< T >( z - v.z ) }; }
	constexpr Vec3 operator*( const Vec3& v ) const noexcept { return { static_cast< T >( x * v.x ), static_cast< T >( y * v.y ), static_cast< T >( z * v.z ) }; }
	constexpr Vec3 operator/( const Vec3& v ) const noexcept { return { static_cast< T >( x / v.x ), static_cast< T >( y / v.y ), static_cast< T >( z / v.z ) }; }

	constexpr Vec3& operator*=( const Vec3& v ) noexcept { x *= v.x; y *= v.y; z *= v.z; return *this; }
	constexpr Vec3& operator+=( const Vec3& v ) noexcept { x += v.x; y += v.y; z += v.z; return *this; }
	constexpr Vec3& operator-=( const Vec3& v ) noexcept { x -= v.x; y -= v.y; z -= v.z; return *this; }
	constexpr Vec3& operator/=( const Vec3& v ) noexcept { x /= v.x; y /= v.y; z /= v.z; return *this; }

	constexpr bool operator>( const Vec3& v ) const noexcept { return x > v.x && y > v.y && z > v.z; }
	constexpr bool operator<( const Vec3& v ) const noexcept { return x < v.x && y < v.y && z < v.z; }

	constexpr bool operator>=( const Vec3& v ) const noexcept { return x >= v.x && y >= v.y && z >= v.z; }
	constexpr bool operator<=( const Vec3& v ) const noexcept { return x <= v.x && y <= v.y && z <= v.z; }

	constexpr bool operator==( const Vec3& v ) const noexcept { return x == v.x && y == v.y && z == v.z; }

	template <typename U = T>
	constexpr Vec3<U> As( ) const noexcept { return { static_cast< U >( x ), static_cast< U >( y ), static_cast< U >( z ) }; }

	constexpr void Lerp( const Vec3& v, const float& t ) noexcept {
		x += static_cast< T >( ( v.x - x ) * t );
		y += static_cast< T >( ( v.y - y ) * t );
		z += static_cast< T >( ( v.z - z ) * t );
	}
};

// Template class for 4D vector
template <typename T = int32_t>
class Vec4 {
public:
	T x{}, y{}, z{}, w{};

	constexpr Vec4( ) noexcept = default;
	constexpr Vec4( T x, T y, T z, T w ) noexcept : x( x ), y( y ), z( z ), w( w ) {}

	constexpr Vec4 operator+( const Vec4& v ) const noexcept { return { static_cast< T >( x + v.x ), static_cast< T >( y + v.y ), static_cast< T >( z + v.z ), static_cast< T >( w + v.w ) }; }
	constexpr Vec4 operator-( const Vec4& v ) const noexcept { return { static_cast< T >( x - v.x ), static_cast< T >( y - v.y ), static_cast< T >( z - v.z ), static_cast< T >( w - v.w ) }; }
	constexpr Vec4 operator*( const Vec4& v ) const noexcept { return { static_cast< T >( x * v.x ), static_cast< T >( y * v.y ), static_cast< T >( z * v.z ), static_cast< T >( w * v.w ) }; }
	constexpr Vec4 operator/( const Vec4& v ) const noexcept { return { static_cast< T >( x / v.x ), static_cast< T >( y / v.y ), static_cast< T >( z / v.z ), static_cast< T >( w / v.w ) }; }

	constexpr Vec4& operator*=( const Vec4& v ) noexcept { x *= v.x; y *= v.y; z *= v.z; w *= v.w; return *this; }
	constexpr Vec4& operator+=( const Vec4& v ) noexcept { x += v.x; y += v.y; z += v.z; w += v.w; return *this; }
	constexpr Vec4& operator-=( const Vec4& v ) noexcept { x -= v.x; y -= v.y; z -= v.z; w -= v.w; return *this; }
	constexpr Vec4& operator/=( const Vec4& v ) noexcept { x /= v.x; y /= v.y; z /= v.z; w /= v.w; return *this; }

	constexpr bool operator>( const Vec4& v ) const noexcept { return x > v.x && y > v.y && z > v.z && w > v.w; }
	constexpr bool operator<( const Vec4& v ) const noexcept {return x < v.x && y < v.y && z < v.z && w < v.w; }

	constexpr bool operator>=( const Vec4& v ) const noexcept { return x >= v.x && y >= v.y && z >= v.z && w >= v.w; }
	constexpr bool operator<=( const Vec4& v ) const noexcept { return x <= v.x && y <= v.y && z <= v.z && w <= v.w; }

	constexpr bool operator==( const Vec4& v ) const noexcept { return x == v.x && y == v.y && z == v.z && w == v.w; }

	template <typename U = T>
	constexpr Vec4<U> As( ) const noexcept { return { static_cast< U >( x ), static_cast< U >( y ), static_cast< U >( z ), static_cast< U >( w ) }; }

	constexpr void Lerp( const Vec4& v, const float& t ) noexcept {
		x += static_cast< T >( ( v.x - x ) * t );
		y += static_cast< T >( ( v.y - y ) * t );
		z += static_cast< T >( ( v.z - z ) * t );
		w += static_cast< T >( ( v.w - w ) * t );
	}
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

	template <typename U = T>
	constexpr Rect<U> As( ) const noexcept { return { static_cast< U >( x ), static_cast< U >( y ), static_cast< U >( w ), static_cast< U >( h ) }; }

	constexpr void Lerp( const Rect& v, const float& t ) noexcept {
		x += static_cast< T >( ( v.x - x ) * t );
		y += static_cast< T >( ( v.y - y ) * t );
		w += static_cast< T >( ( v.w - w ) * t );
		h += static_cast< T >( ( v.h - h ) * t );
	}
};

class Vertex {
public:
	float x{ 0 }, y{ 0 }, z{ 0 }, rhw{ 0 };
	DWORD Color{ 0 };
	float u{ 0 }, v{ 0 };

	constexpr Vertex( ) noexcept = default;
	constexpr Vertex( float x, float y, float z, float rhw, DWORD Color, float u = 0.f, float v = 0.f ) noexcept
		: x( x ), y( y ), z( z ), rhw( rhw ), Color( Color ), u( u ), v( v ) {}
};

#define CreateHexFromRGBA(r, g, b, a) ((DWORD)((((a) & 0xff) << 24) | (((r) & 0xff) << 16) | (((g) & 0xff) << 8) | ((b) & 0xff)))

class Color {
public:
	DWORD Hex{ 0x0 };

	constexpr Color( ) noexcept = default;
	constexpr Color( uint8_t r, uint8_t g, uint8_t b, uint8_t a = 255 ) noexcept
		: Hex( CreateHexFromRGBA( r, g, b, a ) ) {}

	uint8_t getR( ) const noexcept {
		return ( Hex >> 24 ) & 0xFF;
	}

	uint8_t getG( ) const noexcept {
		return ( Hex >> 16 ) & 0xFF;
	}

	uint8_t getB( ) const noexcept {
		return ( Hex >> 8 ) & 0xFF;
	}

	uint8_t getA( ) const noexcept {
		return Hex & 0xFF;
	}
};

struct Glyph {
	Vec2<int32_t> Size, Bearing;
	IDirect3DTexture9* Texture{};
	int32_t Advance{};
};

class Font {
public:
	std::string Path;
	Vec2<int16_t> Padding;
	bool Valid{ false };
	int16_t Size{ 0 };

	std::vector<Glyph> CharSet{ 256 };

	void Release( ) {
		for ( auto& Glyph : CharSet ) {
			if ( Glyph.Texture )
				Glyph.Texture->Release( );

			Glyph.Texture = nullptr;
		}
	}
};

class Image {
public:
	IDirect3DTexture9* Texture;
	Vec2<int16_t> Size;
};

class CommandResources {
public:
	std::vector<IDirect3DTexture9*> TextureStack;
	std::vector<RECT> ClipStack;
};

class DrawCommand {
public:
	int8_t Primitive{ 0 };
	CommandResources Resources;
	std::vector<Vertex> Vertices;
	std::vector<std::int32_t> Indices;
	int VerticesCount{ 0 }, IndicesCount{ 0 };

	constexpr DrawCommand( ) noexcept = default;
	constexpr DrawCommand( int8_t Primitive, std::vector<Vertex> Vertices, std::vector<std::int32_t> Indices, CommandResources Resources, int VerticesCount, int IndicesCount ) noexcept
		: Primitive( Primitive ), Vertices( Vertices ), Indices( Indices ), Resources( Resources ), VerticesCount( VerticesCount ), IndicesCount( IndicesCount ) {}
};

class CompiledDrawCommand {
public:
	std::vector<Vertex> Vertices;
	std::vector<std::int32_t> Indices;
	int VerticesCount{ 0 }, IndicesCount{ 0 };

	constexpr CompiledDrawCommand( ) noexcept = default;
	constexpr CompiledDrawCommand( std::vector<Vertex> Vertices, std::vector<std::int32_t> Indices, int VerticesCount, int IndicesCount ) noexcept
		: Vertices( Vertices ), Indices( Indices ), VerticesCount( VerticesCount ), IndicesCount( IndicesCount ) {}

	void reset( ) {
		if ( !Vertices.empty( ) )
			Vertices.clear( );

		if ( !Indices.empty( ) )
			Indices.clear( );

		VerticesCount = 0;
		IndicesCount = 0;
	}
};