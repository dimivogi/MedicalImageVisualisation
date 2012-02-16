#include	"DisplayWindowController.h"




DisplayWindowController::DisplayWindowController() : OpenGLController()	{};
DisplayWindowController::DisplayWindowController(OpenGLWindow* subject , DataManager* manage, void (*threadFunc)( OpenGLController* ) ) : 
	OpenGLController(subject,manage,threadFunc)	{};
DisplayWindowController::~DisplayWindowController()	{};


void				DisplayWindowController::setMinimap( OpenGLController* minimap )	{ this->minimap = minimap; };
OpenGLController*	DisplayWindowController::getMinimap()								{ return this->minimap; };

void				DisplayWindowController::setTransfer( OpenGLController* transfer )	{ this->transfer = transfer; };
OpenGLController*	DisplayWindowController::getTransfer()								{ return this->transfer; };	

void				DisplayWindowController::setOptions( OptionsController*	opt )		{ this->options = opt; };


int	DisplayWindowController::leftButtonUp(WPARAM state, int x, int y)
{
	bool	left , right;



	this->manager->mouseDown(GLUT_LEFT_BUTTON,GLUT_UP,x,y);
	this->manager->getMouseStatus(left,right);
	
	if ( !left && !right )
		ReleaseCapture();

	if ( this->controlSubject->getHandle() )
		SetFocus(this->controlSubject->getHandle());



	return 0;
}

int	DisplayWindowController::leftButtonDown(WPARAM state, int x, int y)
{
	SetCapture(this->controlSubject->getHandle());
	this->manager->mouseDown(GLUT_LEFT_BUTTON,GLUT_DOWN,x,y);
	


	return 0;
}

int	DisplayWindowController::rightButtonUp(WPARAM state, int x, int y)
{
	bool	left , right;
	


	this->manager->mouseDown(GLUT_RIGHT_BUTTON,GLUT_UP,x,y);
	this->manager->getMouseStatus(left,right);
	
	if ( !left && !right )
		ReleaseCapture();

	if ( this->controlSubject->getHandle() )
		SetFocus(this->controlSubject->getHandle());



	return 0;
}

int	DisplayWindowController::rightButtonDown(WPARAM state, int x, int y)
{
	SetCapture(this->controlSubject->getHandle());
	this->manager->mouseDown(GLUT_RIGHT_BUTTON,GLUT_DOWN,x,y);

		

	return 0;
}

int	DisplayWindowController::mouseMove(WPARAM state, int x, int y)
{
	this->manager->mouseMove(x,y);
	this->requestTrackbarUpdate();


	return 0;
}

int	DisplayWindowController::mouseWheelMove( int state , int delta , int x , int y )
{
	this->manager->zoomCamera(0.05*(short)delta/120);
	this->requestTrackbarUpdate();


	return 0;
}

void	DisplayWindowController::requestTrackbarUpdate()
{
	this->options->updateTrackbars();
}
