#include "DirectX9.hpp"

bool cGraphics::Init( HWND hwnd, const bool device_only ) {
    if ( !device_only ) {
        m_Hwnd = hwnd;
        m_Direct3D = Direct3DCreate9( D3D_SDK_VERSION );

        m_Parameters.Windowed = TRUE;
        m_Parameters.SwapEffect = D3DSWAPEFFECT_DISCARD;
        m_Parameters.BackBufferFormat = D3DFMT_UNKNOWN;
        m_Parameters.EnableAutoDepthStencil = TRUE;
        m_Parameters.AutoDepthStencilFormat = D3DFMT_D16;
        m_Parameters.PresentationInterval = D3DPRESENT_INTERVAL_IMMEDIATE;

        m_Parameters.MultiSampleType = D3DMULTISAMPLE_4_SAMPLES;
        m_Parameters.MultiSampleQuality = 0;
    }

    if ( m_Direct3D->CreateDevice( D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, m_Hwnd, D3DCREATE_MIXED_VERTEXPROCESSING, &m_Parameters, &m_Device ) < D3D_OK )
        std::printf( "[ Graphics ] CreateDevice Failed!" );

    UpdateRenderStates( m_Device );

    return true;
}

void cGraphics::Reset( const LPARAM lParam ) {
    gBuffer->Release( );

    m_Parameters.BackBufferWidth = LOWORD( lParam );
    m_Parameters.BackBufferHeight = HIWORD( lParam );

    SafeRelease( m_VertexBuffer );
    SafeRelease( m_IndexBuffer );
    SafeRelease( m_Device );

    Init( m_Hwnd, true );

    gBuffer->Init( );
}

void cGraphics::Release( ) {
    SafeRelease( m_VertexBuffer );
    SafeRelease( m_IndexBuffer );
    SafeRelease( m_Direct3D );
    SafeRelease( m_Device );
}

bool cGraphics::Valid( ) {
    return m_Device != nullptr;
}

#define VERTEX ( D3DFVF_XYZRHW | D3DFVF_DIFFUSE | D3DFVF_TEX1 )

void cGraphics::UpdateRenderStates( IDirect3DDevice9* device ) {
    device->SetRenderState( D3DRS_FILLMODE, D3DFILL_SOLID );
    device->SetRenderState( D3DRS_SHADEMODE, D3DSHADE_GOURAUD );
    device->SetRenderState( D3DRS_ZWRITEENABLE, FALSE );
    device->SetRenderState( D3DRS_ALPHATESTENABLE, FALSE );
    device->SetRenderState( D3DRS_CULLMODE, D3DCULL_NONE );
    device->SetRenderState( D3DRS_ALPHABLENDENABLE, TRUE );
    device->SetRenderState( D3DRS_BLENDOP, D3DBLENDOP_ADD );
    device->SetRenderState( D3DRS_SRCBLEND, D3DBLEND_SRCALPHA );
    device->SetRenderState( D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA );
    device->SetRenderState( D3DRS_SEPARATEALPHABLENDENABLE, TRUE );
    device->SetRenderState( D3DRS_SRCBLENDALPHA, D3DBLEND_ONE );
    device->SetRenderState( D3DRS_DESTBLENDALPHA, D3DBLEND_INVSRCALPHA );
    device->SetRenderState( D3DRS_SCISSORTESTENABLE, TRUE );
    device->SetRenderState( D3DRS_CLIPPING, TRUE );
    device->SetRenderState( D3DRS_LIGHTING, FALSE );
    device->SetRenderState( D3DRS_MULTISAMPLEANTIALIAS, TRUE );
    device->SetRenderState( D3DRS_VERTEXBLEND, FALSE );

    device->SetTextureStageState( 0, D3DTSS_COLOROP, D3DTOP_SELECTARG2 );
    device->SetTextureStageState( 0, D3DTSS_COLORARG1, D3DTA_TEXTURE );
    device->SetTextureStageState( 0, D3DTSS_COLORARG2, D3DTA_DIFFUSE );
    device->SetTextureStageState( 0, D3DTSS_ALPHAOP, D3DTOP_MODULATE );
    device->SetTextureStageState( 0, D3DTSS_ALPHAARG1, D3DTA_TEXTURE );
    device->SetTextureStageState( 0, D3DTSS_ALPHAARG2, D3DTA_DIFFUSE );
    device->SetTextureStageState( 1, D3DTSS_COLOROP, D3DTOP_SELECTARG1 );
    device->SetTextureStageState( 1, D3DTSS_COLORARG1, D3DTA_TEXTURE );
    device->SetTextureStageState( 1, D3DTSS_COLORARG2, D3DTA_DIFFUSE );
    device->SetTextureStageState( 1, D3DTSS_ALPHAOP, D3DTOP_MODULATE );
    device->SetTextureStageState( 1, D3DTSS_ALPHAARG1, D3DTA_TEXTURE );
    device->SetTextureStageState( 1, D3DTSS_ALPHAARG2, D3DTA_DIFFUSE );

    device->SetSamplerState( 0, D3DSAMP_MIPFILTER, D3DTEXF_NONE );
    device->SetSamplerState( 0, D3DSAMP_MINFILTER, D3DTEXF_LINEAR );
    device->SetSamplerState( 0, D3DSAMP_MAGFILTER, D3DTEXF_LINEAR );
    device->SetSamplerState( 1, D3DSAMP_MIPFILTER, D3DTEXF_NONE );
    device->SetSamplerState( 1, D3DSAMP_MINFILTER, D3DTEXF_LINEAR );
    device->SetSamplerState( 1, D3DSAMP_MAGFILTER, D3DTEXF_LINEAR );

    device->SetVertexShader( nullptr );
    device->SetPixelShader( nullptr );
    device->SetTexture( NULL, nullptr );
    device->SetFVF( VERTEX );
}

