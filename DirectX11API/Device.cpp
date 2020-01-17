#pragma once
#include "Device.h"

void Device::init( HWND ghWnd, Transform* ts, Light* li )
{
	g_hWnd = ghWnd;
	RECT rc;
	GetClientRect( g_hWnd, &rc );
	width = rc.right - rc.left;
	height = rc.bottom - rc.top;
	framebuffer = new Framebuffer( );
	createSwapChain( );
	setFramebuffer( );
	setViewPort( 0.f, 1.f );
	setVertexShader( );
	setPixelShader( );
	setPrimitiveTopology( );
	setRasterizerState( );
	transform = ts;
	light = li;
	hr = S_OK;
}

HRESULT Device::createSwapChain( )
{
	// 创建交换链描述
	DXGI_SWAP_CHAIN_DESC sd;
	sd.BufferDesc.Width = width;
	sd.BufferDesc.Height = height;
	sd.BufferDesc.RefreshRate.Numerator = 60;
	sd.BufferDesc.RefreshRate.Denominator = 1;
	sd.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	sd.BufferDesc.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;
	sd.BufferDesc.Scaling = DXGI_MODE_SCALING_UNSPECIFIED;
	sd.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
	sd.BufferCount = 1;
	sd.OutputWindow = g_hWnd;
	sd.Windowed = TRUE;
	sd.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;
	sd.Flags = 0;
	sd.SampleDesc.Count = 1; // 不使用 MSAA
	sd.SampleDesc.Quality = 0;

	// 调试模式
	UINT createDeviceFlags = 0;
#ifdef _DEBUG
	createDeviceFlags |= D3D11_CREATE_DEVICE_DEBUG;
#endif

	D3D_DRIVER_TYPE driverTypes[] =
	{
		D3D_DRIVER_TYPE_HARDWARE,
		D3D_DRIVER_TYPE_WARP,
		D3D_DRIVER_TYPE_REFERENCE,
	};
	UINT numDriverTypes = ARRAYSIZE( driverTypes );
	// 特征等级
	D3D_FEATURE_LEVEL featureLevels[] =
	{
		D3D_FEATURE_LEVEL_11_0,
		D3D_FEATURE_LEVEL_10_1,
		D3D_FEATURE_LEVEL_10_0,
	};
	UINT numFeatureLevels = ARRAYSIZE( featureLevels );

	// 创建 ID3D11Device, ID3D11DeviceContext, IDXGISwapChain
	D3D_FEATURE_LEVEL g_featureLevel = D3D_FEATURE_LEVEL_11_0;
	for( UINT driverTypeIndex = 0; driverTypeIndex < numDriverTypes; driverTypeIndex ++ )
	{
		g_driverType = driverTypes[driverTypeIndex];
		hr = D3D11CreateDeviceAndSwapChain( NULL, g_driverType, NULL, createDeviceFlags, featureLevels, numFeatureLevels,
											D3D11_SDK_VERSION, &sd, &g_pSwapChain, &g_pd3dDevice, &g_featureLevel, &g_pImmediateContext );
		if( SUCCEEDED( hr ) )
			break;
	}

	if( FAILED( hr ) )
	{
		MessageBox( 0, L"D3D11CreateDeviceAndSwapChain Failed.", 0, 0 );
		return hr;
	}

	if( g_featureLevel != D3D_FEATURE_LEVEL_11_0 )
	{
		MessageBox( 0, L"Direct3D Feature Level 11 unsupported.", 0, 0 );
		return hr;
	}
	return S_OK;
}

void Device::setFramebuffer( )
{
	framebuffer->createRenderTargetView( g_pd3dDevice, g_pSwapChain );
	framebuffer->createDepthStencilView( g_pd3dDevice, g_pImmediateContext, width, height );
}

void Device::setViewPort( const float& MinDepth, const float& MaxDepth )
{
	D3D11_VIEWPORT vp;
	vp.Width = ( FLOAT )width;
	vp.Height = ( FLOAT )height;
	vp.MinDepth = MinDepth;
	vp.MaxDepth = MaxDepth;
	vp.TopLeftX = 0;
	vp.TopLeftY = 0;
	g_pImmediateContext->RSSetViewports( 1, &vp );
}

HRESULT Device::compileShaderFromFile( WCHAR* szFileName, LPCSTR szEntryPoint, LPCSTR szShaderModel, ID3DBlob** ppBlobOut )
{
	DWORD dwShaderFlags = D3DCOMPILE_ENABLE_STRICTNESS;
#if defined( DEBUG ) || defined( _DEBUG )

	dwShaderFlags |= D3D10_SHADER_DEBUG;
#endif
	ID3DBlob* pErrorBlob;
	 hr = D3DX11CompileFromFile( szFileName, NULL, NULL, szEntryPoint, szShaderModel, 
		dwShaderFlags, 0, NULL, ppBlobOut, &pErrorBlob, NULL );
	 if( FAILED( hr ) )
	{
		if( pErrorBlob != NULL )
			OutputDebugStringA( ( char* )pErrorBlob->GetBufferPointer( ) );
		if( pErrorBlob ) pErrorBlob->Release( );
		return hr;
	}
	if( pErrorBlob ) pErrorBlob->Release( );
	return S_OK;
}

