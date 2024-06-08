#include "DirectX9.hpp"

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

inline void cGraphics::SetVerticalSync(const bool vertical_sync) {
    m_Parameters.PresentationInterval = vertical_sync ? D3DPRESENT_INTERVAL_DEFAULT : D3DPRESENT_INTERVAL_IMMEDIATE;
    m_VerticalSync = vertical_sync;
    ResetDevice();
}

inline bool cGraphics::GetVerticalSync() {
    return m_VerticalSync;
}

inline void cGraphics::SetDevice( IDirect3DDevice9* device ) {
    m_Device = device;
}

inline IDirect3DDevice9* cGraphics::GetDevice( ) const {
    return m_Device;
}

void cGraphics::SetClearColor( Color clear_color ) {
    m_ClearColor = clear_color;
}

inline Color cGraphics::GetClearColor( ) const {
    return m_ClearColor;
}

template <typename type>
inline void cGraphics::SafeRelease( type*& obj ) {
	if ( obj ) {
		obj->Release( );
		obj = nullptr;
	}
}