#include "Context.hpp"

inline int cContext::GetFrameRate( ) {
	return m_Fps;
}

inline float cContext::GetRealTime( ) {
	return m_RealTime;
}

inline float cContext::GetDeltaTime( ) {
	return m_DeltaTime;
}

inline std::chrono::steady_clock::time_point cContext::getTimePoint( ) {
	return m_TimePoint;
}