#include "DirectX11.hpp"

bool cGraphics::Init( ) {
    HRESULT result = D3D11CreateDevice(
        nullptr,
        D3D_DRIVER_TYPE_HARDWARE,
        nullptr,
        D3D11_CREATE_DEVICE_BGRA_SUPPORT,
        nullptr,
        0,
        D3D11_SDK_VERSION,
        &m_Device,
        nullptr,
        &m_DeviceContext
    );

    if ( FAILED( result ) )
        throw std::runtime_error( "[GraphicsManager::Initialize] Failed to create DirectX 11 device." );

    DXGI_SWAP_CHAIN_DESC scDesc;
    ZeroMemory( &scDesc, sizeof( scDesc ) );
    scDesc.BufferCount = 1;
    scDesc.BufferDesc.Width = gWindow->GetSize( ).x;
    scDesc.BufferDesc.Height = gWindow->GetSize( ).y;
    scDesc.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
    scDesc.BufferDesc.RefreshRate.Numerator = 60;
    scDesc.BufferDesc.RefreshRate.Denominator = 1;
    scDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
    scDesc.OutputWindow = gWindow->GetHwnd( );
    scDesc.SampleDesc.Count = 1;
    scDesc.SampleDesc.Quality = 0;
    scDesc.Windowed = TRUE;

    result = D3D11CreateDeviceAndSwapChain(
        nullptr,
        D3D_DRIVER_TYPE_HARDWARE,
        nullptr,
        0,
        nullptr,
        0,
        D3D11_SDK_VERSION,
        &scDesc,
        &m_SwapChain,
        &m_Device,
        nullptr,
        &m_DeviceContext
    );

    if ( FAILED( result ) )
        throw std::runtime_error( "[GraphicsManager::Initialize] Failed to create DirectX 11 device and swap chain." );

    ID3D11Texture2D* backBuffer;
    result = m_SwapChain->GetBuffer( 0, __uuidof( ID3D11Texture2D ), ( void** ) &backBuffer );
    if ( FAILED( result ) )
        throw std::runtime_error( "[GraphicsManager::Initialize] Failed to get back buffer." );

    result = m_Device->CreateRenderTargetView( backBuffer, nullptr, &m_RenderTargetView );
    if ( FAILED( result ) )
        throw std::runtime_error( "[GraphicsManager::Initialize] Failed to create render target view." );

    backBuffer->Release( );

    D3D11_TEXTURE2D_DESC depthStencilDesc;
    ZeroMemory( &depthStencilDesc, sizeof( depthStencilDesc ) );
    depthStencilDesc.Width = gWindow->GetSize( ).x;
    depthStencilDesc.Height = gWindow->GetSize( ).y;
    depthStencilDesc.MipLevels = 1;
    depthStencilDesc.ArraySize = 1;
    depthStencilDesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
    depthStencilDesc.SampleDesc.Count = 1;
    depthStencilDesc.SampleDesc.Quality = 0;
    depthStencilDesc.Usage = D3D11_USAGE_DEFAULT;
    depthStencilDesc.BindFlags = D3D11_BIND_DEPTH_STENCIL;
    depthStencilDesc.CPUAccessFlags = 0;
    depthStencilDesc.MiscFlags = 0;

    result = m_Device->CreateTexture2D( &depthStencilDesc, nullptr, &m_DepthStencil );
    if ( FAILED( result ) )
        throw std::runtime_error( "[GraphicsManager::Initialize] Failed to create depth stencil buffer." );

    result = m_Device->CreateDepthStencilView( m_DepthStencil, nullptr, &m_DepthStencilView );
    if ( FAILED( result ) )
        throw std::runtime_error( "[GraphicsManager::Initialize] Failed to create depth stencil view." );

    m_DeviceContext->OMSetRenderTargets( 1, &m_RenderTargetView, m_DepthStencilView );

    D3D11_VIEWPORT viewport;
    viewport.Width = static_cast< float >( gWindow->GetSize( ).x );
    viewport.Height = static_cast< float >( gWindow->GetSize( ).y );
    viewport.MinDepth = 0.0f;
    viewport.MaxDepth = 1.0f;
    viewport.TopLeftX = 0;
    viewport.TopLeftY = 0;

    m_DeviceContext->RSSetViewports( 1, &viewport );

    return true;
}


