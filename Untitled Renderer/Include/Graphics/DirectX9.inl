#include "DirectX9.hpp"

inline void cGraphics::CreateTextureFromBytes( IDirect3DTexture9* Texture, const std::vector<BYTE>* Bytes, const Vec2<int16_t> Size ) {
	if ( D3DXCreateTextureFromFileInMemoryEx( m_Device, Bytes->data( ), Bytes->size( ), Size.x, Size.y, D3DX_DEFAULT, NULL, D3DFMT_UNKNOWN, D3DPOOL_DEFAULT, D3DX_DEFAULT, D3DX_DEFAULT, NULL, NULL, NULL, &Texture ) != D3D_OK )
		MessageBoxA( nullptr, "Graphics", std::vformat( "Failed To Create Texture From Bytes ({}kb)", std::make_format_args( Bytes->size( ) ) ).c_str( ), 0 );
}

inline void cGraphics::CreateTextureFromFile( IDirect3DTexture9** Texture, const char* FileName ) {
	if ( D3DXCreateTextureFromFile( m_Device, FileName, Texture ) != D3D_OK )
		MessageBoxA( nullptr, "Graphics", std::vformat( "Failed To Create Texture ({})", std::make_format_args( FileName ) ).c_str( ), 0 );
}

inline std::string cGraphics::GetFontPath( const char* FontName ) {
	HKEY key;

	if ( RegOpenKeyExA( HKEY_LOCAL_MACHINE, "Software\\Microsoft\\Windows NT\\CurrentVersion\\Fonts", 0, KEY_READ, &key ) != ERROR_SUCCESS )
		std::printf( "[ Buffer ] get_path failed ( failed to open registry )" );

	std::string str_path;
	char str_buffer[ MAX_PATH ];

	for ( auto i = 0;; i++ ) {
		DWORD buf_size = MAX_PATH;
		memset( str_buffer, 0, MAX_PATH );

		if ( RegEnumValueA( key, i, str_buffer, &buf_size, nullptr, nullptr, nullptr, nullptr ) != ERROR_SUCCESS )
			std::printf( "[ Buffer ] get_path failed ( invalid font )" );

		if ( std::string( str_buffer ).find( FontName ) != std::string::npos ) {
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

inline IDirect3DDevice9* cGraphics::GetDevice( ) {
    return m_Device;
}

template <typename type>
inline void cGraphics::SafeRelease( type*& obj ) {
	if ( obj ) {
		obj->Release( );
		obj = nullptr;
	}
}