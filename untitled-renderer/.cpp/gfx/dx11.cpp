#include "../../.hpp/gfx/dx11.hpp"

#ifdef UNTITLED_USE_DX11

void c_gfx::set_render_states( ID3D11Device* device ) {

}

void c_gfx::create_texture( texture* resource, const std::vector<BYTE> bytes, const vector2_t<uint16_t> size ) {

}

bool c_gfx::create_device( HWND hwnd ) {
    DXGI_SWAP_CHAIN_DESC sd = {
        .BufferDesc.Width = g_win32->get_size().x,
        .BufferDesc.Height = g_win32->get_size( ).y,
        .BufferDesc.Format = DXGI_FORMAT_B8G8R8A8_UNORM,
        .BufferDesc.RefreshRate.Numerator = 0,
        .BufferDesc.RefreshRate.Denominator = 0,
        .BufferDesc.Scaling = DXGI_MODE_SCALING_UNSPECIFIED,
        .BufferDesc.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED,
        .SampleDesc.Count = 1,
        .SampleDesc.Quality = 0,
        .BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT,
        .BufferCount = 1,
        .OutputWindow = hwnd,
        .Windowed = TRUE,
        .SwapEffect = DXGI_SWAP_EFFECT_DISCARD,
        .Flags = 0
    };

    D3D11CreateDeviceAndSwapChain(
        nullptr,
        D3D_DRIVER_TYPE_HARDWARE,
        nullptr,
        0,
        nullptr,
        0,
        D3D11_SDK_VERSION,
        &sd,
        &m_swap_chain,
        &m_device,
        nullptr,
        &m_context
    );

    ID3D11Resource* back_buffer = nullptr;
    m_swap_chain->GetBuffer( 0, __uuidof( ID3D11Resource ), reinterpret_cast< void** >( &back_buffer ) );

    m_device->CreateRenderTargetView( back_buffer, nullptr, &m_target );

    safe_release( back_buffer );

    return true;
}

void c_gfx::begin_scene( ) {
    const float color[] = {0.f, 0.f, 0.f, 1.f};
    m_context->ClearRenderTargetView( m_target, color );

}

void c_gfx::end_scene( ) {
    m_swap_chain->Present( 1, NULL );
}

void c_gfx::render_draw_data( ) {
    set_render_states( m_device );

    auto draw_commands = g_buffer->get_draw_commands( );
    g_buffer->build_draw_commands( g_buffer->get_draw_commands( ) );
    auto draw_command = g_buffer->get_draw_command( );

    uint32_t    stride = sizeof( vertex_t ),
                start_vertex = 0,
                start_index = 0;

    for ( const draw_command_t& command : draw_commands ) {
        D3D11_BUFFER_DESC buffer_description{
            .BindFlags = D3D11_BIND_VERTEX_BUFFER,
            .Usage = D3D11_USAGE_DEFAULT,
            .CPUAccessFlags = 0,
            .MiscFlags = 0,
            .ByteWidth = sizeof( command.vertices ),
            .StructureByteStride = sizeof( vertex_t )
        };

        // Create a vertex buffer
        D3D11_BUFFER_DESC vertexBufferDesc;
        ZeroMemory( &vertexBufferDesc, sizeof( D3D11_BUFFER_DESC ) );
        vertexBufferDesc.Usage = D3D11_USAGE_DEFAULT;
        vertexBufferDesc.ByteWidth = sizeof( vertex_t ) * 3; // 3 vertices for the triangle
        vertexBufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
        vertexBufferDesc.CPUAccessFlags = 0;

        D3D11_SUBRESOURCE_DATA vertexData;
        ZeroMemory( &vertexData, sizeof( D3D11_SUBRESOURCE_DATA ) );
        vertexData.pSysMem = command.vertices.data();

        ID3D11Buffer* vertex_buffer;
        m_device->CreateBuffer( &buffer_description, &vertexData, &vertex_buffer );

        m_context->IASetVertexBuffers( 0, 1, &vertex_buffer, &stride, &start_vertex );
        m_context->Draw( draw_command.vertices_count, start_vertex );
    }

    g_buffer->clear_commands( );
}

bool c_gfx::valid( ) {
    return m_device != nullptr;
}

void c_gfx::reset( ) {

}

void c_gfx::release( ) {
    safe_release( m_swap_chain );
    safe_release( m_context );
    safe_release( m_target );
    safe_release( m_device );
}

template <typename type>
void c_gfx::safe_release( type*& obj ) {
    if ( obj ) {
        obj->Release( );
        obj = nullptr;
    }
}

#endif