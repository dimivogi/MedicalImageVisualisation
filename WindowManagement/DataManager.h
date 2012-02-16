#include	"..\DICOM\DicomParser.h"
#include	"..\Visualisation\Shader.h"
#include	"..\Visualisation\VectorTemplate.h"



#ifndef	DATA_MANAGER
#define	DATA_MANAGER


#define	MAX_KEYS	256

// Class responsible for handling all OpenGL content and states
class DataManager
{
	private:

		// OpenGL windows
		// OpenGL windows height and width
		float		winWidth;
		float		winHeight;
		float		minimapWidth;
		float		minimapHeight;
		float		transferWidth;
		float		transferHeight;

		// DICOM Images
		// Images are locked, pending updates
		bool					locked;
		// Full update pending
		bool					fullUpdate;
		// Update is pending
		bool					updatePending;
		// Minimap and Main Display Window update status
		bool					updated[2];
		// Loaded Images list
		list<DicomFileImage>	dicomImages;
		// Image list smallest value set
		unsigned long			windowSmallest;
		// Image list center value set
		unsigned long			windowCenter;
		// Image list max value set
		unsigned long			windowWidth;
		// Image list real max value
		unsigned long			imageMaxValue;
		// Image width
		unsigned long			imageWidth;
		// Image height
		unsigned long			imageHeight;
		// Number of images
		unsigned long			imageDepth;
		// Image thickness + Image Spacing
		unsigned long			imageSize;

		// Transfer Function
		// Update pending
		bool					transferUpdatePending;
		// Transfer Function window, Minimap Window and Main Display Window update status 
		bool					transferUpdated[3];
		// Transfer Function Points
		Vector<float,5>			transferStartPoint;
		Vector<float,5>			transferPoint1Point;
		Vector<float,5>			transferPoint2Point;
		Vector<float,5>			transferEndPoint;

		// Main Display Window
		// The ray casting shader
		Shader					shader;
		// OpenGL IDs
		GLint					backLoc;
		GLint					frontLoc;
		GLint					transferLoc;
		GLint					volumeLoc;
		GLint					transparentLoc;
		GLint					xCullingLoc;
		GLint					yCullingLoc;
		GLint					zCullingLoc;
		GLint					cameraPosLoc;
		GLint					lightPosLoc;
		GLint					widthLoc;
		GLint					heightLoc;
		GLint					depthLoc;

		// Minimap Window
		// The ray casting shader
		Shader					miniShader;
		// OpenGL IDs
		GLint					miniBackLoc;
		GLint					miniFrontLoc;
		GLint					miniTransferLoc;
		GLint					miniVolumeLoc;
		GLint					miniTransparentLoc;
		GLint					miniXCullingLoc;
		GLint					miniYCullingLoc;
		GLint					miniZCullingLoc;
		GLint					miniCameraPosLoc;
		GLint					miniLightPosLoc;
		GLint					miniWidthLoc;
		GLint					miniHeightLoc;
		GLint					miniDepthLoc;

		// Main Display Window Textures
		GLuint					frameBuffer;
		GLuint					renderBuffer;
		GLuint					backRender;
		GLuint					finalRender;
		GLuint					transferFunction;
		GLuint					dicomData;

		// Minimap Window Textures
		GLuint					miniFrameBuffer;
		GLuint					miniRenderBuffer;
		GLuint					miniBackRender;
		GLuint					miniFinalRender;
		GLuint					miniTransferFunction;
		GLuint					miniDicomData;

		// Transfer Function Window Textures
		GLuint					transTransferFunction;

		// Left and Right mouse button state
		bool					mouseState[2];
		// Culling axis movement state
		bool					cullingState[6];
		// Whether we are currently moving a culling axis
		bool					cull;
		// Surrounding Volume size
		float					size;
		// Camera Til (X axis)
		float					tilt;
		// Camera Pan (Y axis)
		float					pan;
		// Current zoom value
		float					zoom;
		// Transparent Mode
		bool					transparent;
		// Original Camera Position
		Vector<float,3>			camera;
		// Current Camera Position for GPU use
		Vector<GLfloat,3>		cameraPosition;
		// Mouse Position
		Vector<float,2>			mouse;
		// Culling Axis Position
		Vector<float,6>			culling;
		// Original Light Position
		Vector<GLfloat,4>		light;
		// Current Light Position for GPU use
		Vector<GLfloat,4>		lightPosition;
		// Background Colour
		Vector<float,4>			background;


		// Adjust DICOM image colours, in order to display correctly on screen
		template < typename type >
		void					adjustColour( void* data , unsigned long minValue , unsigned long maxValue , unsigned long typeSize , char monochrome );
		// Create Textures from the DICOM images
		void					texturizeData( GLenum type , bool minimap );
		// Update the DICOM Images textures
		void					updateDicomDataTexture( bool findWinCenter , bool findSize , bool minimap );
		// Create/Update the transfer function texture
		void					createTransferFunction( int window );
		
