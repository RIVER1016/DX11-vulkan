#pragma once
#include <d3d11.h>
#include <xnamath.h>
#include <d3dx11.h>

struct Vertex
{
	XMFLOAT3 Pos;
	XMFLOAT3 Normal;
	XMFLOAT4 Color;
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

class Geo
{
public:
	Geo( Vertex* verticesOrigin, const int &verticesSize, UINT* indicesOrigin, const int &indicesSize );
	void init( ID3DBlob* pb );
	void draw( ID3D11Device* g_pd3dDevice, ID3D11DeviceContext* g_pImmediateContext, D3D11_BUFFER_DESC &bd );
	int getindicesLen( );
private:
	ID3DBlob* pVSBlob;
	Vertex vertices[100];
	UINT indices[100];
	int indicesLen;
	int verticesLen;
	void setInputLayout( ID3D11Device* g_pd3dDevice, ID3D11DeviceContext* g_pImmediateContext );
	HRESULT setVertexBuffer( ID3D11Device* g_pd3dDevice, ID3D11DeviceContext* g_pImmediateContext, D3D11_BUFFER_DESC &bd );
	HRESULT setIndexBuffer( ID3D11Device* g_pd3dDevice, ID3D11DeviceContext* g_pImmediateContext );
};
