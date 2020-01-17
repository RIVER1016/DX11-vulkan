#pragma once
#include "Camera.h"

void Camera::init( FXMVECTOR p, FXMVECTOR l, FXMVECTOR u )
{
	position = p;
	look = l;
	up = u;
	right = XMVector3Cross( up, look );
}

// ǰ������������, �ƶ�position
void Camera::walk( const float& x, const float& y, const float& z )
{
	XMMATRIX ts;
	XMVECTOR translate = up * -y + look * -z + right * -x;
	ts = XMMatrixTranslationFromVector( translate );
	position = XMVector3Transform( position, ts );
}

// ��б ��right����ת ̧ͷ/��ͷ
void Camera::pitch( const float& r )
{
	XMMATRIX rt;
	rt = XMMatrixRotationAxis( right, r );
	look = XMVector3Transform( look, rt );
	up = XMVector3Transform( up, rt );
}

// ƫ�� ��up����ת ����תͷ
void Camera::yaw( const float& r )
{
	XMMATRIX rt;
	rt = XMMatrixRotationAxis( up, r );
	right = XMVector3Transform( right, rt );
	look = XMVector3Transform( look, rt );
}

// ��ת	��look����ת ��ͷ
void Camera::roll( const float& r )
{
	XMMATRIX rt;
	rt = XMMatrixRotationAxis( look, r );
	right = XMVector3Transform( right, rt );
	up =  XMVector3Transform( up, rt );
}

XMVECTOR Camera::getPosition( )
{
	return position;
}

XMVECTOR Camera::getRight( )
{
	return right;
}

XMVECTOR Camera::getLook( )
{
	return look;
}

XMVECTOR Camera::getUp( )
{
	return up;
}
