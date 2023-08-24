#include "rbx.hpp"

// @note: thanks mark zuckerberg.
std::string c_rbx::compress( const std::string& data ) {
    std::string output = "RSB1";

    std::size_t data_size = data.size( );
    std::size_t max_size = ZSTD_compressBound( data_size );

    std::vector<char> compressed( max_size );
    std::size_t comp_size = ZSTD_compress( compressed.data( ), max_size, data.c_str( ), data_size, ZSTD_maxCLevel( ) );

    output.append( reinterpret_cast< char* >( &data_size ), sizeof( data_size ) );
    output.append( compressed.data( ), comp_size );

    std::uint32_t first_hash = XXH32( output.data( ), output.size( ), 42U );
    std::uint8_t hashed_bytes[ 4 ];

    std::memcpy( hashed_bytes, &first_hash, sizeof( first_hash ) );

    for ( std::size_t i = 0; i < output.size( ); ++i )
        output[ i ] ^= hashed_bytes[ i % 4 ] + static_cast< std::uint8_t >( i * 41U );

    return output;
}

void c_rbx::run_script( uintptr_t rl, lua_State* l, const std::string& source ) {
    bytecode_encoder_t enc;

    std::string bytecode = Luau::compile( "spawn(function()\n" + source + "\nend)", {}, {}, &enc );
    size_t bytecode_size = 0;

    char* error_only = luau_compile( source.c_str( ), source.size( ), NULL, &bytecode_size );

    if ( luau_load( l, "", error_only, bytecode_size, 0 ) ) {
        const char* error_message = lua_tostring( l, -1 );
        g_hooks->print( print_level::error, error_message );
        lua_pop( l, 1 );
    }
    else {
        std::string compressed = compress( bytecode );
        g_hooks->lua_vm_load( rl, &compressed, "", 0 );
        g_hooks->task_defer( rl );

        uintptr_t* top_new = reinterpret_cast< uintptr_t* >( rl + 20 );
        *top_new -= 16;
    }
}