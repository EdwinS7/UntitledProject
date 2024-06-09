#pragma once
#ifndef LOGGER_H
#define LOGGER_H

#include "../../../Common.hpp"

enum class LogLevel {
	Normal = 0,
	Information,
	Warning,
	Error,
	Success,
	Unknown,
	END
};

class cLogger {
public:
	void Log( LogLevel level, const std::string& message );

	std::vector<std::pair<DWORD, std::string>> GetLogs( LogLevel level ) const;
	void ClearLogs( LogLevel level );

private:
	std::unordered_map<LogLevel, std::vector<std::pair<DWORD, std::string>>> m_Logs;
};

inline const auto gLogger = std::make_unique<cLogger>( );

#include "Logger.inl"
#endif