#include "DirectX9.hpp"

bool cGraphics::Init( HWND hwnd, const bool init ) {
    if ( init ) {
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
        throw std::runtime_error( "[cGraphics::Init] CreateDevice Failed?" );

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

    Init( m_Hwnd, false );

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

    // textured
    device->SetTextureStageState( 0, D3DTSS_COLOROP, D3DTOP_MODULATE );
    device->SetTextureStageState( 0, D3DTSS_COLORARG1, D3DTA_TEXTURE );
    device->SetTextureStageState( 0, D3DTSS_COLORARG2, D3DTA_DIFFUSE );
    device->SetTextureStageState( 0, D3DTSS_ALPHAOP, D3DTOP_MODULATE );
    device->SetTextureStageState( 0, D3DTSS_ALPHAARG1, D3DTA_TEXTURE );
    device->SetTextureStageState( 0, D3DTSS_ALPHAARG2, D3DTA_DIFFUSE );
    device->SetTextureStageState( 1, D3DTSS_COLOROP, D3DTOP_DISABLE );
    device->SetTextureStageState( 1, D3DTSS_ALPHAOP, D3DTOP_DISABLE );

    // non-textured
    device->SetTextureStageState( 1, D3DTSS_COLOROP, D3DTOP_SELECTARG2 );
    device->SetTextureStageState( 1, D3DTSS_COLORARG1, D3DTA_DIFFUSE );
    device->SetTextureStageState( 1, D3DTSS_COLORARG2, D3DTA_DIFFUSE );
    device->SetTextureStageState( 1, D3DTSS_ALPHAOP, D3DTOP_DISABLE );


    device->SetSamplerState( 0, D3DSAMP_MINFILTER, D3DTEXF_LINEAR );
    device->SetSamplerState( 0, D3DSAMP_MAGFILTER, D3DTEXF_LINEAR );

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
            throw std::runtime_error( "[cGraphics::RenderDrawData] CreateVertexBuffer Failed?" );
    }

    if ( !m_IndexBuffer || DrawCommand.IndicesCount * sizeof( std::int32_t ) > m_IndexBufferSize ) {
        if ( m_IndexBuffer ) {
            m_IndexBuffer->Release( );
            m_IndexBuffer = nullptr;
        }

        m_IndexBufferSize = DrawCommand.IndicesCount + 10000;

        if ( m_Device->CreateIndexBuffer( m_IndexBufferSize * sizeof( std::int32_t ),
            D3DUSAGE_DYNAMIC |D3DUSAGE_WRITEONLY, D3DFMT_INDEX32, D3DPOOL_DEFAULT, &m_IndexBuffer, nullptr ) < D3D_OK )
            throw std::runtime_error( "[cGraphics::RenderDrawData] CreateIndexBuffer Failed?" );
    }

    Vertex* vertex_data{ };
    int32_t* index_data{ };

    if ( m_VertexBuffer->Lock( 0, ( int ) ( DrawCommand.VerticesCount * sizeof( Vertex ) ), ( void** ) &vertex_data, D3DLOCK_DISCARD ) < 0 )
        throw std::runtime_error( "[cGraphics::RenderDrawData] m_VertexBuffer->Lock Failed?" );

    if ( m_IndexBuffer->Lock( 0, ( int ) ( DrawCommand.IndicesCount * sizeof( std::int32_t ) ), ( void** ) &index_data, D3DLOCK_DISCARD ) < 0 )
        throw std::runtime_error( "[cGraphics::RenderDrawData] m_IndexBuffer->Lock Failed?" );

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

    Init( m_Hwnd, false );

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

        gBuffer->Text(
            gBuffer->GetDefaultFont( ), display_info, Vec2<int16_t>( 5, 5 ), Color( 160, 217, 255, 255 )
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
        MessageBoxA(nullptr, "Graphics", std::vformat( "Failed To Create Texture From Bytes ({}kb)", std::make_format_args( Bytes->size( ) ) ).c_str( ), 0 );
}

void cGraphics::CreateTextureFromFile( IDirect3DTexture9** Texture, const char* FileName ) {
    if ( D3DXCreateTextureFromFile( m_Device, FileName, Texture ) != D3D_OK )
        MessageBoxA( nullptr, "Graphics", std::vformat( "Failed To Create Texture ({})", std::make_format_args( FileName ) ).c_str( ), 0 );
}

void cGraphics::CreateFontFromName( Font* font, const char* font_name, int16_t size, int16_t weight, int16_t padding, bool antialiasing ) {
    FT_Library lib;
    FT_Face face;

    font->Path = GetFontPath( font_name );
    font->Padding = padding;
    font->Size = size;

    if ( FT_Init_FreeType( &lib ) != FT_Err_Ok ) {
        throw std::runtime_error( "[cGraphics::CreateFontFromName] FT_Init_FreeType Failed?" );
        return;
    }

    if ( FT_New_Face( lib, font->Path.c_str( ), 0, &face ) ) {
        throw std::runtime_error( "[cGraphics::CreateFontFromName] FT_New_Face Failed?" );
        FT_Done_FreeType( lib );
        return;
    }

    FT_Set_Char_Size( face, size * 64, 0, GetDpiForWindow( gWindow->GetHwnd( ) ), 0 );
    FT_Select_Charmap( face, FT_ENCODING_UNICODE );

    for ( unsigned char i = 0; i < 128; ++i ) {
        if ( FT_Load_Char( face, i, antialiasing ? FT_LOAD_RENDER : FT_LOAD_RENDER | FT_LOAD_TARGET_MONO ) ) {
            throw std::runtime_error( "[cGraphics::CreateFontFromName] FT_Load_Char Failed, Font Does Not Exist!" );
            continue;
        }

        int32_t width = face->glyph->bitmap.width ? face->glyph->bitmap.width : 16;
        int32_t height = face->glyph->bitmap.rows ? face->glyph->bitmap.rows : 16;

        if ( gGraphics->GetDevice( )->CreateTexture( width, height, 1, D3DUSAGE_DYNAMIC, D3DFMT_A8, D3DPOOL_DEFAULT, &font->CharSet[ i ].Texture, NULL ) ) {
            throw std::runtime_error( "[cGraphics::CreateFontFromName] CreateTexture Failed?" );
            continue;
        }

        D3DLOCKED_RECT lockedRect;
        font->CharSet[ i ].Texture->LockRect( 0, &lockedRect, nullptr, D3DLOCK_DISCARD );

        UCHAR* source = face->glyph->bitmap.buffer;
        UCHAR* destination = reinterpret_cast< UCHAR* >( lockedRect.pBits );

        if ( source && destination ) {
            switch ( face->glyph->bitmap.pixel_mode ) {
            case FT_PIXEL_MODE_MONO:
                for ( int32_t y = 0; y < height; ++y, source += face->glyph->bitmap.pitch, destination += lockedRect.Pitch ) {
                    int8_t bits = 0;
                    const uint8_t* bitsPtr = source;

                    for ( int32_t x = 0; x < width; ++x, bits <<= 1 ) {
                        if ( ( x & 7 ) == 0 )
                            bits = *bitsPtr++;

                        destination[ x ] = ( bits & 0x80 ) ? 255 : 0;
                    }
                }
                break;

            case FT_PIXEL_MODE_GRAY:
                for ( int32_t j = 0; j < height; ++j ) {
                    memcpy( destination, source, width );

                    source += face->glyph->bitmap.pitch;
                    destination += lockedRect.Pitch;
                }
                break;
            }
        }

        font->CharSet[ i ].Texture->UnlockRect( 0 );

        font->CharSet[ i ].Size = { width, height };
        font->CharSet[ i ].Bearing = { static_cast< int32_t >( face->glyph->bitmap_left ), static_cast< int32_t >( face->glyph->bitmap_top ) };
        font->CharSet[ i ].Advance = face->glyph->Advance.x;
    }

    FT_Done_Face( face );
    FT_Done_FreeType( lib );
}