		// Create / Delete the OpenGL used
		// Main Display Window
		void					createTextures();
		void					deleteTextures();
		// Minimap Window
		void					createMinimapTextures();
		void					deleteMinimapTextures();
		// Transfer Function Window
		void					createTransferTextures();
		void					deleteTransferTextures();
		
		// Manage Frame and Render Buffers
		// Main Display Window
		void					createBuffers();
		void					deleteBuffers();
		void					enableBuffers();
		void					disableBuffers();
		void					updateBuffers();
		// Minimap Window
		void					createMinimapBuffers();
		void					deleteMinimapBuffers();
		void					enableMinimapBuffers();
		void					disableMinimapBuffers();
		void					updateMinimapBuffers();

		// Draw a vertex at location (x,y,z) with colour (x,y,z) and texture coordinates (x,y,z)
		static void				drawVertex( float x , float y , float z );
		// Draw a vertex at location (x,y,z) with colour and texture coordinates (r,g,b)
		static void				drawVertex( float x , float y , float z , float r , float g , float b );
		// Draw a square with edge size, with center the position (0,0,zPos)
		static void				drawSquare( float size , float zPos);
		// Create a cube with edge size cubeSize, scaled in the x,y,z axis by (cubeWidth/max(cubeHeight,cubeWidth,cubeDepth),cubeHeight/max(cubeHeight,cubeWidth,cubeDepth),cubeDepth/max(cubeHeight,cubeWidth,cubeDepth)
		static void				drawCube( float cubeWidth , float cubeHeight , float cubeDepth , float cubeSize );
		// Create a cube with edge size cubeSize
		void					drawCube( float cubeSize );
		// front face culling
		void					drawBack();
		// back face culling
		void					drawFront();
		// implement ray casting
		void					drawFinal();

		// Update the camera position
		void					updateCameraPosition();
		// Set the camera position
		void					setCamera();
		// Set the camera position for minimap use (ignoring zoom)
		void					setMinimapCamera();
		// Set orthocanonical projection
		void					setOrtho();

		// move a culling line
		void					moveCullingLine( int line , float amount );



	public:

		DataManager();
		~DataManager();


		// Draw functions
		// Main Display Window
		void					draw();
		// Minimap Window
		void					drawMinimap();
		// Transfer Function Window
		void					drawTransfer();
		// Setup Functions
		// Init does all the necessary initializations and creations
		// Setup does all the necessary OpenGL state initializations
		// Main Window
		bool					init();
		void					setup();
		// Minimap Window
		void					minimapSetup();
		// Transfer Function Window
		void					transferSetup();
		// Destruction Functions
		// Main Window
		void					destroy();
		// Minimap Window
		void					minimapDestroy();
		// Transfer Function Window
		void					transferDestroy();


		// Window Resize Functions
		// Main Display Window
		void					resize( int width , int height );
		// Minimap Window
		void					resizeMinimap( int width , int height );
		// Transfer Function Window
		void					resizeTransfer( int width , int height );
		// Viewport Functions
		// Main Window
		void					setViewport();
		// Minimap Window
		void					setMinimapViewport();
		// Transfer Function Window
		void					setTransferViewport();


		// Camera manipulation functions
		void					tiltCamera( float amount );
		void					panCamera( float amount );
		void					zoomCamera( float amount );
		void					zoomToCamera( int position );
		// Get current zoom level
		float					getZoom();


		// Key and Mouse Button functions
		void					keyUp( unsigned char key , int x , int y);
		void					mouseMove( int x , int y );
		void					mouseDown( int button , int state , int x , int y );
		void					getMouseStatus( bool& left , bool& right );


		// DICOM Image functions
		// Load DICOM folder
		void					getDicomData( wstring path );
		// Check if there is an update pending
		bool					isUpdatePending();
		// Parse a pending update
		void					parsePendingUpdate( bool minimap );
		// Disallow update requests
		void					lockUpdates();
		// Allow update requests 
		void					unlockUpdates();
		// Check if update requests are allowed
		bool					isLocked();
		// Request an update
		void					requestUpdate();
		// Change the windowCenter value and update the image textures (requestUpdate is automatically called)
		void					changeWindowCenter( unsigned long amount );
		// Change the windowWidth value and update the image textures (requestUpdate is automatically called)
		void					changeWindowWidth( unsigned long amount );
		// Get image windowSmallest, windowCenter, windowWidth
		void					getWindowStats( unsigned long& s , unsigned long& c , unsigned long& w );

		// Transfer Function
		// Check if there is a transfer function update pending
		bool					isTransferUpdatePending();
		// Request a transfer function texture update
		void					requestTransferUpdate();
		// Parse any pending updates
		void					parseTransferPendingUpdate( int window );
		// Move the key points of the transfer function and update the transfer function (requestTransferUpdate is automatically called)
		void					changeTransferFunctionPoints( Vector<float,20>&	values );

		
		// Set transparency mode
		void					setTransparency( bool transparent );
		// Get transperency mode
		bool					getTransparency();
		// Move a culling line to position
		void					moveCullingLineTo( int line , int position );
		// Get the position of all culling lines
		void					getCullingLines( float& left , float& right , float& bottom , float& top , float& near , float& far );
};


#endif	/* DATA_MANAGER */