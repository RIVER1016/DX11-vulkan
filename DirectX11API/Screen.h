#pragma once
#include <Windows.h>

class Screen
{
public:
	HRESULT InitWindow( HINSTANCE hInstance, int nCmdShow );
	HWND getHwnd( );
private:
	HWND g_hWnd;
};
