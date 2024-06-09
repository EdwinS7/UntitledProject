#pragma once
#ifndef WRAPPER_H
#define WRAPPER_H

#include "../../../Common.hpp"

struct cLuaCallback {
	sol::protected_function Function;
};

class cWrapper {
public:
	void Init( );

	int LoadScript( const std::string& source );
	int LoadScriptFromFile( const std::string& folder_path, const std::string& file_name );

	inline void RegisterCallback( const std::string& event_name, sol::protected_function callback );
	inline void UnregisterCallbacks( );

	inline std::vector<cLuaCallback> GetCallbacks( const std::string& callback_name );
	inline void RunCallback( const std::string& callback_name );

private:
	sol::state Lua;

	std::unordered_map<std::string, std::vector<cLuaCallback>> m_Callbacks;
};

inline const auto gWrapper = std::make_unique<cWrapper>( );

#include "Wrapper.inl"
#endif