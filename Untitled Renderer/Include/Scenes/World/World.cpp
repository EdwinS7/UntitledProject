#include "World.hpp"

void cWorld::Init( ) {
    
}

void cWorld::Draw( ) {
    gWrapper->RunCallback( "OnWorldPaint" );
}