#include "Interface.hpp"

void cInterface::Init( ) {
    for ( auto& Callback : gWrapper->GetCallbacks( "OnInterfaceSpawn" ) ) {
        auto Result = Callback.Function( );

        if ( !Result.valid( ) ) {
            std::cout << static_cast< std::string >( Result.get<sol::error>( ).what( ) ) << "\n";
            gWrapper->GetCallbacks( "OnInterfacePaint" ).clear( );
        }
    }
}

void cInterface::Draw( ) {
    for ( auto& Callback : gWrapper->GetCallbacks( "OnInterfacePaint" ) ) {
        auto Result = Callback.Function( );

        if ( !Result.valid( ) ) {
            std::cout << static_cast< std::string >( Result.get<sol::error>( ).what( ) ) << "\n";
            gWrapper->GetCallbacks( "OnInterfacePaint" ).clear( );
        }
    }
}