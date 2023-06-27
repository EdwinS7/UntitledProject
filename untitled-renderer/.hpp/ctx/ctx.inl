#include "ctx.hpp"

ALWAYS_INLINE int c_ctx::get_framerate( ) {
	return m_fps;
}

ALWAYS_INLINE float c_ctx::get_real_time( ) {
	return m_real_time;
}

ALWAYS_INLINE timepoint_t c_ctx::get_time_point( ) {
	return m_timepoint;
}