#pragma once
#include "Transform.h"
#include "Light.h"
class Constbuffer
{
public:
	void setVec( const XMFLOAT4* v, const int &size );
private:
	XMFLOAT4 vec[12];
};

class VSConstBuffer : public Constbuffer
{
public:
	VSConstBuffer( Transform* transform );
	HRESULT setVSConstBuffer( ID3D11Device* g_pd3dDevice, ID3D11DeviceContext* g_pImmediateContext, D3D11_BUFFER_DESC &bd );
};

class PSConstBuffer : public Constbuffer
{
public:
	PSConstBuffer( Light* light );
	HRESULT setPSConstBuffer( ID3D11Device* g_pd3dDevice, ID3D11DeviceContext* g_pImmediateContext, D3D11_BUFFER_DESC &bd );
};
