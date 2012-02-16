#include	"GenericWindow.h"
#include	"GenericController.h"



#ifndef	DIALOG_WINDOW
#define	DIALOG_WINDOW


// Class handling a dialog window
class	DialogWindow	: public GenericWindow
{
	private:
		
		// Window ID
		WORD			id;



	public:
		
		DialogWindow();
		DialogWindow(  WORD id , HINSTANCE hInst , const wchar_t* name , HWND hParent );
		~DialogWindow();


		// For WM_CREATE, because we need a different create process than a generic window
		virtual bool	create();                          // create a dialog
};


#endif	/* DIALOG_WINDOW */