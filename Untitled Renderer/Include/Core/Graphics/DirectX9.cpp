#include "DirectX9.hpp"

void cGraphics::Init( HWND hwnd ) {
    m_Direct3D = Direct3DCreate9( D3D_SDK_VERSION );

    if ( !m_Direct3D )
		throw std::runtime_error( "[cGraphics::Init] Direct3DCreate9 Failed" );
    
    UpdatePresentationParameters( hwnd );

    if ( m_Direct3D->CreateDevice( D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, hwnd, D3DCREATE_HARDWARE_VERTEXPROCESSING, &m_Parameters, &m_Device ) < D3D_OK ) 
        throw std::runtime_error( "[cGraphics::Init] CreateDevice Failed" );

    UpdateRenderStates( m_Device );
}

void cGraphics::Release( ) {
    gBuffer->Release( );

    SafeRelease( m_VertexBuffer );
    SafeRelease( m_IndexBuffer );
    SafeRelease( m_StateBlock );
    SafeRelease( m_Direct3D );
    SafeRelease( m_Device );

    ReleaseTextures( );
    ReleaseFonts( );
}

void cGraphics::ResetDevice( HWND hwnd ) {
    gBuffer->Release( );

    SafeRelease( m_VertexBuffer );
    SafeRelease( m_IndexBuffer );
    SafeRelease( m_Device );

    ReleaseTextures( );
    ReleaseFonts( );

    UpdatePresentationParameters( hwnd );

    if ( m_Direct3D->CreateDevice( D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, hwnd, D3DCREATE_HARDWARE_VERTEXPROCESSING, &m_Parameters, &m_Device ) < D3D_OK ) {
        throw std::runtime_error( "[cGraphics::ResetDevice] CreateDevice Failed" );
    }

    UpdateRenderStates( m_Device );

    gBuffer->Init( );
}