HRESULT Device::setVertexShader( )
{
	// Compile the vertex shader
	ID3D11VertexShader* g_pVertexShader;
	pVSBlob = NULL;
	hr = compileShaderFromFile( L"Tech.fx", "VS", "vs_5_0", &pVSBlob );
	if( FAILED( hr ) )
	{
		MessageBox( NULL,
					L"The FX file cannot be compiled.  Please run this executable from the directory that contains the FX file.", L"Error", MB_OK );
		return hr;
	}

	// Create the vertex shader
	hr = g_pd3dDevice->CreateVertexShader( pVSBlob->GetBufferPointer( ), pVSBlob->GetBufferSize( ), NULL, &g_pVertexShader );
	if( FAILED( hr ) )
	{	
		pVSBlob->Release( );
		return hr;
	}
	g_pImmediateContext->VSSetShader( g_pVertexShader, NULL, 0 );
	g_pVertexShader->Release( );
	return S_OK;
}

HRESULT Device::setPixelShader( )
{
	// Compile the vertex shader
	ID3D11PixelShader* g_pPixelShader;
	ID3DBlob* pPSBlob = NULL;
	hr = compileShaderFromFile( L"Tech.fx", "PS", "ps_5_0", &pPSBlob );
	if( FAILED( hr ) )
	{
		MessageBox( NULL,
					L"The FX file cannot be compiled.  Please run this executable from the directory that contains the FX file.", L"Error", MB_OK );
		return hr;
	}

	// Create the vertex shader
	hr = g_pd3dDevice->CreatePixelShader( pPSBlob->GetBufferPointer( ), pPSBlob->GetBufferSize( ), NULL, &g_pPixelShader );
	if( FAILED( hr ) )
	{	
		pPSBlob->Release( );
		return hr;
	}
	g_pImmediateContext->PSSetShader( g_pPixelShader, NULL, 0 );
	pPSBlob->Release( );
	g_pPixelShader->Release( );
	return S_OK;
}

void Device::setPrimitiveTopology( )
{
	g_pImmediateContext->IASetPrimitiveTopology( D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST );
}

void Device::setRasterizerState( )
{
	// Set RasterizerState
	ID3D11RasterizerState*	g_pRasterizerState;
	D3D11_RASTERIZER_DESC rsDesc;
	ZeroMemory( &rsDesc, sizeof( D3D11_RASTERIZER_DESC ) );
	rsDesc.FillMode = D3D11_FILL_SOLID;
	rsDesc.CullMode = D3D11_CULL_NONE;
	rsDesc.FrontCounterClockwise = false;
	rsDesc.DepthClipEnable = true;
	hr = g_pd3dDevice->CreateRasterizerState( &rsDesc, &g_pRasterizerState );
	g_pImmediateContext->RSSetState( g_pRasterizerState );
	g_pRasterizerState->Release( );
}

void Device::Release( )
{
	if( g_pImmediateContext ) g_pImmediateContext->ClearState( );
	if ( framebuffer ) framebuffer->Release( );
	if( g_pSwapChain ) g_pSwapChain->Release( );
	if( g_pImmediateContext ) g_pImmediateContext->Release( );
	if( g_pd3dDevice ) g_pd3dDevice->Release( );
}

VSConstBuffer Device::rotateWorld( )
{
	// Update our time
	static float t = 0.0f;
	if( g_driverType == D3D_DRIVER_TYPE_REFERENCE )
	{
		t += ( float )XM_PI * 0.0125f;
	}
	else
	{
		static DWORD dwTimeStart = 0;
		DWORD dwTimeCur = GetTickCount( );
		if( dwTimeStart == 0 )
			dwTimeStart = dwTimeCur;
		t = ( dwTimeCur - dwTimeStart ) / 1000.0f;
	}
	XMMATRIX g_World = XMMatrixRotationY( t );
	transform->setWorld( g_World );
	VSConstBuffer cb = VSConstBuffer( transform );

	return cb;
}

void Device::clearDevice( float* clearColor )
{
	framebuffer->clearRenderTargetView( g_pImmediateContext, clearColor );
	framebuffer->clearDepthStencilView( g_pImmediateContext );
}

MSG Device::idle( Geo geo )
{
	MSG msg = { 0 };
	// input geo
	geo.init( pVSBlob );
	geo.draw( g_pd3dDevice, g_pImmediateContext, bd );

	// Initialize VS cbuffer
	VSConstBuffer cb = VSConstBuffer( transform );
	cb.setVSConstBuffer( g_pd3dDevice, g_pImmediateContext, bd );

	// Initialize PS cbuffer
	PSConstBuffer cbl = PSConstBuffer( light );
	cbl.setPSConstBuffer( g_pd3dDevice, g_pImmediateContext, bd );
	
	while ( WM_QUIT != msg.message )
	{
		if ( PeekMessage( &msg, NULL, 0, 0, PM_REMOVE ) )
		{
			TranslateMessage( &msg );
			DispatchMessage( &msg );
		}
		else
		{
			render( geo );
		}
	}
	Release( );
	delete framebuffer;
	return msg;
}

void Device::render( Geo geo )
{
	// rotate world
	VSConstBuffer cb2 = rotateWorld( );
	cb2.setVSConstBuffer( g_pd3dDevice, g_pImmediateContext, bd );

	// clear renderTargetView & depthStenilView
	float ClearColor[4] = { 0.f, 0.f, 0.f, 1.f };
	clearDevice( ClearColor );

	// draw geo
	g_pImmediateContext->DrawIndexed( geo.getindicesLen( ), 0, 0 );
	g_pSwapChain->Present( 0, 0 );
}
