#include "rbx.hpp"

// @note: thanks mark zuckerberg.
std::string c_rbx::compress_to_bytecode( const std::string& bytecode ) {
	const auto data_size = bytecode.size( );
	const auto max_size = ZSTD_compressBound( data_size );
	auto buffer = std::vector<char>( max_size + 8 );

	memcpy( &buffer[ 0 ], "RSB1", 4u );
	memcpy( &buffer[ 4 ], &data_size, 4u );

	const auto compressed_size = ZSTD_compress( &buffer[ 8 ], max_size, bytecode.data( ), data_size, ZSTD_maxCLevel( ) );
	if ( ZSTD_isError( compressed_size ) )
		throw std::runtime_error( "Failed to compress the bytecode." );

	const auto size = compressed_size + 8;
	const auto key = XXH32( buffer.data( ), size, 42u );
	const auto bytes = reinterpret_cast< const uint8_t* >( &key );

	for ( auto i = 0u; i < size; ++i )
		buffer[ i ] ^= bytes[ i % 4 ] + i * 41u;

	return std::string( buffer.data( ), size );
}

void c_rbx::run_script( uintptr_t rl, const std::string& source ) {
    if ( source.length( ) == 0 )
        return;

    static auto encoder = bytecode_encoder_t( );
    const auto bytecode = Luau::compile( "task.wait()\n" + source, {}, {}, &encoder );

	auto compressed = compress_to_bytecode( bytecode );

	if ( g_hooks->lua_vm_load( rl, &compressed, "", 0 ) )
		throw std::runtime_error( "Unexpected error during execution." );

	// Spawn the proto and pop it off the stack.
	g_hooks->task_spawn( rl );
	pop_stack( rl, 1 );
}