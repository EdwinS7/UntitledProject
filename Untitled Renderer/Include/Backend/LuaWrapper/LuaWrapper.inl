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

inline void cLuaWrapper::RunCallback( const std::string& callback_name ) {
    for ( auto& Callback : GetCallbacks( callback_name ) ) {
        auto Result = Callback( );

        if ( !Result.valid( ) ) {
            std::cout << "Lua Error:" << static_cast< std::string >( Result.get<sol::error>( ).what( ) ) << "\n";
            m_Callbacks.clear( );
        }
    }

    auto PriorityCallbacks = GetCallbacks( "__" + callback_name );

    if ( !PriorityCallbacks.empty( ) ) {
        for ( auto& Callback : PriorityCallbacks ) {
            auto Result = Callback( );

            if ( !Result.valid( ) ) {
                std::cout << "Lua Error:" << static_cast< std::string >( Result.get<sol::error>( ).what( ) ) << "\n";
                m_Callbacks.clear( );
            }
        }
    }
}