void cGraphics::UpdatePresentationParameters( HWND hwnd ) {
	D3DDISPLAYMODE DisplayMode;
	m_Direct3D->GetAdapterDisplayMode( D3DADAPTER_DEFAULT, &DisplayMode );

	LONG_PTR Style = GetWindowLongPtr( hwnd, GWL_STYLE );
	bool Fullscreen = !( Style & WS_CAPTION ) && !( Style & WS_THICKFRAME );

	if ( Fullscreen ) {
		m_Parameters.Windowed = FALSE;
		m_Parameters.BackBufferWidth = DisplayMode.Width;
		m_Parameters.BackBufferHeight = DisplayMode.Height;
	}
	else {
		auto GetRect = Fullscreen ? GetWindowRect : GetClientRect;

		RECT rect{};
		GetRect( hwnd, &rect );

		m_Parameters.Windowed = TRUE;
		m_Parameters.BackBufferWidth = rect.right - rect.left;
		m_Parameters.BackBufferHeight = rect.bottom - rect.top;
	}

	m_Parameters.hDeviceWindow = hwnd;

	m_Parameters.SwapEffect = D3DSWAPEFFECT_DISCARD;

	m_Parameters.BackBufferCount = 1;
	m_Parameters.BackBufferFormat = D3DFMT_X8R8G8B8;

	m_Parameters.EnableAutoDepthStencil = TRUE;
	m_Parameters.AutoDepthStencilFormat = D3DFMT_D16;

	m_Parameters.PresentationInterval = D3DPRESENT_INTERVAL_IMMEDIATE;
}

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
    gBuffer->BuildDrawCommands( DrawCommands );

    auto DrawCommand = gBuffer->GetDrawCommand( );

    if ( !m_VertexBuffer || DrawCommand->VerticesCount * sizeof( Vertex ) > m_VertexBufferSize ) {
        if ( m_VertexBuffer ) {
            m_VertexBuffer->Release( );
            m_VertexBuffer = nullptr;
        }

        m_VertexBufferSize = DrawCommand->VerticesCount + 5000;

        if ( m_Device->CreateVertexBuffer( m_VertexBufferSize * sizeof( Vertex ), 
            D3DUSAGE_DYNAMIC | D3DUSAGE_WRITEONLY, VERTEX,  D3DPOOL_DEFAULT, &m_VertexBuffer, nullptr ) < D3D_OK )
            throw std::runtime_error( "[cGraphics::RenderDrawData] CreateVertexBuffer Failed" );
    }

    if ( !m_IndexBuffer || DrawCommand->IndicesCount * sizeof( std::int32_t ) > m_IndexBufferSize ) {
        if ( m_IndexBuffer ) {
            m_IndexBuffer->Release( );
            m_IndexBuffer = nullptr;
        }

        m_IndexBufferSize = DrawCommand->IndicesCount + 10000;

        if ( m_Device->CreateIndexBuffer( m_IndexBufferSize * sizeof( std::int32_t ),
            D3DUSAGE_DYNAMIC |D3DUSAGE_WRITEONLY, D3DFMT_INDEX32, D3DPOOL_DEFAULT, &m_IndexBuffer, nullptr ) < D3D_OK )
            throw std::runtime_error( "[cGraphics::RenderDrawData] CreateIndexBuffer Failed" );
    }

    if ( m_Device->CreateStateBlock( D3DSBT_ALL, &m_StateBlock ) < 0 )
        throw std::runtime_error( "[cGraphics::RenderDrawData] CreateStateBlock Failed" );

    if ( m_StateBlock->Capture( ) < 0 ) {
        m_StateBlock->Release( );
    }

    D3DMATRIX LastWorld, LastView, LastProjection;
    m_Device->GetTransform( D3DTS_WORLD, &LastWorld );
    m_Device->GetTransform( D3DTS_VIEW, &LastView );
    m_Device->GetTransform( D3DTS_PROJECTION, &LastProjection );

    Vertex* vertex_data{ };
    int32_t* index_data{ };

    if ( m_VertexBuffer->Lock( 0, ( int ) ( DrawCommand->VerticesCount * sizeof( Vertex ) ), ( void** ) &vertex_data, D3DLOCK_DISCARD ) < 0 )
        throw std::runtime_error( "[cGraphics::RenderDrawData] m_VertexBuffer->Lock Failed" );

    if ( m_IndexBuffer->Lock( 0, ( int ) ( DrawCommand->IndicesCount * sizeof( std::int32_t ) ), ( void** ) &index_data, D3DLOCK_DISCARD ) < 0 )
        throw std::runtime_error( "[cGraphics::RenderDrawData] m_IndexBuffer->Lock Failed" );

    memcpy( vertex_data, DrawCommand->Vertices.data( ), DrawCommand->VerticesCount * sizeof( Vertex ) );
    memcpy( index_data, DrawCommand->Indices.data( ), DrawCommand->IndicesCount * sizeof( int32_t ) );

    m_VertexBuffer->Unlock( );
    m_IndexBuffer->Unlock( );

    m_Device->SetStreamSource( 0, m_VertexBuffer, 0, sizeof( Vertex ) );
    m_Device->SetIndices( m_IndexBuffer );
    m_Device->SetFVF( VERTEX );

    UpdateRenderStates( m_Device );

    int StartVertex = 0, 
        StartIndex = 0;

    for ( const auto& Command : *DrawCommands ) {
        m_Device->SetScissorRect( &Command.Resources.ClipStack.back( ) );
        m_Device->SetTexture( 0, Command.Resources.TextureStack.back( ) );

        m_Device->DrawIndexedPrimitive( D3DPRIMITIVETYPE( Command.Primitive ), StartVertex, 0, Command.VerticesCount, StartIndex, Command.IndicesCount / 3 );

        StartVertex += Command.VerticesCount;
        StartIndex += Command.IndicesCount;
    }

    m_Device->SetTransform( D3DTS_WORLD, &LastWorld );
    m_Device->SetTransform( D3DTS_VIEW, &LastView );
    m_Device->SetTransform( D3DTS_PROJECTION, &LastProjection );

    m_StateBlock->Apply( );
    m_StateBlock->Release( );

    gBuffer->ClearCommands( );
}

void cGraphics::DrawScene( ) {
    m_Device->Clear( 0, nullptr, D3DCLEAR_TARGET, m_ClearColor.Hex, 1.f, 0 );

    if ( m_Device->BeginScene( ) >= 0 ) {
        RenderDrawData( );
        m_Device->EndScene( );
    }

    m_Device->Present( nullptr, nullptr, nullptr, nullptr );
}

