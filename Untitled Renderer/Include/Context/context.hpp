#pragma once
#ifndef CONTEXT_H
#define CONTEXT_H

#include "../../Common.hpp"

class cContext {
public:
	void Update( );

	inline int GetFrameRate( );
	inline float GetRealTime( );
	inline float GetDeltaTime( );
	inline std::chrono::steady_clock::time_point getTimePoint( );

private:
	int m_Fps, m_FrameCount;
	float m_RealTime, m_DeltaTime, m_WhenToUpdate;

	std::chrono::steady_clock::time_point m_TimePoint, m_FrameTime;
};

inline const auto gContext = std::make_unique<cContext>( );

#include "Context.inl"
#endif