void cGraphics::ResetDevice( LPARAM lparam ) {
    // Release resources
    gBuffer->Release( );
    SafeRelease( m_VertexBuffer );
    SafeRelease( m_IndexBuffer );
    SafeRelease( m_RenderTargetView );
    SafeRelease( m_DepthStencilView );
    SafeRelease( m_DepthStencil );
    SafeRelease( m_SwapChain );
    SafeRelease( m_DeviceContext );

    // Update presentation parameters
    UpdatePresentParameters( lparam );

    // Recreate device and swap chain
    DXGI_SWAP_CHAIN_DESC scDesc;
    ZeroMemory( &scDesc, sizeof( scDesc ) );
    scDesc.BufferCount = 1;
    scDesc.BufferDesc.Width = gWindow->GetSize( ).x;
    scDesc.BufferDesc.Height = gWindow->GetSize( ).y;
    scDesc.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
    scDesc.BufferDesc.RefreshRate.Numerator = 60;
    scDesc.BufferDesc.RefreshRate.Denominator = 1;
    scDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
    scDesc.OutputWindow = gWindow->GetHwnd( );
    scDesc.SampleDesc.Count = 1;
    scDesc.SampleDesc.Quality = 0;
    scDesc.Windowed = TRUE;

    HRESULT result = D3D11CreateDeviceAndSwapChain(
        nullptr,
        D3D_DRIVER_TYPE_HARDWARE,
        nullptr,
        0,
        nullptr,
        0,
        D3D11_SDK_VERSION,
        &scDesc,
        &m_SwapChain,
        &m_Device,
        nullptr,
        &m_DeviceContext
    );

    if ( FAILED( result ) )
        throw std::runtime_error( "[GraphicsManager::ResetDevice] Failed to create DirectX 11 device and swap chain." );

    // Create render target view
    ID3D11Texture2D* backBuffer;
    result = m_SwapChain->GetBuffer( 0, __uuidof( ID3D11Texture2D ), ( void** ) &backBuffer );
    if ( FAILED( result ) )
        throw std::runtime_error( "[GraphicsManager::ResetDevice] Failed to get back buffer." );

    result = m_Device->CreateRenderTargetView( backBuffer, nullptr, &m_RenderTargetView );
    if ( FAILED( result ) )
        throw std::runtime_error( "[GraphicsManager::ResetDevice] Failed to create render target view." );

    backBuffer->Release( );

    D3D11_TEXTURE2D_DESC depthStencilDesc;
    ZeroMemory( &depthStencilDesc, sizeof( depthStencilDesc ) );
    depthStencilDesc.Width = gWindow->GetSize( ).x;
    depthStencilDesc.Height = gWindow->GetSize( ).y;
    depthStencilDesc.MipLevels = 1;
    depthStencilDesc.ArraySize = 1;
    depthStencilDesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
    depthStencilDesc.SampleDesc.Count = 1;
    depthStencilDesc.SampleDesc.Quality = 0;
    depthStencilDesc.Usage = D3D11_USAGE_DEFAULT;
    depthStencilDesc.BindFlags = D3D11_BIND_DEPTH_STENCIL;
    depthStencilDesc.CPUAccessFlags = 0;
    depthStencilDesc.MiscFlags = 0;

    result = m_Device->CreateTexture2D( &depthStencilDesc, nullptr, &m_DepthStencil );
    if ( FAILED( result ) )
        throw std::runtime_error( "[GraphicsManager::ResetDevice] Failed to create depth stencil buffer." );

    result = m_Device->CreateDepthStencilView( m_DepthStencil, nullptr, &m_DepthStencilView );
    if ( FAILED( result ) )
        throw std::runtime_error( "[GraphicsManager::ResetDevice] Failed to create depth stencil view." );

    m_DeviceContext->OMSetRenderTargets( 1, &m_RenderTargetView, m_DepthStencilView );

    D3D11_VIEWPORT viewport;
    viewport.Width = static_cast< float >( gWindow->GetSize( ).x );
    viewport.Height = static_cast< float >( gWindow->GetSize( ).y );
    viewport.MinDepth = 0.0f;
    viewport.MaxDepth = 1.0f;
    viewport.TopLeftX = 0;
    viewport.TopLeftY = 0;

    m_DeviceContext->RSSetViewports( 1, &viewport );

    UpdateRenderStates( m_DeviceContext );

    gBuffer->Init( );
}
void cGraphics::Release( ) {
    SafeRelease( m_VertexBuffer );
    SafeRelease( m_IndexBuffer );
    SafeRelease( m_RenderTargetView );
    SafeRelease( m_DepthStencilView );
    SafeRelease( m_DepthStencil );
    SafeRelease( m_SwapChain );
    SafeRelease( m_DeviceContext );
    SafeRelease( m_Device );
}

