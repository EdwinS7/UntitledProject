#pragma once

#include "../../Common.hpp"

// FOR TEXTURES: ID3D11Texture2D

class cGraphics {
public:
	bool Init( );
	void ResetDevice( LPARAM lparam );
	void Release( );
	void Draw( );

	void UpdateRenderStates( ID3D11DeviceContext* deviceContext );
	void UpdatePresentParameters( LPARAM lparam );

	void RenderDrawData( );
	inline bool IsDeviceValid( );

	void CreateFontFromName( Font* font, const char* font_name, const int16_t size, const int16_t weight, const int16_t padding, const bool antialiasing );

	inline void CreateTextureFromBytes( ID3D11Texture2D* texture, const std::vector<BYTE>* bytes, const Vec2<int16_t> size );
	inline void CreateTextureFromFile( ID3D11Texture2D* texture, const char* file_name );

	inline std::string GetFontPath( const char* font_name );
	inline ID3D11Device* GetDevice( );

private:
	ID3D11Device* m_Device{};
	IDXGISwapChain* m_SwapChain{};
	ID3D11Texture2D* m_DepthStencil{};
	ID3D11DeviceContext* m_DeviceContext{};
	DXGI_SWAP_CHAIN_DESC m_SwapChainDesc{};
	ID3D11RenderTargetView* m_RenderTargetView{};
	ID3D11DepthStencilView* m_DepthStencilView{};

	int	m_VertexBufferSize{ 5000 },
		m_IndexBufferSize{ 10000 };

	ID3D11Buffer* m_VertexBuffer{};
	ID3D11Buffer* m_IndexBuffer{};

	Color m_ClearColor{ 20, 20, 20, 255 };

	template <typename type>
	inline void SafeRelease( type*& obj );
};

inline const auto gGraphics = std::make_unique<cGraphics>( );

#include "DirectX9.inl"