#include "World.hpp"

void cWorld::Init( ) {
    for ( auto& Callback : gWrapper->GetCallbacks( "OnWorldSpawn" ) ) {
        auto Result = Callback.Function( );

        if ( !Result.valid( ) )
            std::cout << static_cast< std::string >( Result.get<sol::error>( ).what( ) ) << "\n";
    }
}

void cWorld::Draw( ) {
    for ( auto& Callback : gWrapper->GetCallbacks( "OnWorldPaint" ) ) {
        auto Result = Callback.Function( );

        if ( !Result.valid( ) )
            std::cout << static_cast< std::string >( Result.get<sol::error>( ).what( ) ) << "\n";
    }

    //gBuffer->Cube( Vec3<float>( 5.f, 5.f, 5.f ), 25.f, Color( 255, 255, 255, 255 ) );
}