void cGraphics::RenderDrawData( ) {
    auto DrawCommands = gBuffer->GetDrawCommands( );
    gBuffer->BuildDrawCommands( gBuffer->GetDrawCommands() );
    auto DrawCommand = gBuffer->GetDrawCommand( );

    if ( !m_VertexBuffer || DrawCommand.VerticesCount * sizeof( Vertex ) > m_VertexBufferSize ) {
        if ( m_VertexBuffer ) {
            m_VertexBuffer->Release( );
            m_VertexBuffer = nullptr;
        }

        m_VertexBufferSize = DrawCommand.VerticesCount + 5000;

        if ( m_Device->CreateVertexBuffer( m_VertexBufferSize * sizeof( Vertex ), 
            D3DUSAGE_DYNAMIC | D3DUSAGE_WRITEONLY, VERTEX,  D3DPOOL_DEFAULT, &m_VertexBuffer, nullptr ) < D3D_OK )
            std::printf( "[ Graphics ] CreateVertexBuffer Failed!" );
    }

    if ( !m_IndexBuffer || DrawCommand.IndicesCount * sizeof( std::int32_t ) > m_IndexBufferSize ) {
        if ( m_IndexBuffer ) {
            m_IndexBuffer->Release( );
            m_IndexBuffer = nullptr;
        }

        m_IndexBufferSize = DrawCommand.IndicesCount + 10000;

        if ( m_Device->CreateIndexBuffer( m_IndexBufferSize * sizeof( std::int32_t ),
            D3DUSAGE_DYNAMIC |D3DUSAGE_WRITEONLY, D3DFMT_INDEX32, D3DPOOL_DEFAULT, &m_IndexBuffer, nullptr ) < D3D_OK )
            std::printf( "[ Graphics ] CreateIndexBuffer Failed!" );
    }

    Vertex* vertex_data{ };
    int32_t* index_data{ };

    if ( m_VertexBuffer->Lock( 0, ( int ) ( DrawCommand.VerticesCount * sizeof( Vertex ) ), ( void** ) &vertex_data, D3DLOCK_DISCARD ) < 0 )
        std::printf( "[ Graphics ] m_vertex_buffer->Lock Failed!" );

    if ( m_IndexBuffer->Lock( 0, ( int ) ( DrawCommand.IndicesCount * sizeof( std::int32_t ) ), ( void** ) &index_data, D3DLOCK_DISCARD ) < 0 )
        std::printf( "[ Graphics ] m_index_buffer->Lock Failed!" );

    memcpy( vertex_data, DrawCommand.Vertices.data( ), DrawCommand.VerticesCount * sizeof( Vertex ) );
    memcpy( index_data, DrawCommand.Indices.data( ), DrawCommand.IndicesCount * sizeof( int32_t ) );

    m_VertexBuffer->Unlock( );
    m_IndexBuffer->Unlock( );

    m_Device->SetStreamSource( 0, m_VertexBuffer, 0, sizeof( Vertex ) );
    m_Device->SetIndices( m_IndexBuffer );
    m_Device->SetFVF( VERTEX );

    UpdateRenderStates( m_Device );

    int StartVertex = 0, 
        StartIndex = 0;

    for ( const auto& Command : DrawCommands ) {
        m_Device->SetScissorRect( &Command.Resources.Clips.back( ) );
        m_Device->SetTexture( 0, Command.Resources.Textures.back( ) );

        m_Device->DrawIndexedPrimitive( D3DPRIMITIVETYPE( Command.Primitive), StartVertex, 0, Command.VerticesCount, StartIndex, Command.IndicesCount / 3 );

        StartVertex += Command.VerticesCount;
        StartIndex += Command.IndicesCount;
    }

    gBuffer->ClearCommands( );
}

