#pragma once
#include "Device.h"
#include "Screen.h"

Screen*	screen = NULL;
Device*	device = NULL;
Transform* transform = NULL;
Light* light = NULL;
Camera* camera = NULL;

const int winWidth = 640, winHeight = 480;
MSG msg = { 0 };

void drawTriangle( )
{
	// Initialize Vertex buffer data
	XMFLOAT3 normal9, normal10, normal11;
	XMVECTOR normal = XMVectorSet( -1.f, -1.f, -1.f, 0.f );
	normal = XMVector3Normalize( normal );
	XMStoreFloat3( &normal9, normal );
	normal = XMVectorSet( -1.f, +1.f, -1.f, 0.f );
	normal = XMVector3Normalize( normal );
	XMStoreFloat3( &normal10, normal );
	normal = XMVectorSet( +1.f, +1.f, -1.f, 0.f );
	normal = XMVector3Normalize( normal );
	XMStoreFloat3( &normal11, normal );
	Vertex verticesOrigin[3] = 
	{
		{ XMFLOAT3( -1.f, -1.f, -1.f ), normal9, ( const float* )&Colors::Red },
		{ XMFLOAT3( -1.f, +1.f, -1.f ), normal10, ( const float* )&Colors::Red },
		{ XMFLOAT3( +1.f, +1.f, -1.f ), normal11, ( const float* )&Colors::Red }
	};
	// Initialize index buffer data
	UINT indicesOrigin[3] =
	{
		0, 1, 2
	};
	Geo tr1 = Geo( verticesOrigin, 3, indicesOrigin, 3 );
	msg = device->idle( tr1 );
}

void setCamera( )
{
	XMVECTOR Eye = XMVectorSet( 0.0f, 0.0f, -5.0f, 0.0f );
	XMVECTOR At = XMVectorSet( 0.0f, 1.0f, 0.0f, 0.0f );
	XMVECTOR Up = XMVectorSet( 0.0f, 1.0f, 0.0f, 0.0f );
	XMVECTOR Look = Eye - At;
	camera->init( Eye, Look, Up );
}

int WINAPI wWinMain( HINSTANCE hInstance, HINSTANCE hPrevInstance, LPWSTR lpCmdLine, int nCmdShow )
{
	UNREFERENCED_PARAMETER( hPrevInstance );
	UNREFERENCED_PARAMETER( lpCmdLine );
	screen = new Screen( );
	if ( FAILED( screen->InitWindow( hInstance, nCmdShow ) ) )
		return 0;
	HWND hWnd = screen->getHwnd( );
	camera = new Camera( );
	setCamera( );
	transform = new Transform( );
	transform->init( winWidth, winHeight, camera );

	XMFLOAT4 g_LightDirection = XMFLOAT4( 0.f, 0.f, 1.f, 1.f );
	XMFLOAT4 g_LightPosition = XMFLOAT4( 1.f, 1.f, 1.f, 1.f );
	XMFLOAT4 g_LightColor = XMFLOAT4( 1.f, 1.f, 1.f, 1.f );
	XMFLOAT4 materialSpecColor = XMFLOAT4( 2.f, 2.f, 2.f, 1.f );
	XMFLOAT4 gEyePos = XMFLOAT4(0.0f, 0.0f, -5.0f, 0.0f );
	light = new Light( g_LightDirection, g_LightPosition, g_LightColor, gEyePos, materialSpecColor );
	device = new Device( );
	device->init( hWnd, transform, light );

	drawTriangle( );

	delete device;
	delete screen;
	delete transform;
	delete camera;
	return ( int )msg.wParam;
}
