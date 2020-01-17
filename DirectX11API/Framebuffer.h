#pragma once
#include <d3dx11.h>

class Framebuffer
{
public:
	HRESULT createRenderTargetView( ID3D11Device* g_pd3dDevice, IDXGISwapChain* g_pSwapChain );
	HRESULT createDepthStencilView( ID3D11Device* g_pd3dDevice, ID3D11DeviceContext* g_pImmediateContext, const int &width, const int &height );
	void clearRenderTargetView( ID3D11DeviceContext* g_pImmediateContext, float* ClearColor );
	void clearDepthStencilView( ID3D11DeviceContext* g_pImmediateContext );
	void Release( );
private:
	ID3D11RenderTargetView*	g_pRenderTargetView;
	ID3D11DepthStencilView*	g_pDepthStencilView;
};
