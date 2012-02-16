#include	"OpenGLController.h"
#include	"DialogWindow.h"
#include	"DataManager.h"



#ifndef	OPTIONS_CONTROLLER
#define	OPTIONS_CONTROLLER


// a controller class responsible for managing the options dialog window
class	OptionsController	: public GenericController
{
	private:

		// an OpenGL window controller for the minimap window
		OpenGLController	mapController;
		// an OpenGL window controller for the transfer function window
		OpenGLController	transferController;



	public:

		OptionsController();
		OptionsController( DialogWindow* subject , OpenGLWindow* map , OpenGLWindow* transfer , DataManager* model ,  void (*threadFunc)( OpenGLController* ) );
		~OptionsController();


		// Set the data manager
		void				setManager( DataManager* model );
		// Get the data manager
		DataManager*		getManager();
		// Get the minimap window controller
		OpenGLController*	getMapController();
		// Get the transfer fucntion window controller
		OpenGLController*	getTransferController();


		// Parse the WM_COMMAND message
		int					command( LPARAM msg , WPARAM wParam , LPARAM lParam );
		// Parse the WM_HORIZONTALSCROLL message
		int					horizontalScroll( WPARAM wParam , LPARAM lParam );

		// Update all trackbars
		void				updateTrackbars();
};


#endif	/* OPTIONS_CONTROLLER */