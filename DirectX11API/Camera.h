#pragma once
#include <d3d11.h>
#include <d3dx11.h>
#include <xnamath.h>
class Camera
{
public:
	void init( FXMVECTOR p, FXMVECTOR l, FXMVECTOR u );
	void walk( const float& x, const float& y, const float& z );	// 前后左右上下走, 移动position
	void pitch( const float& r );	// 倾斜 绕right轴旋转 抬头/低头
	void yaw( const float& r );	// 偏航 绕up轴旋转 左右转头
	void roll( const float& r );	// 滚转	绕look轴旋转 侧头
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
