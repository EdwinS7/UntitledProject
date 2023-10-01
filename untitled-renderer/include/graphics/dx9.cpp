#include "dx9.hpp"

#ifdef UNTITLED_USE_DX9

bool c_gfx::initialize( HWND hwnd, const bool device_only ) {
    if ( !device_only ) {
        m_hwnd = hwnd;
        m_d3d = Direct3DCreate9( D3D_SDK_VERSION );

        m_parameters.Windowed = TRUE;
        m_parameters.SwapEffect = D3DSWAPEFFECT_DISCARD;
        m_parameters.BackBufferFormat = D3DFMT_UNKNOWN;
        m_parameters.EnableAutoDepthStencil = TRUE;
        m_parameters.AutoDepthStencilFormat = D3DFMT_D16;
        m_parameters.PresentationInterval = D3DPRESENT_INTERVAL_IMMEDIATE;

        /*m_parameters.MultiSampleType = D3DMULTISAMPLE_4_SAMPLES;
        m_parameters.MultiSampleQuality = 0;*/
    }

    if ( m_d3d->CreateDevice( D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, m_hwnd, D3DCREATE_HARDWARE_VERTEXPROCESSING, &m_parameters, &m_device ) < D3D_OK )
        std::printf("create_device failed");

    set_render_states( m_device );

    return true;
}

void c_gfx::set_render_states( IDirect3DDevice9* device ) {
    device->SetRenderState( D3DRS_FILLMODE, D3DFILL_SOLID );
    device->SetRenderState( D3DRS_SHADEMODE, D3DSHADE_GOURAUD );
    device->SetRenderState( D3DRS_ZWRITEENABLE, FALSE );
    device->SetRenderState( D3DRS_ALPHATESTENABLE, FALSE );
    device->SetRenderState( D3DRS_CULLMODE, D3DCULL_NONE );
    device->SetRenderState( D3DRS_ZENABLE, FALSE );
    device->SetRenderState( D3DRS_ALPHABLENDENABLE, TRUE );
    device->SetRenderState( D3DRS_BLENDOP, D3DBLENDOP_ADD );
    device->SetRenderState( D3DRS_SRCBLEND, D3DBLEND_SRCALPHA );
    device->SetRenderState( D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA );
    device->SetRenderState( D3DRS_SEPARATEALPHABLENDENABLE, TRUE );
    device->SetRenderState( D3DRS_SRCBLENDALPHA, D3DBLEND_ONE );
    device->SetRenderState( D3DRS_DESTBLENDALPHA, D3DBLEND_INVSRCALPHA );
    device->SetRenderState( D3DRS_SCISSORTESTENABLE, TRUE );
    device->SetRenderState( D3DRS_FOGENABLE, FALSE );
    device->SetRenderState( D3DRS_RANGEFOGENABLE, FALSE );
    device->SetRenderState( D3DRS_SPECULARENABLE, FALSE );
    device->SetRenderState( D3DRS_STENCILENABLE, FALSE );
    device->SetRenderState( D3DRS_CLIPPING, TRUE );
    device->SetRenderState( D3DRS_LIGHTING, FALSE );
    device->SetRenderState( D3DRS_SRGBWRITEENABLE, FALSE );
    device->SetRenderState( D3DRS_MULTISAMPLEANTIALIAS, TRUE );
    device->SetRenderState( D3DRS_VERTEXBLEND, FALSE );

    device->SetTextureStageState( 0, D3DTSS_COLOROP, D3DTOP_SELECTARG2 );
    device->SetTextureStageState( 0, D3DTSS_COLORARG1, D3DTA_TEXTURE );
    device->SetTextureStageState( 0, D3DTSS_COLORARG2, D3DTA_DIFFUSE );
    device->SetTextureStageState( 0, D3DTSS_ALPHAOP, D3DTOP_MODULATE );
    device->SetTextureStageState( 0, D3DTSS_ALPHAARG1, D3DTA_TEXTURE );
    device->SetTextureStageState( 0, D3DTSS_ALPHAARG2, D3DTA_DIFFUSE );
    device->SetTextureStageState( 1, D3DTSS_COLOROP, D3DTOP_DISABLE );
    device->SetTextureStageState( 1, D3DTSS_ALPHAOP, D3DTOP_DISABLE );

    device->SetSamplerState( 0, D3DSAMP_MIPFILTER, D3DTEXF_NONE );
    device->SetSamplerState( 0, D3DSAMP_MINFILTER, D3DTEXF_LINEAR );
    device->SetSamplerState( 0, D3DSAMP_MAGFILTER, D3DTEXF_LINEAR );

    device->SetFVF( D3DFVF_XYZRHW | D3DFVF_DIFFUSE | D3DFVF_TEX1 );
    device->SetTexture( NULL, NULL );
    device->SetVertexShader( nullptr );
    device->SetPixelShader( nullptr );
}

