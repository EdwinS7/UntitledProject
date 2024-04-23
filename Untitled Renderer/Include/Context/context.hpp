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
	int m_Fps{ 0 }, m_FrameCount{ 0 };
	float m_RealTime{ 0 }, m_DeltaTime{ 0 }, m_WhenToUpdate{ 0 };

	std::chrono::steady_clock::time_point m_TimePoint, m_FrameTime;
};

inline const auto gContext = std::make_unique<cContext>( );

#include "Context.inl"
#endif