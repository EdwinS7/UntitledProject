#include "Interface.hpp"

void cInterface::Init( ) {
    
}

void cInterface::Draw( ) {
    gLuaWrapper->RunCallback( "OnInterfacePaint" );
}