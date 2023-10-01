#include "font.hpp"

ALWAYS_INLINE std::string c_font::get_path( const char* font_name ) {
	HKEY key;

	if ( RegOpenKeyExA( HKEY_LOCAL_MACHINE, "Software\\Microsoft\\Windows NT\\CurrentVersion\\Fonts", 0, KEY_READ, &key ) != ERROR_SUCCESS )
		std::printf( "get_path failed ( failed to open registry )" );

	std::string str_path;
	char str_buffer[ MAX_PATH ];

	for ( auto i = 0;; i++ ) {
		DWORD buf_size = MAX_PATH;
		memset( str_buffer, 0, MAX_PATH );

		if ( RegEnumValueA( key, i, str_buffer, &buf_size, nullptr, nullptr, nullptr, nullptr ) != ERROR_SUCCESS )
			std::printf( "get_path failed ( invalid font )" );

		if ( std::string( str_buffer ).find( font_name ) != std::string::npos ) {
			buf_size = MAX_PATH;
			RegQueryValueExA( key, str_buffer, nullptr, nullptr, reinterpret_cast< LPBYTE >( str_buffer ), &buf_size );
			str_path = str_buffer;
			break;
		}
	}

	memset( str_buffer, 0, MAX_PATH );
	SHGetFolderPathA( nullptr, CSIDL_FONTS, nullptr, 0, str_buffer );

	return std::string( str_buffer ) + '\\' + str_path;
}