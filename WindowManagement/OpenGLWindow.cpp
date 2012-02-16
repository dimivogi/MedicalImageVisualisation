#include	"OpenGLWindow.h"
#include	"OpenGLController.h"
#include	"resource.h"
#include	<string>




OpenGLWindow::OpenGLWindow() : handleDC(NULL), handleGLRC(NULL)	{};
OpenGLWindow::OpenGLWindow( HINSTANCE inst , const wchar_t* name , HWND parentWindow ) : handleDC(NULL), handleGLRC(NULL) , GenericWindow(inst,name,parentWindow)	{};
OpenGLWindow::~OpenGLWindow()	{};


void OpenGLWindow::closeContext()
{
	HWND	handle;



	if ( !handleDC || !handleGLRC )
		return;

	if ( this->getHandle() == NULL )
		handle = ((OpenGLController*)this->getParameters())->getHandle();
	else
		handle = this->getHandle();


	// delete DC and RC
	wglMakeCurrent(0, 0);
	wglDeleteContext(this->handleGLRC);
	ReleaseDC(handle,this->handleDC);

	handleDC = 0;
	handleGLRC = 0;
};

bool OpenGLWindow::createContext( int colorBits, int depthBits, int stencilBits)
{
	bool	returnValue = false;
	HWND	handle;



	if ( this->getHandle() == NULL )
		handle = ((OpenGLController*)this->getParameters())->getHandle();
	else
		handle = this->getHandle();


	// retrieve a handle to a display device context
	handleDC = GetDC(handle);
	// set pixel format
	if ( !setPixelFormat(this->handleDC, colorBits, depthBits, stencilBits) )
		throw exception("Cannot set a suitable pixel format!");
	else
	{
		// create a new OpenGL rendering context
		handleGLRC = wglCreateContext(this->handleDC);
		returnValue = true;
	}



	ReleaseDC(handle,this->handleDC);
	return returnValue;
};


///////////////////////////////////////////////////////////////////////////////
// choose pixel format
// By default, pdf.dwFlags is set PFD_DRAW_TO_WINDOW, PFD_DOUBLEBUFFER and PFD_SUPPORT_OPENGL.
///////////////////////////////////////////////////////////////////////////////
bool OpenGLWindow::setPixelFormat(HDC hdc, int colorBits, int depthBits, int stencilBits)
{
	int						pixelFormat;
	bool					returnValue = false;
	PIXELFORMATDESCRIPTOR	pfd;



	// find out the best matched pixel format
	pixelFormat = findPixelFormat(hdc, colorBits, depthBits, stencilBits);
	if ( pixelFormat )
	{
		// set members of PIXELFORMATDESCRIPTOR with given mode ID
		DescribePixelFormat(hdc, pixelFormat, sizeof(pfd), &pfd);

		// set the fixel format
		if( SetPixelFormat(hdc, pixelFormat, &pfd) )
			returnValue = true;
	}



	return returnValue;
};

int OpenGLWindow::findPixelFormat(HDC hdc, int colorBits, int depthBits, int stencilBits)
{
	int						bestMode = 0;                           // return value, best pixel format
	int						bestScore = 0;                          // points of best candidate
	PIXELFORMATDESCRIPTOR	pfd;



	// search the available formats for the best mode
	for( int currMode = 1;  DescribePixelFormat(hdc, currMode, sizeof(pfd), &pfd) > 0;  currMode++ )
	{
		int	currScore;



		// ignore if cannot support opengl
		if ( !( pfd.dwFlags  &  PFD_SUPPORT_OPENGL ) )	continue;
		// ignore if cannot render into a window
		if ( !( pfd.dwFlags  &  PFD_DRAW_TO_WINDOW ) )	continue;
		// ignore if cannot support rgba mode
		if ( ( pfd.iPixelType  !=  PFD_TYPE_RGBA )  ||  ( pfd.dwFlags  &  PFD_NEED_PALETTE ) )	continue;
		// ignore if not double buffer
		if ( !( pfd.dwFlags  &  PFD_DOUBLEBUFFER )  )	continue;


		// find the best candidate
		currScore = 0;

		// colour bits
		if ( pfd.cColorBits > colorBits ) currScore++;
		else if ( pfd.cColorBits == colorBits ) currScore += 2;

		// depth bits
		if ( pfd.cDepthBits > depthBits ) currScore++;
		else if ( pfd.cDepthBits == depthBits ) currScore += 2;

		// stencil bits
		if ( pfd.cStencilBits > stencilBits ) currScore++;
		else if ( pfd.cStencilBits == stencilBits ) currScore += 2;

		// alpha bits
		if ( pfd.cAlphaBits > 0 ) currScore++;


		// check if it is the best mode so far
		if ( currScore > bestScore )
		{
			bestScore = currScore;
			bestMode = currMode;
		}
	}



	return bestMode;
}

void OpenGLWindow::swapBuffers()			{ SwapBuffers(this->handleDC); };




HDC		OpenGLWindow::getHandleDC() const	{ return this->handleDC; };
HGLRC	OpenGLWindow::getHandleGLRC() const	{ return this->handleGLRC; };