void c_gfx::render_draw_data( ) {
    set_render_states( m_device );

    auto draw_commands = g_buffer->get_draw_commands( );
    g_buffer->build_draw_commands( g_buffer->get_draw_commands() );
    auto draw_command = g_buffer->get_draw_command( );

    if ( !m_vertex_buffer || draw_command.vertices_count * sizeof( vertex_t ) > m_vertex_buffer_size ) {
        if ( m_vertex_buffer ) {
            m_vertex_buffer->Release( );
            m_vertex_buffer = nullptr;
        }

        m_vertex_buffer_size = draw_command.vertices_count + 5000;

        if ( m_device->CreateVertexBuffer( m_vertex_buffer_size * sizeof( vertex_t ), 
            D3DUSAGE_DYNAMIC | D3DUSAGE_WRITEONLY, D3DFVF_XYZRHW | D3DFVF_DIFFUSE | D3DFVF_TEX1,  D3DPOOL_DEFAULT, &m_vertex_buffer, nullptr ) < D3D_OK )
            std::printf( "render_draw_data failed (CreateVertexBuffer)" );
    }

    if ( !m_index_buffer || draw_command.indices_count * sizeof( std::int32_t ) > m_index_buffer_size ) {
        if ( m_index_buffer ) {
            m_index_buffer->Release( );
            m_index_buffer = nullptr;
        }

        m_index_buffer_size = draw_command.indices_count + 10000;

        if ( m_device->CreateIndexBuffer( m_index_buffer_size * sizeof( std::int32_t ), 
            D3DUSAGE_DYNAMIC |D3DUSAGE_WRITEONLY, D3DFMT_INDEX32, D3DPOOL_DEFAULT, &m_index_buffer, nullptr ) < D3D_OK )
            std::printf( "render_draw_data failed (CreateIndexBuffer)" );
    }

    vertex_t* vertex_data{ };
    int32_t* index_data{ };

    if ( m_vertex_buffer->Lock( 0, ( int ) ( draw_command.vertices_count * sizeof( vertex_t ) ), ( void** ) &vertex_data, D3DLOCK_DISCARD ) < 0 )
        std::printf("render_draw_data error (vtx_buffer->Lock)");

    if ( m_index_buffer->Lock( 0, ( int ) ( draw_command.indices_count * sizeof( std::int32_t ) ), ( void** ) &index_data, D3DLOCK_DISCARD ) < 0 )
        std::printf("render_draw_data error (idx_buffer->Lock)");

    memcpy( vertex_data, draw_command.vertices.data( ), draw_command.vertices_count * sizeof( vertex_t ) );
    memcpy( index_data, draw_command.indices.data( ), draw_command.indices_count * sizeof( int32_t ) );

    m_vertex_buffer->Unlock( );
    m_index_buffer->Unlock( );

    m_device->SetStreamSource( 0, m_vertex_buffer, 0, sizeof( vertex_t ) );
    m_device->SetIndices( m_index_buffer );

    int start_vertex = 0, 
        start_index = 0;

    for ( const draw_command_t& command : draw_commands ) {
        m_device->SetScissorRect( &command.command.clips.back( ) );
        m_device->SetTexture( 0, command.command.textures.back( ) );

        m_device->DrawIndexedPrimitive( D3DPRIMITIVETYPE(command.primitive), start_vertex, 0, command.vertices_count, start_index, command.indices_count / 3 );

        start_vertex += command.vertices_count;
        start_index += command.indices_count;
    }

    g_buffer->clear_commands( );
}

void c_gfx::reset( const LPARAM lParam ) {
    g_buffer->destroy_objects( );

    m_parameters.BackBufferWidth = LOWORD( lParam );
    m_parameters.BackBufferHeight = HIWORD( lParam );

    safe_release( m_vertex_buffer );
    safe_release( m_index_buffer );
    safe_release( m_device );

    initialize( m_hwnd, true );

    g_buffer->create_objects( );
}

void c_gfx::set_clear_color( const color_t clear_color ) {
    m_clear_color = clear_color;
}

void c_gfx::set_vsync( const bool state ) {
    g_buffer->destroy_objects( );

    m_parameters.PresentationInterval = state ? D3DPRESENT_INTERVAL_DEFAULT : D3DPRESENT_INTERVAL_IMMEDIATE;

    safe_release( m_vertex_buffer );
    safe_release( m_index_buffer );
    safe_release( m_device );

    initialize( m_hwnd, true );

    g_buffer->create_objects( );
}

bool c_gfx::valid( ) {
    return m_device != nullptr;
}

void c_gfx::draw( ) {
    if ( !valid( ) )
        return;

    m_device->Clear( 0, nullptr, D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER, m_clear_color.hex, 1.f, 0 );

    if ( m_device->BeginScene( ) >= 0 ) {
        render_draw_data( );
        m_device->EndScene( );
    }

    m_device->Present( nullptr, nullptr, nullptr, nullptr );
}

void c_gfx::release( ) {
    g_buffer->clear_commands( );

    safe_release( m_vertex_buffer );
    safe_release( m_index_buffer );
    safe_release( m_device );
    safe_release( m_d3d );
}

template <typename type>
void c_gfx::safe_release( type*& obj ) {
    if ( obj ) {
        obj->Release( );
        obj = nullptr;
    }
}

/* utilities */

void c_gfx::create_texture_from_bytes( texture* resource, const std::vector<BYTE> bytes, const vector2_t<int16_t> size ) {
    if ( D3DXCreateTextureFromFileInMemoryEx( m_device, bytes.data( ), bytes.size( ), size.x, size.y, D3DX_DEFAULT, NULL, D3DFMT_UNKNOWN, D3DPOOL_DEFAULT, D3DX_DEFAULT, D3DX_DEFAULT, NULL, NULL, NULL, resource ) != D3D_OK )
        std::printf( std::vformat( "[ graphics ] failed to load texture resource ( {} bytes )\n", std::make_format_args( bytes.size( ) ) ).c_str( ) );
}

void c_gfx::create_texture_from_file( texture* resource, const char* file_name ) {
    if ( D3DXCreateTextureFromFile( m_device, file_name, resource ) != D3D_OK )
        std::printf( std::vformat( "[ graphics ] failed to load texture resource ( {} )\n", std::make_format_args( file_name ) ).c_str( ) );
}

IDirect3DDevice9* c_gfx::get_device( ) {
    return m_device;
}

#endif