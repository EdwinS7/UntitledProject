#pragma once
#ifndef WRAPPER_H
#define WRAPPER_H

#include "../../../Common.hpp"

#define WRAPPER_DEFAULT_SCRIPT R"(function OnInputUpdate()
    if Input.IsKeyPressed(Keys.F11) then
        Window.SetFullscreen(not Window.GetFullscreen())
    end
end

function OnPaint()
    local TextSize = Renderer.GetTextSize(Renderer.GetDefaultFont(), tostring(Client.GetFramerate()))

    Renderer.FilledRectangle(Vector2.new(5, 5), TextSize, Color.new(0, 0, 0, 255), 0, CornerFlags.None)
    Renderer.Text(Renderer.GetDefaultFont(), tostring(Client.GetFramerate()), Vector2.new(5, 5), Color.new(255, 255, 255, 255))
end

AddCallback("OnInputUpdate", OnInputUpdate)
AddCallback("OnInterfacePaint", OnPaint)

LoadScript("Testing.lua"))"

class cLuaWrapper {
public:
	void Init( );

	int LoadScript( const std::string& source );
	int LoadScriptFromFile( const std::string& folder_path, const std::string& file_name );

	void RunCallback( const std::string& callback_name );

	inline void RegisterCallback( const std::string& event_name, sol::protected_function callback );
	inline void UnregisterCallbacks( );

	inline std::vector<sol::protected_function> GetCallbacks( const std::string& callback_name );

private:
	sol::state Lua;

	std::unordered_map<std::string, std::vector<sol::protected_function>> m_Callbacks;
};

inline const auto gLuaWrapper = std::make_unique<cLuaWrapper>( );

#include "LuaWrapper.inl"
#endif