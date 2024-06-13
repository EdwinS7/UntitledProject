#include "LuaWrapper.hpp"

inline void cLuaWrapper::RegisterCallback( const std::string& callback_name, sol::protected_function callback ) {
    this->m_Callbacks[ callback_name ].push_back( callback );
}

inline void cLuaWrapper::UnregisterCallbacks( ) {
    if ( !m_Callbacks.empty( ) )
        m_Callbacks.clear( );
}

inline std::vector<sol::protected_function> cLuaWrapper::GetCallbacks( const std::string& callback_name ) {
    return this->m_Callbacks[ callback_name ];
}