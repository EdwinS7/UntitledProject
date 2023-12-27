#pragma once

// includes
#include "../../common.hpp"

class cContext {
public:
	void Update( );

	inline int GetFrameRate( );
	inline float GetRealTime( );
	inline float GetDeltaTime( );
	inline std::chrono::steady_clock::time_point getTimePoint( );

private:
	int m_Fps;
	int m_FrameCount;
	float m_RealTime;
	float m_DeltaTime;
	float m_WhenToUpdate;

	std::chrono::steady_clock::time_point m_TimePoint;
	std::chrono::steady_clock::time_point m_FrameTime;
};

inline const auto gContext = std::make_unique<cContext>( );

#include "context.inl"