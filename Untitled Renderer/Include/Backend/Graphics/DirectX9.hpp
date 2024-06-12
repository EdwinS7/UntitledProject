#pragma once
#ifndef GRAPHICS_H
#define GRAPHICS_H

#include <Common.hpp>
#include "Components.hpp"
#include "../Logger/Logger.hpp"
#include "../FileSystem/FileSystem.hpp"
#include "../Window/Window.hpp"
#include "../Buffer/Buffer.hpp"

class cGraphics {
public:
	bool Init( HWND hwnd );
	void Release( );

	void ResetDevice( HWND hwnd );

	void UpdateRenderStates( IDirect3DDevice9* device );
	void UpdatePresentationParameters( HWND hwnd );

	void RenderDrawData( );
	void DrawScene( );

	void CreateFontFromName( Font* font, const std::string& font_name, int16_t size, int16_t weight, Vec2<int16_t> padding, bool antialiasing );
	void CreateTextureFromBytes( IDirect3DTexture9* texture, std::vector<BYTE>* bytes, Vec2<int16_t> size );
	void CreateTextureFromFile( IDirect3DTexture9* texture, const std::string& file_name );
	void CreateImageFromFile( Image* image, const std::string& file_name );

	void SetVerticalSync( bool vertical_sync );
	inline bool GetVerticalSync( ) const;

	inline void SetDevice( IDirect3DDevice9* device );
	inline IDirect3DDevice9* GetDevice( ) const;

	inline void SetClearColor( Color clear_color );
	inline Color GetClearColor( ) const;

	inline void ReleaseTextures( );
	inline void ReleaseFonts( );

private:
	IDirect3D9* m_Direct3D{};
	IDirect3DDevice9* m_Device{};
	D3DPRESENT_PARAMETERS m_Parameters{};

	int	m_VertexBufferSize, m_IndexBufferSize;

	IDirect3DVertexBuffer9* m_VertexBuffer{};
	IDirect3DIndexBuffer9* m_IndexBuffer{};
	IDirect3DStateBlock9* m_StateBlock{};

	std::vector<IDirect3DTexture9*> m_Textures;
	std::vector<Font*> m_Fonts;

	template <typename T>
	inline void SafeRelease( T*& obj );

	Color m_ClearColor{ 160, 215, 232 };
};

inline const auto gGraphics = std::make_unique<cGraphics>( );

#include "DirectX9.inl"
#endif