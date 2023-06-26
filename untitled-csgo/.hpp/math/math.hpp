#pragma once

using vec2_t = ImVec2;

using vec3_t = ImVec3;

using qangle_t = ImVec3;

struct mat3x4_t {
	__forceinline float* operator []( const int i ) { return m_matrix[ i ]; }

	__forceinline const float* operator []( const int i ) const { return m_matrix[ i ]; }

	__forceinline vec3_t at( int i ) const { return vec3_t{ m_matrix[ 0 ][ i ], m_matrix[ 1 ][ i ], m_matrix[ 2 ][ i ] }; }

	__forceinline vec3_t get_origin( ) const { return at( 3 ); }

	__forceinline void set_origin( vec3_t const& p ) {
		m_matrix[ 0 ][ 3 ] = p.x;
		m_matrix[ 1 ][ 3 ] = p.y;
		m_matrix[ 2 ][ 3 ] = p.z;
	}

	float m_matrix[ 3u ][ 4u ]{};
};

struct mat4x4_t {
	__forceinline float* operator []( const int i ) { return m_matrix[ i ]; }

	__forceinline const float* operator []( const int i ) const { return m_matrix[ i ]; }

	float m_matrix[ 4u ][ 4u ]{};
};

namespace math {
	constexpr auto k_pi = 3.14159265358979323846f;

	constexpr auto k_pi2 = k_pi * 2.f;

	constexpr auto k_rad_pi = 180.f / k_pi;

	constexpr auto k_deg_pi = k_pi / 180.f;

	__forceinline void sin_cos( const float rad, float& sin, float& cos );

	__forceinline float to_deg( const float rad );

	__forceinline float to_rad( const float deg );

	__forceinline float angle_diff( float src, float dst );

	__forceinline void concat_transforms( const mat3x4_t& in0, const mat3x4_t& in1, mat3x4_t& out );

	__forceinline void angle_vectors(
		const qangle_t& angle, vec3_t* const fwd, vec3_t* const right = nullptr, vec3_t* const up = nullptr
	);

	__forceinline vec3_t calc_ang( const vec3_t& src, const vec3_t& dst );

	__forceinline void vector_angles( const vec3_t& in, qangle_t& out );

	__forceinline float calc_fov( const qangle_t& view_angles, const vec3_t& src, const vec3_t& dst );

	__forceinline void vector_transform( const vec3_t& in, const mat3x4_t& matrix, vec3_t& out );

	__forceinline void vector_rotate( const vec3_t& in, const mat3x4_t& matrix, vec3_t& out );

	__forceinline void vector_i_transform( const vec3_t& in, const mat3x4_t& matrix, vec3_t& out );

	__forceinline void vector_i_rotate( const vec3_t& in, const mat3x4_t& matrix, vec3_t& out );

	__forceinline mat3x4_t vector_matrix( const vec3_t& in );

	__forceinline float segment_to_segment( const vec3_t& s1, const vec3_t& s2, const vec3_t& k1, const vec3_t& k2 );

	__forceinline bool line_vs_bb( const vec3_t& src, const vec3_t& dst, const vec3_t& min, const vec3_t& max );

	__forceinline bool to_screen( const vec3_t& world, const vec2_t& screen_size, const mat4x4_t& matrix, vec2_t& out );
}

#include "../../.hpp/math/inl/math.inl"