#pragma once
#ifndef LUA_API_H
#define LUA_API_H

#include <Common.hpp>

#include "../Utils/Logger/Logger.hpp"
#include "../Utils/Animation/Animation.hpp"
#include "../Utils/Networking/Networking.hpp"

#include "../Core/Window/Window.hpp"
#include "../Core/Graphics/DirectX9.hpp"
#include "../Core/Camera/Camera.hpp"
#include "../Core/Buffer/Buffer.hpp"
#include "../Core/Input/Input.hpp"
#include "../Core/Audio/Audio.hpp"

#include "Core/Context/Context.hpp"

#include "Enviornment/LuaEnviornment.hpp"

class cLuaAPI {
public:
	void Init( );
	std::unique_ptr<cLuaEnviornment> NewEnviornment( );

	void RunConnection( const std::string& callback_name );
	std::vector<sol::protected_function> GetConnections( const std::string& callback_name );

	void AddConnection( const std::string& connection_name, sol::protected_function function );

public:
	std::vector<std::string> ConnectionNames = {
		"OnInterfacePaint", "__OnInterfacePaint",
		"OnWorldPaint", "__OnWorldPaint",
		"OnInputUpdate", "__OnInputUpdate",
		"OnObjectCreation" "__OnInterfacePaint"
	};

private:

	std::unordered_map<std::string, std::vector<sol::protected_function>> m_Connections;
};

inline const auto gLuaAPI = std::make_unique<cLuaAPI>( );

#endif // LUA_API_H