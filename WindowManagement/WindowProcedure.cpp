#ifndef	WIN32_LEAN_AND_MEAN
	#define	WIN32_LEAN_AND_MEAN
#endif
#include	<Windows.h>
#include	<shobjidl.h> 	
#include	"WindowProcedure.h"
#include	"GenericController.h"
#include	"resource.h"
#include	<iostream>



LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	LRESULT						returnValue = 0;
	static GenericController	*ctrl;



	// find controller associated with window handle
	ctrl = (GenericController*)GetWindowLongPtr(hWnd, GWL_USERDATA);

	// Non-Client Create
	if( message == WM_NCCREATE )
	{
		// WM_NCCREATE message is called before non-client parts (border,titlebar, menu,etc) are created.
		ctrl = (GenericController*)(((CREATESTRUCT*)lParam)->lpCreateParams);
		ctrl->setHandle(hWnd);

		// Second, store the pointer to the Controller into GWL_USERDATA,
		// so other messages can be routed to the associated Controller.
		SetWindowLongPtr(hWnd, GWL_USERDATA, (LONG_PTR)ctrl);



		return DefWindowProcW(hWnd, message, wParam, lParam);
	}


	// Check if we have a NULL pointer
	if ( ctrl )
	{
		switch (message)
		{
			case WM_CREATE :		returnValue = ctrl->create(); break;
			case WM_DESTROY :		returnValue = ctrl->destroy();break;

			case WM_SIZE :			returnValue = ctrl->size(LOWORD(lParam),HIWORD(lParam),(int)wParam); break;	// width, height, type

			case WM_CLOSE :			returnValue = ctrl->close();break;
			case WM_ENABLE :	
									bool	flag;



									if ( wParam )
										flag = true;
									else
										flag = false;

									returnValue = ctrl->enable(flag); break;	// TRUE or FALSE
			case WM_ACTIVATE :		returnValue = ctrl->activate(wParam,lParam); break;

			case WM_TIMER :			returnValue = ctrl->timer(LOWORD(wParam), HIWORD(wParam)); break;
			case WM_NOTIFY :		returnValue = ctrl->notify((int)wParam, lParam); break;	// controllerID, lParam
			case WM_CONTEXTMENU :	returnValue = ctrl->contextMenu((HWND)wParam, LOWORD(lParam), HIWORD(lParam));	// handle, x, y (from screen coords)

			case WM_SYSCOMMAND:		returnValue = ctrl->systemCommand(wParam, lParam); break;
			case WM_COMMAND :		returnValue = ctrl->command(message,wParam,lParam); break;	// id, code, msg


			case WM_KEYDOWN :
			case WM_SYSKEYDOWN :	returnValue = ctrl->keyDown((int)wParam, lParam); break;	// keyCode, keyDetail
		
			case WM_KEYUP :
			case WM_SYSKEYUP :		returnValue = ctrl->keyUp((int)wParam, lParam); break;	 // keyCode, keyDetail

			case WM_LBUTTONDOWN :	returnValue = ctrl->leftButtonDown(wParam, LOWORD(lParam), HIWORD(lParam)); break;	// state, x, y
			case WM_LBUTTONUP :		returnValue = ctrl->leftButtonUp(wParam, LOWORD(lParam), HIWORD(lParam)); break;	// state, x, y

			case WM_RBUTTONDOWN :	returnValue = ctrl->rightButtonDown(wParam, LOWORD(lParam), HIWORD(lParam)); break;	// state, x, y
			case WM_RBUTTONUP :		returnValue = ctrl->rightButtonUp(wParam, LOWORD(lParam), HIWORD(lParam)); break;	// state, x, y

			case WM_MBUTTONDOWN :	returnValue = ctrl->middleButtonDown(wParam, LOWORD(lParam), HIWORD(lParam)); break;	// state, x, y
			case WM_MBUTTONUP :		returnValue = ctrl->middleButtonUp(wParam, LOWORD(lParam), HIWORD(lParam)); break;	// state, x, y

			case WM_MOUSEWHEEL :	returnValue = ctrl->mouseWheelMove(LOWORD(wParam), HIWORD(wParam), LOWORD(lParam), HIWORD(lParam)); break;	// state, delta, x, y

			case WM_HSCROLL :		returnValue = ctrl->horizontalScroll(wParam, lParam); break;
			case WM_VSCROLL :		returnValue = ctrl->verticalScroll(wParam, lParam); break;

			case WM_MOUSEMOVE :		returnValue = ctrl->mouseMove(wParam, LOWORD(lParam), HIWORD(lParam)); break;	// state, x, y


			case WM_PAINT :			returnValue = ctrl->paint(); break;
			case WM_ERASEBKGND :	returnValue = ctrl->eraseBkgnd((HDC)wParam); break;	// handle to device context

			
			case WM_ENTERSIZEMOVE :	ctrl->setMovement(true); returnValue = DefWindowProcW(hWnd,message,wParam,lParam); break;
			case WM_EXITSIZEMOVE :	ctrl->setMovement(false); returnValue = DefWindowProcW(hWnd,message,wParam,lParam); break;


			default:			returnValue = DefWindowProcW(hWnd, message, wParam, lParam);
		}
	}
	else
		returnValue = DefWindowProcW(hWnd, message, wParam, lParam);



	return returnValue;
};


BOOL CALLBACK dialogProcedure(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	BOOL						returnValue = FALSE;
	static	GenericController*	ctrl;



	ctrl = (GenericController*)::GetWindowLongPtr(hwnd, GWL_USERDATA);

	// WM_INITDIALOG message is called before displaying the dialog box.
	if(msg == WM_INITDIALOG)
	{
		ctrl = (GenericController*)lParam;
		ctrl->setHandle(hwnd);

		SetWindowLongPtr(hwnd, GWL_USERDATA, (LONG_PTR)ctrl);

		// When WM_INITDIALOG is called, all controls in the dialog are created.
		// We don't handle WM_CREATE message for dialogs because the message is sent before controls have been created
		ctrl->create();



		return TRUE;
	}


	// Check if we have a NULL pointer
	if ( ctrl )
	{
		switch ( msg )
		{
			case WM_COMMAND :	
									ctrl->command(msg , wParam, lParam);   // id, code, msg
									returnValue = TRUE; break;

			case WM_TIMER :
									ctrl->timer(LOWORD(wParam), HIWORD(wParam));
									returnValue = TRUE; break;

			case WM_PAINT :
									ctrl->paint();
									returnValue = TRUE; break;

			case WM_DESTROY :
									ctrl->destroy();
									returnValue = TRUE; break;

			case WM_CLOSE :
									ctrl->close();
									returnValue = TRUE; break;

			case WM_HSCROLL :
									ctrl->horizontalScroll(wParam, lParam);
									returnValue = TRUE; break;

			case WM_VSCROLL :
									ctrl->verticalScroll(wParam, lParam);
									returnValue = TRUE; break;

			case WM_NOTIFY :
									ctrl->notify((int)wParam, lParam);
									returnValue = TRUE; break;

			case WM_MOUSEMOVE :
									ctrl->mouseMove(wParam, LOWORD(lParam), HIWORD(lParam));
									returnValue = TRUE; break;

			case WM_CONTEXTMENU :
									ctrl->contextMenu((HWND)wParam, LOWORD(lParam), HIWORD(lParam)); 
									returnValue = TRUE; break;

			default :
									returnValue = FALSE; break;
		}
	}
	else
	{
		DefWindowProcW(hwnd,msg,wParam,lParam);
		returnValue = TRUE;
	}



	return	returnValue;
};