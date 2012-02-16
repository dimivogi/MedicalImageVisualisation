#include	"OptionsController.h"
#include	"OptionsWindow.h"
#include	"TrackBar.h"
#include	"resource.h"
#include	"..\Visualisation\VectorTemplate.h"




OptionsController::OptionsController() : GenericController() , mapController() , transferController()	{};
OptionsController::OptionsController( DialogWindow* subject , OpenGLWindow* map , OpenGLWindow* transfer , DataManager* model ,  void (*threadFunc)( OpenGLController* ) )  :
	GenericController(subject) , mapController(map,model,threadFunc) , transferController(transfer,model,threadFunc)	{};
OptionsController::~OptionsController()	{};


void				OptionsController::setManager( DataManager* model )	{ this->mapController.setManager(model); };
DataManager*		OptionsController::getManager()						{ return this->mapController.getManager(); };
OpenGLController*	OptionsController::getMapController()				{ return &this->mapController; };
OpenGLController*	OptionsController::getTransferController()			{ return &this->transferController; };


int	OptionsController::command( LPARAM msg , WPARAM wParam , LPARAM lParam )
{
	static bool	flag = false;

	

	switch ( LOWORD(wParam) )
	{
		case IDC_BUTTON_TRANSPARENCY :	
										if ( HIWORD(wParam) == BN_CLICKED  &&  this->controlSubject->getHandle() )	
										{	
											flag = !flag;
											((OptionsWindow*)this->controlSubject)->updateButton(LOWORD(wParam),flag);
											flag = this->mapController.getManager()->getTransparency();
											flag = !flag;
											this->mapController.getManager()->setTransparency(flag);
										}
										break;
		case IDC_EDIT_START_R :
		case IDC_EDIT_START_G :
		case IDC_EDIT_START_B :
		case IDC_EDIT_START_A :
		case IDC_EDIT_START_POS :
		case IDC_EDIT_POINT1_R :
		case IDC_EDIT_POINT1_G :
		case IDC_EDIT_POINT1_B :
		case IDC_EDIT_POINT1_A :
		case IDC_EDIT_POINT1_POS :
		case IDC_EDIT_POINT2_R :
		case IDC_EDIT_POINT2_G :
		case IDC_EDIT_POINT2_B :
		case IDC_EDIT_POINT2_A :
		case IDC_EDIT_POINT2_POS :
		case IDC_EDIT_END_R :
		case IDC_EDIT_END_G :
		case IDC_EDIT_END_B :
		case IDC_EDIT_END_A :
		case IDC_EDIT_END_POS :	
										if ( HIWORD(wParam) == EN_CHANGE  &&  this->controlSubject->getHandle() )
										{
											wchar_t				buff[5];
											Vector<float,20>	values;



											SendMessageW((HWND)lParam,WM_GETTEXT,5,(LPARAM)buff);
											((OptionsWindow*)this->controlSubject)->updateEditbox(LOWORD(wParam),buff);
											values = ((OptionsWindow*)this->controlSubject)->getEditboxes();
											this->mapController.getManager()->changeTransferFunctionPoints(values);
										}
										break;
	}



	return 0;
}


int	OptionsController::horizontalScroll(WPARAM wParam, LPARAM lParam)
{
	HWND trackbarHandle = (HWND)lParam;
	int position = HIWORD(wParam);



	if(trackbarHandle)
	{
		// Get control ID
		int trackbarId = ::GetDlgCtrlID(trackbarHandle);

		switch(LOWORD(wParam))
		{
			// WM_LBUTTONUP
			case TB_THUMBPOSITION :
									break;
			// VK_RIGHT, VK_DOWN
			case TB_LINEUP :
									break;
			// VK_LEFT, VK_UP
			case TB_LINEDOWN :	
									break;
			// VK_HOME
			case TB_TOP :
									break;
			// VK_END
			case TB_BOTTOM :	
									break;
			// VK_PRIOR
			case TB_PAGEUP :
									break;
			// VK_NEXT
			case TB_PAGEDOWN:	
									break;


			// Slider was dragged
			case TB_THUMBTRACK :	
									((OptionsWindow*)this->controlSubject)->updateTrackbar(trackbarId,position);

									if ( trackbarId == IDC_ZOOM )
										this->mapController.getManager()->zoomToCamera(position);
									else if ( trackbarId == IDC_CULLING_LEFT )
										this->mapController.getManager()->moveCullingLineTo(0,position);
									else if ( trackbarId == IDC_CULLING_RIGHT )
										this->mapController.getManager()->moveCullingLineTo(1,position);
									else if ( trackbarId == IDC_CULLING_BOTTOM )
										this->mapController.getManager()->moveCullingLineTo(2,position);
									else if ( trackbarId == IDC_CULLING_TOP )
										this->mapController.getManager()->moveCullingLineTo(3,position);
									else if ( trackbarId == IDC_CULLING_NEAR )
										this->mapController.getManager()->moveCullingLineTo(4,position);
									else if ( trackbarId == IDC_CULLING_FAR )
										this->mapController.getManager()->moveCullingLineTo(5,position);
									else if ( trackbarId == IDC_WINDOW_CENTER  ||  trackbarId == IDC_WINDOW_WIDTH )
										this->mapController.getManager()->lockUpdates();

									break;

			// WM_KEYUP
			case TB_ENDTRACK :
									position = SendMessageW(trackbarHandle, TBM_GETPOS, 0, 0);
									((OptionsWindow*)this->controlSubject)->updateTrackbar(trackbarId,position);

									if ( trackbarId == IDC_ZOOM )
										this->mapController.getManager()->zoomToCamera(position);
									else if ( trackbarId == IDC_WINDOW_CENTER )
									{
										this->mapController.getManager()->changeWindowCenter(position);
										this->mapController.getManager()->unlockUpdates();
									}
									else if ( trackbarId == IDC_WINDOW_WIDTH )
									{
										this->mapController.getManager()->changeWindowWidth(position);
										this->mapController.getManager()->unlockUpdates();
									}
									else if ( trackbarId == IDC_CULLING_LEFT )
										this->mapController.getManager()->moveCullingLineTo(0,position);
									else if ( trackbarId == IDC_CULLING_RIGHT )
										this->mapController.getManager()->moveCullingLineTo(1,position);
									else if ( trackbarId == IDC_CULLING_BOTTOM )
										this->mapController.getManager()->moveCullingLineTo(2,position);
									else if ( trackbarId == IDC_CULLING_TOP )
										this->mapController.getManager()->moveCullingLineTo(3,position);
									else if ( trackbarId == IDC_CULLING_NEAR )
										this->mapController.getManager()->moveCullingLineTo(4,position);
									else if ( trackbarId == IDC_CULLING_FAR )
										this->mapController.getManager()->moveCullingLineTo(5,position);

									break;
		}
	}



	return 0;
}

void	OptionsController::updateTrackbars()
{
	float	zoom , left , right , bottom , top , nearp , farp;

	zoom = this->mapController.getManager()->getZoom();
	this->mapController.getManager()->getCullingLines(left,right,bottom,top,nearp,farp);
	((OptionsWindow*)this->controlSubject)->updateTrackbars(zoom,left,right,bottom,top,nearp,farp);
}