void cGraphics::Draw( ) {
    if ( !IsDeviceValid( ) )
        return;

    // Clear the render target and depth stencil buffer
    m_DeviceContext->ClearRenderTargetView( m_RenderTargetView, reinterpret_cast< const float* >( &m_ClearColor ) );
    m_DeviceContext->ClearDepthStencilView( m_DepthStencilView, D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0 );

    // Optionally, set other render states here

    // Set the viewport
    D3D11_VIEWPORT viewport;
    viewport.Width = static_cast< float >( gWindow->GetSize( ).x );
    viewport.Height = static_cast< float >( gWindow->GetSize( ).y );
    viewport.MinDepth = 0.0f;
    viewport.MaxDepth = 1.0f;
    viewport.TopLeftX = 0;
    viewport.TopLeftY = 0;

    m_DeviceContext->RSSetViewports( 1, &viewport );

    // Begin drawing
    m_DeviceContext->IASetPrimitiveTopology( D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST );

    // Render text
    auto InfoString = std::vformat(
            "{} FPS\n{} COMMANDS\n{} VERTICES\n{} INDICES",
            std::make_format_args(
                gContext->GetFrameRate( ),
                gBuffer->GetCommandsCount( ),
                gBuffer->GetVerticesCount( ),
                gBuffer->GetIndicesCount( )
            )
    );

    gBuffer->Text( gBuffer->GetDefaultFont( ), InfoString, Vec2<int16_t>( 5, 5 ), Color( 160, 217, 255, 255 ) );

    RenderDrawData( );

    m_SwapChain->Present( 0, 0 );
}

void cGraphics::UpdatePresentParameters( LPARAM lparam ) {
    DXGI_MODE_DESC modeDesc;
    ZeroMemory( &modeDesc, sizeof( modeDesc ) );
    modeDesc.Width = LOWORD( lparam );
    modeDesc.Height = HIWORD( lparam );
    modeDesc.RefreshRate.Numerator = 60;
    modeDesc.RefreshRate.Denominator = 1;
    modeDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
    modeDesc.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;
    modeDesc.Scaling = DXGI_MODE_SCALING_UNSPECIFIED;

    m_SwapChainDesc.BufferDesc = modeDesc;
    m_SwapChainDesc.SampleDesc.Count = 1;
    m_SwapChainDesc.SampleDesc.Quality = 0;
    m_SwapChainDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
    m_SwapChainDesc.BufferCount = 1;
    m_SwapChainDesc.OutputWindow = gWindow->GetHwnd( );
    m_SwapChainDesc.Windowed = TRUE;
    m_SwapChainDesc.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;
    m_SwapChainDesc.Flags = 0;
}

