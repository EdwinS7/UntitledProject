#pragma once

// includes
#include "../../common.hpp"

class c_ctx {
public:
	void update( );

	ALWAYS_INLINE int get_framerate( ) const;
	ALWAYS_INLINE float get_real_time( ) const;
	ALWAYS_INLINE float get_delta_time( ) const;
	ALWAYS_INLINE timepoint_t get_time_point( ) const;

private:
	int m_fps;
	int m_frame_count;
	float m_real_time;
	float m_delta_time;
	float m_when_to_update;

	timepoint_t m_time_point;
	timepoint_t m_frame_time;
};

inline const auto g_ctx = std::make_unique<c_ctx>( );

#include "context.inl"