#pragma once

// includes
#include "../../common.hpp"

class cGraphics {
public:
	bool Init( HWND hwnd, const bool device_only = false );
	void Reset( const LPARAM lParam );
	void Release( );
	bool Valid( );

	void UpdateRenderStates( IDirect3DDevice9* device );
	
	void RenderDrawData( );
	
	void SetVSync( const bool state );
	void SetClearColor( const Color clear_color );

	// Shortcuts
	void Draw( );

	// utilities
	void CreateTextureFromBytes( IDirect3DTexture9* Resource, const std::vector<BYTE>* Bytes, const Vec2<int16_t> Size );
	void CreateTextureFromFile( IDirect3DTexture9* Resource, const char* FileName );

	inline IDirect3DDevice9* GetDevice( );

private:
	IDirect3D9* m_d3d;
	IDirect3DDevice9* m_device;
	D3DPRESENT_PARAMETERS m_parameters;

	int	m_vertex_buffer_size{ 5000 },
		m_index_buffer_size{ 10000 };

	IDirect3DVertexBuffer9* m_vertex_buffer;
	IDirect3DIndexBuffer9* m_index_buffer;

	Color m_clear_color{ 0, 0, 0, 255 };

	HWND m_hwnd;

	template <typename type>
	void safe_release( type*& obj );
};

inline const auto gGraphics = std::make_unique<cGraphics>( );

#include "DirectX9.inl"