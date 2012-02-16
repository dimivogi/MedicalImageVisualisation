#include	"GenericWindow.h"



#ifndef	OPENGL_WINDOW
#define	OPENGL_WINDOW


// Class containing a window with OpenGL content
class	OpenGLWindow	: public GenericWindow
{
	private:

		// Handle to device context
		HDC			handleDC; 
		// Handle to OpenGL rendering context
		HGLRC		handleGLRC;
		
		// Set the pixel format
		static bool	setPixelFormat(HDC hdc, int colorBits, int depthBits, int stencilBits);
		// Find best matched pixel format ID
		static int	findPixelFormat(HDC hdc, int colorbits, int depthBits, int stencilBits);



	public:

		OpenGLWindow();
		OpenGLWindow( HINSTANCE inst , const wchar_t* name , HWND parentWindow );
		~OpenGLWindow();


		// Create OpenGL rendering context
		bool		createContext(int colorBits, int depthBits, int stencilBits);  
		// Close OpenGL rendering context
		void		closeContext();
		// Swap rendering buffers
		void		swapBuffers();


		// Get handle functions
		HDC			getHandleDC() const;
		HGLRC		getHandleGLRC() const;
};


#endif	/* OPENGL_WINDOW */