#include "../../.hpp/hooks/hooks.hpp"

HRESULT hooks::resize_buffers( IDXGISwapChain* swap_chain, UINT buffer_count, UINT width, UINT height, DXGI_FORMAT format, UINT flags ) {
	printf( "resize_buffers called\n" );

	const auto original = original_resize_buffers( swap_chain, buffer_count, width, height, format, flags );

	ImGui_ImplDX11_InvalidateDeviceObjects( );

	if ( SUCCEEDED( original ) ) {
		ID3D11Texture2D* render_target_texture = nullptr;

		swap_chain->GetDevice( __uuidof( ID3D11Device ), reinterpret_cast< void** >( &g_ctx->m_device ) );
		g_ctx->m_device->GetImmediateContext( &g_ctx->m_context );
		swap_chain->GetBuffer( 0, __uuidof( ID3D11Texture2D ), reinterpret_cast< void** >( &render_target_texture ) );

		D3D11_RENDER_TARGET_VIEW_DESC desc{ };
		memset( &desc, 0, sizeof( desc ) );
		desc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
		desc.ViewDimension = D3D11_RTV_DIMENSION_TEXTURE2DMS;

		g_ctx->m_device->CreateRenderTargetView( render_target_texture, &desc, &g_ctx->m_render_target_view );
		render_target_texture->Release( );

		ImGui_ImplDX11_CreateDeviceObjects( );
	}

	return original;
}

HRESULT hooks::present( IDXGISwapChain* swap_chain, UINT sync_interval, UINT flags ) {
	printf( "present called\n" );

	static bool init = false;

	if ( !init ) {
		ID3D11Texture2D* render_target_texture = nullptr;

		swap_chain->GetDevice( __uuidof( ID3D11Device ), reinterpret_cast< void** >( &g_ctx->m_device ) );
		g_ctx->m_device->GetImmediateContext( &g_ctx->m_context );
		swap_chain->GetBuffer( 0, __uuidof( ID3D11Texture2D ), reinterpret_cast< void** >( &render_target_texture ) );

		D3D11_RENDER_TARGET_VIEW_DESC desc{ };
		memset( &desc, 0, sizeof( desc ) );
		desc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
		desc.ViewDimension = D3D11_RTV_DIMENSION_TEXTURE2DMS;

		g_ctx->m_device->CreateRenderTargetView( render_target_texture, &desc, &g_ctx->m_render_target_view );
		render_target_texture->Release( );

		ImGui::CreateContext( );
		ImGui::StyleColorsDark( );

		ImGui::GetStyle( ).WindowMinSize = { 600.f, 440.f };
		ImGui::GetStyle( ).WindowRounding = 5.f;
		ImGui::GetStyle( ).FrameRounding = 5.f;
		ImGui::GetStyle( ).ChildRounding = 3.f;
		ImGui::GetStyle( ).PopupRounding = 3.f;
		ImGui::GetStyle( ).GrabRounding = 3.f;
		ImGui::GetStyle( ).ScrollbarRounding = 3.f;
		ImGui::GetStyle( ).TabRounding = 3.f;

		auto& imgui_io = ImGui::GetIO( );

		imgui_io.IniFilename = imgui_io.LogFilename = nullptr;

		ImGui_ImplWin32_Init( g_ctx->m_hwnd );
		ImGui_ImplDX11_Init( g_ctx->m_device, g_ctx->m_context );

		init = true;
	}

	ImGui_ImplDX11_NewFrame( );
	ImGui_ImplWin32_NewFrame( );

	ImGui::NewFrame( );

	if ( g_gui->is_open )
		g_gui->draw( );

	ImGui::Render( );

	g_ctx->m_context->OMSetRenderTargets( 1, &g_ctx->m_render_target_view, nullptr );
	ImGui_ImplDX11_RenderDrawData( ImGui::GetDrawData( ) );

	return original_present( swap_chain, sync_interval, flags );
}