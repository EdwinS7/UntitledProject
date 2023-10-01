#include "buffer.hpp"

class view_angles_t {
public:
	float pitch, yaw;

	constexpr view_angles_t( ) noexcept = default;
	constexpr view_angles_t( const float pitch, const float yaw ) noexcept : pitch( pitch ), yaw( yaw ) {}

	constexpr view_angles_t operator+( const view_angles_t& viewangles ) const noexcept { return { static_cast< float >( pitch + viewangles.pitch ), static_cast< float >( yaw + viewangles.yaw ) }; }
	constexpr view_angles_t operator-( const view_angles_t& viewangles ) const noexcept { return { static_cast< float >( pitch - viewangles.pitch ), static_cast< float >( yaw - viewangles.yaw ) }; }
	constexpr view_angles_t operator*( const view_angles_t& viewangles ) const noexcept { return { static_cast< float >( pitch * viewangles.pitch ), static_cast< float >( yaw * viewangles.yaw ) }; }
	constexpr view_angles_t operator/( const view_angles_t& viewangles ) const noexcept { return { static_cast< float >( pitch / viewangles.pitch ), static_cast< float >( yaw / viewangles.yaw ) }; }
	constexpr bool operator>( const view_angles_t& viewangles ) const noexcept { return pitch > viewangles.pitch && yaw > viewangles.yaw; }
	constexpr bool operator>=( const view_angles_t& viewangles ) const noexcept { return pitch >= viewangles.pitch && yaw >= viewangles.yaw; }
	constexpr bool operator<( const view_angles_t& viewangles ) const noexcept { return pitch < viewangles.pitch && yaw < viewangles.yaw; }
	constexpr bool operator<=( const view_angles_t& viewangles ) const noexcept { return pitch <= viewangles.pitch && yaw <= viewangles.yaw; }
	constexpr view_angles_t operator*=( const float& viewangles ) noexcept { pitch *= viewangles.pitch; yaw *= viewangles.yaw; return *this; }

#ifdef USE_CUSTOM_VIEW_ANGLES_OPERATORS
	USE_CUSTOM_VIEW_ANGLES_OPERATORS
#endif
};


class Vector2 {
public:
	int x, y;

	constexpr Vector2( ) noexcept = default;
	constexpr Vector2( const int x, const int y ) noexcept : x( x ), y( y ) {}

	constexpr Vector2 operator+( const Vector2& vector ) const noexcept { return { static_cast< int >( x + vector.x ), static_cast< int >( y + vector.y ) }; }
	constexpr Vector2 operator-( const Vector2& vector ) const noexcept { return { static_cast< int >( x - vector.x ), static_cast< int >( y - vector.y ) }; }
	constexpr Vector2 operator*( const Vector2& vector ) const noexcept { return { static_cast< int >( x * vector.x ), static_cast< int >( y * vector.y ) }; }
	constexpr Vector2 operator/( const Vector2& vector ) const noexcept { return { static_cast< int >( x / vector.x ), static_cast< int >( y / vector.y ) }; }
	constexpr bool operator>( const Vector2& vector ) const noexcept { return x > vector.x && y > vector.y; }
	constexpr bool operator>=( const Vector2& vector ) const noexcept { return x >= vector.x && y >= vector.y; }
	constexpr bool operator<( const Vector2& vector ) const noexcept { return x < vector.x && y < vector.y; }
	constexpr bool operator<=( const Vector2& vector ) const noexcept { return x <= vector.x && y <= vector.y; }
	constexpr Vector2& operator*=( const int& val ) noexcept { return { x *= val, y *= val }; }

#ifdef USE_CUSTOM_VECTOR2_OPERATORS
	USE_CUSTOM_VECTOR2_OPERATORS
#endif
};

class Vector3 {
public:
	float x, y, z;

	constexpr Vector3( ) noexcept = default;
	constexpr Vector3( const float x, const float y, const float z ) noexcept : x( x ), y( y ), z( z ) {}

	constexpr Vector3 operator+( const Vector3& vector ) const noexcept { return { static_cast< float >( x + vector.x ), static_cast< float >( y + vector.y ), static_cast< float >( z + vector.z ) }; }
	constexpr Vector3 operator-( const Vector3& vector ) const noexcept { return { static_cast< float >( x - vector.x ), static_cast< float >( y - vector.y ), static_cast< float >( z - vector.z ) }; }
	constexpr Vector3 operator*( const Vector3& vector ) const noexcept { return { static_cast< float >( x * vector.x ), static_cast< float >( y * vector.y ), static_cast< float >( z * vector.z ) }; }
	constexpr Vector3 operator/( const Vector3& vector ) const noexcept { return { static_cast< float >( x / vector.x ), static_cast< float >( y / vector.y ), static_cast< float >( z / vector.z ) }; }
	constexpr bool operator>( const Vector3& vector ) const noexcept { return x > vector.x && y > vector.y && z > vector.z; }
	constexpr bool operator>=( const Vector3& vector ) const noexcept { return x >= vector.x && y >= vector.y && z >= vector.z; }
	constexpr bool operator<( const Vector3& vector ) const noexcept { return x < vector.x && y < vector.y && z < vector.z; }
	constexpr bool operator<=( const Vector3& vector ) const noexcept { return x <= vector.x && y <= vector.y && z <= vector.z; }
	constexpr Vector3& operator*=( const float& val ) noexcept { return { static_cast< float >( x * val ), static_cast< float >( y * val ), static_cast< float >( z * val ) }; }

#ifdef USE_CUSTOM_VECTOR3_OPERATORS
	USE_CUSTOM_VECTOR3_OPERATORS
#endif
};

class Vector4 {
public:
	float w, x, y, z;

	constexpr Vector4( ) noexcept = default;
	constexpr Vector4( const float w, const float x, const float y, const float z ) noexcept : w( w ), x( x ), y( y ), z( z ) {}

	constexpr Vector4 operator+( const Vector4& vector ) const noexcept { return { static_cast< float >( w + vector.w ), static_cast< float >( x + vector.x ), static_cast< float >( y + vector.y ), static_cast< float >( z + vector.z ) }; }
	constexpr Vector4 operator-( const Vector4& vector ) const noexcept { return { static_cast< float >( w - vector.w ), static_cast< float >( x - vector.x ), static_cast< float >( y - vector.y ), static_cast< float >( z - vector.z ) }; }
	constexpr Vector4 operator*( const Vector4& vector ) const noexcept { return { static_cast< float >( w * vector.w ), static_cast< float >( x * vector.x ), static_cast< float >( y * vector.y ), static_cast< float >( z * vector.z ) }; }
	constexpr Vector4 operator/( const Vector4& vector ) const noexcept { return { static_cast< float >( w / vector.w ), static_cast< float >( x / vector.x ), static_cast< float >( y / vector.y ), static_cast< float >( z / vector.z ) }; }
	constexpr bool operator>( const Vector4& vector ) const noexcept { return w > vector.w && x > vector.x && y > vector.y && z > vector.z; }
	constexpr bool operator>=( const Vector4& vector ) const noexcept { return w >= vector.w && x >= vector.x && y >= vector.y && z >= vector.z; }
	constexpr bool operator<( const Vector4& vector ) const noexcept { return w < vector.w && x < vector.x && y < vector.y && z < vector.z; }
	constexpr bool operator<=( const Vector4& vector ) const noexcept { return w <= vector.w && x <= vector.x && y <= vector.y && z <= vector.z; }
	constexpr Vector4 operator*=( const float& val ) noexcept { return { static_cast< float >( w * val ), static_cast< float >( x * val ), static_cast< float >( y * val ), static_cast< float >( z * val ) }; }

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