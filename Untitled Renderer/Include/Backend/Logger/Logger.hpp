#pragma once
#ifndef LOGGER_H
#define LOGGER_H

#include "../../../Common.hpp"

enum class LogLevel {
	Normal = 0,
	Information,
	Warning,
	Error
};

class cLogger {
public:
	void Print( LogLevel level, const std::string& message );

private:
	
};

inline const auto gLogger = std::make_unique<cLogger>( );

#include "Logger.inl"
#endif