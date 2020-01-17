#pragma once
#include "Light.h"

Light::Light( const XMFLOAT4 &ld, const XMFLOAT4 &lp, const XMFLOAT4 &lc, const XMFLOAT4 &ep, const XMFLOAT4 &msc )
{
	gLightDirection = ld;
	gLightPosition = lp;
	gLightColor = lc;
	gEyePos = ep;
	materialSpecColor = msc;
}

void Light::setLightDir( const XMFLOAT4 &ld )
{
	gLightDirection = ld;
}

void Light::setLightPos( const XMFLOAT4 &lp )
{
	gLightPosition = lp;
}

void Light::setLightColor( const XMFLOAT4 &lc )
{
	gLightColor = lc;
}

void Light::setEyePos( const XMFLOAT4 &ep )
{
	gEyePos = ep;
}

void Light::setMaterialSpecColor( const XMFLOAT4 &msc )
{
	materialSpecColor = msc;
}
XMFLOAT4 Light::getLightDir( )
{
	return gLightDirection;
}

XMFLOAT4 Light::getLightPos( )
{
	return gLightPosition;
}

XMFLOAT4 Light::getLightColor( )
{
	return gLightColor;
}

XMFLOAT4 Light::getEyePos( )
{
	return gEyePos;
}

XMFLOAT4 Light::getMaterialSpecColor( )
{
	return materialSpecColor;
}
