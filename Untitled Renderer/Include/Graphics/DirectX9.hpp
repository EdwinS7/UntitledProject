#pragma once

#include "../../common.hpp"

class cGraphics {
public:
	bool Init( HWND hwnd, const bool init = true );
	void Reset( const LPARAM lParam );
	void Release( );
	bool Valid( );

	void UpdateRenderStates( IDirect3DDevice9* device );
	
	void RenderDrawData( );
	
	void SetVSync( const bool state );
	void SetClearColor( const Color clear_color );

	void Draw( );

	void CreateTextureFromBytes( IDirect3DTexture9* texture, const std::vector<BYTE>* bytes, const Vec2<int16_t> size );
	void CreateTextureFromFile( IDirect3DTexture9** texture, const char* file_name );

	void CreateFontFromName( Font* font, const char* font_name, const int16_t size, const int16_t weight, const int16_t padding, const bool antialiasing );

	inline std::string GetFontPath( const char* font_name );
	inline IDirect3DDevice9* GetDevice( );

private:
	IDirect3D9* m_Direct3D;
	IDirect3DDevice9* m_Device;
	D3DPRESENT_PARAMETERS m_Parameters;

	int	m_VertexBufferSize{ 5000 },
		m_IndexBufferSize{ 10000 };

	IDirect3DVertexBuffer9* m_VertexBuffer;
	IDirect3DIndexBuffer9* m_IndexBuffer;

	Color m_ClearColor{ 20, 20, 20, 255 };

	HWND m_Hwnd;

	template <typename type>
	void SafeRelease( type*& obj );
};

inline const auto gGraphics = std::make_unique<cGraphics>( );

#include "DirectX9.inl"