void cGraphics::UpdateRenderStates( ID3D11DeviceContext* device ) {
    D3D11_RASTERIZER_DESC rasterDesc;
    ZeroMemory( &rasterDesc, sizeof( rasterDesc ) );
    rasterDesc.FillMode = D3D11_FILL_SOLID;
    rasterDesc.CullMode = D3D11_CULL_NONE;
    rasterDesc.DepthClipEnable = TRUE;
    rasterDesc.MultisampleEnable = TRUE;
    rasterDesc.AntialiasedLineEnable = TRUE;
    ID3D11RasterizerState* rasterState;
    m_Device->CreateRasterizerState( &rasterDesc, &rasterState );
    m_DeviceContext->RSSetState( rasterState );

    // Set blend state
    D3D11_BLEND_DESC blendDesc;
    ZeroMemory( &blendDesc, sizeof( blendDesc ) );
    blendDesc.RenderTarget[ 0 ].BlendEnable = TRUE;
    blendDesc.RenderTarget[ 0 ].SrcBlend = D3D11_BLEND_SRC_ALPHA;
    blendDesc.RenderTarget[ 0 ].DestBlend = D3D11_BLEND_INV_SRC_ALPHA;
    blendDesc.RenderTarget[ 0 ].BlendOp = D3D11_BLEND_OP_ADD;
    blendDesc.RenderTarget[ 0 ].SrcBlendAlpha = D3D11_BLEND_ONE;
    blendDesc.RenderTarget[ 0 ].DestBlendAlpha = D3D11_BLEND_INV_SRC_ALPHA;
    blendDesc.RenderTarget[ 0 ].BlendOpAlpha = D3D11_BLEND_OP_ADD;
    blendDesc.RenderTarget[ 0 ].RenderTargetWriteMask = D3D11_COLOR_WRITE_ENABLE_ALL;
    ID3D11BlendState* blendState;
    m_Device->CreateBlendState( &blendDesc, &blendState );
    float blendFactor[] = { 0.0f, 0.0f, 0.0f, 0.0f };
    UINT sampleMask = 0xffffffff;
    m_DeviceContext->OMSetBlendState( blendState, blendFactor, sampleMask );

    D3D11_DEPTH_STENCIL_DESC depthStencilDesc;
    ZeroMemory( &depthStencilDesc, sizeof( depthStencilDesc ) );
    depthStencilDesc.DepthEnable = FALSE;
    depthStencilDesc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;
    depthStencilDesc.DepthFunc = D3D11_COMPARISON_LESS;
    depthStencilDesc.StencilEnable = FALSE;
    depthStencilDesc.StencilReadMask = D3D11_DEFAULT_STENCIL_READ_MASK;
    depthStencilDesc.StencilWriteMask = D3D11_DEFAULT_STENCIL_WRITE_MASK;
    depthStencilDesc.FrontFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
    depthStencilDesc.FrontFace.StencilDepthFailOp = D3D11_STENCIL_OP_KEEP;
    depthStencilDesc.FrontFace.StencilPassOp = D3D11_STENCIL_OP_KEEP;
    depthStencilDesc.FrontFace.StencilFunc = D3D11_COMPARISON_ALWAYS;
    depthStencilDesc.BackFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
    depthStencilDesc.BackFace.StencilDepthFailOp = D3D11_STENCIL_OP_KEEP;
    depthStencilDesc.BackFace.StencilPassOp = D3D11_STENCIL_OP_KEEP;
    depthStencilDesc.BackFace.StencilFunc = D3D11_COMPARISON_ALWAYS;
    ID3D11DepthStencilState* depthStencilState;
    m_Device->CreateDepthStencilState( &depthStencilDesc, &depthStencilState );
    m_DeviceContext->OMSetDepthStencilState( depthStencilState, 0 );
}

