

#include <iostream>
#include <ctime>
#include "XTime.h"

using namespace std;

#include "D3dclass.h"
#include <d3d11.h>
#pragma comment(lib,"d3d11.lib")


#include <DirectXMath.h>
#include <DirectXPackedVector.h>
#include <DirectXColors.h>
#include <DirectXCollision.h>
#include "SceneRenderer.h"
using namespace DirectX;
#include "XTime.h"

//#define BACKBUFFER_WIDTH	100
//#define BACKBUFFER_HEIGHT	100



class DEMO_APP
{	
	HINSTANCE						application;
	WNDPROC							appWndProc;
	HWND							window;
	

	D3dclass d3dclass;


public:

	SceneRenderer scene;
	
	XTime timer;
	DEMO_APP(HINSTANCE hinst, WNDPROC proc);
	bool Run(MSG msg);
	bool ShutDown();
};



DEMO_APP::DEMO_APP(HINSTANCE hinst, WNDPROC proc)
{
	
	application = hinst; 
	appWndProc = proc; 

	WNDCLASSEX  wndClass;
    ZeroMemory( &wndClass, sizeof( wndClass ) );
    wndClass.cbSize         = sizeof( WNDCLASSEX );             
    wndClass.lpfnWndProc    = appWndProc;						
    wndClass.lpszClassName  = L"DirectXApplication";            
	wndClass.hInstance      = application;		               
    wndClass.hCursor        = LoadCursor( NULL, IDC_ARROW );    
    wndClass.hbrBackground  = ( HBRUSH )( COLOR_WINDOWFRAME ); 

    RegisterClassEx( &wndClass );

	RECT window_size = { 0, 0, SCREEN_WIDTH, SCREEN_HEIGHT };
	AdjustWindowRect(&window_size, WS_OVERLAPPEDWINDOW, false);
	
	window = CreateWindow(	L"DirectXApplication", L"Project Portfolio V",	WS_OVERLAPPEDWINDOW & ~(WS_THICKFRAME|WS_MAXIMIZEBOX), 
							CW_USEDEFAULT, CW_USEDEFAULT, window_size.right-window_size.left, window_size.bottom-window_size.top,					
							NULL, NULL,	application, this );												

    ShowWindow( window, SW_SHOW );

	d3dclass.init(window);
	scene.init(d3dclass);
	
	
}


bool DEMO_APP::Run(MSG msg)
{
		timer.Signal();
		scene.Render();
		scene.UpdateCamera(msg,timer);
		
		return true; 
	
}


bool DEMO_APP::ShutDown()
{
	scene.Shutdown();
	
	UnregisterClass( L"DirectXApplication", application ); 
	return true;
}


	
int WINAPI wWinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPTSTR lpCmdLine,	int nCmdShow );						   
LRESULT CALLBACK WndProc(HWND hWnd,	UINT message, WPARAM wparam, LPARAM lparam );		
int WINAPI wWinMain( HINSTANCE hInstance, HINSTANCE, LPTSTR, int )
{
	srand(unsigned int(time(0)));
	DEMO_APP myApp(hInstance,(WNDPROC)WndProc);	
    MSG msg; ZeroMemory( &msg, sizeof( msg ) );
    while ( msg.message != WM_QUIT && myApp.Run(msg) )
    {	
	    if ( PeekMessage( &msg, NULL, 0, 0, PM_REMOVE ) )
        { 
            TranslateMessage( &msg );
            DispatchMessage( &msg ); 
        }
    }
	myApp.ShutDown(); 
	return 0; 
}
LRESULT CALLBACK WndProc( HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam )
{
    if(GetAsyncKeyState(VK_ESCAPE))
		message = WM_DESTROY;
    switch ( message )
    {
        case ( WM_DESTROY ): { PostQuitMessage( 0 ); }
        break;
    }
    return DefWindowProc( hWnd, message, wParam, lParam );
}
