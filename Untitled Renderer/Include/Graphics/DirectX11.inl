#include "DirectX11.hpp"

inline void cGraphics::CreateTextureFromBytes( ID3D11Texture2D* texture, const std::vector<BYTE>* bytes, const Vec2<int16_t> size ) {
	/*if ( D3DXCreateTextureFromFileInMemoryEx( m_Device, Bytes->data( ), Bytes->size( ), Size.x, Size.y, D3DX_DEFAULT, NULL, D3DFMT_UNKNOWN, D3DPOOL_DEFAULT, D3DX_DEFAULT, D3DX_DEFAULT, NULL, NULL, NULL, &Texture ) != D3D_OK )
		MessageBoxA( nullptr, "Graphics", std::vformat( "Failed To Create Texture From Bytes ({}kb)", std::make_format_args( Bytes->size( ) ) ).c_str( ), 0 );*/
}

inline void cGraphics::CreateTextureFromFile( ID3D11Texture2D* texture, const char* file_name ) {
	/*if ( D3DXCreateTextureFromFile( m_Device, FileName, Texture ) != D3D_OK )
		MessageBoxA( nullptr, "Graphics", std::vformat( "Failed To Create Texture ({})", std::make_format_args( FileName ) ).c_str( ), 0 );*/
}

inline ID3D11Device* cGraphics::GetDevice( ) {
	return m_Device;
}