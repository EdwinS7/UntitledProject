#include "util.hpp"

#define IS_IN_RANGE( value, max, min ) ( value >= max && value <= min )
#define GET_BITS( value ) ( IS_IN_RANGE( value, '0', '9' ) ? ( value - '0' ) : ( ( value & ( ~0x20 ) ) - 'A' + 0xA ) )
#define GET_BYTE( value ) ( GET_BITS( value[0] ) << 4 | GET_BITS( value[1] ) )

std::string c_util::bytes_to_ida_pattern( byte* bytes, size_t size ) {
	std::stringstream ida_pattern;

	ida_pattern << std::hex << std::setfill( '0' );
	for ( size_t i = 0; i < size; i++ ) {
		const int32_t current_byte = bytes[ i ];
		if ( current_byte != 255 )
			ida_pattern << std::setw( 2 ) << current_byte;
		else
			ida_pattern << std::setw( 1 ) << xorstr_( "?" );

		if ( i != size - 1 )
			ida_pattern << xorstr_( " " );
	}

	return ida_pattern.str( );
}

uintptr_t c_util::pattern_scan( uintptr_t mod, const char* sig, size_t size ) {
	static auto pattern_to_byte = [ ] ( const char* pattern )
	{
		auto bytes = std::vector<int>{ };
		auto start = const_cast< char* >( pattern );
		auto end = const_cast< char* >( pattern ) + strlen( pattern );

		for ( auto current = start; current < end; ++current ) {
			if ( *current == '?' ) {
				++current;

				if ( *current == '?' )
					++current;

				bytes.push_back( -1 );
			}
			else
				bytes.push_back( strtoul( current, &current, 16 ) );
		}
		return bytes;
	};


	const auto image_size = size;

	if ( !image_size )
		return { };

	auto pattern_bytes = pattern_to_byte( sig );

	const auto image_bytes = reinterpret_cast< byte* >( mod );

	const auto signature_size = pattern_bytes.size( );
	const auto signature_bytes = pattern_bytes.data( );

	for ( auto i = 0ul; i < image_size - signature_size; ++i ) {
		auto byte_sequence_found = true;

		for ( auto j = 0ul; j < signature_size; ++j ) {
			if ( image_bytes[ i + j ] != signature_bytes[ j ] && signature_bytes[ j ] != -1 ) {
				byte_sequence_found = false;
				break;
			}
		}

		if ( byte_sequence_found )
			return uintptr_t( &image_bytes[ i ] );
	}

	return { };
}

std::vector<uintptr_t> c_util::get_xrefs_to( uintptr_t address, uintptr_t start, uintptr_t size ) {
	std::vector<uintptr_t> xrefs = { };

	const std::string ida_pattern = bytes_to_ida_pattern( ( byte* ) &address, 4 );

	const uintptr_t end = start + size;
	while ( start && start < end ) {
		uintptr_t xref = ( uintptr_t ) pattern_scan( start, ida_pattern.c_str( ), size );

		if ( !xref )
			break;

		xrefs.push_back( xref );
		start = xref + 4;
	}

	return xrefs;
}

std::uintptr_t c_util::scan( const std::string& image_name, const std::string& signature ) {
	auto image = GetModuleHandleA( image_name.c_str( ) );
	return scan( ( std::uintptr_t ) image, signature );
}

std::uintptr_t c_util::scan( const std::uintptr_t image, const std::string& signature ) {
	if ( !image ) {
		return 0u;
	}

	auto image_base = ( std::uintptr_t ) ( image );
	auto image_dos_hdr = ( IMAGE_DOS_HEADER* ) ( image_base );

	if ( image_dos_hdr->e_magic != IMAGE_DOS_SIGNATURE )
		return 0u;

	auto image_nt_hdrs = ( IMAGE_NT_HEADERS* ) ( image_base + image_dos_hdr->e_lfanew );

	if ( image_nt_hdrs->Signature != IMAGE_NT_SIGNATURE )
		return 0u;

	auto scan_begin = ( std::uint8_t* ) ( image_base );
	auto scan_end = ( std::uint8_t* ) ( image_base + image_nt_hdrs->OptionalHeader.SizeOfImage );

	std::uint8_t* scan_result = nullptr;
	std::uint8_t* scan_data = ( std::uint8_t* ) ( signature.c_str( ) );

	for ( auto current = scan_begin; current < scan_end; current++ ) {
		if ( *( std::uint8_t* ) scan_data == '\?' || *current == GET_BYTE( scan_data ) ) {
			if ( !scan_result )
				scan_result = current;

			if ( !scan_data[ 2 ] )
				return ( std::uintptr_t ) ( scan_result );

			scan_data += ( *( std::uint16_t* ) scan_data == '\?\?' || *( std::uint8_t* ) scan_data != '\?' ) ? 3 : 2;

			if ( !*scan_data )
				return ( std::uintptr_t ) ( scan_result );
		}
		else if ( scan_result ) {
			current = scan_result;
			scan_data = ( std::uint8_t* ) ( signature.c_str( ) );
			scan_result = nullptr;
		}
	}

	return 0u;
}