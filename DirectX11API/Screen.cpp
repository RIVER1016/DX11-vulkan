#include "Screen.h"

LRESULT CALLBACK WndProc( HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam )
{
	PAINTSTRUCT ps;
	HDC hdc;

	switch( message )
	{
		case WM_PAINT:
			hdc = BeginPaint( hWnd, &ps );
			EndPaint( hWnd, &ps );
			break;
		case WM_DESTROY:
			PostQuitMessage( 0 );
			break;
		default:
			return DefWindowProc( hWnd, message, wParam, lParam );
	}

	return 0;
}

HWND Screen::getHwnd( )
{
	return g_hWnd;
}

int Screen::getWidth( )
{
	return width;
}

int Screen::getHeight( )
{
	return height;
}

HRESULT Screen::InitWindow( HINSTANCE hInstance, int nCmdShow )
{
	// Register class
	WNDCLASSEX wcex;
	wcex.cbSize = sizeof( WNDCLASSEX );
	wcex.style = CS_HREDRAW | CS_VREDRAW;
	wcex.lpfnWndProc = WndProc;
	wcex.cbClsExtra = 0;
	wcex.cbWndExtra = 0;
	wcex.hInstance = hInstance;
	wcex.hIcon = LoadIcon( NULL,IDC_ARROW );
	wcex.hCursor = LoadCursor( NULL, IDC_ARROW );
	wcex.hbrBackground = ( HBRUSH )( COLOR_WINDOW + 1 );
	wcex.lpszMenuName = NULL;
	wcex.lpszClassName = L"Triangle3D";
	wcex.hIconSm = LoadIcon( NULL,IDC_ARROW );
	if( !RegisterClassEx( &wcex ) )
		return E_FAIL;

	// Create window
	HINSTANCE g_hInst = hInstance;
	RECT rc = { 0, 0, 640, 480 };
	AdjustWindowRect( &rc, WS_OVERLAPPEDWINDOW, FALSE );
	g_hWnd = CreateWindow( L"Triangle3D", L"Triangle3D", WS_OVERLAPPEDWINDOW,
							CW_USEDEFAULT, CW_USEDEFAULT, rc.right - rc.left, rc.bottom - rc.top, NULL, NULL, hInstance,
							NULL );
	if( !g_hWnd )
		return E_FAIL;

	ShowWindow( g_hWnd, nCmdShow );
	width = rc.right - rc.left;
	height = rc.bottom - rc.top;
	return S_OK;
}
