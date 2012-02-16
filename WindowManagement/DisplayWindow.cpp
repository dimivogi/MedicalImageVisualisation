#ifndef	WIN32_LEAN_AND_MEAN
	#define	WIN32_LEAN_AND_MEAN
#endif
#include	<Windows.h>
#include	<commctrl.h>
#include	"DisplayWindow.h"
#include	"WindowProcedure.h"
#include	"resource.h"




DisplayWindow::DisplayWindow( HINSTANCE instance , wchar_t* name , HWND parent , DataManager* manager ) :
	OpenGLWindow(instance,name,parent) , controller(this,manager,runThread)
{
	this->setStyle(WS_CHILD | WS_VISIBLE | WS_CLIPSIBLINGS | WS_CLIPCHILDREN);
	this->setWindowClassStyle(CS_OWNDC);
	this->setWindowClassProcedure(WndProc);
	this->setParameters(&controller);
};
DisplayWindow::~DisplayWindow()	{};


DisplayWindowController*	DisplayWindow::getController()	{ return &(this->controller); };


void DisplayWindow::runThread( OpenGLController* ctrl )
{
	RECT rect;



	wcout << L"Rendering Thread Created" << endl;

	// Set the current RC in this thread
	wglMakeCurrent(((OpenGLWindow*)ctrl->getControlSubject())->getHandleDC(), ((OpenGLWindow*)ctrl->getControlSubject())->getHandleGLRC());
	
	// If we can initialize all necessary states and structures for at least the Main Display Window
	if ( ctrl->getManager()->init() )
	{
		// Initialize OpenGL states
		ctrl->getManager()->setup();

		wglMakeCurrent( ((OpenGLWindow*)(((DisplayWindowController*)ctrl)->getMinimap()->getControlSubject()))->getHandleDC() , ((OpenGLWindow*)(((DisplayWindowController*)ctrl)->getMinimap()->getControlSubject()))->getHandleGLRC() );
		ctrl->getManager()->minimapSetup();

		wglMakeCurrent( ((OpenGLWindow*)(((DisplayWindowController*)ctrl)->getTransfer()->getControlSubject()))->getHandleDC() , ((OpenGLWindow*)(((DisplayWindowController*)ctrl)->getTransfer()->getControlSubject()))->getHandleGLRC() );
		ctrl->getManager()->transferSetup();

		// Rendering loop
		while(ctrl->getLoopFlag())
		{
			// Allow other processes to function as well
			Sleep(1);
		
		
			// If there are any DICOM Images updates pending and we are not in lockdown
			if ( !ctrl->getManager()->isLocked()  &&  ctrl->getManager()->isUpdatePending() )
			{
				unsigned long	small , center , width;
				unsigned int	min , max;


				// Update the DICOM Images
				wglMakeCurrent(((OpenGLWindow*)ctrl->getControlSubject())->getHandleDC(), ((OpenGLWindow*)ctrl->getControlSubject())->getHandleGLRC());
				ctrl->getManager()->parsePendingUpdate(false);

				wglMakeCurrent( ((OpenGLWindow*)(((DisplayWindowController*)ctrl)->getMinimap()->getControlSubject()))->getHandleDC() , ((OpenGLWindow*)(((DisplayWindowController*)ctrl)->getMinimap()->getControlSubject()))->getHandleGLRC() );
				ctrl->getManager()->parsePendingUpdate(true);

				// Update the windowCenter and windowWidth trackbars
				ctrl->getManager()->getWindowStats(small,center,width);
				SendMessageW(GetDlgItem((((DisplayWindowController*)ctrl)->getMinimap()->getControlSubject())->getParent(),IDC_WINDOW_CENTER),TBM_SETRANGE,(WPARAM)true,(LPARAM)MAKELONG(small,width));
				SendMessageW(GetDlgItem((((DisplayWindowController*)ctrl)->getMinimap()->getControlSubject())->getParent(),IDC_WINDOW_CENTER),TBM_SETPOS,(WPARAM)true,(LPARAM)center);
				SendMessageW(GetDlgItem((((DisplayWindowController*)ctrl)->getMinimap()->getControlSubject())->getParent(),IDC_WINDOW_CENTER), TBM_SETTICFREQ, (WPARAM)1, 0);

				min = (unsigned int)SendMessageW(GetDlgItem((((DisplayWindowController*)ctrl)->getMinimap()->getControlSubject())->getParent(),IDC_WINDOW_WIDTH),TBM_GETRANGEMIN, 0, 0); 
				max = (unsigned int)SendMessageW(GetDlgItem((((DisplayWindowController*)ctrl)->getMinimap()->getControlSubject())->getParent(),IDC_WINDOW_WIDTH), TBM_GETRANGEMAX, 0, 0);
				if ( max <= width )
				{
					SendMessageW(GetDlgItem((((DisplayWindowController*)ctrl)->getMinimap()->getControlSubject())->getParent(),IDC_WINDOW_WIDTH),TBM_SETRANGE,(WPARAM)true,(LPARAM)MAKELONG(width*0.5,width*1.5+1));
					SendMessageW(GetDlgItem((((DisplayWindowController*)ctrl)->getMinimap()->getControlSubject())->getParent(),IDC_WINDOW_WIDTH),TBM_SETPOS,(WPARAM)true,(LPARAM)width);
					SendMessageW(GetDlgItem((((DisplayWindowController*)ctrl)->getMinimap()->getControlSubject())->getParent(),IDC_WINDOW_WIDTH), TBM_SETTICFREQ, (WPARAM)1, 0);
				}
				else if ( min == width )
				{
					int	smallest;

					if ( width*0.5 < small )
						smallest = small;
					else
						smallest = width*0.5;

					SendMessageW(GetDlgItem((((DisplayWindowController*)ctrl)->getMinimap()->getControlSubject())->getParent(),IDC_WINDOW_WIDTH),TBM_SETRANGE,(WPARAM)true,(LPARAM)MAKELONG(smallest,width*1.5+1));
					SendMessageW(GetDlgItem((((DisplayWindowController*)ctrl)->getMinimap()->getControlSubject())->getParent(),IDC_WINDOW_WIDTH),TBM_SETPOS,(WPARAM)true,(LPARAM)width);
					SendMessageW(GetDlgItem((((DisplayWindowController*)ctrl)->getMinimap()->getControlSubject())->getParent(),IDC_WINDOW_WIDTH), TBM_SETTICFREQ, (WPARAM)1, 0);
				}
			}

			// If there is a transfer function updatePending
			if ( ctrl->getManager()->isTransferUpdatePending() )
			{
				// Update the transfer function
				wglMakeCurrent(((OpenGLWindow*)ctrl->getControlSubject())->getHandleDC(), ((OpenGLWindow*)ctrl->getControlSubject())->getHandleGLRC());
				ctrl->getManager()->parseTransferPendingUpdate(0);

				wglMakeCurrent( ((OpenGLWindow*)(((DisplayWindowController*)ctrl)->getMinimap()->getControlSubject()))->getHandleDC() , ((OpenGLWindow*)(((DisplayWindowController*)ctrl)->getMinimap()->getControlSubject()))->getHandleGLRC() );
				ctrl->getManager()->parseTransferPendingUpdate(1);

				wglMakeCurrent( ((OpenGLWindow*)(((DisplayWindowController*)ctrl)->getTransfer()->getControlSubject()))->getHandleDC() , ((OpenGLWindow*)(((DisplayWindowController*)ctrl)->getTransfer()->getControlSubject()))->getHandleGLRC() );
				ctrl->getManager()->parseTransferPendingUpdate(2);
			}


			// if the window is not moving, and the display window exists
			if ( !ctrl->isMoving()  &&  ctrl->getControlSubject()->getHandle() )
			{
				// draw the contents of the display windpw

				wglMakeCurrent(((OpenGLWindow*)ctrl->getControlSubject())->getHandleDC(), ((OpenGLWindow*)ctrl->getControlSubject())->getHandleGLRC());

				// cofigure projection matrix
				GetClientRect(ctrl->getControlSubject()->getHandle(), &rect);
				ctrl->getManager()->resize(rect.right,rect.bottom);
				ctrl->getManager()->setViewport();

				ctrl->getManager()->draw();
				((OpenGLWindow*)ctrl->getControlSubject())->swapBuffers();
			}


			// If the minimap window exists
			if ( ((DisplayWindowController*)ctrl)->getMinimap()->getControlSubject()->getHandle() )
			{
				// draw the contents of the minimap window

				wglMakeCurrent( ((OpenGLWindow*)(((DisplayWindowController*)ctrl)->getMinimap()->getControlSubject()))->getHandleDC() , ((OpenGLWindow*)(((DisplayWindowController*)ctrl)->getMinimap()->getControlSubject()))->getHandleGLRC() );
			
				// cofigure projection matrix
				GetClientRect(((OpenGLWindow*)(((DisplayWindowController*)ctrl)->getMinimap()->getControlSubject()))->getHandle(), &rect);
				ctrl->getManager()->resizeMinimap(rect.right,rect.bottom);
				ctrl->getManager()->setMinimapViewport();

				ctrl->getManager()->drawMinimap();
				((OpenGLWindow*)(((DisplayWindowController*)ctrl)->getMinimap()->getControlSubject()))->swapBuffers();
			}


			// if the transfer functon window exists
			if ( ((DisplayWindowController*)ctrl)->getTransfer()->getControlSubject()->getHandle() )
			{
				// draw the contents of the transfer function window

				wglMakeCurrent( ((OpenGLWindow*)(((DisplayWindowController*)ctrl)->getTransfer()->getControlSubject()))->getHandleDC() , ((OpenGLWindow*)(((DisplayWindowController*)ctrl)->getTransfer()->getControlSubject()))->getHandleGLRC() );
			
				// cofigure projection matrix
				GetClientRect(((OpenGLWindow*)(((DisplayWindowController*)ctrl)->getTransfer()->getControlSubject()))->getHandle(), &rect);
				ctrl->getManager()->resizeTransfer(rect.right,rect.bottom);
				ctrl->getManager()->setTransferViewport();

				ctrl->getManager()->drawTransfer();
				((OpenGLWindow*)(((DisplayWindowController*)ctrl)->getTransfer()->getControlSubject()))->swapBuffers();
			}

		}


		// delete any OpenGL structures we are using
		if ( ctrl->getControlSubject()->getHandle() )
		{
			wglMakeCurrent(((OpenGLWindow*)ctrl->getControlSubject())->getHandleDC(), ((OpenGLWindow*)ctrl->getControlSubject())->getHandleGLRC());
			ctrl->getManager()->destroy();
		}

		if ( ((DisplayWindowController*)ctrl)->getMinimap()->getControlSubject()->getHandle() )
		{
			wglMakeCurrent( ((OpenGLWindow*)(((DisplayWindowController*)ctrl)->getMinimap()->getControlSubject()))->getHandleDC() , ((OpenGLWindow*)(((DisplayWindowController*)ctrl)->getMinimap()->getControlSubject()))->getHandleGLRC() );
			ctrl->getManager()->minimapDestroy();
		}

		if ( ((DisplayWindowController*)ctrl)->getTransfer()->getControlSubject()->getHandle() )
		{
			wglMakeCurrent( ((OpenGLWindow*)(((DisplayWindowController*)ctrl)->getTransfer()->getControlSubject()))->getHandleDC() , ((OpenGLWindow*)(((DisplayWindowController*)ctrl)->getTransfer()->getControlSubject()))->getHandleGLRC() );
			ctrl->getManager()->transferDestroy();
		}

		wcout	<< L"Exiting Rendering Thread" << endl;
	}
	else	// Something has gone wrong
	{
		MessageBoxW(0,L"Shaders could not be loaded. Terminating",L"Error",MB_ICONEXCLAMATION | MB_OK);
		ctrl->sendMessage(WM_CLOSE,0,0);
	}
	

	// Unset the RC
	wglMakeCurrent(0, 0);
	// Terminate the rendering thread
	CloseHandle(ctrl->getThreadHandle());
};