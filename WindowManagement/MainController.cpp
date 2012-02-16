#include	"MainController.h"
#include	"MainWindow.h"
#include	"resource.h"




MainController::MainController() : 
	GenericController()	{};
MainController::MainController( GenericWindow* subject ) :
	GenericController(subject)	{};
MainController::~MainController()	{};


int MainController::create()
{
	int returnValue = 0;



	if ( SUCCEEDED(CoInitializeEx(NULL,COINIT_APARTMENTTHREADED|COINIT_DISABLE_OLE1DDE)) )
	{
		if ( !SUCCEEDED(CoCreateInstance(CLSID_FileOpenDialog, NULL, CLSCTX_ALL,IID_PPV_ARGS(&(this->openFolder)))) )
		{
			CoUninitialize();
			returnValue = -1;
		}
		else
			openFolder->SetOptions(FOS_PICKFOLDERS);
	}
	else
		returnValue = -1;



	return returnValue;
};

int	MainController::destroy()
{
	openFolder->Release();
	CoUninitialize();
	return GenericController::destroy();
};

int	MainController::size(int w, int h, WPARAM wParam)
{
	// Hack the maximized state size, because it ignores the windows taskbar
	if ( wParam == SIZE_MAXIMIZED )
	{
		RECT	workArea;



		SystemParametersInfo(SPI_GETWORKAREA,0,&workArea,0);

		MoveWindow(this->controlSubject->getHandle(),0,0,workArea.right-1,workArea.bottom-1,TRUE);
		this->controlSubject->setDimensions(workArea.right-1,workArea.bottom-1);
		this->controlSubject->setPosition(0,0);
	}
	else
		this->controlSubject->setDimensions(w,h);
	


	return 0;
};


int	MainController::systemCommand( WPARAM wParam, LPARAM lParam)
{
	int	returnValue;



	switch ( wParam & 0xFFF0 )
	{
		case SC_CLOSE :	
						returnValue = 0;
						this->sendMessage(WM_CLOSE,0,0);
						break;

		default :		
						returnValue = DefWindowProc(this->controlSubject->getHandle(), WM_SYSCOMMAND, wParam, lParam);
						break;
	}



	return returnValue;
};

int	MainController::command( LPARAM msg , WPARAM wParam , LPARAM lParam )
{
	 int returnValue = 0;
	 int id = LOWORD(wParam);
	 int cmd = HIWORD(wParam);



	// Parse the menu selections:
	switch (id)
	{
		case ID_HELP_ABOUT :
										HWND	hwnd;



										hwnd = CreateDialogW(this->controlSubject->getInstance(), MAKEINTRESOURCEW(IDD_POPUP_ABOUT), this->handle, &(this->About));
										ShowWindow(hwnd,true);
										break;

		case ID_PROGRAM_EXIT :
										this->sendMessage(WM_CLOSE,0,0);
										break;

		case ID_PROGRAM_OPENFOLDER :
										HRESULT	error;



										error = this->openFolder->Show(NULL);
										if ( SUCCEEDED(error) )
										{
											IShellItem *pItem;



											error = openFolder->GetResult(&pItem);
											if (SUCCEEDED(error))
											{
												PWSTR pszFolderPath;



												error = pItem->GetDisplayName(SIGDN_FILESYSPATH, &pszFolderPath);
												// Display the file name to the user.
												if (SUCCEEDED(error))
												{
													((MainWindow*)this->controlSubject)->loadFolder(pszFolderPath);
													CoTaskMemFree(pszFolderPath);
												}
												pItem->Release();
											}
										}

										break;

		default :
										returnValue =  DefWindowProc(this->handle, msg, wParam, lParam);
	}



	return returnValue;
};

int	MainController::keyUp( int wParam , LPARAM lParam )
{
	int	returnValue;



	switch ( wParam )
	{
		case VK_F4 :	
						if ( GetKeyState(VK_MENU) )	this->sendMessage(WM_CLOSE,0,0);
						returnValue = 0;
						break;

		case 'X' :
		case 'x' :
						if ( GetKeyState(VK_CONTROL) )	this->sendMessage(WM_CLOSE,0,0);
						returnValue = 0;
						break;

		case 'O' :
		case 'o' :
						if ( GetKeyState(VK_CONTROL) )	this->sendMessage(WM_COMMAND,ID_PROGRAM_OPENFOLDER,0);
						returnValue = 0;
						break;

		default :
						returnValue = 0;
						break;
	}



	return returnValue;
}

int	MainController::leftButtonUp( WPARAM state , int x , int y )
{
	SetFocus(this->controlSubject->getHandle());
	return 0;
};

int MainController::rightButtonUp( WPARAM state , int x , int y )
{
	SetFocus(this->controlSubject->getHandle());
	return 0;
}

void	MainController::setMovement( bool move )
{
	((MainWindow*)this->controlSubject)->setMovement(move);
}




INT_PTR CALLBACK	MainController::About(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
	UNREFERENCED_PARAMETER(lParam);
	switch (message)
	{
		case WM_INITDIALOG:
			return (INT_PTR)TRUE;

		case WM_COMMAND:
			if ( LOWORD(wParam) == IDC_ABOUT_BUTTON_OK  ||  LOWORD(wParam) == IDCANCEL )
			{
				EndDialog(hDlg, LOWORD(wParam));
				return (INT_PTR)TRUE;
			}
			break;
	}


	return (INT_PTR)FALSE;
};