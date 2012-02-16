#include	"OptionsWindow.h"
#include	"WindowProcedure.h"
#include	"resource.h"
#include	<iostream>



OptionsWindow::OptionsWindow( HINSTANCE hInstance , HWND parent , DataManager* manager )	:
	DialogWindow(IDD_OPTIONS_MENU , hInstance , L"OptionsMenu" , parent) , 
	controller(this,&map,&transfer,manager,NULL) , 
	map(instance,L"OptionsMinimap",NULL) , 
	transfer(instance,L"OptionsTransferFunction",NULL) , 
	windowCenter() , windowWidth() , 
	leftCulling() , rightCulling() , topCulling() , bottomCulling() , nearCulling() , farCulling()
{
	this->setParameters(&controller);

	this->map.setStyle(WS_CHILD | WS_VISIBLE | WS_CLIPSIBLINGS | WS_CLIPCHILDREN);
	this->map.setWindowClassStyle(CS_OWNDC);
	this->map.setWindowClassProcedure(WndProc);
	this->map.setParameters(controller.getMapController());

	this->transfer.setStyle(WS_CHILD | WS_VISIBLE | WS_CLIPSIBLINGS | WS_CLIPCHILDREN);
	this->transfer.setWindowClassStyle(CS_OWNDC);
	this->transfer.setWindowClassProcedure(WndProc);
	this->transfer.setParameters(controller.getTransferController());


	this->map.setDimensions(340,130);
	this->map.setPosition(11,11);

	this->transfer.setDimensions(325,40);
	this->transfer.setPosition(18,637);
};


