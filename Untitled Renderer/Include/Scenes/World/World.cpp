#include "World.hpp"

void cWorld::Init( ) {
    for ( auto& Callback : gWrapper->GetCallbacks( "OnWorldSpawn" ) ) {
        auto Result = Callback.Function( );

        if ( !Result.valid( ) ) {
            std::cout << static_cast< std::string >( Result.get<sol::error>( ).what( ) ) << "\n";
            gWrapper->GetCallbacks( "OnInterfacePaint" ).clear( );
        }
    }
}

void cWorld::Draw( ) {
    for ( auto& Callback : gWrapper->GetCallbacks( "OnWorldPaint" ) ) {
        auto Result = Callback.Function( );

        if ( !Result.valid( ) ) {
            std::cout << static_cast< std::string >( Result.get<sol::error>( ).what( ) ) << "\n";
            gWrapper->GetCallbacks( "OnInterfacePaint" ).clear( );
        }
    }
}