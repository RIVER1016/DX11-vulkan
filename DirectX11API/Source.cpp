#include <windows.h>
#include <d3d11.h>
#include <d3dx11.h>
#include <d3dcompiler.h>
#include <d3dx11effect.h>
#include <xnamath.h>
#include <DxErr.h>
struct Vertex
{
	XMFLOAT3 Pos;
	XMFLOAT4 Color;
};

struct ConstantBuffer
{
	XMMATRIX gWorldViewProj;
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

HINSTANCE               g_hInst = NULL;
HWND                    g_hWnd = NULL;
D3D_DRIVER_TYPE         g_driverType = D3D_DRIVER_TYPE_NULL;
D3D_FEATURE_LEVEL       g_featureLevel = D3D_FEATURE_LEVEL_11_0;
ID3D11Device*           g_pd3dDevice = NULL;
ID3D11DeviceContext*    g_pImmediateContext = NULL;
IDXGISwapChain*         g_pSwapChain = NULL;
ID3D11RenderTargetView* g_pRenderTargetView = NULL;
ID3D11Texture2D*        g_pDepthStencil = NULL;
ID3D11DepthStencilView* g_pDepthStencilView = NULL;
ID3D11VertexShader*     g_pVertexShader = NULL;
ID3D11PixelShader*      g_pPixelShader = NULL;
ID3D11InputLayout*      g_pVertexLayout = NULL;
ID3D11Buffer*           g_pVertexBuffer = NULL;
ID3D11Buffer*           g_pIndexBuffer = NULL;
ID3D11Buffer*           g_pVSConstBuffer = NULL;
ID3D11Buffer*           g_pPSConstBuffer = NULL;
ID3D11Buffer*           g_pConstantBuffer = NULL;
ID3D11RasterizerState*  g_pRasterizerState = NULL;
UINT                    g_IndexNum = 0;
XMMATRIX                g_World;
XMMATRIX                g_View;
XMMATRIX                g_Projection;


HRESULT InitWindow( HINSTANCE hInstance, int nCmdShow );
LRESULT CALLBACK WndProc( HWND, UINT, WPARAM, LPARAM );
HRESULT InitDevice( );
void  CleanupDevice( );
void  Render( );

int WINAPI wWinMain( HINSTANCE hInstance, HINSTANCE hPrevInstance, LPWSTR lpCmdLine, int nCmdShow )
{
	UNREFERENCED_PARAMETER( hPrevInstance );
	UNREFERENCED_PARAMETER( lpCmdLine );

	if( FAILED( InitWindow( hInstance, nCmdShow ) ) )
		return 0;

	if( FAILED( InitDevice( ) ) )
	{
		CleanupDevice( );
		return 0;
	}

	// Main message loop
	MSG msg = { 0 };
	while( WM_QUIT != msg.message )
	{
		if( PeekMessage( &msg, NULL, 0, 0, PM_REMOVE ) )
		{
			TranslateMessage( &msg );
			DispatchMessage( &msg );
		}
		else
		{
			Render( );
		}
	}

	CleanupDevice( );

	return ( int )msg.wParam;
}

HRESULT InitWindow( HINSTANCE hInstance, int nCmdShow )
{
	// Register class
	WNDCLASSEX wcex;
	wcex.cbSize = sizeof( WNDCLASSEX );
	wcex.style = CS_HREDRAW | CS_VREDRAW;
	wcex.lpfnWndProc = WndProc;
	wcex.cbClsExtra = 0;
	wcex.cbWndExtra = 0;
	wcex.hInstance = hInstance;
	wcex.hIcon = LoadIcon( NULL,IDC_ARROW );
	wcex.hCursor = LoadCursor( NULL, IDC_ARROW );
	wcex.hbrBackground = ( HBRUSH )( COLOR_WINDOW + 1 );
	wcex.lpszMenuName = NULL;
	wcex.lpszClassName = L"Triangle3D";
	wcex.hIconSm = LoadIcon( NULL,IDC_ARROW );
	if( !RegisterClassEx( &wcex ) )
		return E_FAIL;

	// Create window
	g_hInst = hInstance;
	RECT rc = { 0, 0, 640, 480 };
	AdjustWindowRect( &rc, WS_OVERLAPPEDWINDOW, FALSE );
	g_hWnd = CreateWindow( L"Triangle3D", L"Triangle3D", WS_OVERLAPPEDWINDOW,
							CW_USEDEFAULT, CW_USEDEFAULT, rc.right - rc.left, rc.bottom - rc.top, NULL, NULL, hInstance,
							NULL );
	if( !g_hWnd )
		return E_FAIL;

	ShowWindow( g_hWnd, nCmdShow );

	return S_OK;
}

LRESULT CALLBACK WndProc( HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam )
{
	PAINTSTRUCT ps;
	HDC hdc;

	switch( message )
	{
		case WM_PAINT:
			hdc = BeginPaint( hWnd, &ps );
			EndPaint( hWnd, &ps );
			break;

		case WM_DESTROY:
			PostQuitMessage( 0 );
			break;

		default:
			return DefWindowProc( hWnd, message, wParam, lParam );
	}

	return 0;
}

HRESULT InitDevice( )
{
	HRESULT hr = S_OK;
	// 窗口信息
	RECT rc;
	GetClientRect( g_hWnd, &rc );
	UINT width = rc.right - rc.left;
	UINT height = rc.bottom - rc.top;

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
	for( UINT driverTypeIndex = 0; driverTypeIndex < numDriverTypes; driverTypeIndex++ )
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
		return false;
	}