bool	OptionsWindow::create()
{
	bool	val1 , val2 , val3 ;

	val1 = DialogWindow::create();
	this->map.setParent(this->handle);
	val2 = this->map.create();
	this->transfer.setParent(this->handle);
	val3 = this->transfer.create();
	BringWindowToTop(this->transfer.getHandle());

	this->transparency.setParent(this->handle);
	this->zoom.setParent(this->handle);
	this->windowCenter.setParent(this->handle);
	this->windowWidth.setParent(this->handle);
	this->leftCulling.setParent(this->handle);
	this->rightCulling.setParent(this->handle);
	this->topCulling.setParent(this->handle);
	this->bottomCulling.setParent(this->handle);
	this->nearCulling.setParent(this->handle);
	this->farCulling.setParent(this->handle);
	this->startR.setParent(this->handle);
	this->startG.setParent(this->handle);
	this->startB.setParent(this->handle);
	this->startA.setParent(this->handle);
	this->startPos.setParent(this->handle);
	this->point1R.setParent(this->handle);
	this->point1G.setParent(this->handle);
	this->point1B.setParent(this->handle);
	this->point1A.setParent(this->handle);
	this->point1Pos.setParent(this->handle);
	this->point2R.setParent(this->handle);
	this->point2G.setParent(this->handle);
	this->point2B.setParent(this->handle);
	this->point2A.setParent(this->handle);
	this->point2Pos.setParent(this->handle);
	this->endR.setParent(this->handle);
	this->endG.setParent(this->handle);
	this->endB.setParent(this->handle);
	this->endA.setParent(this->handle);
	this->endPos.setParent(this->handle);

	this->transparency.setID(IDC_BUTTON_TRANSPARENCY);
	this->zoom.setID(IDC_ZOOM);
	this->windowCenter.setID(IDC_WINDOW_CENTER);
	this->windowWidth.setID(IDC_WINDOW_WIDTH);
	this->leftCulling.setID(IDC_CULLING_LEFT);
	this->rightCulling.setID(IDC_CULLING_RIGHT);
	this->topCulling.setID(IDC_CULLING_TOP);
	this->bottomCulling.setID(IDC_CULLING_BOTTOM);
	this->nearCulling.setID(IDC_CULLING_NEAR);
	this->farCulling.setID(IDC_CULLING_FAR);
	this->startR.setID(IDC_EDIT_START_R);
	this->startG.setID(IDC_EDIT_START_G);
	this->startB.setID(IDC_EDIT_START_B);
	this->startA.setID(IDC_EDIT_START_A);
	this->startPos.setID(IDC_EDIT_START_POS);
	this->point1R.setID(IDC_EDIT_POINT1_R);
	this->point1G.setID(IDC_EDIT_POINT1_G);
	this->point1B.setID(IDC_EDIT_POINT1_B);
	this->point1A.setID(IDC_EDIT_POINT1_A);
	this->point1Pos.setID(IDC_EDIT_POINT1_POS);
	this->point2R.setID(IDC_EDIT_POINT2_R);
	this->point2G.setID(IDC_EDIT_POINT2_G);
	this->point2B.setID(IDC_EDIT_POINT2_B);
	this->point2A.setID(IDC_EDIT_POINT2_A);
	this->point2Pos.setID(IDC_EDIT_POINT2_POS);
	this->endR.setID(IDC_EDIT_END_R);
	this->endG.setID(IDC_EDIT_END_G);
	this->endB.setID(IDC_EDIT_END_B);
	this->endA.setID(IDC_EDIT_END_A);
	this->endPos.setID(IDC_EDIT_END_POS);

	this->zoom.setMinMaxValues(0,130);
	this->leftCulling.setMinMaxValues(0,51);
	this->rightCulling.setMinMaxValues(0,51);
	this->topCulling.setMinMaxValues(0,51);
	this->bottomCulling.setMinMaxValues(0,51);
	this->nearCulling.setMinMaxValues(0,51);
	this->farCulling.setMinMaxValues(0,51);
	this->startR.setMaxLength(3);
	this->startG.setMaxLength(3);
	this->startB.setMaxLength(3);
	this->startA.setMaxLength(3);
	this->startPos.setMaxLength(4);
	this->point1R.setMaxLength(3);
	this->point1G.setMaxLength(3);
	this->point1B.setMaxLength(3);
	this->point1A.setMaxLength(3);
	this->point1Pos.setMaxLength(4);
	this->point2R.setMaxLength(3);
	this->point2G.setMaxLength(3);
	this->point2B.setMaxLength(3);
	this->point2A.setMaxLength(3);
	this->point2Pos.setMaxLength(4);
	this->endR.setMaxLength(3);
	this->endG.setMaxLength(3);
	this->endB.setMaxLength(3);
	this->endA.setMaxLength(3);
	this->endPos.setMaxLength(4);

	this->transparency.setText(L"Not Transparent");
	this->zoom.setValue(0);
	this->leftCulling.setValue(0);
	this->rightCulling.setValue(51);
	this->topCulling.setValue(51);
	this->bottomCulling.setValue(0);
	this->nearCulling.setValue(0);
	this->farCulling.setValue(51);

	this->endR.setText(L"255");
	this->endG.setText(L"255");
	this->endB.setText(L"217");
	this->endA.setText(L"255");
	this->endPos.setText(L"0.35");
	this->point2R.setText(L"250");
	this->point2G.setText(L"237");
	this->point2B.setText(L"199");
	this->point2A.setText(L"0");
	this->point2Pos.setText(L"0.28");
	this->point1R.setText(L"241");
	this->point1G.setText(L"208");
	this->point1B.setText(L"178");
	this->point1A.setText(L"128");
	this->point1Pos.setText(L"0.18");
	this->startR.setText(L"232");
	this->startG.setText(L"179");
	this->startB.setText(L"156");
	this->startA.setText(L"0");
	this->startPos.setText(L"0.07");



	return val1  &&  val2  && val3;
};


void	OptionsWindow::updateTrackbar( int id , int position )
{
	if ( id == IDC_ZOOM )
		this->zoom.setValue(position);
	else if ( id == IDC_WINDOW_CENTER )
		this->windowCenter.setValue(position);
	else if ( id == IDC_WINDOW_WIDTH )
		this->windowWidth.setValue(position);
	else if ( id == IDC_CULLING_LEFT)
		this->leftCulling.setValue(position);
	else if ( id == IDC_CULLING_RIGHT )
		this->rightCulling.setValue(position);
	else if ( id == IDC_CULLING_TOP )
		this->topCulling.setValue(position);
	else if ( id == IDC_CULLING_BOTTOM )
		this->bottomCulling.setValue(position);
	else if ( id == IDC_CULLING_NEAR )
		this->nearCulling.setValue(position);
	else if ( id == IDC_CULLING_FAR )
		this->farCulling.setValue(position);
}

