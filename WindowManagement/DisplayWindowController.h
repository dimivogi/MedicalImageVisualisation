#include	"OpenGLController.h"
#include	"OptionsController.h"



#ifndef	DISPLAY_WINDOW_CONTROLLER
#define	DISPLAY_WINDOW_CONTROLLER


// A controller class responsible for handling all the messages sent to the display window
class	DisplayWindowController	: public OpenGLController
{
	private:

		// An OpenGL controller for the minimap window
		OpenGLController*	minimap;
		// An OpenGL controller for the transfer function window
		OpenGLController*	transfer;
		// An OpenGL controller for the options window
		OptionsController*	options;



	public:

		DisplayWindowController();
		DisplayWindowController( OpenGLWindow* subject , DataManager* model , void (*threadFunc)( OpenGLController* ) );
		~DisplayWindowController();


		// Set the minimap window controller 
		void				setMinimap( OpenGLController* minimap );
		// Get the minimap window controller
		OpenGLController*	getMinimap();

		// Set the transfer function window controller
		void				setTransfer( OpenGLController* transfer );
		// Get the transfer function window controller
		OpenGLController*	getTransfer();

		// Set the options window controller
		void				setOptions( OptionsController* options );



		// Handling the important messages sent to the display window and ignoring the rest
		// WM_LBUTTONUP
		int					leftButtonUp( WPARAM state , int x , int y );
		// WM_LBUTTONDOWN
		int					leftButtonDown( WPARAM state , int x , int y );
		// WM_RBUTTONUP
		int					rightButtonUp( WPARAM state , int x , int y );
		// WM_RBUTTONDOWN
		int					rightButtonDown( WPARAM state , int x , int y );
		// WM_MOUSEMOVE
		int					mouseMove( WPARAM state , int x , int y );
		// WM_MOUSEWHEEL
		int					mouseWheelMove( int state , int delta , int x , int y );

		// Update the trackbars in the options window
		void				requestTrackbarUpdate();
};


#endif	/* DISPLAY_WINDOW_CONTROLLER */