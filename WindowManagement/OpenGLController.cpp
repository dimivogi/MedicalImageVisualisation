#include	<process.h>
#include	"OpenGLController.h"




OpenGLController::OpenGLController() : 
	GenericController() , manager(NULL) , 
	threadFunction(NULL) , threadHandle(0) , threadID(0) , loopFlag(false) , movement(false)	{};

OpenGLController::OpenGLController(OpenGLWindow* subject , DataManager* manage, void (*threadFunc)( OpenGLController* ) ) : 
	GenericController(subject) , manager(manage) ,
	threadFunction(threadFunc) , threadHandle(0), threadID(0) , loopFlag(false) , movement(false)	{};

OpenGLController::~OpenGLController()	{};


void	OpenGLController::setManager( DataManager* model )	{ if(model) this->manager = model; };
void	OpenGLController::setMovement( bool move )			{ this->movement = move; };


int	OpenGLController::close()
{
	loopFlag = false;
	if ( this->threadHandle != NULL )
		WaitForSingleObject(this->threadHandle, INFINITE);   // wait for rendering thread is terminated

	// close OpenGL Rendering context
	((OpenGLWindow*)this->getControlSubject())->closeContext();//this->controlSubject->getHandle());
	this->controlSubject->destroy();



	return 0;
}

int	OpenGLController::create()
{
	int	returnValue = 0;



	// create a OpenGL rendering context
	if ( !((OpenGLWindow*)this->controlSubject)->createContext(32,24,8) )
		returnValue =  -1;
	else
	{
		// create a thread for OpenGL rendering
		// The params of _beginthreadex() are security, stackSize, functionPtr, argPtr, initFlag, threadID.
		if ( threadFunction != NULL )
		{
			threadHandle = (HANDLE)_beginthreadex(0, 0, (unsigned (__stdcall *)(void *))threadFunction, this, 0, &threadID);

			if ( threadHandle )
				loopFlag = true;
			else
			{
				((OpenGLWindow*)this->controlSubject)->closeContext();
				returnValue = -1;
				throw exception("Cannot create the rendering thread.");
			}
		}
	}



	return returnValue;
}


DataManager*	OpenGLController::getManager() const		{ return this->manager; };
bool			OpenGLController::getLoopFlag() const		{ return this->loopFlag; };
bool			OpenGLController::isMoving() const			{ return this->movement; };
const HANDLE	OpenGLController::getThreadHandle() const	{ return this->threadHandle; };