void	OptionsWindow::updateTrackbars( float zoom , float left , float right , float bottom , float top , float nearp , float farp )
{
	float	zoomValue , leftValue , rightValue , bottomValue , topValue , nearValue , farValue;



	zoomValue = zoom*100;
	leftValue = (left+0.01)*100;
	rightValue = (right-0.5)*100;
	bottomValue = (bottom+0.01)*100;
	topValue = (top-0.5)*100;
	nearValue = (nearp+0.01)*100;
	farValue = (farp-0.5)*100;

	this->zoom.setValue(zoomValue);
	this->leftCulling.setValue(leftValue);
	this->rightCulling.setValue(rightValue);
	this->bottomCulling.setValue(bottomValue);
	this->topCulling.setValue(topValue);
	this->nearCulling.setValue(nearValue);
	this->farCulling.setValue(farValue);
}

void	OptionsWindow::updateButton( int id , bool flag )
{
	if ( id == IDC_BUTTON_TRANSPARENCY )
	{
		if ( flag )
			this->transparency.setText(L"Transparent");
		else
			this->transparency.setText(L"Not Transparent");
	}
}

void	OptionsWindow::updateEditbox( int id , wstring value )
{
	wchar_t	change[5];
	bool	wrongValue = false;



	if ( id == IDC_EDIT_START_POS  ||  id == IDC_EDIT_POINT1_POS  ||  id == IDC_EDIT_POINT2_POS  ||  id == IDC_EDIT_END_POS )
	{
		float	val = 0.0;
		float	temp , temp2;



		val = _wtof(value.c_str());

		if ( val < 0.0 )
		{
			wrongValue = true;
			val = 0.0;
		}
		else if ( val > 1.0 )	
		{
			wrongValue = true;
			val = 1.0;
		}

		if ( id == IDC_EDIT_START_POS )
		{
			temp = _wtof(this->point1Pos.getText().c_str());

			if ( temp < val )
			{
				wrongValue = true;
				val = temp;
			}
		}
		else if ( id == IDC_EDIT_POINT1_POS )
		{
			temp = _wtof(this->startPos.getText().c_str());
			temp2 = _wtof(this->point2Pos.getText().c_str());

			if ( val < temp )
			{
				wrongValue = true;
				val = temp;
			}
			else if ( val > temp2 )
			{
				wrongValue = true;
				val = temp2;
			}
		}
		else if ( id == IDC_EDIT_POINT2_POS )
		{
			temp = _wtof(this->point1Pos.getText().c_str());
			temp2 = _wtof(this->endPos.getText().c_str());

			if ( val < temp )
			{
				wrongValue = true;
				val = temp;
			}
			else if ( val > temp2 )
			{
				wrongValue = true;
				val = temp2;
			}
		}

		swprintf(change,5,L"%.2f",val);
	}
	else
	{
		int	val = 0;



		val = _wtoi(value.c_str());

		if ( val < 0 )
		{
			wrongValue = true;
			val = 0;
		}
		else if ( val > 255 )
		{
			wrongValue = true;
			val = 255;
		}

		swprintf(change,4,L"%.1d",val);
	}


	if ( !wrongValue )
	{
		if ( id == IDC_EDIT_START_R )
			this->startR.updateText();
		else if ( id == IDC_EDIT_START_G )
			this->startG.updateText();
		else if ( id == IDC_EDIT_START_B )
			this->startB.updateText();
		else if ( id == IDC_EDIT_START_A )
			this->startA.updateText();
		else if ( id == IDC_EDIT_START_POS )
			this->startPos.updateText();
		else if ( id == IDC_EDIT_POINT1_R )
			this->point1R.updateText();
		else if ( id == IDC_EDIT_POINT1_G )
			this->point1G.updateText();
		else if ( id == IDC_EDIT_POINT1_B )
			this->point1B.updateText();
		else if ( id == IDC_EDIT_POINT1_A )
			this->point1A.updateText();
		else if ( id == IDC_EDIT_POINT1_POS )
			this->point1Pos.updateText();
		else if ( id == IDC_EDIT_POINT2_R )
			this->point2R.updateText();
		else if ( id == IDC_EDIT_POINT2_G )
			this->point2G.updateText();
		else if ( id == IDC_EDIT_POINT2_B )
			this->point2B.updateText();
		else if ( id == IDC_EDIT_POINT2_A )
			this->point2A.updateText();
		else if ( id == IDC_EDIT_POINT2_POS )
			this->point2Pos.updateText();
		else if ( id == IDC_EDIT_END_R )
			this->endR.updateText();
		else if ( id == IDC_EDIT_END_G )
			this->endG.updateText();
		else if ( id == IDC_EDIT_END_B )
			this->endB.updateText();
		else if ( id == IDC_EDIT_END_A )
			this->endA.updateText();
		else if ( id == IDC_EDIT_END_POS )
			this->endPos.updateText();
	}
	else
	{
		if ( id == IDC_EDIT_START_R )
			this->startR.setText(change);
		else if ( id == IDC_EDIT_START_G )
			this->startG.setText(change);
		else if ( id == IDC_EDIT_START_B )
			this->startB.setText(change);
		else if ( id == IDC_EDIT_START_A )
			this->startA.setText(change);
		else if ( id == IDC_EDIT_START_POS )
			this->startPos.setText(change);
		else if ( id == IDC_EDIT_POINT1_R )
			this->point1R.setText(change);
		else if ( id == IDC_EDIT_POINT1_G )
			this->point1G.setText(change);
		else if ( id == IDC_EDIT_POINT1_B )
			this->point1B.setText(change);
		else if ( id == IDC_EDIT_POINT1_A )
			this->point1A.setText(change);
		else if ( id == IDC_EDIT_POINT1_POS )
			this->point1Pos.setText(change);
		else if ( id == IDC_EDIT_POINT2_R )
			this->point2R.setText(change);
		else if ( id == IDC_EDIT_POINT2_G )
			this->point2G.setText(change);
		else if ( id == IDC_EDIT_POINT2_B )
			this->point2B.setText(change);
		else if ( id == IDC_EDIT_POINT2_A )
			this->point2A.setText(change);
		else if ( id == IDC_EDIT_POINT2_POS )
			this->point2Pos.setText(change);
		else if ( id == IDC_EDIT_END_R )
			this->endR.setText(change);
		else if ( id == IDC_EDIT_END_G )
			this->endG.setText(change);
		else if ( id == IDC_EDIT_END_B )
			this->endB.setText(change);
		else if ( id == IDC_EDIT_END_A )
			this->endA.setText(change);
		else if ( id == IDC_EDIT_END_POS )
			this->endPos.setText(change);
	}
}

