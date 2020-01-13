#pragma once

#include <Windows.h>
class Screen
{
public:
	HRESULT InitWindow( HINSTANCE hInstance, int nCmdShow );
	HWND getHwnd( );
	int getWidth( );
	int getHeight( );
private:
	HWND g_hWnd;
	int width;
	int height;
};
