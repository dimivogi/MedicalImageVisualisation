#include	"MainController.h"
#include	"DisplayWindow.h"
#include	"OptionsWindow.h"
#include	"DataManager.h"



#ifndef	MAIN_WINDOW
#define	MAIN_WINDOW


// A class containing the main window of the application
class	MainWindow	: public GenericWindow
{
	private:
		
		// A controller handling the messages sent to the main window
		MainController	control;
		// A window handling the display of the application
		DisplayWindow	display;
		// A window handling the options of the applications
		OptionsWindow	options;
		// The data manager of the application
		DataManager		manager;

	

	public:

		MainWindow( HINSTANCE instance , wchar_t* name , int width , int height );
		~MainWindow();


		
		// Create the main window
		bool			create();

		// Set the main window's and all the subwindow's dimensions
		void			setDimensions( int width , int height );
		// Set the main window's position
		void			setPosition( int x , int y );
		// Set whether the window is moving
		void			setMovement( bool move );

		// Load a DICOM folder
		void			loadFolder( wstring folder );
		// Set all mouse buttons to MOUSE_UP position
		void			invalidateButtons();
};


#endif	/* MAIN_WINDOW */