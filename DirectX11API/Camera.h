#pragma once
#include <d3d11.h>
#include <d3dx11.h>
#include <xnamath.h>
class Camera
{
public:
	void init( FXMVECTOR p, FXMVECTOR l, FXMVECTOR u );
	void walk( const float& x, const float& y, const float& z );	// ǰ������������, �ƶ�position
	void pitch( const float& r );	// ��б ��right����ת ̧ͷ/��ͷ
	void yaw( const float& r );	// ƫ�� ��up����ת ����תͷ
	void roll( const float& r );	// ��ת	��look����ת ��ͷ
	XMVECTOR getPosition( );
	XMVECTOR getRight( );
	XMVECTOR getLook( );
	XMVECTOR getUp( );

private:
	XMVECTOR position;
	XMVECTOR right;
	XMVECTOR look;
	XMVECTOR up;
};
