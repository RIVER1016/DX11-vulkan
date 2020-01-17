#pragma once
#include "Shader.h"

void Constbuffer::setVec( const XMFLOAT4* v, const int &size )
{
	for ( int i = 0; i < size; i++ )
		vec[i] = v[i];
}

VSConstBuffer::VSConstBuffer( Transform* transform )
{
	XMFLOAT4 vec[12];
	XMMATRIX gWorld = transform->getWorld( );
	XMMATRIX gView = transform->getView( );
	XMMATRIX gProjection = transform->getProjection( );

	vec[0] = gWorld.m[0];
	vec[1] = gWorld.m[1];
	vec[2] = gWorld.m[2];
	vec[3] = gWorld.m[3];

	vec[4] = gView.m[0];
	vec[5] = gView.m[1];
	vec[6] = gView.m[2];
	vec[7] = gView.m[3];

	vec[8] = gProjection.m[0];
	vec[9] = gProjection.m[1];
	vec[10] = gProjection.m[2];
	vec[11] = gProjection.m[3];
	setVec( vec, 12 );
}

HRESULT VSConstBuffer::setVSConstBuffer( ID3D11Device* g_pd3dDevice, ID3D11DeviceContext* g_pImmediateContext, D3D11_BUFFER_DESC &bd )
{
	HRESULT hr = S_OK;
	ID3D11Buffer* g_pVSConstBuffer;
	bd.Usage = D3D11_USAGE_DEFAULT;
	bd.ByteWidth = sizeof( VSConstBuffer );
	bd.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	bd.CPUAccessFlags = 0;
	hr = g_pd3dDevice->CreateBuffer( &bd, NULL, &g_pVSConstBuffer );
	if( FAILED( hr ) )
		return hr;

	g_pImmediateContext->UpdateSubresource( g_pVSConstBuffer, 0, NULL, &( *this ), 0, 0 );
	g_pImmediateContext->VSSetConstantBuffers( 0, 1, &g_pVSConstBuffer );
	g_pVSConstBuffer->Release( );
	return S_OK;
}

PSConstBuffer::PSConstBuffer( Light* light )
{
	XMFLOAT4 vec[5];
	vec[0] = light->getLightDir( );
	vec[1] = light->getLightPos( );
	vec[2] = light->getLightColor( );
	vec[3] = light->getEyePos( );
	vec[4] = light->getMaterialSpecColor( );
	setVec( vec, 5 );
}

HRESULT PSConstBuffer::setPSConstBuffer( ID3D11Device* g_pd3dDevice, ID3D11DeviceContext* g_pImmediateContext, D3D11_BUFFER_DESC &bd )
{
	HRESULT hr = S_OK;
	// Create the g_pPSConstBuffer
	ID3D11Buffer* g_pPSConstBuffer;
	bd.Usage = D3D11_USAGE_DEFAULT;
	bd.ByteWidth = sizeof( PSConstBuffer );
	bd.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	bd.CPUAccessFlags = 0;
	hr = g_pd3dDevice->CreateBuffer( &bd, NULL, &g_pPSConstBuffer );
	if( FAILED( hr ) )
		return hr;

	g_pImmediateContext->UpdateSubresource( g_pPSConstBuffer, 0, NULL, &( *this ), 0, 0 );
	g_pImmediateContext->PSSetConstantBuffers( 0, 1, &g_pPSConstBuffer );
	g_pPSConstBuffer->Release( );
	return S_OK;
}
