#pragma once

#include "../../../Common.hpp"

#define VERTEX ( D3DFVF_XYZRHW | D3DFVF_DIFFUSE | D3DFVF_TEX1 )

#define TRIANGLE D3DPT_TRIANGLESTRIP
#define LINE D3DPT_LINESTRIP

class cGraphics {
public:
	bool Init( HWND hwnd );
	void Release( );

	void UpdateRenderStates( IDirect3DDevice9* device );
	void UpdatePresentationParameters( LPARAM lparam );

	void RenderDrawData( );
	void DrawScene( );

	void CreateFontFromName( Font* font, std::string font_name, int16_t size, int16_t weight, Vec2<int16_t> padding, bool antialiasing );
	void CreateTextureFromBytes( IDirect3DTexture9* texture, std::vector<BYTE>* bytes, Vec2<int16_t> size );
	void CreateTextureFromFile( IDirect3DTexture9* texture, std::string file_name );
	void CreateImageFromFile( Image* image, std::string file_name );

	std::string GetFontPath( std::string font_name );

	void ResetDevice( );

	inline void SetVerticalSync( const bool vertical_sync );
	inline bool GetVerticalSync( );

	inline void SetDevice( IDirect3DDevice9* device );
	inline IDirect3DDevice9* GetDevice( ) const;

	inline void SetClearColor( Color clear_color );
	inline Color GetClearColor( ) const;

	inline void ReleaseFonts( );
	inline void ReleaseTextures( );

	std::vector<std::string> RegistryFontList;

private:
	IDirect3D9* m_Direct3D{};
	IDirect3DDevice9* m_Device{};
	D3DPRESENT_PARAMETERS m_Parameters{};

	int	m_VertexBufferSize{ 5000 },
		m_IndexBufferSize{ 10000 };

	IDirect3DVertexBuffer9* m_VertexBuffer{};
	IDirect3DIndexBuffer9* m_IndexBuffer{};
	IDirect3DStateBlock9* m_StateBlock{};

	std::vector<Font*> m_Fonts;
	std::vector<IDirect3DTexture9*> m_Textures;

	template <typename type>
	inline void SafeRelease( type*& obj );

	// Default Settings
	bool  m_VerticalSync { false };
	Color m_ClearColor{ 90, 125, 255, 255 };
};

inline const auto gGraphics = std::make_unique<cGraphics>( );

#include "DirectX9.inl"