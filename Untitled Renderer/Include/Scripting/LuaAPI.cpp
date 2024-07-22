#include "LuaAPI.hpp"

void cLuaAPI::Init( ) {
    static std::unique_ptr<cLuaEnvironment> GameEnviornment = this->NewEnvironment( );

    // Default Guis
    GameEnviornment->LoadScriptFromFile( FS_DEFAULT_SCRIPTS_FOLDER, "Startup.lua" );
}

std::unique_ptr<cLuaEnvironment> cLuaAPI::NewEnvironment( ) {
    std::unique_ptr<cLuaEnvironment> Enviornment = std::make_unique<cLuaEnvironment>( );

    if ( !Enviornment )
        return nullptr;

    Enviornment->Init( );

    for ( auto& Script : gFileSystem->GetFilesInFolder( FS_DEFAULT_SCRIPTS_FOLDER ) ) {
        if ( Script == "Startup.lua" ) {
            continue;
        }

        Enviornment->LoadScriptFromFile( FS_DEFAULT_SCRIPTS_FOLDER, Script );
    }

    return Enviornment;
}

void cLuaAPI::RunConnection( const std::string& connection_name ) {
    auto RunConnections = [ this ] ( const std::vector<sol::protected_function>& connections ) {
        for ( const auto& connection : connections ) {
            auto result = connection( );

            if ( !result.valid( ) ) {
                sol::error Error = result.get<sol::error>( );
                std::ostringstream LogStream;

                LogStream << "Lua Error: " << Error.what( ) << " | Connection Address: " << static_cast< const void* >( &connection );

                gLogger->Log( LogLevel::Error, LogStream.str( ) );

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