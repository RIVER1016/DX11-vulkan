#pragma once
#include "Camera.h"

class Transform
{
public:
	void init( const int &w, const int &h, Camera* ca );
	XMMATRIX getWorld( );
	XMMATRIX getView( );
	XMMATRIX getProjection( );
	void setWorld( CXMMATRIX worldMatrix );
	void setView( );
	void setProjection( );

private:
	int				width;
	Camera*			camera;
	int				height;
	XMMATRIX		g_World;
	XMMATRIX		g_View;
	XMMATRIX		g_Projection;
};
