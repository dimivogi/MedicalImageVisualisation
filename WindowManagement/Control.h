#ifndef	WIN32_LEAN_AND_MEAN
	#define	WIN32_LEAN_AND_MEAN
#endif
#include	<Windows.h>
#include	<commctrl.h>



#ifndef	CONTROL
#define	CONTROL


// Class handling a control window
class	Control
{
	protected:

		// Control window's handle
		HWND		handle;
		// Parent's handle
		HWND		parent;
		// Control window's id
		int			id;



	public:

		Control();
		Control( HWND parent , int id );
		~Control();


		// Enable the control
		void		enable();
		// Disable the control
		void		disable();
		// Show the control
		void		show();
		// Hide the control
		void		hide();
		// Set focus on the control
		void		focus();
		// Whether the control is visible or not
		bool		isVisible();


		// Set the parent window
		void		setParent( HWND parent );
		// Set the control window's id
		void		setID( int id );


		// Get functions
		const HWND	getHandle();
		const HWND	getParent();
		int			getID();
};


#endif	/* CONTROL */