void cGraphics::RenderDrawData( ) {
    auto drawCommands = gBuffer->GetDrawCommands( );
    gBuffer->BuildDrawCommands( gBuffer->GetDrawCommands( ) );
    auto drawCommand = gBuffer->GetDrawCommand( );

    // Create or resize vertex buffer
    if ( !m_VertexBuffer || drawCommand.VerticesCount * sizeof( Vertex ) > m_VertexBufferSize ) {
        SafeRelease( m_VertexBuffer );

        m_VertexBufferSize = drawCommand.VerticesCount + 5000;

        D3D11_BUFFER_DESC vertexBufferDesc;
        ZeroMemory( &vertexBufferDesc, sizeof( vertexBufferDesc ) );
        vertexBufferDesc.Usage = D3D11_USAGE_DYNAMIC;
        vertexBufferDesc.ByteWidth = m_VertexBufferSize * sizeof( Vertex );
        vertexBufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
        vertexBufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;

        if ( m_Device->CreateBuffer( &vertexBufferDesc, nullptr, &m_VertexBuffer ) < 0 )
            throw std::runtime_error( "[cGraphics::RenderDrawData] CreateVertexBuffer Failed?" );
    }

    // Create or resize index buffer
    if ( !m_IndexBuffer || drawCommand.IndicesCount * sizeof( uint32_t ) > m_IndexBufferSize ) {
        SafeRelease( m_IndexBuffer );

        m_IndexBufferSize = drawCommand.IndicesCount + 10000;

        D3D11_BUFFER_DESC indexBufferDesc;
        ZeroMemory( &indexBufferDesc, sizeof( indexBufferDesc ) );
        indexBufferDesc.Usage = D3D11_USAGE_DYNAMIC;
        indexBufferDesc.ByteWidth = m_IndexBufferSize * sizeof( uint32_t );
        indexBufferDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
        indexBufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;

        if ( m_Device->CreateBuffer( &indexBufferDesc, nullptr, &m_IndexBuffer ) < 0 )
            throw std::runtime_error( "[cGraphics::RenderDrawData] CreateIndexBuffer Failed?" );
    }

    // Map and copy vertex data
    D3D11_MAPPED_SUBRESOURCE mappedResource;
    if ( m_DeviceContext->Map( m_VertexBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource ) < 0 )
        throw std::runtime_error( "[cGraphics::RenderDrawData] m_VertexBuffer->Map Failed?" );
    memcpy( mappedResource.pData, drawCommand.Vertices.data( ), drawCommand.VerticesCount * sizeof( Vertex ) );
    m_DeviceContext->Unmap( m_VertexBuffer, 0 );

    // Map and copy index data
    if ( m_DeviceContext->Map( m_IndexBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource ) < 0 )
        throw std::runtime_error( "[cGraphics::RenderDrawData] m_IndexBuffer->Map Failed?" );
    memcpy( mappedResource.pData, drawCommand.Indices.data( ), drawCommand.IndicesCount * sizeof( uint32_t ) );
    m_DeviceContext->Unmap( m_IndexBuffer, 0 );

    // Set vertex and index buffers
    UINT stride = sizeof( Vertex );
    UINT offset = 0;
    m_DeviceContext->IASetVertexBuffers( 0, 1, &m_VertexBuffer, &stride, &offset );
    m_DeviceContext->IASetIndexBuffer( m_IndexBuffer, DXGI_FORMAT_R32_UINT, 0 );
    m_DeviceContext->IASetPrimitiveTopology( D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST );

    // Update render states
    UpdateRenderStates( m_DeviceContext );

    // Bind shader resource views and draw commands
    std::vector<ID3D11ShaderResourceView*> SRVs;
    for ( const auto& command : drawCommands ) {
        RECT scissorRect;
        scissorRect.left = command.Resources.ClipStack.back( ).left;
        scissorRect.top = command.Resources.ClipStack.back( ).top;
        scissorRect.right = command.Resources.ClipStack.back( ).right;
        scissorRect.bottom = command.Resources.ClipStack.back( ).bottom;
        m_DeviceContext->RSSetScissorRects( 1, &scissorRect );

        ID3D11ShaderResourceView* srv = nullptr;
        ID3D11Texture2D* texture = command.Resources.TextureStack.back( );
        if ( texture ) {
            D3D11_TEXTURE2D_DESC desc;
            texture->GetDesc( &desc );

            D3D11_SHADER_RESOURCE_VIEW_DESC srvDesc = {};
            srvDesc.Format = desc.Format;
            srvDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
            srvDesc.Texture2D.MostDetailedMip = 0;
            srvDesc.Texture2D.MipLevels = desc.MipLevels;

            if ( m_Device->CreateShaderResourceView( texture, &srvDesc, &srv ) < 0 )
                throw std::runtime_error( "[cGraphics::RenderDrawData] CreateShaderResourceView Failed?" );
            SRVs.push_back( srv );
        }

        m_DeviceContext->DrawIndexed( command.IndicesCount, 0, 0 );
    }

    // Bind shader resource views to pixel shader
    m_DeviceContext->PSSetShaderResources( 0, static_cast< UINT >( SRVs.size( ) ), SRVs.data( ) );

    // Release shader resource views
    for ( ID3D11ShaderResourceView* srv : SRVs ) {
        if ( srv ) {
            srv->Release( );
        }
    }

    gBuffer->ClearCommands( );
}


inline bool cGraphics::IsDeviceValid( ) {
    return m_Device != nullptr;
}

