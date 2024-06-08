#include "Wrapper.hpp"

inline void cWrapper::RegisterCallback( std::string event_name, sol::protected_function Callback ) {
    this->m_Callbacks[ event_name ].push_back( cLuaCallback( Callback ) );
}

inline void cWrapper::UnregisterCallbacks( ) {
    if ( !m_Callbacks.empty( ) )
        m_Callbacks.clear( );
}

inline std::vector<cLuaCallback> cWrapper::GetCallbacks( std::string event_name ) {
    return this->m_Callbacks[ event_name ];
}

inline void cWrapper::RunCallback( const std::string& callback_name ) {
    for ( auto& Callback : GetCallbacks( callback_name ) ) {
        auto Result = Callback.Function( );

        if ( !Result.valid( ) ) {
            std::cout << "Lua Error:" << static_cast< std::string >( Result.get<sol::error>( ).what( ) ) << "\n";
            m_Callbacks.clear( );
        }
    }
}