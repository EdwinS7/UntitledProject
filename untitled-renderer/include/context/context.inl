#include "context.hpp"

ALWAYS_INLINE int c_ctx::get_framerate( ) const {
	return m_fps;
}

ALWAYS_INLINE float c_ctx::get_real_time( ) const {
	return m_real_time;
}

ALWAYS_INLINE timepoint_t c_ctx::get_time_point( ) const {
	return m_timepoint;
}