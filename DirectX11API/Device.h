#pragma once
#include <windows.h>
#include <d3d11.h>
#include <d3dx11.h>
#include <d3dcompiler.h>
#include <xnamath.h>
#include <DxErr.h>

struct Vertex
{
	XMFLOAT3 Pos;
	XMFLOAT3 Normal;
	XMFLOAT4 Color;
};

struct VSConstBuffer
{
	XMMATRIX gWorld;
	XMMATRIX gView;
	XMMATRIX gProjection;
};

struct PSConstBuffer
{
	XMFLOAT4 gLightDirection;
	XMFLOAT4 gLightPosition;
	XMFLOAT4 gLightColor;
	XMFLOAT4 gEyePos;
	XMFLOAT4 materialSpecColor;
};

namespace Colors
{
	XMGLOBALCONST XMVECTORF32 White = { 1.f, 1.f, 1.f, 1.f };
	XMGLOBALCONST XMVECTORF32 Black = { 0.f, 0.f, 0.f, 1.f };
	XMGLOBALCONST XMVECTORF32 Red = { 1.f, 0.f, 0.f, 1.f };
	XMGLOBALCONST XMVECTORF32 Green = { 0.f, 1.f, 0.f, 1.f };
	XMGLOBALCONST XMVECTORF32 Blue = { 0.f, 0.f, 1.f, 1.f };
	XMGLOBALCONST XMVECTORF32 Yellow = { 1.f, 1.f, 0.f, 1.f };
	XMGLOBALCONST XMVECTORF32 Cyan = { 0.f, 1.f, 1.f, 1.f };
	XMGLOBALCONST XMVECTORF32 Magenta = { 1.f, 0.f, 1.f, 1.f };

}

class Device
{
public:
	void init( HWND g_hWnd );
	HRESULT createSwapChain( );
	HRESULT createRenderTargetView( );
	HRESULT createDepth( );
	HRESULT compileShaderFromFile( WCHAR* szFileName, LPCSTR szEntryPoint, LPCSTR szShaderModel, ID3DBlob** ppBlobOut );
	HRESULT setVertexShader( );
	HRESULT setPixelShader( );
	void setViewPort( const float& MinDepth, const float& MaxDepth );
	void setInputLayout( );
	HRESULT setVertexBuffer( const Vertex* vertices );
	void setIndexBuffer( const UINT* indices );
	void setPrimitiveTopology( );
	HRESULT setVSConstBuffer( const VSConstBuffer& cb );
	HRESULT setPSConstBuffer( const PSConstBuffer& cbl );
	void setRasterizerState( );
	void render( );
	void cleanUp( );
private:
	HWND					g_hWnd;
	UINT					width;
	UINT					height;
	D3D_DRIVER_TYPE			g_driverType;
	ID3D11Device*			g_pd3dDevice;
	ID3D11DeviceContext*	g_pImmediateContext;
	IDXGISwapChain*			g_pSwapChain;
	ID3D11RenderTargetView*	g_pRenderTargetView;
	ID3D11DepthStencilView*	g_pDepthStencilView;
	ID3D11Buffer*			g_pVSConstBuffer;
	ID3D11Buffer*			g_pPSConstBuffer;
	ID3DBlob*				pVSBlob;
	HRESULT					hr;
	XMMATRIX				g_World;
	XMMATRIX				g_View;
	XMMATRIX				g_Projection;
	D3D11_BUFFER_DESC		bd;
};