	if( g_featureLevel != D3D_FEATURE_LEVEL_11_0 )
	{
		MessageBox( 0, L"Direct3D Feature Level 11 unsupported.", 0, 0 );
		return false;
	}

	// 创建 render target view
	ID3D11Texture2D* pBackBuffer = NULL;
	hr = g_pSwapChain->GetBuffer( 0, __uuidof( ID3D11Texture2D ), ( LPVOID* )&pBackBuffer );
	if( FAILED( hr ) )
		return hr;

	hr = g_pd3dDevice->CreateRenderTargetView( pBackBuffer, NULL, &g_pRenderTargetView );
	pBackBuffer->Release( );
	if( FAILED( hr ) )
		return hr;

	// 创建深度模板缓冲
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

	// 设置 viewport
	D3D11_VIEWPORT vp;
	vp.Width = ( FLOAT )width;
	vp.Height = ( FLOAT )height;
	vp.MinDepth = 0.0f;
	vp.MaxDepth = 1.0f;
	vp.TopLeftX = 0;
	vp.TopLeftY = 0;
	g_pImmediateContext->RSSetViewports( 1, &vp );

	D3D11_INPUT_ELEMENT_DESC layout[] =
	{
		{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0,
		D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "COLOR", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 12,
		D3D11_INPUT_PER_VERTEX_DATA, 0 }
	};
	DWORD shaderFlags = 0;
#if defined( DEBUG ) || defined( _DEBUG )

	shaderFlags |= D3D10_SHADER_DEBUG;
	shaderFlags |= D3D10_SHADER_SKIP_OPTIMIZATION;
#endif
	ID3D10Blob* compiledShader = 0;
	ID3D10Blob* compilationMsgs = 0;
	D3DX11CompileFromFile( L"Tech.fx", 0, 0, 0, "fx_5_0", shaderFlags, 0, 0, &compiledShader, &compilationMsgs, 0 );

	if ( compilationMsgs != 0 )
	{
		MessageBoxA( 0, ( char* )compilationMsgs->GetBufferPointer( ), 0, 0 );
		compilationMsgs->Release( );
	}

