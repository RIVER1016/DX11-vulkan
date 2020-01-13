#include "Device.h"
#include "Screen.h"

Screen*	screen = NULL;
Device*	device = NULL;

int WINAPI wWinMain( HINSTANCE hInstance, HINSTANCE hPrevInstance, LPWSTR lpCmdLine, int nCmdShow )
{
	UNREFERENCED_PARAMETER( hPrevInstance );
	UNREFERENCED_PARAMETER( lpCmdLine );
	screen = new Screen( );
	if( FAILED( screen->InitWindow( hInstance, nCmdShow ) ) )
		return 0;
	HWND g_hWnd = screen->getHwnd( );
	device = new Device( );
	device->init( g_hWnd );

	// Main message loop
	MSG msg = { 0 };
	while( WM_QUIT != msg.message )
	{
		if( PeekMessage( &msg, NULL, 0, 0, PM_REMOVE ) )
		{
			TranslateMessage( &msg );
			DispatchMessage( &msg );
		}
		else
		{
			device->render( );
		}
	}
	device->cleanUp( );
	delete device;
	delete screen;
	return ( int )msg.wParam;
}
