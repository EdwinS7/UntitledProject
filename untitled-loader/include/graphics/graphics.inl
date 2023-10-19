#include "graphics.hpp"
#pragma once

INLINE ID3D11Device* CGraphics::GetDevice( ) {
    return pDevice;
}

INLINE void CGraphics::SetDevice( ID3D11Device* device ) {
    pDevice = device;
}

INLINE IDXGISwapChain* CGraphics::GetDeviceSwapChain( ) {
    return pSwapChain;
}

INLINE void CGraphics::SetDeviceSwapChain( IDXGISwapChain* swap_chain ) {
    pSwapChain = swap_chain;
}

INLINE ID3D11DeviceContext* CGraphics::GetDeviceContext( ) {
    return pDeviceContext;
}

INLINE void CGraphics::SetDeviceContext( ID3D11DeviceContext* device_context ) {
    pDeviceContext = device_context;
}

INLINE ID3D11RenderTargetView* CGraphics::GetDeviceRenderTarget( ) {
    return pRenderTarget;
}

INLINE void CGraphics::SetDeviceRenderTarget( ID3D11RenderTargetView* render_target ) {
    pRenderTarget = render_target;
}

INLINE void CGraphics::SetResizeHeight( int height ) {
    m_ResizeHeight = height;
}

INLINE void CGraphics::SetResizeWidth( int width ) {
    m_ResizeWidth = width;
}