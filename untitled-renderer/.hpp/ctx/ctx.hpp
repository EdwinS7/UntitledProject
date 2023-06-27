#pragma once

// includes
#include "../../inc.hpp"

class c_ctx {
public:
	void update( );

	ALWAYS_INLINE int get_framerate( );
	ALWAYS_INLINE float get_real_time( );
	ALWAYS_INLINE timepoint_t get_time_point( );

private:
	int m_fps;
	int m_frame_count;
	float m_real_time;
	float when_to_update;

	timepoint_t m_timepoint;

};

inline const auto g_ctx = std::make_unique<c_ctx>( );

#include "ctx.inl"