#include "DirectX9.hpp"

inline void cGraphics::SetVerticalSync( bool vertical_sync ) {
    m_Parameters.PresentationInterval = vertical_sync ? D3DPRESENT_INTERVAL_DEFAULT : D3DPRESENT_INTERVAL_IMMEDIATE;
    m_VerticalSync = vertical_sync;
    ResetDevice();
}

inline bool cGraphics::GetVerticalSync() const {
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

inline void cGraphics::ReleaseFonts( ) {
    for ( auto& font : m_Fonts ) {
        SafeRelease( font );
    }

    m_Fonts.clear( );
}

inline void cGraphics::ReleaseTextures( ) {
    for ( auto& texture : m_Textures ) {
        SafeRelease( texture );
    }

    m_Textures.clear( );
}

template <typename type>
inline void cGraphics::SafeRelease( type*& obj ) {
	if ( obj ) {
		obj->Release( );
		obj = nullptr;
	}
}