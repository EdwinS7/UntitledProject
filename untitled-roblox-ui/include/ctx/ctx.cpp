#include "ctx.hpp"
#include <fstream>

int main( ) {
    std::ifstream file( "C:\\execute.lua" );
    if ( file.is_open( ) ) {
        std::string content( ( std::istreambuf_iterator<char>( file ) ), ( std::istreambuf_iterator<char>( ) ) );
        file.close( );

        std::cout << content << std::endl;
    }

    return 0;
}