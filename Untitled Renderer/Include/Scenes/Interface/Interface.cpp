#include "Interface.hpp"

void cInterface::Init( ) {
    
}

void cInterface::Draw( ) {
    gWrapper->RunCallback( "OnInterfacePaint" );
}