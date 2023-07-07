#include "../../.hpp/mem/mem.hpp"

std::uintptr_t c_mem::find_pattern( const code_section_t& module, const std::string& sequence ) {
	using seq_byte_t = std::pair< std::uint8_t, bool >;

	std::string str{};
	std::vector< seq_byte_t > byte_seq{};

	std::stringstream stream{ sequence };

	while ( stream >> str ) {
		if ( str[ 0u ] == '?' ) {
			byte_seq.emplace_back( 0u, true );

			continue;
		}

		if ( !std::isxdigit( str[ 0u ] )
			|| !std::isxdigit( str[ 1u ] ) )
			continue;

		byte_seq.emplace_back(
			static_cast< std::uint8_t >(
				std::strtoul( str.data( ), nullptr, 16 )
				), false
		);
	}

	const auto end = reinterpret_cast< std::uint8_t* >( module.m_base + module.m_size );

	const auto ret = std::search(
		reinterpret_cast< std::uint8_t* >( module.m_base ), end, byte_seq.begin( ), byte_seq.end( ),
		[ ] ( const std::uint8_t byte, const seq_byte_t& seq_byte ) {
		return std::get< bool >( seq_byte ) || byte == std::get< std::uint8_t >( seq_byte );
	}
	);

	return reinterpret_cast< std::uintptr_t >( ret );
}