void cGraphics::SetVSync( const bool state ) {
    m_Parameters.PresentationInterval = state ? D3DPRESENT_INTERVAL_DEFAULT : D3DPRESENT_INTERVAL_IMMEDIATE;
    
    gBuffer->Release( );

    SafeRelease( m_VertexBuffer );
    SafeRelease( m_IndexBuffer );
    SafeRelease( m_Device );

    Init( m_Hwnd, true );

    gBuffer->Init( );
}

void cGraphics::SetClearColor( const Color clear_color ) {
    m_ClearColor = clear_color;
}

void cGraphics::Draw( ) {
    if ( !Valid( ) )
        return;

    m_Device->Clear( 0, nullptr, D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER, m_ClearColor.Hex, 1.f, 0 );

    if ( m_Device->BeginScene( ) >= 0 ) {
        auto display_info = std::vformat(
            "{} FPS\n{} COMMANDS\n{} VERTICES\n{} INDICES",

            std::make_format_args(
                gContext->GetFrameRate( ),
                gBuffer->GetCommandsCount( ),
                gBuffer->GetVerticesCount( ),
                gBuffer->GetIndicesCount( )
            )
        );

        gBuffer->String(
            &gBuffer->Fonts.Default, display_info, Vec2<int16_t>( 5, 5 ), Color( 160, 217, 255, 255 )
        );

        RenderDrawData( );
        m_Device->EndScene( );
    }

    m_Device->Present( nullptr, nullptr, nullptr, nullptr );
}

template <typename type>
void cGraphics::SafeRelease( type*& obj ) {
    if ( obj ) {
        obj->Release( );
        obj = nullptr;
    }
}

void cGraphics::CreateTextureFromBytes( IDirect3DTexture9* Texture, const std::vector<BYTE>* Bytes, const Vec2<int16_t> Size ) {
    if ( D3DXCreateTextureFromFileInMemoryEx( m_Device, Bytes->data( ), Bytes->size( ), Size.x, Size.y, D3DX_DEFAULT, NULL, D3DFMT_UNKNOWN, D3DPOOL_DEFAULT, D3DX_DEFAULT, D3DX_DEFAULT, NULL, NULL, NULL, &Texture ) != D3D_OK )
        std::printf( std::vformat( "[ Graphics ] Failed to create resource from bytes ( {}kb )\n", std::make_format_args( Bytes->size( ) ) ).c_str( ) );
    else
        std::printf( std::vformat( "[ Graphics ] Created resource from bytes ( {}kb )\n", std::make_format_args( Bytes->size( ) ) ).c_str( ) );

}

void cGraphics::CreateTextureFromFile( IDirect3DTexture9** Texture, const char* FileName ) {
    if ( D3DXCreateTextureFromFile( m_Device, FileName, Texture ) != D3D_OK )
        std::printf( std::vformat( "[ Graphics ] Failed to create resource ( name: {} )\n", std::make_format_args( FileName ) ).c_str( ) );
    else
        std::printf( std::vformat( "[ Graphics ] Created resource ( name: {} )\n", std::make_format_args( FileName ) ).c_str( ) );
}