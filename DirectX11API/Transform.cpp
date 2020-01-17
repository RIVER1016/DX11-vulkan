#pragma once
#include "Transform.h"

void Transform::init( const int &w, const int &h, Camera* ca )
{
	width = w;
	height = h;
	camera = ca;
	XMMATRIX g_World = XMMatrixIdentity( );
	setWorld( g_World );
	setView( );
	setProjection( );
}

void Transform::setWorld( CXMMATRIX worldMatrix )
{
	g_World = worldMatrix;
}

void Transform::setView( )
{
	XMVECTOR Eye = camera->getPosition( );
	XMVECTOR At = Eye - camera->getLook( );
	XMVECTOR Up = camera->getUp( );
	g_View = XMMatrixLookAtLH( Eye, At, Up );
}

void Transform::setProjection( )
{
	g_Projection = XMMatrixPerspectiveFovLH( XM_PIDIV2, width / ( FLOAT )height, 0.01f, 100.0f );
}

XMMATRIX Transform::getWorld( )
{
	return g_World;
}

XMMATRIX Transform::getView( )
{
	return g_View;
}

XMMATRIX Transform::getProjection( )
{
	return g_Projection;
}
