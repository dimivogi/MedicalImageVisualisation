#include	"OpenGLWindow.h"
#include	"OpenGLController.h"
#include	"DataManager.h"
#include	"DisplayWindowController.h"



#ifndef	DISPLAY_WINDOW
#define	DISPLAY_WINDOW


// Class containing the window responsible for handling the output of the application
class	DisplayWindow : public OpenGLWindow
{
	private:

		// Controller handling all the messages sent tot he application
		DisplayWindowController		controller;

		// Function running in a seperate thread, which is resposible for the output of the applciation 
		static void					runThread( OpenGLController* ctrl );



	public:

		DisplayWindow( HINSTANCE instance , wchar_t* name , HWND parent , DataManager* manager );
		~DisplayWindow();

		
		// Get the controller
		DisplayWindowController*	getController();
};


#endif	/* DISPLAY_WINDOW */