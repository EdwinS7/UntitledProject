#include "font.hpp"

ALWAYS_INLINE const char* c_font::get_path( const char* font_name ) {
	HKEY key;
	RegOpenKeyExA( HKEY_LOCAL_MACHINE, "Software\\Microsoft\\Windows NT\\CurrentVersion\\Fonts", 0, KEY_READ, &key );

	std::string str_path;
	char str_buffer[ MAX_PATH ];

	for ( auto i = 0;; i++ ) {
		memset( str_buffer, 0, MAX_PATH );
		DWORD buf_size = MAX_PATH;

		if ( RegEnumValueA( key, i, str_buffer, &buf_size, nullptr, nullptr, nullptr, nullptr ) != ERROR_SUCCESS )
			throw std::runtime_error{ "font_t failed to get font path. invalid font" };

		if ( std::string( str_buffer ).find( _strdup( font_name ) ) != std::string::npos ) {
			buf_size = MAX_PATH;
			RegQueryValueExA( key, str_buffer, nullptr, nullptr, reinterpret_cast< LPBYTE >( str_buffer ), &buf_size );
			str_path = str_buffer;
			break;
		}
	}

	memset( str_buffer, 0, MAX_PATH );
	SHGetFolderPathA( nullptr, CSIDL_FONTS, nullptr, 0, str_buffer );

	return (std::string( str_buffer ) + '\\' + str_path).c_str();
}