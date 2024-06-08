#include "Interface.hpp"

void cInterface::Init( ) {
    
}

void cInterface::Draw( ) {
    gWrapper->RunCallback( "OnInterfacePaint" );

    std::vector<Vec2<int16_t>> Points = {
        Vec2<int16_t>(100, 100),
        Vec2<int16_t>( 100, 150 ),
        Vec2<int16_t>( 200, 150 ),
        Vec2<int16_t>( 300, 200 ),
        Vec2<int16_t>( 350, 100 ),
        //Vec2<int16_t>( 100, 100 )
    };

    //gBuffer->Polygon( Points, Color( 255, 255, 255, 255 ) );
}