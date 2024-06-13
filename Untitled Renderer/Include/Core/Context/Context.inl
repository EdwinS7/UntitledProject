#include "Context.hpp"

inline int cContext::GetFrameRate( ) const {
	return m_Fps;
}

inline float cContext::GetRealTime( ) const {
	return m_RealTime;
}

inline float cContext::GetDeltaTime( ) const {
	return m_DeltaTime;
}

inline std::string cContext::GetUsername( ) const {
	return m_Username;
}

inline std::chrono::steady_clock::time_point cContext::getTimePoint( ) const {
	return m_TimePoint;
}