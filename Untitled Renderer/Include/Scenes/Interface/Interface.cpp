#include "Interface.hpp"

void cInterface::Init( ) {
    // All done in Lua so far.
}

void cInterface::Draw( ) {
    gLuaAPI->RunConnection( "OnInterfacePaint" );
}