void cGraphics::CreateFontFromName( Font* font, const char* font_name, int16_t size, int16_t weight, int16_t padding, bool antialiasing ) {
    std::string fontPath = GetFontPath( font_name );
    font->Path = std::string( fontPath.begin( ), fontPath.end( ) );
    font->Padding = padding;
    font->Size = size;

    FT_Library library;
    FT_Face face;

    if ( FT_Init_FreeType( &library ) != FT_Err_Ok ) {
        throw std::runtime_error( "[cGraphics::CreateFontFromName] FT_Init_FreeType Failed?" );
        return;
    }

    if ( FT_New_Face( library, font->Path.c_str( ), 0, &face ) ) {
        throw std::runtime_error( "[cGraphics::CreateFontFromName] FT_New_Face Failed?" );
        FT_Done_FreeType( library );
        return;
    }

    FT_Set_Char_Size( face, size * 64, 0, GetDpiForWindow( gWindow->GetHwnd( ) ), 0 );
    FT_Select_Charmap( face, FT_ENCODING_UNICODE );

    for ( unsigned char i = 0; i < 128; ++i ) {
        if ( FT_Load_Char( face, i, antialiasing ? FT_LOAD_RENDER : FT_LOAD_RENDER | FT_LOAD_TARGET_MONO ) ) {
            throw std::runtime_error( "[cGraphics::CreateFontFromName] FT_Load_Char Failed, Font Does Not Exist!" );
            continue;
        }

        int width = face->glyph->bitmap.width ? face->glyph->bitmap.width : 16;
        int height = face->glyph->bitmap.rows ? face->glyph->bitmap.rows : 16;

        D3D11_TEXTURE2D_DESC textureDesc;
        ZeroMemory( &textureDesc, sizeof( textureDesc ) );
        textureDesc.Width = width;
        textureDesc.Height = height;
        textureDesc.MipLevels = 1;
        textureDesc.ArraySize = 1;
        textureDesc.Format = DXGI_FORMAT_A8_UNORM;
        textureDesc.SampleDesc.Count = 1;
        textureDesc.SampleDesc.Quality = 0;
        textureDesc.Usage = D3D11_USAGE_DYNAMIC;
        textureDesc.BindFlags = D3D11_BIND_SHADER_RESOURCE;
        textureDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;

        ID3D11Texture2D* texture;
        if ( m_Device->CreateTexture2D( &textureDesc, nullptr, &texture ) != S_OK ) {
            throw std::runtime_error( "[cGraphics::CreateFontFromName] CreateTexture Failed?" );
            continue;
        }

        D3D11_MAPPED_SUBRESOURCE mappedResource;
        if ( m_DeviceContext->Map( texture, 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource ) != S_OK ) {
            throw std::runtime_error( "[cGraphics::CreateFontFromName] Map Failed?" );
            continue;
        }

        UCHAR* source = face->glyph->bitmap.buffer;
        UCHAR* destination = reinterpret_cast< UCHAR* >( mappedResource.pData );
        if ( source && destination ) {
            switch ( face->glyph->bitmap.pixel_mode ) {
            case FT_PIXEL_MODE_MONO:
                for ( size_t y = 0; y < height; ++y, source += face->glyph->bitmap.pitch, destination += mappedResource.RowPitch ) {
                    int8_t bits = 0;
                    const uint8_t* bitsPtr = source;

                    for ( size_t x = 0; x < width; ++x, bits <<= 1 ) {
                        if ( ( x & 7 ) == 0 )
                            bits = *bitsPtr++;

                        destination[ x ] = ( bits & 0x80 ) ? 255 : 0;
                    }
                }
                break;

            case FT_PIXEL_MODE_GRAY:
                for ( size_t j = 0; j < height; ++j ) {
                    memcpy( destination, source, width );

                    source += face->glyph->bitmap.pitch;
                    destination += mappedResource.RowPitch;
                }
                break;
            }
        }

        m_DeviceContext->Unmap( texture, 0 );

        font->CharSet[ i ].Texture = texture;
        font->CharSet[ i ].Size = { width, height };
        font->CharSet[ i ].Bearing = { static_cast< int32_t >( face->glyph->bitmap_left ), static_cast< int32_t >( face->glyph->bitmap_top ) };
        font->CharSet[ i ].Advance = face->glyph->Advance.x;
    }

    FT_Done_Face( face );
    FT_Done_FreeType( library );
}

inline std::string cGraphics::GetFontPath( const char* font_name ) {
    HKEY key;

    if ( RegOpenKeyExA( HKEY_LOCAL_MACHINE, "Software\\Microsoft\\Windows NT\\CurrentVersion\\Fonts", 0, KEY_READ, &key ) != ERROR_SUCCESS )
        std::printf( "[ Buffer ] get_path failed ( failed to open registry )" );

    std::string str_path;
    char str_buffer[ MAX_PATH ];

    for ( auto i = 0;; i++ ) {
        DWORD buf_size = MAX_PATH;
        memset( str_buffer, 0, MAX_PATH );

        if ( RegEnumValueA( key, i, str_buffer, &buf_size, nullptr, nullptr, nullptr, nullptr ) != ERROR_SUCCESS )
            std::printf( "[ Buffer ] get_path failed ( invalid font )" );

        if ( std::string( str_buffer ).find( font_name ) != std::string::npos ) {
            buf_size = MAX_PATH;
            RegQueryValueExA( key, str_buffer, nullptr, nullptr, reinterpret_cast< LPBYTE >( str_buffer ), &buf_size );
            str_path = str_buffer;
            break;
        }
    }

    memset( str_buffer, 0, MAX_PATH );
    SHGetFolderPathA( nullptr, CSIDL_FONTS, nullptr, 0, str_buffer );

    return std::string( str_buffer ) + '\\' + str_path;
}

template <typename type>
inline void cGraphics::SafeRelease( type*& obj ) {
    if ( obj ) {
        obj->Release( );
        obj = nullptr;
    }
}