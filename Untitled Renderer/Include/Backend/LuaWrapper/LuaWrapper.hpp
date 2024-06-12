#pragma once
#ifndef WRAPPER_H
#define WRAPPER_H

#include <Common.hpp>
#include "../Logger/Logger.hpp"
#include "../Window/Window.hpp"
#include "../Graphics/DirectX9.hpp"
#include "../Animation/Animation.hpp"
#include "../Context/Context.hpp"
#include "../Camera/Camera.hpp"
#include "../Buffer/Buffer.hpp"
#include "../Input/Input.hpp"
#include "../Audio/Audio.hpp"

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