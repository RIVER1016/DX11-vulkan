#pragma once
#include "Geo.h"
#include <d3d11.h>

Geo::Geo( Vertex* verticesOrigin, const int &verticesSize, UINT* indicesOrigin, const int &indicesSize )
{
	indicesLen = 0;
	verticesLen = 0;
	for( int i = verticesLen; i < verticesLen + verticesSize; i ++ )
	{
		vertices[i] = verticesOrigin[ i - verticesLen ];
	}
	verticesLen += verticesSize;
	
	for( int i = indicesLen; i < indicesLen + indicesSize; i ++ )
	{
		indices[i] = indicesOrigin[ i - indicesLen ];
	}
	indicesLen += indicesSize;
}

void Geo::init( ID3DBlob* pb )
{
	pVSBlob = pb;
}

void Geo::setInputLayout( ID3D11Device* g_pd3dDevice, ID3D11DeviceContext* g_pImmediateContext )
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

HRESULT Geo::setVertexBuffer( ID3D11Device* g_pd3dDevice, ID3D11DeviceContext* g_pImmediateContext, D3D11_BUFFER_DESC &bd )
{
	HRESULT hr = S_OK;
	ID3D11Buffer* g_pVertexBuffer;
	ZeroMemory( &bd, sizeof( bd ) );
	bd.Usage = D3D11_USAGE_DEFAULT;
	bd.ByteWidth = sizeof( Vertex ) * verticesLen;
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

HRESULT Geo::setIndexBuffer( ID3D11Device* g_pd3dDevice, ID3D11DeviceContext* g_pImmediateContext )
{
	HRESULT hr = S_OK;
	D3D11_BUFFER_DESC ibd;
	ibd.Usage = D3D11_USAGE_IMMUTABLE;
	ibd.ByteWidth = sizeof( UINT ) * indicesLen;
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
	return S_OK;
}

void Geo::draw( ID3D11Device* g_pd3dDevice, ID3D11DeviceContext* g_pImmediateContext, D3D11_BUFFER_DESC &bd )
{
	setInputLayout( g_pd3dDevice, g_pImmediateContext );
	setVertexBuffer( g_pd3dDevice, g_pImmediateContext, bd );
	setIndexBuffer( g_pd3dDevice, g_pImmediateContext );
}

int Geo::getindicesLen( )
{
	return indicesLen;
}
