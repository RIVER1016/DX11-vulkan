#pragma once
#include "Framebuffer.h"

HRESULT Framebuffer::createRenderTargetView( ID3D11Device* g_pd3dDevice, IDXGISwapChain* g_pSwapChain )
{
	HRESULT hr;
	// 创建 render target view
	ID3D11Texture2D* pBackBuffer = NULL;
	hr = g_pSwapChain->GetBuffer( 0, __uuidof( ID3D11Texture2D ), ( LPVOID* )&pBackBuffer );
	if( FAILED( hr ) )
		return hr;

	hr = g_pd3dDevice->CreateRenderTargetView( pBackBuffer, NULL, &g_pRenderTargetView );
	pBackBuffer->Release( );
	if( FAILED( hr ) )
		return hr;
	return S_OK;
}

HRESULT Framebuffer::createDepthStencilView( ID3D11Device* g_pd3dDevice, ID3D11DeviceContext* g_pImmediateContext, const int &width, const int &height )
{
	HRESULT hr = S_OK;
	// 创建深度模板缓冲
	ID3D11Texture2D* g_pDepthStencil = NULL;
	D3D11_TEXTURE2D_DESC descDepth;
	ZeroMemory( &descDepth, sizeof( descDepth ) );
	descDepth.Width = width;
	descDepth.Height = height;
	descDepth.MipLevels = 1;
	descDepth.ArraySize = 1;
	descDepth.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
	descDepth.SampleDesc.Count = 1;
	descDepth.SampleDesc.Quality = 0;
	descDepth.Usage = D3D11_USAGE_DEFAULT;
	descDepth.BindFlags = D3D11_BIND_DEPTH_STENCIL;
	descDepth.CPUAccessFlags = 0;
	descDepth.MiscFlags = 0;
	hr = g_pd3dDevice->CreateTexture2D( &descDepth, NULL, &g_pDepthStencil );
	if ( FAILED( hr ) )
		return hr;
	// 创建深度模板视图
	D3D11_DEPTH_STENCIL_VIEW_DESC descDSV;
	ZeroMemory( &descDSV, sizeof( descDSV ) );
	descDSV.Format = descDepth.Format;
	descDSV.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;
	descDSV.Texture2D.MipSlice = 0;
	hr = g_pd3dDevice->CreateDepthStencilView( g_pDepthStencil, &descDSV, &g_pDepthStencilView );
	if ( FAILED( hr ) )
		return hr;

	g_pImmediateContext->OMSetRenderTargets( 1, &g_pRenderTargetView, g_pDepthStencilView );
	g_pDepthStencil->Release( );
	return S_OK;
}

void Framebuffer::clearRenderTargetView( ID3D11DeviceContext* g_pImmediateContext, float* ClearColor )
{
	g_pImmediateContext->ClearRenderTargetView( g_pRenderTargetView, ClearColor );
}

void Framebuffer::clearDepthStencilView( ID3D11DeviceContext* g_pImmediateContext )
{
	g_pImmediateContext->ClearDepthStencilView( g_pDepthStencilView, D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0 );
}

void Framebuffer::Release( )
{
	if ( g_pRenderTargetView ) g_pRenderTargetView->Release( );
	if ( g_pDepthStencilView ) g_pDepthStencilView->Release( );
}
