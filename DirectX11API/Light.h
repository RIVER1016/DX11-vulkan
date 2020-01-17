#pragma once
#include <d3dx11.h>
#include <xnamath.h>

class Light
{
public:
	Light( const XMFLOAT4 &ld, const XMFLOAT4 &lp, const XMFLOAT4 &lc, const XMFLOAT4 &ep, const XMFLOAT4 &msc );
	void setLightDir( const XMFLOAT4 &ld );
	void setLightPos( const XMFLOAT4 &lp );
	void setLightColor( const XMFLOAT4 &lc );
	void setEyePos( const XMFLOAT4 &ep );
	void setMaterialSpecColor( const XMFLOAT4 &msc );
	XMFLOAT4 getLightDir( );
	XMFLOAT4 getLightPos( );
	XMFLOAT4 getLightColor( );
	XMFLOAT4 getEyePos( );
	XMFLOAT4 getMaterialSpecColor( );
private:
	XMFLOAT4 gLightDirection;
	XMFLOAT4 gLightPosition;
	XMFLOAT4 gLightColor;
	XMFLOAT4 gEyePos;
	XMFLOAT4 materialSpecColor;
};
