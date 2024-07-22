#include "World.hpp"

void cWorld::Init( ) {
    // Nothing here yet, im lazy.
}

void cWorld::Draw( ) {
    gLuaAPI->RunConnection( "OnWorldPaint" );
}