#pragma once
#ifndef WRAPPER_H
#define WRAPPER_H

#include "../../../Common.hpp"

#define WRAPPER_DEFAULT_SCRIPT R"(function OnPaint()
    local TextSize = Renderer.GetTextSize(Renderer.GetDefaultFont(), tostring(Client.GetFramerate()))
    Renderer.FilledRectangle(Vector2.new(5, 5), TextSize, Color.new(0, 0, 0, 255), 0)
    Renderer.Text(Renderer.GetDefaultFont(), tostring(Client.GetFramerate()), Vector2.new(5, 5), Color.new(255, 255, 255, 255))
end

AddCallback("OnInterfacePaint", OnPaint)
LoadScript("Demo.lua"))"

class cLuaWrapper {
public:
	void Init( );

	int LoadScript( const std::string& source );
	int LoadScriptFromFile( const std::string& folder_path, const std::string& file_name );

	inline void RegisterCallback( const std::string& event_name, sol::protected_function callback );
	inline void UnregisterCallbacks( );

	inline std::vector<sol::protected_function> GetCallbacks( const std::string& callback_name );
	inline void RunCallback( const std::string& callback_name );

private:
	sol::state Lua;

	std::unordered_map<std::string, std::vector<sol::protected_function>> m_Callbacks;
};

inline const auto gLuaWrapper = std::make_unique<cLuaWrapper>( );

#include "LuaWrapper.inl"
#endif