	if ( FAILED( hr ) )
	{
		DXTrace( __FILE__, (DWORD)__LINE__, hr, L"D3DX11CompileFromFile", true );
	}
	// Input Layout
	ID3DX11Effect* mFX;
	hr = D3DX11CreateEffectFromMemory(
	compiledShader->GetBufferPointer( ),
	compiledShader->GetBufferSize( ),
	0, g_pd3dDevice, &mFX );
	compiledShader->Release( );
	ID3DX11EffectTechnique* mTech;
	// 创建 effect
	mTech = mFX->GetTechniqueByIndex( 0 );
	ID3DX11EffectPass *g_EffectPass; // No need to be Release()-d.
	g_EffectPass = mTech->GetPassByIndex( 0 );
	g_EffectPass->Apply( 0, g_pImmediateContext );

	D3DX11_PASS_SHADER_DESC effectVsDesc;
	g_EffectPass->GetVertexShaderDesc( &effectVsDesc );
	D3DX11_EFFECT_SHADER_DESC effectVsDesc2;
	effectVsDesc.pShaderVariable->GetShaderDesc( effectVsDesc.ShaderIndex, &effectVsDesc2 );
	const void *vsCodePtr = effectVsDesc2.pBytecode;
	unsigned vsCodeLen = effectVsDesc2.BytecodeLength;
	g_pd3dDevice->CreateInputLayout( layout, _countof( layout ), vsCodePtr, vsCodeLen, &g_pVertexLayout );
	g_pImmediateContext->IASetInputLayout( g_pVertexLayout );

	// create Vertex Buffer
	Vertex vertices[] = 
	{
		{ XMFLOAT3( -1.f, -1.f, -1.f ), ( const float* )&Colors::White },
		{ XMFLOAT3( -1.f, +1.f, -1.f ), ( const float* )&Colors::Red },
		{ XMFLOAT3( +1.f, +1.f, -1.f ), ( const float* )&Colors::Red },
		{ XMFLOAT3( +1.f, -1.f, -1.f ), ( const float* )&Colors::Green },
		{ XMFLOAT3( -1.f, -1.f, +1.f ), ( const float* )&Colors::Blue },
		{ XMFLOAT3( -1.f, +1.f, +1.f ), ( const float* )&Colors::Yellow },
		{ XMFLOAT3( +1.f, +1.f, +1.f ), ( const float* )&Colors::Cyan },
		{ XMFLOAT3( +1.f, -1.f, +1.f ), ( const float* )&Colors::Magenta }
	};
	D3D11_BUFFER_DESC bd;
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

	// Set vertex buffer
	UINT stride = sizeof( Vertex );
	UINT offset = 0;
	g_pImmediateContext->IASetVertexBuffers( 0, 1, &g_pVertexBuffer, &stride, &offset );

	UINT indices[36] =
	{
		0, 1, 2, // Triangle 0
		0, 2, 3, // Triangle 1
		0, 3, 4, // Triangle 2
		3, 4, 7, // Triangle 3
		0, 5, 1, // Triangle 4
		0, 4, 5, // Triangle 5
		4, 5, 6, // Triangle 6
		4, 6, 7, // Triangle 7
		2, 6, 7, // Triangle 2
		2, 3, 7, // Triangle 3
		1, 2, 6, // Triangle 4
		1, 5, 6, // Triangle 5
	};
	// Describe the index buffer we are going to create.
	D3D11_BUFFER_DESC ibd;
	ibd.Usage = D3D11_USAGE_IMMUTABLE;
	ibd.ByteWidth = sizeof( UINT ) * 36;
	ibd.BindFlags =  D3D11_BIND_INDEX_BUFFER ;
	ibd.CPUAccessFlags = 0;
	ibd.MiscFlags = 0;
	ibd.StructureByteStride = 0;

	// Specify the data to iInitDatathe index buffer.
	D3D11_SUBRESOURCE_DATA iInitData;
	iInitData.pSysMem = indices;

