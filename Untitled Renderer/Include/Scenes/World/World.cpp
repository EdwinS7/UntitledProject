#include "World.hpp"

void cWorld::Init( ) {

}

void cWorld::Draw( ) {
    for ( auto& Callback : gWrapper->GetCallbacks( "OnWorldPaint" ) ) {
        auto Result = Callback.Function( );

        if ( !Result.valid( ) )
            std::cout << static_cast< std::string >( Result.get<sol::error>( ).what( ) ) << "\n";
    }
}