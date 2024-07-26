#pragma once

#define VERTEX ( D3DFVF_XYZRHW | D3DFVF_DIFFUSE | D3DFVF_TEX1 )

#define TRIANGLE D3DPT_TRIANGLESTRIP
#define LINE D3DPT_LINESTRIP

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

	constexpr Vec3 Min( const Vec3& v ) const noexcept {
		return { x < v.x ? x : v.x, y < v.y ? y : v.y, z < v.z ? z : v.z };
	}

	constexpr Vec3 Max( const Vec3& v ) const noexcept {
		return { x > v.x ? x : v.x, y > v.y ? y : v.y, z > v.z ? z : v.z };
	}

	constexpr void Lerp( const Vec3& v, const float& t ) noexcept {
		x += static_cast< T >( ( v.x - x ) * t );
		y += static_cast< T >( ( v.y - y ) * t );
		z += static_cast< T >( ( v.z - z ) * t );
	}

	constexpr Vec3 DistanceTo( const Vec3& v ) const noexcept {
		return { static_cast< int16_t >( v.x - x ), static_cast< int16_t >( v.y - y ), static_cast< int16_t >( v.z - z ) };
	}

	template<typename U = T>
	constexpr Vec3<U> As( ) const noexcept {
		return { static_cast< U >( x ), static_cast< U >( y ), static_cast< U >( z ) };
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
	constexpr bool operator<( const Vec4& v ) const noexcept { return x < v.x && y < v.y && z < v.z && w < v.w; }

	constexpr bool operator>=( const Vec4& v ) const noexcept { return x >= v.x && y >= v.y && z >= v.z && w >= v.w; }
	constexpr bool operator<=( const Vec4& v ) const noexcept { return x <= v.x && y <= v.y && z <= v.z && w <= v.w; }

	constexpr bool operator==( const Vec4& v ) const noexcept { return x == v.x && y == v.y && z == v.z && w == v.w; }

	constexpr Vec4 Min( const Vec4& v ) const noexcept {
		return { x < v.x ? x : v.x, y < v.y ? y : v.y, z < v.z ? z : v.z, w < v.w ? w : v.w };
	}

	constexpr Vec4 Max( const Vec4& v ) const noexcept {
		return { x > v.x ? x : v.x, y > v.y ? y : v.y, z > v.z ? z : v.z, w > v.w ? w : v.w };
	}

	constexpr void Lerp( const Vec4& v, const float& t ) noexcept {
		x += static_cast< T >( ( v.x - x ) * t );
		y += static_cast< T >( ( v.y - y ) * t );
		z += static_cast< T >( ( v.z - z ) * t );
		w += static_cast< T >( ( v.w - w ) * t );
	}

	constexpr Vec4 DistanceTo( const Vec4& v ) const noexcept {
		return { static_cast< int16_t >( v.x - x ), static_cast< int16_t >( v.y - y ), static_cast< int16_t >( v.z - z ), static_cast< int16_t >( v.w - w ) };
	}

	template<typename U = T>
	constexpr Vec4<U> As( ) const noexcept {
		return { static_cast< U >( x ), static_cast< U >( y ), static_cast< U >( z ), static_cast< U >( w ) };
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

	constexpr Rect& operator*=( const Rect& v ) noexcept { x *= v.x; y *= v.y; w *= v.w; h *= v.h; return *this; }
	constexpr Rect& operator+=( const Rect& v ) noexcept { x += v.x; y += v.y; w += v.w; h += v.h; return *this; }
	constexpr Rect& operator-=( const Rect& v ) noexcept { x -= v.x; y -= v.y; w -= v.w; h -= v.h; return *this; }
	constexpr Rect& operator/=( const Rect& v ) noexcept { x /= v.x; y /= v.y; w /= v.w; h /= v.h; return *this; }

	constexpr bool operator>( const Rect& v ) const noexcept { return x > v.x && y > v.y && w > v.w && h > v.h; }
	constexpr bool operator<( const Rect& v ) const noexcept { return x < v.x && y < v.y && w < v.w && h < v.h; }

	constexpr bool operator>=( const Rect& v ) const noexcept { return x >= v.x && y >= v.y && w >= v.w && h >= v.h; }
	constexpr bool operator<=( const Rect& v ) const noexcept { return x <= v.x && y <= v.y && w <= v.w && h <= v.h; }

	constexpr bool operator==( const Rect& v ) const noexcept { return x == v.x && y == v.y && w == v.w && h == v.h; }

	constexpr Rect Min( const Rect& r ) const noexcept {
		return { x < r.x ? x : r.x, y < r.y ? y : r.y, w < r.w ? w : r.w, h < r.h ? h : r.h };
	}

	constexpr Rect Max( const Rect& r ) const noexcept {
		return { x > r.x ? x : r.x, y > r.y ? y : r.y, w > r.w ? w : r.w, h > r.h ? h : r.h };
	}

	constexpr void Lerp( const Rect& r, const float& t ) noexcept {
		x += static_cast< T >( ( r.x - x ) * t );
		y += static_cast< T >( ( r.y - y ) * t );
		w += static_cast< T >( ( r.w - w ) * t );
		h += static_cast< T >( ( r.h - h ) * t );
	}

	constexpr Rect DistanceTo( const Rect& r ) const noexcept {
		return { static_cast< int16_t >( r.x - x ), static_cast< int16_t >( r.y - y ), static_cast< int16_t >( r.w - w ), static_cast< int16_t >( r.h - h ) };
	}

	template<typename U = T>
	constexpr Rect<U> As( ) const noexcept {
		return { static_cast< U >( x ), static_cast< U >( y ), static_cast< U >( w ), static_cast< U >( h ) };
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

	uint8_t GetR( ) const noexcept {
		return ( Hex >> 24 ) & 0xFF;
	}

	uint8_t GetG( ) const noexcept {
		return ( Hex >> 16 ) & 0xFF;
	}

	uint8_t GetB( ) const noexcept {
		return ( Hex >> 8 ) & 0xFF;
	}

	uint8_t GetA( ) const noexcept {
		return Hex & 0xFF;
	}

	// Used in Lua API Function Wrapper
	static Color fromRGBA( uint8_t r, uint8_t g, uint8_t b, uint8_t a ) {
		return Color( r, g, b, a );
	}

	static Color fromHSVA( float h, float s, float v, uint8_t a ) {
		float r = 0, g = 0, b = 0;

		int i = static_cast< int >( h * 6 );
		float f = h * 6 - i;
		float p = v * ( 1 - s );
		float q = v * ( 1 - f * s );
		float t = v * ( 1 - ( 1 - f ) * s );

		switch ( i % 6 ) {
		case 0: r = v, g = t, b = p; break;
		case 1: r = q, g = v, b = p; break;
		case 2: r = p, g = v, b = t; break;
		case 3: r = p, g = q, b = v; break;
		case 4: r = t, g = p, b = v; break;
		case 5: r = v, g = p, b = q; break;
		}

		return Color( static_cast< uint8_t >( r * 255 ), static_cast< uint8_t >( g * 255 ), static_cast< uint8_t >( b * 255 ), a );
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

		Valid = false;
	}
};

class Image {
public:
	IDirect3DTexture9* Texture;
	Vec2<int16_t> Size;
};