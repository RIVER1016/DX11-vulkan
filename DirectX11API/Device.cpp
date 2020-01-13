#include "Device.h"

void Device::init( HWND ghWnd )
{
	g_hWnd = ghWnd;
	RECT rc;
	GetClientRect( g_hWnd, &rc );
	width = rc.right - rc.left;
	height = rc.bottom - rc.top;
	// Initialize index buffer data
	UINT indices[36] =
	{
		0, 1, 2,
		0, 2, 3,
		0, 3, 4,
		3, 4, 7,
		0, 5, 1,
		0, 4, 5,
		4, 5, 6,
		4, 6, 7,
		2, 6, 7,
		2, 3, 7,
		1, 2, 6,
		1, 5, 6,
	};
	// Initialize Vertex buffer data
	XMFLOAT3 normal1, normal2, normal3, normal4, normal5, normal6, normal7, normal8;
	XMVECTOR normal = XMVectorSet( -1.f, -1.f, -1.f, 0.f );
	normal = XMVector3Normalize( normal );
	XMStoreFloat3( &normal1, normal );
	normal = XMVectorSet( -1.f, +1.f, -1.f, 0.f );
	normal = XMVector3Normalize( normal );
	XMStoreFloat3( &normal2, normal );
	normal = XMVectorSet( +1.f, +1.f, -1.f, 0.f );
	normal = XMVector3Normalize( normal );
	XMStoreFloat3( &normal3, normal );
	normal = XMVectorSet( +1.f, -1.f, -1.f, 0.f );
	normal = XMVector3Normalize( normal );
	XMStoreFloat3( &normal4, normal );
	normal = XMVectorSet( -1.f, -1.f, +1.f, 0.f );
	normal = XMVector3Normalize( normal );
	XMStoreFloat3( &normal5, normal );
	normal = XMVectorSet( -1.f, +1.f, +1.f, 0.f );
	normal = XMVector3Normalize( normal );
	XMStoreFloat3( &normal6, normal );
	normal = XMVectorSet( +1.f, +1.f, +1.f, 0.f );
	normal = XMVector3Normalize( normal );
	XMStoreFloat3( &normal7, normal );
	normal = XMVectorSet( +1.f, -1.f, +1.f, 0.f );
	normal = XMVector3Normalize( normal );
	XMStoreFloat3( &normal8, normal );

	Vertex vertices[] = 
	{
		{ XMFLOAT3( -1.f, -1.f, -1.f ), normal1, ( const float* )&Colors::White },
		{ XMFLOAT3( -1.f, +1.f, -1.f ), normal2, ( const float* )&Colors::Red },
		{ XMFLOAT3( +1.f, +1.f, -1.f ), normal3, ( const float* )&Colors::Red },
		{ XMFLOAT3( +1.f, -1.f, -1.f ), normal4, ( const float* )&Colors::Green },
		{ XMFLOAT3( -1.f, -1.f, +1.f ), normal5, ( const float* )&Colors::Blue },
		{ XMFLOAT3( -1.f, +1.f, +1.f ), normal6, ( const float* )&Colors::Yellow },
		{ XMFLOAT3( +1.f, +1.f, +1.f ), normal7, ( const float* )&Colors::Cyan },
		{ XMFLOAT3( +1.f, -1.f, +1.f ), normal8, ( const float* )&Colors::Magenta }
	};
	// Initialize PS cbuffer
	XMFLOAT4 g_LightDirection = XMFLOAT4( 0.f, 0.f, 1.f, 1.f );
	XMFLOAT4 g_LightPosition = XMFLOAT4( 1.f, 1.f, 1.f, 1.f );
	XMFLOAT4 g_LightColor = XMFLOAT4( 1.f, 1.f, 1.f, 1.f );
	XMFLOAT4 materialSpecColor = XMFLOAT4( 2.f, 2.f, 2.f, 1.f );
	XMFLOAT4 gEyePos = XMFLOAT4(0.0f, 0.0f, -5.0f, 0.0f );
	PSConstBuffer cbl;
	cbl.gLightColor = g_LightColor;
	cbl.gLightDirection = g_LightDirection;
	cbl.gLightPosition = g_LightPosition;
	cbl.gEyePos = gEyePos;
	cbl.materialSpecColor = materialSpecColor;

	// Initialize VS cbuffer
	g_World = XMMatrixIdentity( );
	XMVECTOR Eye = XMVectorSet( 0.0f, 0.0f, -5.0f, 0.0f );
	XMVECTOR At = XMVectorSet( 0.0f, 1.0f, 0.0f, 0.0f );
	XMVECTOR Up = XMVectorSet( 0.0f, 1.0f, 0.0f, 0.0f );
	g_View = XMMatrixLookAtLH( Eye, At, Up );
	g_Projection = XMMatrixPerspectiveFovLH( XM_PIDIV2, width / ( FLOAT )height, 0.01f, 100.0f );
	
	VSConstBuffer cb;
	XMMATRIX gWorld = g_World;
	XMMATRIX gView = g_View;
	XMMATRIX gProjection = g_Projection;
	cb.gWorld = XMMatrixTranspose( gWorld );
	cb.gView = XMMatrixTranspose( gView );
	cb.gProjection = XMMatrixTranspose( gProjection );

	createSwapChain( );
	createRenderTargetView( );
	createDepth( );
	setViewPort( 0.f, 1.f );
	setVertexShader( );
	setPixelShader( );
	setInputLayout( );
	setVertexBuffer( vertices );
	setIndexBuffer( indices );
	setPrimitiveTopology( );
	setVSConstBuffer( cb );
	setPSConstBuffer( cbl );
	setRasterizerState( );
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

HRESULT Device::createRenderTargetView( )
{
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

HRESULT Device::createDepth( )
{
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

void Device::setInputLayout( )
{
	D3D11_INPUT_ELEMENT_DESC layout[] =
	{
		{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "NORMAL", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "COLOR", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 24, D3D11_INPUT_PER_VERTEX_DATA, 0 }
	};
	UINT numElements = ARRAYSIZE( layout );
	ID3D11InputLayout* g_pVertexLayout = NULL;
	g_pd3dDevice->CreateInputLayout( layout, numElements, pVSBlob->GetBufferPointer( ), pVSBlob->GetBufferSize( ), &g_pVertexLayout );
	pVSBlob->Release( );
	g_pImmediateContext->IASetInputLayout( g_pVertexLayout );
	g_pVertexLayout->Release( );
}

HRESULT Device::setVertexBuffer( const Vertex* vertices )
{
	ID3D11Buffer* g_pVertexBuffer;
	ZeroMemory( &bd, sizeof( bd ) );
	bd.Usage = D3D11_USAGE_DEFAULT;
	bd.ByteWidth = sizeof( Vertex ) * 8;
	bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	bd.CPUAccessFlags = 0;
	D3D11_SUBRESOURCE_DATA InitData;
	ZeroMemory( &InitData, sizeof( InitData ) );
	InitData.pSysMem = vertices;
	hr = g_pd3dDevice->CreateBuffer( &bd, &InitData, &g_pVertexBuffer );
	if( FAILED( hr ) )
		return hr;

	UINT stride = sizeof( Vertex );
	UINT offset = 0;
	g_pImmediateContext->IASetVertexBuffers( 0, 1, &g_pVertexBuffer, &stride, &offset );
	g_pVertexBuffer->Release( );
	return S_OK;
}

void Device::setIndexBuffer( const UINT* indices )
{
	D3D11_BUFFER_DESC ibd;
	ibd.Usage = D3D11_USAGE_IMMUTABLE;
	ibd.ByteWidth = sizeof( UINT ) * 36;
	ibd.BindFlags =  D3D11_BIND_INDEX_BUFFER ;
	ibd.CPUAccessFlags = 0;
	ibd.MiscFlags = 0;
	ibd.StructureByteStride = 0;

	D3D11_SUBRESOURCE_DATA iInitData;
	iInitData.pSysMem = indices;

	ID3D11Buffer* g_pIndexBuffer;
	hr = g_pd3dDevice->CreateBuffer( &ibd, &iInitData, &g_pIndexBuffer );
	g_pImmediateContext->IASetIndexBuffer( g_pIndexBuffer, DXGI_FORMAT_R32_UINT, 0 );
	g_pIndexBuffer->Release( );
}

void Device::setPrimitiveTopology( )
{
	g_pImmediateContext->IASetPrimitiveTopology( D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST );
}

HRESULT Device::setVSConstBuffer( const VSConstBuffer& cb )
{
	// Create the g_pVSConstBuffer
	bd.Usage = D3D11_USAGE_DEFAULT;
	bd.ByteWidth = sizeof( VSConstBuffer );
	bd.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	bd.CPUAccessFlags = 0;
	hr = g_pd3dDevice->CreateBuffer( &bd, NULL, &g_pVSConstBuffer );
	if( FAILED( hr ) )
		return hr;

	g_pImmediateContext->UpdateSubresource( g_pVSConstBuffer, 0, NULL, &cb, 0, 0 );
	g_pImmediateContext->VSSetConstantBuffers( 0, 1, &g_pVSConstBuffer );
	return S_OK;
}

HRESULT Device::setPSConstBuffer( const PSConstBuffer& cbl )
{
	// Create the g_pPSConstBuffer
	bd.Usage = D3D11_USAGE_DEFAULT;
	bd.ByteWidth = sizeof( PSConstBuffer );
	bd.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	bd.CPUAccessFlags = 0;
	hr = g_pd3dDevice->CreateBuffer( &bd, NULL, &g_pPSConstBuffer );
	if( FAILED( hr ) )
		return hr;

	g_pImmediateContext->UpdateSubresource( g_pPSConstBuffer, 0, NULL, &cbl, 0, 0 );
	g_pImmediateContext->PSSetConstantBuffers( 0, 1, &g_pPSConstBuffer );
	g_pPSConstBuffer->Release( );
	return S_OK;
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

void Device::cleanUp( )
{
	if( g_pImmediateContext ) g_pImmediateContext->ClearState( );
	if( g_pVSConstBuffer ) g_pVSConstBuffer->Release( );
	if( g_pDepthStencilView ) g_pDepthStencilView->Release( );
	if( g_pRenderTargetView ) g_pRenderTargetView->Release( );
	if( g_pSwapChain ) g_pSwapChain->Release( );
	if( g_pImmediateContext ) g_pImmediateContext->Release( );
	if( g_pd3dDevice ) g_pd3dDevice->Release( );
}
void Device::render( )
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

	g_World = XMMatrixRotationY( t );

	float ClearColor[4] = { 0.f, 0.f, 0.f, 1.f };
	g_pImmediateContext->ClearRenderTargetView( g_pRenderTargetView, ClearColor );
	g_pImmediateContext->ClearDepthStencilView( g_pDepthStencilView, D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0 );

	VSConstBuffer cb;
	XMMATRIX gWorld = g_World;
	XMMATRIX gView = g_View;
	XMMATRIX gProjection = g_Projection;
	cb.gWorld = XMMatrixTranspose( gWorld );
	cb.gView = XMMatrixTranspose( gView );
	cb.gProjection = XMMatrixTranspose( gProjection );

	g_pImmediateContext->UpdateSubresource( g_pVSConstBuffer, 0, NULL, &cb, 0, 0 );
	g_pImmediateContext->VSSetConstantBuffers( 0, 1, &g_pVSConstBuffer );
	g_pImmediateContext->DrawIndexed( 36, 0, 0 );

	g_pSwapChain->Present( 0, 0 );
}

