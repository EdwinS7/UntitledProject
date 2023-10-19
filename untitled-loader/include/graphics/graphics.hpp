#pragma once

#include "../../common.hpp"

class CGraphics {
public:
	int CreateDeviceAndSwapChain( );
	void BeginScene( );
	void EndScene( );

	INLINE ID3D11Device* GetDevice( );
	INLINE void SetDevice( ID3D11Device* device );

	INLINE IDXGISwapChain* GetDeviceSwapChain( );
	INLINE void SetDeviceSwapChain( IDXGISwapChain* swap_chain );

	INLINE ID3D11DeviceContext* GetDeviceContext( );
	INLINE void SetDeviceContext( ID3D11DeviceContext* device_context );

	INLINE ID3D11RenderTargetView* GetDeviceRenderTarget( );
	INLINE void SetDeviceRenderTarget( ID3D11RenderTargetView* render_target );

	INLINE void SetResizeHeight( int height );
	INLINE void SetResizeWidth( int width );

private:
	ID3D11Device* pDevice = nullptr;
	IDXGISwapChain* pSwapChain = nullptr;
	ID3D11Texture2D* pBackBuffer = nullptr;
	ID3D11DeviceContext* pDeviceContext = nullptr;
	ID3D11RenderTargetView* pRenderTarget = nullptr;
	int m_ResizeWidth, m_ResizeHeight;
};

inline const auto Graphics = std::make_unique<CGraphics>( );

#include "graphics.inl"