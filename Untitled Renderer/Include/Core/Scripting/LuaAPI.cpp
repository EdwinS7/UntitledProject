#include "LuaAPI.hpp"

void cLuaAPI::Init( ) {
    static std::unique_ptr<cLuaEnvironment> GameEnviornment = this->CreateEnvironment( );

    // Default Guis
    GameEnviornment->LoadScriptFromFile( FS_DEFAULT_SCRIPTS_FOLDER, "Startup.lua" );
}

std::unique_ptr<cLuaEnvironment> cLuaAPI::CreateEnvironment( ) {
    std::unique_ptr<cLuaEnvironment> Environment = std::make_unique<cLuaEnvironment>( );

    if ( !Environment ) {
        gLogger->Log( LogLevel::Error, "Failed to create Lua environment." );
		return nullptr;
	}

    try {
        Environment->Init( );

        auto scripts = gFileSystem->GetFilesInFolder( FS_DEFAULT_SCRIPTS_FOLDER );
        for ( const auto& Script : scripts ) {
            if ( Script == "Startup.lua" ) {
                continue;
            }

            if ( !Environment->LoadScriptFromFile( FS_DEFAULT_SCRIPTS_FOLDER, Script ) ) {
                gLogger->Log( LogLevel::Warning, "Failed to load script: " + Script );
            }
        }
    }
    catch ( const std::exception& e ) {
        gLogger->Log( LogLevel::Error, "Exception caught in CreateEnvironment: " + std::string( e.what( ) ) );
        return nullptr;
    }

    return Environment;
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
    auto Connections = GetConnections( connection_name );
    RunConnections( Connections );

    auto PriorityConnections = GetConnections( "__" + connection_name );
    RunConnections( PriorityConnections );
}

std::vector<sol::protected_function> cLuaAPI::GetConnections( const std::string& connection_name ) {
    return this->m_Connections[ connection_name ];
}

void cLuaAPI::AddConnection( const std::string& connection_name, sol::protected_function function ) {
    this->m_Connections[ connection_name ].push_back( function );
}