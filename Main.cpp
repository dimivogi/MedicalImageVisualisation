#ifndef	WIN32_LEAN_AND_MEAN
	#define	WIN32_LEAN_AND_MEAN
#endif
#include	<Windows.h>
#include	<CommCtrl.h>
#include	<iostream>
#include	"WindowManagement\resource.h"
#include	"WindowManagement\MainWindow.h"
#include	"WindowManagement\RedirectIOToConsole.h"
using namespace	std;



int WINAPI	wWinMain( HINSTANCE hInst , HINSTANCE , PWSTR pCmdLine , int nCmdShow )
{
	int		exitCode;
	wchar_t	name[(MAX_LENGTH-2)/2];
	wchar_t	version[(MAX_LENGTH-2)/2];
	wchar_t namePversion[MAX_LENGTH];
	MSG		msg;
	int		mainH =823+58 , mainW = 1231+16;



	if ( wcsstr(pCmdLine,L"-debug") )
		RedirectIOToConsole();


	LoadStringW(hInst,IDS_APPLICATION_NAME, name,(MAX_LENGTH-2)/2);
	LoadStringW(hInst,IDS_APPLICATION_VERSION,version,(MAX_LENGTH-2)/2);
	wcscpy_s(namePversion,(MAX_LENGTH-2)/2,name);
	namePversion[wcslen(name)] = L' ';
	namePversion[wcslen(name)+1] = L'v';
	wcscpy_s(namePversion+wcslen(name)+2,(MAX_LENGTH-2)/2,version);

	try
	{
		MainWindow				main(hInst,namePversion,mainW,mainH);
		INITCOMMONCONTROLSEX	commonCtrls;
   
		
		
		commonCtrls.dwSize = sizeof(commonCtrls);
		commonCtrls.dwICC = ICC_BAR_CLASSES;
		InitCommonControlsEx(&commonCtrls);
		main.create();

		while ( GetMessage(&msg, (HWND) NULL, 0, 0) > 0 ) 
		{ 
			HWND	activeHandle = GetActiveWindow();



			if ( !( (GetWindowLongPtr(activeHandle, GWL_EXSTYLE) & WS_EX_CONTROLPARENT)  &&  IsDialogMessage(activeHandle, &msg) ) )
			{
				TranslateMessage(&msg); 
				DispatchMessage(&msg); 
			}
		}

		exitCode = msg.wParam;
	}
	catch ( exception& e )
	{
		cerr << e.what() << endl;
		MessageBoxW(NULL,L"A runtime error has occured. Terminating",L"Error",MB_ICONWARNING | MB_OK);
		exitCode = -1;
	}

	if ( wcsstr(pCmdLine,L"-debug") )
		system("PAUSE");


	exit(exitCode);
}