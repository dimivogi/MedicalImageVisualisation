#include	"DialogWindow.h"
#include	"OpenGLWindow.h"
#include	"TrackBar.h"
#include	"Button.h"
#include	"TextBox.h"
#include	"OptionsController.h"
#include	"DataManager.h"
#include	"..\Visualisation\VectorTemplate.h"


#ifndef	OPTIONS_WINDOW
#define	OPTIONS_WINDOW


// Class contraining the options window
class	OptionsWindow : public DialogWindow
{
	private:

		// Controller responsible for handling the messages sent to the options window
		OptionsController	controller;

		// The minimap window
		OpenGLWindow		map;
		// The transfer function window
		OpenGLWindow		transfer;

		// The transparency button
		Button				transparency;
		// The zoom trackbar
		TrackBar			zoom;	

		// The windowCenter trackbar
		TrackBar			windowCenter;
		// The windowWidth trackbar
		TrackBar			windowWidth;

		// The culling axis trackbars
		TrackBar			leftCulling;
		TrackBar			rightCulling;
		TrackBar			topCulling;
		TrackBar			bottomCulling;
		TrackBar			nearCulling;
		TrackBar			farCulling;

		// The transfer function point text boxes
		TextBox				startR;
		TextBox				startG;
		TextBox				startB;
		TextBox				startA;
		TextBox				startPos;
		TextBox				point1R;
		TextBox				point1G;
		TextBox				point1B;
		TextBox				point1A;
		TextBox				point1Pos;
		TextBox				point2R;
		TextBox				point2G;
		TextBox				point2B;
		TextBox				point2A;
		TextBox				point2Pos;
		TextBox				endR;
		TextBox				endG;
		TextBox				endB;
		TextBox				endA;
		TextBox				endPos;



	public:

		OptionsWindow( HINSTANCE hInstance , HWND parent , DataManager* manager );
		~OptionsWindow()	{};


		// Create the options window
		bool				create();
		// Update a specific trackbar
		void				updateTrackbar( int id , int position );
		// Update all trackbars
		void				updateTrackbars( float zoom , float left , float right , float bottom , float top , float near , float far );
		// Update the transparency button
		void				updateButton( int id , bool flag );
		// Update a specific text box
		void				updateEditbox( int id  , wstring value );
		// Get the values of the transfer function text boxes
		Vector<float,20>	getEditboxes();

		// Get the controller
		OptionsController*	getController();
};


#endif	/* OPTIONS_WINDOW */