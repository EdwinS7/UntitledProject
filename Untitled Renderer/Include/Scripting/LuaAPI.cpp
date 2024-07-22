#include "LuaAPI.hpp"

void cLuaAPI::Init( ) {
    static std::unique_ptr<cLuaEnviornment> GameEnviornment = std::make_unique<cLuaEnviornment>( );

    if ( !GameEnviornment )
        return;

    GameEnviornment->Init( );

    for ( auto& Script : gFileSystem->GetFilesInFolder( FS_DEFAULT_SCRIPTS_FOLDER ) ) {
        if ( Script == "Startup.lua" ) {
            continue;
        }

        GameEnviornment->LoadScriptFromFile( FS_DEFAULT_SCRIPTS_FOLDER, Script );
    }

    // Default game scripts
    GameEnviornment->LoadScriptFromFile( FS_DEFAULT_SCRIPTS_FOLDER, "Startup.lua" );
}

std::unique_ptr<cLuaEnviornment> cLuaAPI::NewEnviornment( ) {
    std::unique_ptr<cLuaEnviornment> Enviornment = std::make_unique<cLuaEnviornment>( );

    if ( !Enviornment )
        return nullptr;

    Enviornment->Init( );

    return Enviornment;
}

void cLuaAPI::RunConnection( const std::string& connection_name ) {
    auto RunConnections = [ this ] ( const std::vector<sol::protected_function>& callbacks ) {
        for ( const auto& callback : callbacks ) {
            auto result = callback( );

            if ( !result.valid( ) ) {
                gLogger->Log( LogLevel::Error, "Lua Error: " + static_cast< std::string >( result.get<sol::error>( ).what( ) ) );
                m_Connections.clear( );
            }
        }
    };

    // Redo this in the future, not sure what to do just yet.
    auto PriorityCallbacks = GetConnections( "__" + connection_name );
    RunConnections( PriorityCallbacks );

    auto Callbacks = GetConnections( connection_name );
    RunConnections( Callbacks );
}

std::vector<sol::protected_function> cLuaAPI::GetConnections( const std::string& connection_name ) {
    return this->m_Connections[ connection_name ];
}

void cLuaAPI::AddConnection( const std::string& connection_name, sol::protected_function function ) {
    this->m_Connections[ connection_name ].push_back( function );
}