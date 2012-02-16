#ifndef	WIN32_LEAN_AND_MEAN
	#define	WIN32_LEAN_AND_MEAN
#endif
#include	<Windows.h>
#include	"OpenGLWindow.h"
#include	"GenericController.h"
#include	"DataManager.h"



#ifndef	OPENGL_CONTROLLER
#define	OPENGL_CONTROLLER


// Class containing a controller for a OpenGL window
class	OpenGLController : public GenericController
{
	protected:
		
		// Data manager class
		DataManager*				manager;
		// Handle to the thread responsible for the rendering
		HANDLE						threadHandle;
		// Thread's ID
		unsigned int				threadID;
		// Whether to keep drawing or not
		bool						loopFlag;
		// Stop rendering if the window is moving to avoid flickering
		bool						movement;

		// The function used in the rendering thread
		void						(*threadFunction)( OpenGLController* );



	public:

		OpenGLController();
		OpenGLController( OpenGLWindow* subject , DataManager* model , void (*threadFunc)( OpenGLController* ) );
		~OpenGLController();


		// Set the data manager
		void						setManager(DataManager* model);
		// Set whether the window is moving or not
		void						setMovement( bool move );


		// Create the OpenGL window
		int							create();
		// Close the OpenGL window
		int							close();


		// Get the data manager
		DataManager*				getManager() const;
		// Get the loop flag
		bool						getLoopFlag() const;
		// Check whether the window is moving or not
		bool						isMoving()	const;
		// Get the rendering thead's handle
		const HANDLE				getThreadHandle() const;
};


#endif	/* OPENGL_CONTROLLER */