	// Create the index buffer.
	ID3D11Buffer* mIB;
	hr = g_pd3dDevice->CreateBuffer( &ibd, &iInitData, &mIB );
	g_pImmediateContext->IASetIndexBuffer( mIB, DXGI_FORMAT_R32_UINT, 0 );

	 // Set primitive topology
	g_pImmediateContext->IASetPrimitiveTopology( D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST );

	// Create the constant buffer
	bd.Usage = D3D11_USAGE_DEFAULT;
	bd.ByteWidth = sizeof( ConstantBuffer );
	bd.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	bd.CPUAccessFlags = 0;
	hr = g_pd3dDevice->CreateBuffer( &bd, NULL, &g_pConstantBuffer );
	if( FAILED( hr ) )
		return hr;

	// Initialize the world matrix
	g_World = XMMatrixIdentity( );

	// Initialize the view matrix
	XMVECTOR Eye = XMVectorSet( 0.0f, 2.0f, -5.0f, 0.0f );
	XMVECTOR At = XMVectorSet( 0.0f, 1.0f, 0.0f, 0.0f );
	XMVECTOR Up = XMVectorSet( 0.0f, 1.0f, 0.0f, 0.0f );
	g_View = XMMatrixLookAtLH( Eye, At, Up );

	// Initialize the projection matrix
	g_Projection = XMMatrixPerspectiveFovLH( XM_PIDIV2, width / ( FLOAT )height, 0.01f, 100.0f );

	// Set RasterizerState
	D3D11_RASTERIZER_DESC rsDesc;
	ZeroMemory( &rsDesc, sizeof( D3D11_RASTERIZER_DESC ) );
	rsDesc.FillMode = D3D11_FILL_SOLID;
	rsDesc.CullMode = D3D11_CULL_NONE;
	rsDesc.FrontCounterClockwise = false;
	rsDesc.DepthClipEnable = true;
	hr = g_pd3dDevice->CreateRasterizerState( &rsDesc, &g_pRasterizerState );
	g_pImmediateContext->RSSetState( g_pRasterizerState );

	return S_OK;
}

void CleanupDevice( )
{
	if( g_pImmediateContext ) g_pImmediateContext->ClearState( );

	 if( g_pConstantBuffer ) g_pConstantBuffer->Release( );
	if( g_pVertexBuffer ) g_pVertexBuffer->Release( );
	if( g_pIndexBuffer ) g_pIndexBuffer->Release( );
	if( g_pVertexLayout ) g_pVertexLayout->Release( );
	if( g_pVertexShader ) g_pVertexShader->Release( );
	if( g_pPixelShader ) g_pPixelShader->Release( );
	if( g_pRenderTargetView ) g_pRenderTargetView->Release( );
	if( g_pSwapChain ) g_pSwapChain->Release( );
	if( g_pImmediateContext ) g_pImmediateContext->Release( );
	if( g_pd3dDevice ) g_pd3dDevice->Release( );
}

void Render( )
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

	float ClearColor[4] = { 0.f, 0.f, 0.f, 1.f }; //red,green,blue,alpha
	g_pImmediateContext->ClearRenderTargetView( g_pRenderTargetView, ClearColor );
	g_pImmediateContext->ClearDepthStencilView( g_pDepthStencilView, D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0 );

	ConstantBuffer cb;
	XMMATRIX wvp = XMMatrixMultiply( XMMatrixMultiply( g_World, g_View ), g_Projection );
	cb.gWorldViewProj = XMMatrixTranspose( wvp );

	g_pImmediateContext->UpdateSubresource( g_pConstantBuffer, 0, NULL, &cb, 0, 0 );

	g_pImmediateContext->VSSetConstantBuffers( 0, 1, &g_pConstantBuffer );
	g_pImmediateContext->DrawIndexed( 36, 0, 0 );

	g_pSwapChain->Present( 0, 0 );
}
