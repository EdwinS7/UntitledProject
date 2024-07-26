#pragma once
#ifndef LUA_ENVIRONMENT_H
#define LUA_ENVIRONMENT_H

class cLuaEnvironment {
public:
	void Init( );

	int LoadScript( const std::string& source );
	int LoadScriptFromFile( const std::string& folder_path, const std::string& file_name );

public:
	sol::state& GetLua( ) { return Lua; }

private:
	sol::state Lua;
};

#endif // LUA_ENVIRONMENT_H