void cGraphics::CreateFontFromName( Font* font, const std::string& font_name, int16_t size, int16_t weight, Vec2<int16_t> padding, bool antialiasing ) {
    FT_Library Library;
    FT_Face Face;

    font->Path = gFileSystem->GetFontPath( font_name );

    if ( font->Path.empty( ) )
        return;

    font->Padding = padding;
    font->Size = size;

    if ( FT_Init_FreeType( &Library ) != FT_Err_Ok )
        throw std::runtime_error( "[cGraphics::CreateFontFromName] FT_Init_FreeType Failed" );

    if ( FT_New_Face( Library, font->Path.c_str( ), 0, &Face ) ) {
        gLogger->Log( LogLevel::Error, "Failed to open requested font " + font_name );
        FT_Done_FreeType( Library );
        return;
    }

    FT_Set_Char_Size( Face, size * 64, 0, GetDpiForWindow( gWindow->GetHandle( ) ), 0 );
    FT_Select_Charmap( Face, FT_ENCODING_UNICODE );

    for ( unsigned char i = 0; i < 128; ++i ) {
        if ( FT_Load_Char( Face, i, antialiasing ? FT_LOAD_RENDER | FT_LOAD_TARGET_NORMAL : FT_LOAD_RENDER | FT_LOAD_TARGET_MONO ) ) {
            gLogger->Log( LogLevel::Error, "FT_Load_Char failed to load character for " + std::to_string( i ) );
            continue;
        }

        int width = Face->glyph->bitmap.width ? Face->glyph->bitmap.width : 16;
        int height = Face->glyph->bitmap.rows ? Face->glyph->bitmap.rows : 16;

        if ( m_Device->CreateTexture( static_cast< UINT >( width ), static_cast< UINT >( height ), 1, D3DUSAGE_DYNAMIC, D3DFMT_A8, D3DPOOL_DEFAULT, &font->CharSet[ i ].Texture, NULL ) ) {
            gLogger->Log( LogLevel::Error, "CreateTexture failed to load character for " + std::to_string( i ) );
            continue;
        }

        D3DLOCKED_RECT lockedRect;
        font->CharSet[ i ].Texture->LockRect( 0, &lockedRect, nullptr, D3DLOCK_DISCARD );

        UCHAR* source = Face->glyph->bitmap.buffer;
        UCHAR* destination = reinterpret_cast< UCHAR* >( lockedRect.pBits );

        if ( source && destination ) {
            switch ( Face->glyph->bitmap.pixel_mode ) {
            case FT_PIXEL_MODE_MONO:
                for ( int y = 0; y < height; ++y, source += Face->glyph->bitmap.pitch, destination += lockedRect.Pitch ) {
                    uint8_t bits = 0;
                    const uint8_t* bitsPtr = source;

                    for ( int x = 0; x < width; ++x, bits <<= 1 ) {
                        if ( ( x & 7 ) == 0 )
                            bits = *bitsPtr++;

                        destination[ x ] = ( bits & 0x80 ) ? 255 : 0;
                    }
                }
                break;

            case FT_PIXEL_MODE_GRAY:
                for ( int y = 0; y < height; ++y ) {
                    for ( int x = 0; x < width; ++x ) {
                        destination[ x ] = source[ x ];
                    }
                    source += Face->glyph->bitmap.pitch;
                    destination += lockedRect.Pitch;
                }
                break;
            }
        }

        font->CharSet[ i ].Texture->UnlockRect( 0 );

        font->Valid = true;
        font->CharSet[ i ].Size = { width, height };
        font->CharSet[ i ].Bearing = { static_cast< int32_t >( Face->glyph->bitmap_left ), static_cast< int32_t >( Face->glyph->bitmap_top ) };
        font->CharSet[ i ].Advance = Face->glyph->Advance.x;
    }

    m_Fonts.emplace_back( font );

    FT_Done_Face( Face );
    FT_Done_FreeType( Library );
}

void cGraphics::CreateTextureFromBytes( IDirect3DTexture9* texture, std::vector<BYTE>* bytes, Vec2<int16_t> size ) {

}

void cGraphics::CreateTextureFromFile( IDirect3DTexture9* texture, const std::string& file_name ) {
    std::string full_path = FS_TEXTURES_FOLDER + file_name;

    if ( D3DXCreateTextureFromFile( m_Device, full_path.c_str( ), &texture ) != D3D_OK ) {
        gLogger->Log( LogLevel::Error, std::vformat( "Failed To Create Image ({})", std::make_format_args( file_name ) ) );
        return;
    }

    m_Textures.push_back( texture );
}

void cGraphics::CreateImageFromFile( Image* image, const std::string& file_name ) {
    std::string full_path = FS_TEXTURES_FOLDER + file_name;
    IDirect3DTexture9* Texture;

    if ( D3DXCreateTextureFromFile( m_Device, full_path.c_str( ), &Texture ) != D3D_OK ) {
        gLogger->Log( LogLevel::Error, std::vformat( "Failed To Create Image ({})", std::make_format_args( file_name ) ) );
        return;
    }

    D3DSURFACE_DESC desc;
    Texture->GetLevelDesc( 0, &desc );

    m_Textures.push_back( Texture );
    image->Texture = Texture;
    image->Size = {
        static_cast< int16_t >( desc.Width ),
        static_cast< int16_t >( desc.Height )
    };
}

void cGraphics::SetVerticalSync( bool vertical_sync ) {
    m_Parameters.PresentationInterval = vertical_sync ? D3DPRESENT_INTERVAL_DEFAULT : D3DPRESENT_INTERVAL_IMMEDIATE;

    ResetDevice( gWindow->GetHandle( ) );
}