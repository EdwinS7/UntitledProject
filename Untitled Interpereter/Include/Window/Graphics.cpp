#include "graphics.hpp"

ID3D11Device* pDevice = nullptr;
IDXGISwapChain* pSwapChain = nullptr;
ID3D11Texture2D* pBackBuffer = nullptr;
ID3D11DeviceContext* pDeviceContext = nullptr;
ID3D11RenderTargetView* pRenderTarget = nullptr;
int ResizeWidth, ResizeHeight;

int Untitled::Graphics::CreateDeviceAndSwapChain( ) {
    auto Hwnd = Win32::GetHwnd( );

    DXGI_SWAP_CHAIN_DESC SwapChainDesc = {
        { 0, 0, { 60, 1 }, DXGI_FORMAT_R8G8B8A8_UNORM },
        { 1, 0 },
        DXGI_USAGE_RENDER_TARGET_OUTPUT,
        2,
        Hwnd,
        TRUE,
        DXGI_SWAP_EFFECT_DISCARD,
        DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH
    };

    UINT CreateDeviceFlags = 0;
    D3D_FEATURE_LEVEL FeatureLevel;
    D3D_FEATURE_LEVEL FeatureLevelArray[ 2 ] = { D3D_FEATURE_LEVEL_11_0, D3D_FEATURE_LEVEL_10_0 };

    HRESULT Result = D3D11CreateDeviceAndSwapChain(
        nullptr,
        D3D_DRIVER_TYPE_HARDWARE,
        nullptr,
        CreateDeviceFlags,
        FeatureLevelArray, 2, 
        D3D11_SDK_VERSION, 
        &SwapChainDesc,
        &pSwapChain,
        &pDevice,
        &FeatureLevel,
        &pDeviceContext
    );
    
    if ( Result == DXGI_ERROR_UNSUPPORTED ) {
        Result = D3D11CreateDeviceAndSwapChain(
            nullptr,
            D3D_DRIVER_TYPE_WARP,
            nullptr, CreateDeviceFlags,
            FeatureLevelArray,
            2,
            D3D11_SDK_VERSION,
            &SwapChainDesc,
            &pSwapChain,
            &pDevice,
            &FeatureLevel,
            &pDeviceContext
        );
    }
    
    if ( Result != S_OK )
        return 0;

    pSwapChain->GetBuffer( 0, IID_PPV_ARGS( &pBackBuffer ) );
    pDevice->CreateRenderTargetView( pBackBuffer, nullptr, &pRenderTarget );
    pBackBuffer->Release( );

    return 1;
}

void Untitled::Graphics::BeginScene( ) {
    if ( ResizeWidth != 0 && ResizeHeight != 0 ) {
        if ( pRenderTarget ) {
            pRenderTarget->Release( );
            pRenderTarget = nullptr;
        }

        pSwapChain->ResizeBuffers( 0, ResizeWidth, ResizeHeight, DXGI_FORMAT_UNKNOWN, 0 );
        ResizeWidth = ResizeHeight = 0;

        ID3D11Texture2D* pBackBuffer;
        pSwapChain->GetBuffer( 0, IID_PPV_ARGS( &pBackBuffer ) );
        pDevice->CreateRenderTargetView( pBackBuffer, nullptr, &pRenderTarget );
        pBackBuffer->Release( );
    }

    ImGui_ImplDX11_NewFrame( );
    ImGui_ImplWin32_NewFrame( );
    ImGui::NewFrame( );
}

void Untitled::Graphics::EndScene( ) {
    ImGui::Render( );

    const float clear_color[ 4 ] = { ( 0.f ) / 255.f, ( 0.f ) / 255.f, ( 0.f ) / 255.f, ( 255.f) / 255.f };
    pDeviceContext->OMSetRenderTargets( 1, &pRenderTarget, nullptr );
    pDeviceContext->ClearRenderTargetView( pRenderTarget, clear_color );

    ImGui_ImplDX11_RenderDrawData( ImGui::GetDrawData( ) );

    pSwapChain->Present( 1, 0 );
}

ID3D11Device* Untitled::Graphics::GetDevice( ) {
    return pDevice;
}

void Untitled::Graphics::SetDevice( ID3D11Device* device ) {
    pDevice = device;
}

IDXGISwapChain* Untitled::Graphics::GetDeviceSwapChain( ) {
    return pSwapChain;
}

void Untitled::Graphics::SetDeviceSwapChain( IDXGISwapChain* swap_chain ) {
    pSwapChain = swap_chain;
}

ID3D11DeviceContext* Untitled::Graphics::GetDeviceContext( ) {
    return pDeviceContext;
}

void Untitled::Graphics::SetDeviceContext( ID3D11DeviceContext* device_context ) {
    pDeviceContext = device_context;
}

ID3D11RenderTargetView* Untitled::Graphics::GetDeviceRenderTarget( ) {
    return pRenderTarget;
}

void Untitled::Graphics::SetDeviceRenderTarget( ID3D11RenderTargetView* render_target ) {
    pRenderTarget = render_target;
}

void Untitled::Graphics::SetResizeHeight( int height ) {
    ResizeHeight = height;
}

void Untitled::Graphics::SetResizeWidth( int width ) {
    ResizeWidth = width;
}