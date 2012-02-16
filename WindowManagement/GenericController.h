#ifndef	WIN32_LEAN_AND_MEAN
	#define	WIN32_LEAN_AND_MEAN
#endif
#include	<Windows.h>
#include	"GenericWindow.h"



#ifndef	GENERIC_CONTROLLER
#define	GENERIC_CONTROLLER


// Class Responsible for handling messages
class	GenericController
{
	protected:
	
		// Target window
		GenericWindow*			controlSubject;
		// Handle to the target window
		HWND					handle;



	public:

		GenericController();
		GenericController( GenericWindow* subject );
		virtual	~GenericController();
		

		// Set target window
		virtual void			setControlSubject( GenericWindow* subject );
		// Set target window's handle
		virtual void			setHandle( const HWND handle );
		// Send Message to the target window
		virtual	int				sendMessage( UINT message , WPARAM wParam , LPARAM lParam );

		// Message Handling Functions
		// For WM_CREATE
		virtual int				create();
		// For WM_DESTROY
		virtual int				destroy();
		// For WM_ENABLE
		virtual int				enable(bool flag);
		// for WM_CLOSE
		virtual int				close();
		// for WM_ACTIVATE
		virtual int				activate( WPARAM wParam , LPARAM lParam );

		// For WM_NOTIFY
		virtual int				notify(int id, LPARAM lParam);
		// For WM_CONTEXTMENU
		virtual int				contextMenu(HWND handle, int x, int y);
		// For WM_SIZE
		virtual int				size(int w, int h, WPARAM wParam);
		// For WM_TIMER
		virtual int				timer(WPARAM id, LPARAM lParam);	
		// For WM_COMMAND
		virtual int				command(LPARAM msg , WPARAM wParam , LPARAM lParam );
		// For WM_SYSCOMMAND
		virtual int				systemCommand( WPARAM wParam , LPARAM lParam );

		// For WM_KEYUP
		virtual int				keyUp(int key, LPARAM lParam);
		// For WM_KEYDOWN
		virtual int				keyDown(int key, LPARAM lParam);
		// For WM_LBUTTONUP
		virtual int				leftButtonUp(WPARAM state, int x, int y);
		// For WM_LBUTTONDOWN
		virtual int				leftButtonDown(WPARAM state, int x, int y);
		// For WM_MBUTTONUP
		virtual int				middleButtonUp(WPARAM state, int x, int y);
		// For WM_MBUTTONDOWN
		virtual int				middleButtonDown(WPARAM state, int x, int y);	
		// For WM_RBUTTONUP
		virtual int				rightButtonUp(WPARAM wParam, int x, int y);	
		// For WM_RBUTTONDOWN
		virtual int				rightButtonDown(WPARAM wParam, int x, int y);	
		// For WM_HSCROLL
		virtual int				horizontalScroll(WPARAM wParam, LPARAM lParam);	
		// For WM_VSCROLL
		virtual int				verticalScroll(WPARAM wParam, LPARAM lParam);	
		// For WM_MOUSEMOVE
		virtual int				mouseMove(WPARAM state, int x, int y);	
		// For WM_MOUSEWHEEL
		virtual int				mouseWheelMove( int state , int delta , int x , int y );

		// For WM_PAINT
		virtual int				paint();
		// For WM_ERASEBKGND
		virtual int				eraseBkgnd(HDC hdc);	

		// For WM_ENTERSIZING and WM_EXITSIZING
		virtual void			setMovement( bool move );	

		// Returns target window's handle
		const HWND				getHandle() const;
		// Returns target window
		const GenericWindow*	getControlSubject() const;
};


#endif	/* GENERIC_CONTROLLER */