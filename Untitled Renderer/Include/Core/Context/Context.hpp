#pragma once
#ifndef CONTEXT_H
#define CONTEXT_H

#include <Common.hpp>
#include "../../Scripting/LuaWrapper.hpp"

class cContext {
public:
	void Update( );

	inline int GetFrameRate( ) const;

	inline float GetRealTime( ) const;
	inline float GetDeltaTime( ) const;

	inline std::string GetUsername( ) const;

	inline std::chrono::steady_clock::time_point getTimePoint( ) const;

private:
	int m_Fps{ 0 }, m_FrameCount{ 0 };
	float m_RealTime{ 0 }, m_DeltaTime{ 0 }, m_WhenToUpdate{ 0 };

	std::chrono::steady_clock::time_point m_TimePoint, m_FrameTime;

	std::string m_Username{ "Unknown" };
};

inline const auto gContext = std::make_unique<cContext>( );

#include "Context.inl"
#endif