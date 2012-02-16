#include	"MainWindow.h"
#include	"WindowProcedure.h"
#include	"resource.h"




MainWindow::MainWindow( HINSTANCE instance , wchar_t* name , int width , int height ) :
	GenericWindow(instance,name,NULL) , control(this) ,
	display(instance,L"Display Window",NULL,&manager) , 
	options(instance,NULL,&manager)
{
	this->setStyle(WS_OVERLAPPEDWINDOW|WS_VISIBLE|WS_EX_WINDOWEDGE|WS_EX_COMPOSITED|WS_CLIPCHILDREN);
	this->setWindowClassProcedure(WndProc);
	this->setWindowClassMenu(MAKEINTRESOURCEW(IDR_MENU1));
	GenericWindow::setDimensions(width,height);
	GenericWindow::setPosition(25,25);
	this->setParameters(&control);

	options.setPosition(0,0);
	options.setDimensions(364,height);

	display.setPosition(364,0);
	display.setDimensions(width-364,height);

	display.getController()->setMinimap(options.getController()->getMapController());
	display.getController()->setOptions(options.getController());
	display.getController()->setTransfer(options.getController()->getTransferController());
};
MainWindow::~MainWindow()	{};

	
bool	MainWindow::create()
{
	bool	val1, val2 , val3;
	


	val1 = GenericWindow::create();

	options.setParent(this->getHandle());
	val2 = options.create();

	display.setParent(this->getHandle());
	val3 = display.create();

	SetFocus(display.getHandle());

	return val1 && val2 && val3;
};


void	MainWindow::setDimensions( int width , int height )	
{
	int x,y,w,h;



	this->width = width;
	this->height = height;

	GenericWindow::setDimensions(width,height);
	display.setDimensions(this->width-364,this->height);
	options.setDimensions(364,this->height);

	HDWP hdwp = BeginDeferWindowPos(2);
		display.getPosition(x,y);
		display.getDimensions(w,h);
		hdwp = DeferWindowPos(hdwp,display.getHandle(),NULL,x,y,w,h,SWP_NOZORDER|SWP_NOMOVE);
		options.getPosition(x,y);
		options.getDimensions(w,h);
		hdwp = DeferWindowPos(hdwp,options.getHandle(),NULL,x,y,w,h,SWP_NOZORDER|SWP_NOMOVE);
	if ( hdwp )	EndDeferWindowPos(hdwp);
};

void	MainWindow::setPosition( int x , int y )
{
	int tx,ty,w,h;



	GenericWindow::setPosition(x,y);
	display.setPosition(364,0);
	options.setPosition(0,0);

	HDWP hdwp = BeginDeferWindowPos(3);
		hdwp = DeferWindowPos(hdwp,this->handle,NULL,this->xPosition,this->yPosition,this->width,this->height,SWP_NOZORDER|SWP_NOMOVE);
		display.getPosition(tx,ty);
		display.getDimensions(w,h);
		hdwp = DeferWindowPos(hdwp,display.getHandle(),NULL,tx,ty,w,h,SWP_NOZORDER|SWP_NOMOVE);
		options.getPosition(tx,ty);
		options.getDimensions(w,h);
		hdwp = DeferWindowPos(hdwp,options.getHandle(),NULL,tx,ty,w,h,SWP_NOZORDER|SWP_NOMOVE);
	if ( hdwp )	EndDeferWindowPos(hdwp);
};

void	MainWindow::setMovement( bool move )
{
	display.getController()->setMovement(move);
}


void	MainWindow::loadFolder( wstring folder )
{
	this->manager.getDicomData(folder);
}

void	MainWindow::invalidateButtons()
{
	this->manager.mouseDown(GLUT_LEFT_BUTTON,GLUT_UP,0,0);
	this->manager.mouseDown(GLUT_RIGHT_BUTTON,GLUT_UP,0,0);
}