Vector<float,20>	OptionsWindow::getEditboxes()
{
	float	sr , sg , sb , sa , spos ;
	float	p1r , p1g , p1b , p1a , p1pos ;
	float	p2r , p2g , p2b , p2a , p2pos ;
	float	er , eg , eb , ea , epos;



	sr = (_wtof(this->startR.getText().c_str())/255);
	sg = (_wtof(this->startG.getText().c_str())/255);
	sb = (_wtof(this->startB.getText().c_str())/255);
	sa = (_wtof(this->startA.getText().c_str())/255);
	spos = _wtof(this->startPos.getText().c_str());

	p1r = (_wtof(this->point1R.getText().c_str())/255);
	p1g = (_wtof(this->point1G.getText().c_str())/255);
	p1b = (_wtof(this->point1B.getText().c_str())/255);
	p1a = (_wtof(this->point1A.getText().c_str())/255);
	p1pos = _wtof(this->point1Pos.getText().c_str());

	p2r = (_wtof(this->point2R.getText().c_str())/255);
	p2g = (_wtof(this->point2G.getText().c_str())/255);
	p2b = (_wtof(this->point2B.getText().c_str())/255);
	p2a = (_wtof(this->point2A.getText().c_str())/255);
	p2pos = _wtof(this->point2Pos.getText().c_str());

	er = (_wtof(this->endR.getText().c_str())/255);
	eg = (_wtof(this->endG.getText().c_str())/255);
	eb = (_wtof(this->endB.getText().c_str())/255);
	ea = (_wtof(this->endA.getText().c_str())/255);
	epos = _wtof(this->endPos.getText().c_str());

	return Vector<float,20>(20,sr,sg,sb,sa,spos,p1r,p1g,p1b,p1a,p1pos,p2r,p2g,p2b,p2a,p2pos,er,eg,eb,ea,epos);
}



OptionsController*	OptionsWindow::getController()	{ return &(this->controller); };