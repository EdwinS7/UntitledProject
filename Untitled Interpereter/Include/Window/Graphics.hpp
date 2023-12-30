#pragma once

#include "../../Common.hpp"

namespace Untitled::Graphics {
	int CreateDeviceAndSwapChain( );
	void BeginScene( );
	void EndScene( );

	ID3D11Device* GetDevice( );
	void SetDevice( ID3D11Device* device );

	IDXGISwapChain* GetDeviceSwapChain( );
	void SetDeviceSwapChain( IDXGISwapChain* swap_chain );

	ID3D11DeviceContext* GetDeviceContext( );
	void SetDeviceContext( ID3D11DeviceContext* device_context );

	ID3D11RenderTargetView* GetDeviceRenderTarget( );
	void SetDeviceRenderTarget( ID3D11RenderTargetView* render_target );

	void SetResizeHeight( int height );
	void SetResizeWidth( int width );
};