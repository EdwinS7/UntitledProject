#include "rbx.hpp"

class BytecodeEncoderImpl : public Luau::BytecodeEncoder {
public:
	std::uint8_t encodeOp( const std::uint8_t opcode ) override {
		return opcode * 227;
	}
} encoder;

std::string c_rbx::serialize( const std::string& bytecode ) {
    const std::size_t data_size = bytecode.size( );
    const std::size_t max_size = ZSTD_compressBound( data_size );
    std::vector<char> buffer( max_size + 8 );

    std::memcpy( &buffer[ 0 ], xorstr_( "RSB1" ), 4 );
    std::memcpy( &buffer[ 4 ], &data_size, 4 );

    const std::size_t compressed_size = ZSTD_compress( &buffer[ 8 ], max_size, bytecode.data( ), data_size, ZSTD_maxCLevel( ) );

    if ( ZSTD_isError( compressed_size ) ) {
        g_hooks->print( print_level::error, xorstr_( "Unexpected error during serialization." ) );
        return std::string( );
    }

    const std::size_t size = compressed_size + 8;
    const std::uint32_t key = XXH32( buffer.data( ), size, 42 );
    const std::uint8_t* bytes = reinterpret_cast< const std::uint8_t* >( &key );

    for ( std::size_t i = 0; i < size; ++i )
        buffer[ i ] ^= bytes[ i % 4 ] + i * 41;

    return std::string( buffer.data( ), size );
}

void c_rbx::run_script( const std::string& source ) {
    if ( source.length( ) <= 8 )
        return;

	// @note: task.wait is scheduled for next frame.
    std::string script = Luau::compile( source, {}, {}, &encoder );
	std::string bytecode = serialize( script );

	// @note: deserializeFailure may be a problem here, idk.
    if ( g_hooks->lua_vm_load( g_offsets->lua_state, &bytecode, g_util->random_string( 16 ).c_str( ), 0 ) ) {
        g_hooks->print( print_level::error, xorstr_( "Unexpected error during execution." ) );
        return;
    }

	// @note: task_spawn can also be used here.
	g_hooks->task_defer( g_offsets->lua_state );
    pop_stack( 1 );
}