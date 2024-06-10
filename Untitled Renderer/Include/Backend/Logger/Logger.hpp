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
	END // Used by GetLogs & LuaAPI to request all the logs from Information to Unknown
};

class cLogger {
public:
	void Log( LogLevel log_level, const std::string& message );

	std::vector<std::string> GetLogs( LogLevel log_level ) const;
	void ClearLogs( LogLevel log_level );

private:
	mutable std::unordered_map<LogLevel, std::vector<std::string>> m_Logs;
};

inline const auto gLogger = std::make_unique<cLogger>( );

#include "Logger.inl"
#endif