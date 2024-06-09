#include "World.hpp"

void cWorld::Init( ) {
    
}

void cWorld::Draw( ) {
    gLuaWrapper->RunCallback( "OnWorldPaint" );

    gBuffer->Cube( { 10.f, 10.f, 10.f }, 100.f, Color( 255, 0, 0, 255 ) );
}