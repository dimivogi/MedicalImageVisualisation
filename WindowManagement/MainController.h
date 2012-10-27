#include	<shobjidl.h> 
#include	"GenericController.h"



#ifndef	MAIN_CONTROLLER
#define	MAIN_CONTROLLER


// A controller class responsible for handling all messages sent to the main window
class	MainController : public GenericController
{
	private:

		// Open Folder window handler
		IFileOpenDialog*		openFolder;

		// message function handling all the messaged sent to the About popup window
		static INT_PTR CALLBACK	About(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam);



	public:

		MainController();
		MainController( GenericWindow* subject );
		~MainController();


		// Handling all the important messages sent to the main window, and ignoring the rest
		// WM_CREATE
		int						create();
		// WM_DESTROY
		int						destroy();
		// WM_CLOSE
		int						close();
		// WM_SIZE
		int						size( int w , int h , WPARAM wParam );
		// WM_SYSCOMMAND
		int						systemCommand( WPARAM wParam , LPARAM lParam );
		// WM_COMMAND
		int						command(LPARAM msg , WPARAM wParam , LPARAM lParam );
		// WM_KEYUP
		int						keyUp( int wParam , LPARAM lParam );
		// WM_LBUTTONUP
		int						leftButtonUp( WPARAM state , int x , int y );
		// WM_RBUTTONUP
		int						rightButtonUp( WPARAM state , int x , int y );
		// WM_ENTERSIZING and WM_EXITSIZING
		void					setMovement( bool move );
};


#endif	/* MAIN_CONTROLLER */