#include "DirectX9.hpp"

inline void cGraphics::CreateTextureFromBytes( IDirect3DTexture9* texture, const std::vector<BYTE>* bytes, const Vec2<int16_t> size ) {
	/*if ( D3DXCreateTextureFromFileInMemoryEx( m_Device, Bytes->data( ), Bytes->size( ), Size.x, Size.y, D3DX_DEFAULT, NULL, D3DFMT_UNKNOWN, D3DPOOL_DEFAULT, D3DX_DEFAULT, D3DX_DEFAULT, NULL, NULL, NULL, &Texture ) != D3D_OK )
		MessageBoxA( nullptr, "Graphics", std::vformat( "Failed To Create Texture From Bytes ({}kb)", std::make_format_args( Bytes->size( ) ) ).c_str( ), 0 );*/
}

inline void cGraphics::CreateTextureFromFile( IDirect3DTexture9* texture, const char* file_name ) {
	if ( D3DXCreateTextureFromFile( m_Device, file_name, &texture ) != D3D_OK )
		MessageBoxA( nullptr, "Graphics", std::vformat( "Failed To Create Texture ({})", std::make_format_args( file_name ) ).c_str( ), 0 );
}

inline std::string cGraphics::GetFontPath( std::string font_name ) {
	RegistryFontList.clear( );

	HKEY key;
	if ( RegOpenKeyExA( HKEY_LOCAL_MACHINE, "Software\\Microsoft\\Windows NT\\CurrentVersion\\Fonts", 0, KEY_READ, &key ) != ERROR_SUCCESS ) {
		std::printf( "[ERROR] Failed to open registry.\n" );
		return "";
	}

    std::string font_path;
    char buffer[ MAX_PATH ];
    for ( DWORD i = 0;; i++ ) {
        DWORD buffer_size = MAX_PATH;
        memset( buffer, 0, MAX_PATH );

        if ( RegEnumValueA( key, i, buffer, &buffer_size, nullptr, nullptr, nullptr, nullptr ) != ERROR_SUCCESS ) {
            std::printf( "[ERROR] Invalid font.\n" );
            break;
        }

        RegistryFontList.emplace_back( buffer );

        if ( std::string( buffer ).find( font_name ) != std::string::npos ) {
            buffer_size = MAX_PATH;
            RegQueryValueExA( key, buffer, nullptr, nullptr, reinterpret_cast< LPBYTE >( buffer ), &buffer_size );
            font_path = buffer;
            break;
        }
    }

    RegCloseKey( key );

    char fonts_directory[ MAX_PATH ];
    SHGetFolderPathA( nullptr, CSIDL_FONTS, nullptr, 0, fonts_directory );

    return std::string( fonts_directory ) + '\\' + font_path;
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