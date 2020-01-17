#pragma once
#include <d3dcompiler.h>
#include <DxErr.h>
#include "Framebuffer.h"
#include "Geo.h"
#include "Shader.h"
#include "Transform.h"

class Device
{
public:
	void init( HWND ghWnd, Transform* ts, Light* li );
	HRESULT createSwapChain( );
	HRESULT setVertexShader( );
	HRESULT setPixelShader( );
	void setViewPort( const float& MinDepth, const float& MaxDepth );
	void setPrimitiveTopology( );
	void setRasterizerState( );
	void clearDevice( float* clearColor );
	VSConstBuffer rotateWorld( );
	void Release( );
	void setFramebuffer( );
	MSG idle( Geo geo );
	void render( Geo geo );
private:
	HWND					g_hWnd;
	UINT					width;
	UINT					height;
	D3D_DRIVER_TYPE			g_driverType;
	ID3D11Device*			g_pd3dDevice;
	ID3D11DeviceContext*	g_pImmediateContext;
	IDXGISwapChain*			g_pSwapChain;
	Framebuffer*			framebuffer;
	ID3D11Buffer*			g_pPSConstBuffer;
	ID3DBlob*				pVSBlob;
	HRESULT					hr;
	Transform*				transform;
	Light*					light;
	D3D11_BUFFER_DESC		bd;
	HRESULT compileShaderFromFile( WCHAR* szFileName, LPCSTR szEntryPoint, LPCSTR szShaderModel, ID3DBlob** ppBlobOut );
};
