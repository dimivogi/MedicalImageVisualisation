#include	<string>
#include	<limits>
#include	<cmath>
#include	<GL/glew.h>
#include	<GL/glut.h>
#include	"DataManager.h"
#include	"..\Visualisation\VisualisationDefinitions.h"




static const wstring fragPath = VSL_SHADER_FRAGMENT;
static const wstring vertPath = L""; //VSL_SHADER_VERTEX



DataManager::DataManager() : 
	winWidth(0) , winHeight(0) , minimapWidth(0) , minimapHeight(0) , transferWidth(0) , transferHeight(0) , 
	windowSmallest(numeric_limits<unsigned long>::min()) , windowCenter(numeric_limits<unsigned long>::min()) , windowWidth(numeric_limits<unsigned long>::min()) , 
	imageMaxValue(0) , imageWidth(1) , imageHeight(1) , imageDepth(1) , updatePending(false) , fullUpdate(false) , locked(false) ,
	transferStartPoint(5,0.91,0.7,0.61,0.0,0.07) , transferPoint1Point(5,0.98,0.93,0.78,0.50,0.28) , transferPoint2Point(5,0.98,0.93,0.78,0.50,0.28) , transferEndPoint(5,1.0,1.0,0.85,1.0,0.35) , transferUpdatePending(false) , 
	shader(fragPath,vertPath) , backLoc(-1) , frontLoc(-1) , transferLoc(-1) , volumeLoc(-1) , transparentLoc(-1) , xCullingLoc(-1) , yCullingLoc(-1) , zCullingLoc(-1) , cameraPosLoc(-1) , lightPosLoc(-1) , widthLoc(-1) , heightLoc(-1) , depthLoc(-1) ,  
	frameBuffer(0) , renderBuffer(0) , backRender(0) , finalRender(0) , transferFunction(0) , dicomData(0) , 
	miniShader(fragPath,vertPath) , miniBackLoc(-1) , miniFrontLoc(-1) , miniTransferLoc(-1) , miniVolumeLoc(-1) , miniTransparentLoc(-1) , miniXCullingLoc(-1) , miniYCullingLoc(-1) , miniZCullingLoc(-1) , miniCameraPosLoc(-1) , miniLightPosLoc(-1) , miniWidthLoc(-1) , miniHeightLoc(-1) , miniDepthLoc(-1) ,
	miniFrameBuffer(0) , miniRenderBuffer(0) , miniBackRender(0) , miniFinalRender(0) , miniTransferFunction(0) , miniDicomData(0) , 
	size(1.0) , tilt(90.0) , pan(180.0) , cull(false) , zoom(0.0) , transparent(false) , 
	camera(3,0.0,0.0,size*2) , cameraPosition(3,0.0,0.0,size*2) , mouse(2,0.0,0.0) , culling(6,-0.01,1.01,-0.01,1.01,-0.01,1.01) , light(4,1.0,1.0,1.0,0.0) , lightPosition(4,1.0,1.0,1.0,0.0) , background(4,0.0,0.0,0.0,1.0) 
{
	this->updated[0] = false;
	this->updated[1] = false;

	this->transferUpdated[0] = false;
	this->transferUpdated[1] = false;
	this->transferUpdated[2] = false;

	this->mouseState[0] = false;
	this->mouseState[1] = false;

	for ( int i = 0;  i < 6;  i++ )
		this->cullingState[i] = false;
}

DataManager::~DataManager()	{}


void	DataManager::createTransferFunction( int window )
{
	bool			iterated;
	float			rVar , gVar , bVar , aVar;
	float			startR , startG , startB , startA , startPos;
	float			finishR , finishG , finishB , finishA , finishPos;
	float			dist;
	unsigned char	transferFunc[100*4];



	memset(transferFunc,'\0',sizeof(transferFunc));

	for ( int i = 0;  i < 3;  i++ )
	{
		if ( i == 0 )
		{
			startR = this->transferStartPoint.r();
			startG = this->transferStartPoint.g();
			startB = this->transferStartPoint.b();
			startA = this->transferStartPoint.a();
			startPos = this->transferStartPoint.getElement(4)*100;
			finishR = this->transferPoint1Point.r();
			finishG = this->transferPoint1Point.g();
			finishB = this->transferPoint1Point.b();
			finishA = this->transferPoint1Point.a();
			finishPos = this->transferPoint1Point.getElement(4)*100;
		}
		else
		{
			if ( iterated == true )
			{
				if ( i == 1 )
				{
					startR = this->transferPoint1Point.r();
					startG = this->transferPoint1Point.g();
					startB = this->transferPoint1Point.b();
					startA = this->transferPoint1Point.a();
					startPos = this->transferPoint1Point.getElement(4)*100;
				}
				else if ( i == 2 )
				{
					startR = this->transferPoint2Point.r();
					startG = this->transferPoint2Point.g();
					startB = this->transferPoint2Point.b();
					startA = this->transferPoint2Point.a();
					startPos = this->transferPoint2Point.getElement(4)*100;
				}
			}

			
			if ( i == 1 )
			{
				finishR = this->transferPoint2Point.r();
				finishG = this->transferPoint2Point.g();
				finishB = this->transferPoint2Point.b();
				finishA = this->transferPoint2Point.a();
				finishPos = this->transferPoint2Point.getElement(4)*100;
			}
			else if ( i == 2 )
			{
				finishR = this->transferEndPoint.r();
				finishG = this->transferEndPoint.g();
				finishB = this->transferEndPoint.b();
				finishA = this->transferEndPoint.a();
				finishPos = this->transferEndPoint.getElement(4)*100;
			}
		}
	

		rVar = startR;
		gVar = startG;
		bVar = startB;
		aVar = startA;
		dist = ceil(finishPos-startPos);
		iterated = false;
		for ( int i = ceil(startPos*4);  i < ceil(finishPos*4);  i+=4 )
		{
			transferFunc[i] = rVar*255;
			transferFunc[i+1] = gVar*255;
			transferFunc[i+2] = bVar*255;
			transferFunc[i+3] = aVar*255;

			rVar += (finishR - startR) / dist;
			gVar += (finishG - startG) / dist;
			bVar += (finishB - startB) / dist;
			aVar += (finishA - startA) / dist;

			iterated = true;
		}
	}

	for ( int i = ceil(finishPos*4);  i < 100*4;  i+= 4 )
	{
		transferFunc[i] = rVar*255;
		transferFunc[i+1] = gVar*255;
		transferFunc[i+2] = bVar*255;
		transferFunc[i+3] = aVar*255;
	}



	if ( window == 0 )
		glBindTexture(GL_TEXTURE_1D,this->transferFunction);
	else if ( window == 1 )
		glBindTexture(GL_TEXTURE_1D,this->miniTransferFunction);
	else
		glBindTexture(GL_TEXTURE_1D,this->transTransferFunction);

	glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_REPLACE);
	glTexParameteri(GL_TEXTURE_1D,GL_TEXTURE_WRAP_S,GL_CLAMP_TO_BORDER);
	glTexParameteri(GL_TEXTURE_1D,GL_TEXTURE_WRAP_T,GL_CLAMP_TO_BORDER);
	glTexParameteri(GL_TEXTURE_1D,GL_TEXTURE_WRAP_R,GL_CLAMP_TO_BORDER);
	glTexParameteri(GL_TEXTURE_1D,GL_TEXTURE_MIN_FILTER,GL_LINEAR);
	glTexParameteri(GL_TEXTURE_1D,GL_TEXTURE_MAG_FILTER,GL_LINEAR);
	glTexImage1D(GL_TEXTURE_1D,0,GL_RGBA,100,0,GL_RGBA,GL_UNSIGNED_BYTE,transferFunc);
};


template < typename type >
void	DataManager::adjustColour( void* data , unsigned long minValue , unsigned long maxValue , unsigned long typeSize , char monochrome )
{
	type*	color = (type*)data;



	if ( monochrome > 0 )
	{
		int	offset = 0;
		for ( list<DicomFileImage>::iterator it = dicomImages.begin();  it != dicomImages.end();  it++ )
		{
			memcpy(color+offset,(*it).getPixelData(),(*it).getPixelDataSize());

			for ( unsigned int i = 0;  i < (*it).getPixelDataSize()/typeSize;  i ++ )
			{
				if ( color[i+offset] <= (windowCenter-0.5-(windowWidth-1)/2) )
					color[i+offset] = minValue;
				else if ( color[i+offset] > (windowCenter-0.5+(windowWidth-1)/2) )
					color[i+offset] = maxValue;
				else
					color[i+offset]  = ((color[i+offset]-(windowCenter-0.5))/(windowWidth-1)+0.5)*(maxValue-minValue)+0;

				if ( monochrome == 1 )
					color[i+offset] = abs((long)(color[i+offset] - maxValue));

			}

			offset += (*it).getPixelDataSize()/typeSize;
		}
	}
	else
	{
		int	offset = 0;
		for ( list<DicomFileImage>::iterator it = dicomImages.begin();  it != dicomImages.end();  it++ )
		{
			memcpy(color+offset,(*it).getPixelData(),(*it).getPixelDataSize());

			for ( unsigned int i = 0;  i < (*it).getPixelDataSize()/typeSize;  i += 4 )
			{
				for ( unsigned int j = 0;  j < 3;  j++ )
				{
					if ( color[i+j+offset] <= (windowCenter-0.5-(windowWidth-1)/2) )
						color[i+j+offset] = minValue;
					else if ( color[i+j+offset] > (windowCenter-0.5+(windowWidth-1)/2) )
						color[i+j+offset] = maxValue;
					else
						color[i+j+offset]  = ((color[i+j+offset]-(windowCenter-0.5))/(windowWidth-1)+0.5)*(maxValue-minValue)+0;
				}

			}

			offset += 3*(*it).getPixelDataSize()/typeSize;
		}
	}
}

void	DataManager::texturizeData( GLenum type , bool minimap )
{
	void*	data;
	unsigned long	minValue , maxValue , typeSize;
	GLenum	format , internalFormat;
	string	photometric = (*dicomImages.begin()).getPhotometricInterpretation();



	if ( type == GL_UNSIGNED_BYTE )
	{
		minValue = numeric_limits<unsigned char>::min();
		maxValue = numeric_limits<unsigned char>::max();
		typeSize = sizeof(unsigned char);

		if ( photometric == "MONOCROME1"  ||  photometric == "MONOCHROME2" )
		{
			data = (void*)new unsigned char[imageWidth*imageHeight*dicomImages.size()];
			memset(data,'\0',imageWidth*imageHeight*dicomImages.size());
		}
		else if ( photometric == "RGB" )
		{
			data = (void*)new unsigned char[imageWidth*imageHeight*dicomImages.size()*3];
			memset(data,'\0',imageWidth*imageHeight*dicomImages.size()*3);
		}
	}
	else if ( type == GL_UNSIGNED_SHORT )
	{
		minValue = numeric_limits<unsigned short>::min();
		maxValue = numeric_limits<unsigned short>::max();
		typeSize = sizeof(unsigned short);

		if ( photometric == "MONOCROME1"  ||  photometric == "MONOCHROME2" )
		{
			data = (void*)new unsigned short[imageWidth*imageHeight*dicomImages.size()];
			memset(data,'\0',imageWidth*imageHeight*dicomImages.size());
		}
		else if ( photometric == "RGB" )
		{
			data = (void*)new unsigned short[imageWidth*imageHeight*dicomImages.size()*3];
			memset(data,'\0',imageWidth*imageHeight*dicomImages.size()*3);
		}
	}
	else
	{
		minValue = numeric_limits<unsigned long>::min();
		maxValue = numeric_limits<unsigned long>::max();
		typeSize = sizeof(unsigned long);

		if ( photometric == "MONOCROME1"  ||  photometric == "MONOCHROME2" )
		{
			data = (void*)new unsigned long[imageWidth*imageHeight*dicomImages.size()];
			memset(data,'\0',imageWidth*imageHeight*dicomImages.size());
		}
		else if ( photometric == "RGB" )
		{
			data = (void*)new unsigned long[imageWidth*imageHeight*dicomImages.size()*3];
			memset(data,'\0',imageWidth*imageHeight*dicomImages.size()*3);
		}
	}


	if ( photometric == "MONOCROME1"  ||  photometric == "MONOCHROME2" )
	{
		char	mode;

		if ( photometric == "MONOCHROME1" )
			mode = 1;
		else
			mode = 2;
		
		format = GL_LUMINANCE;
	
		if ( type == GL_UNSIGNED_BYTE )
		{
			internalFormat = GL_LUMINANCE8;
			adjustColour<unsigned char>(data,minValue,maxValue,typeSize,mode);
		}
		else if ( type == GL_UNSIGNED_SHORT )
		{
			internalFormat = GL_LUMINANCE16;
			adjustColour<unsigned short>(data,minValue,maxValue,typeSize,mode);
		}
		else
		{
			internalFormat = GL_LUMINANCE;
			adjustColour<unsigned long>(data,minValue,maxValue,typeSize,mode);
		}
	}
	else if ( photometric == "RGB" )
	{
		format = GL_RGB;

		if ( type == GL_UNSIGNED_BYTE )
		{
			internalFormat = GL_RGB8;
			adjustColour<unsigned char>(data,minValue,maxValue,typeSize,0);
		}
		else if ( type == GL_UNSIGNED_SHORT )
		{
			internalFormat = GL_RGB16;
			adjustColour<unsigned short>(data,minValue,maxValue,typeSize,0);
		}
		else
		{
			internalFormat = GL_RGB;
			adjustColour<unsigned long>(data,minValue,maxValue,typeSize,0);
		}
	}
	
	imageMaxValue = maxValue;



	if ( !minimap )
		glBindTexture(GL_TEXTURE_3D,this->dicomData);
	else
		glBindTexture(GL_TEXTURE_3D,this->miniDicomData);
	
	glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_REPLACE);
	glTexParameteri(GL_TEXTURE_3D,GL_TEXTURE_WRAP_S,GL_CLAMP_TO_BORDER);
	glTexParameteri(GL_TEXTURE_3D,GL_TEXTURE_WRAP_T,GL_CLAMP_TO_BORDER);
	glTexParameteri(GL_TEXTURE_3D,GL_TEXTURE_WRAP_R,GL_CLAMP_TO_BORDER);
	glTexParameteri(GL_TEXTURE_3D,GL_TEXTURE_MIN_FILTER,GL_LINEAR);
	glTexParameteri(GL_TEXTURE_3D,GL_TEXTURE_MAG_FILTER,GL_LINEAR);
	glTexImage3D(GL_TEXTURE_3D,0,internalFormat,imageWidth,imageHeight,dicomImages.size(),0,format,type,data);


	delete[] data;
};

void	DataManager::updateDicomDataTexture( bool findWinCenter , bool findSize , bool minimap )
{
	list<DicomFileImage>::iterator	it;



	if ( findWinCenter ) 
	{
		unsigned long	wincenter = 0;
		unsigned long	winwidth = 0;
		unsigned long	smallest = numeric_limits<unsigned long>::max();
		unsigned long	largest = 0;	



		for ( it = dicomImages.begin();  it != dicomImages.end();  it++)
		{
			if ( (*it).getLargestValue() > 0 )
			{
				if ( largest < (*it).getLargestValue() )
				{
					largest = (*it).getLargestValue();

					if ( (*it).getWindowCenter() > 0 )
						wincenter = (*it).getWindowCenter();
					else
						wincenter = largest/2;

					if ( (*it).getWindowWidth() > 0 )
						winwidth = (*it).getWindowWidth();
					else
						winwidth = largest;
				}
			}


			if ( (*it).getSmallestValue() >= 0 )
			{
				if ( smallest > (*it).getSmallestValue() )
					smallest = (*it).getSmallestValue();
			}
		}

		if ( wincenter > 0  &&  winwidth > 0 )
		{
			this->windowSmallest = smallest;
			this->windowCenter = wincenter;
			this->windowWidth = winwidth;
		}
	}


	if ( findSize )
	{
		unsigned long	width = 0;
		unsigned long	height = 0;

		for ( it = dicomImages.begin();  it != dicomImages.end();  it++)
		{
			if ( (*it).getColumns() > width )
				width = (*it).getColumns();

			if ( (*it).getRows() > height )
				height = (*it).getRows();
		}

		this->imageWidth = width;
		this->imageHeight = height;
		this->imageDepth = ((*dicomImages.begin()).getSliceSpacing()+(*dicomImages.begin()).getSliceThickness())*dicomImages.size() - (*dicomImages.begin()).getSliceSpacing();

		this->imageSize = this->imageWidth;
		if ( this->imageHeight > this->imageSize )	this->imageSize = this->imageHeight;
		if ( this->imageDepth > this->imageSize )	this->imageSize = this->imageDepth;
	}


	unsigned long	bitsAllocated = (*dicomImages.begin()).getBitsAllocated();
	switch ( bitsAllocated/8 )
	{
		case	1 :	
					this->texturizeData(GL_UNSIGNED_BYTE,minimap); break;
		case	2 :	
					this->texturizeData(GL_UNSIGNED_SHORT,minimap); break;
		case	4 :	
					this->texturizeData(GL_UNSIGNED_INT,minimap); break;
	}	
};

void	DataManager::getDicomData( wstring path )
{
	try
	{
		this->dicomImages = dicomParser(path);
		this->updatePending = true;
		this->fullUpdate = true;
		this->updated[0] = false;
		this->updated[1] = false;
	}
	catch( exception& e )
	{
		cerr << "Error:\tThe specified DICOM Folder could not be processed. Reason:\t" << e.what() << endl;
	}
};

bool	DataManager::isUpdatePending()	{ return this->updatePending; };
void	DataManager::parsePendingUpdate( bool minimap )
{
	if ( !this->dicomImages.empty() )
	{
		if ( this->updatePending )
		{
			if ( minimap )
			{
				if ( !updated[1] )
				{
					this->updateDicomDataTexture(false,false,true);
					updated[1] = true;
				}
			}
			else
			{
				if ( !updated[0] )
				{
					if ( this->fullUpdate )
						this->updateDicomDataTexture(true,true,false);
					else
						this->updateDicomDataTexture(false,false,false);

					updated[0] = true;
				}
			}

			if ( updated[0] && updated[1] )
			{
				this->updatePending = false;
				this->fullUpdate = false;
			}
		}
	}
}

void	DataManager::lockUpdates()		{ this->locked = true; };
void	DataManager::unlockUpdates()	{ this->locked = false; };
bool	DataManager::isLocked()			{ return this->locked; };
void	DataManager::requestUpdate()
{
	this->updatePending = true;
	this->updated[0] = false;
	this->updated[1] = false; 
};
void	DataManager::changeWindowCenter( unsigned long amount )
{
	if ( amount < this->windowSmallest )	amount = this->windowSmallest;
	else if ( amount > this->windowWidth )	amount = this->windowWidth;

	this->windowCenter = amount;
	this->requestUpdate();
};

void	DataManager::changeWindowWidth( unsigned long amount )
{
	if ( amount < this->windowSmallest )	amount = this->windowSmallest;
	else if ( amount > numeric_limits<unsigned long>::max() )	amount = numeric_limits<unsigned long>::max();

	this->windowWidth = amount;
	if ( this->windowCenter > amount )	this->changeWindowCenter(amount);

	this->requestUpdate();
};

void	DataManager::getWindowStats( unsigned long& small , unsigned long& center , unsigned long& width )
{
	small = this->windowSmallest;
	center = this->windowCenter;
	width = this->windowWidth;
};


bool	DataManager::isTransferUpdatePending()	{ return this->transferUpdatePending; };
void	DataManager::requestTransferUpdate()	
{	
	this->transferUpdatePending = true;	
	this->transferUpdated[0] = false;
	this->transferUpdated[1] = false;
	this->transferUpdated[2] = false; 
};
void	DataManager::parseTransferPendingUpdate( int window )
{
	if ( this->transferUpdatePending )
	{
		if ( window >= 0  &&  window <= 2 )
		{
			if ( !this->transferUpdated[window] )
			{
				this->createTransferFunction(window);
				this->transferUpdated[window] = true;
			}
		}
		
		if ( this->transferUpdated[0]  &&  this->transferUpdated[1]  &&  this->transferUpdated[2] )
			this->transferUpdatePending = false;
	}
};

void	DataManager::changeTransferFunctionPoints( Vector<float,20>& values )
{
	this->transferStartPoint.setElement(0,values.getElement(0));
	this->transferStartPoint.setElement(1,values.getElement(1));
	this->transferStartPoint.setElement(2,values.getElement(2));
	this->transferStartPoint.setElement(3,values.getElement(3));
	this->transferStartPoint.setElement(4,values.getElement(4));
	this->transferPoint1Point.setElement(0,values.getElement(5));
	this->transferPoint1Point.setElement(1,values.getElement(6));
	this->transferPoint1Point.setElement(2,values.getElement(7));
	this->transferPoint1Point.setElement(3,values.getElement(8));
	this->transferPoint1Point.setElement(4,values.getElement(9));
	this->transferPoint2Point.setElement(0,values.getElement(10));
	this->transferPoint2Point.setElement(1,values.getElement(11));
	this->transferPoint2Point.setElement(2,values.getElement(12));
	this->transferPoint2Point.setElement(3,values.getElement(13));
	this->transferPoint2Point.setElement(4,values.getElement(14));
	this->transferEndPoint.setElement(0,values.getElement(15));
	this->transferEndPoint.setElement(1,values.getElement(16));
	this->transferEndPoint.setElement(2,values.getElement(17));
	this->transferEndPoint.setElement(3,values.getElement(18));
	this->transferEndPoint.setElement(4,values.getElement(19));

	this->requestTransferUpdate();
};


		
void	DataManager::createTextures()
{
	glGenTextures(1,&(this->transferFunction));
	glGenTextures(1,&(this->dicomData));
	glGenTextures(1,&(this->backRender));
	glGenTextures(1,&(this->finalRender));
};

void	DataManager::deleteTextures()
{
	glDeleteTextures(1,&(this->dicomData));
	glDeleteTextures(1,&(this->backRender));
	glDeleteTextures(1,&(this->finalRender));
}

void	DataManager::createMinimapTextures()
{
	glGenTextures(1,&(this->miniTransferFunction));
	glGenTextures(1,&(this->miniDicomData));
	glGenTextures(1,&(this->miniBackRender));
	glGenTextures(1,&(this->miniFinalRender));
};

void	DataManager::deleteMinimapTextures()
{
	glDeleteTextures(1,&(this->miniDicomData));
	glDeleteTextures(1,&(this->miniBackRender));
	glDeleteTextures(1,&(this->miniFinalRender));
}

void	DataManager::createTransferTextures()
{
	glGenTextures(1,&this->transTransferFunction);
}

void	DataManager::deleteTransferTextures()
{
	glDeleteTextures(1,&this->transTransferFunction);
}



		
void	DataManager::createBuffers()
{
	glGenFramebuffers(1,&(this->frameBuffer));
	glGenRenderbuffers(1,&(this->renderBuffer));

	this->updateBuffers();
};

void	DataManager::deleteBuffers()
{
	if ( this->frameBuffer > 0 )
		glDeleteFramebuffers(1,&(this->frameBuffer));

	if ( this->renderBuffer > 0 )
		glDeleteRenderbuffers(1,&(this->renderBuffer));
};

void	DataManager::enableBuffers()
{
	glBindFramebuffer(GL_FRAMEBUFFER,this->frameBuffer);
	glBindRenderbuffer(GL_RENDERBUFFER,this->renderBuffer);
};

void	DataManager::disableBuffers()
{
	glBindRenderbuffer(GL_RENDERBUFFER,0);
	glBindFramebuffer(GL_FRAMEBUFFER,0);
};

void	DataManager::updateBuffers()
{
	this->enableBuffers();


	glBindTexture(GL_TEXTURE_2D,this->backRender);
	glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_REPLACE);
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_S,GL_CLAMP_TO_BORDER);
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_T,GL_CLAMP_TO_BORDER);
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR);
	glTexImage2D(GL_TEXTURE_2D,0,GL_RGBA16F,this->winWidth,this->winHeight,0,GL_RGBA,GL_FLOAT,NULL);

	glBindTexture(GL_TEXTURE_2D,this->finalRender);
	glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_REPLACE);
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_S,GL_CLAMP_TO_BORDER);
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_T,GL_CLAMP_TO_BORDER);
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR);
	glTexImage2D(GL_TEXTURE_2D,0,GL_RGBA16F,this->winWidth,this->winHeight,0,GL_RGBA,GL_FLOAT,NULL);

	glRenderbufferStorage(GL_RENDERBUFFER,GL_DEPTH_COMPONENT,this->winWidth,this->winHeight);
	glFramebufferRenderbuffer(GL_FRAMEBUFFER,GL_DEPTH_ATTACHMENT,GL_RENDERBUFFER,this->renderBuffer);


	this->disableBuffers();
};

void	DataManager::createMinimapBuffers()
{
	glGenFramebuffers(1,&(this->miniFrameBuffer));
	glGenRenderbuffers(1,&(this->miniRenderBuffer));

	this->updateMinimapBuffers();
};

void	DataManager::deleteMinimapBuffers()
{
	if ( this->miniFrameBuffer > 0 )
		glDeleteFramebuffers(1,&(this->miniFrameBuffer));

	if ( this->miniRenderBuffer > 0 )
		glDeleteRenderbuffers(1,&(this->miniRenderBuffer));
};

void	DataManager::enableMinimapBuffers()
{
	glBindFramebuffer(GL_FRAMEBUFFER,this->miniFrameBuffer);
	glBindRenderbuffer(GL_RENDERBUFFER,this->miniRenderBuffer);
};

void	DataManager::disableMinimapBuffers()
{
	glBindRenderbuffer(GL_RENDERBUFFER,0);
	glBindFramebuffer(GL_FRAMEBUFFER,0);
};

void	DataManager::updateMinimapBuffers()
{
	this->enableMinimapBuffers();


	glBindTexture(GL_TEXTURE_2D,this->miniBackRender);
	glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_REPLACE);
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_S,GL_CLAMP_TO_BORDER);
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_T,GL_CLAMP_TO_BORDER);
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR);
	glTexImage2D(GL_TEXTURE_2D,0,GL_RGBA16F,this->minimapWidth,this->minimapHeight,0,GL_RGBA,GL_FLOAT,NULL);

	glBindTexture(GL_TEXTURE_2D,this->miniFinalRender);
	glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_REPLACE);
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_S,GL_CLAMP_TO_BORDER);
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_T,GL_CLAMP_TO_BORDER);
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR);
	glTexImage2D(GL_TEXTURE_2D,0,GL_RGBA16F,this->minimapWidth,this->minimapHeight,0,GL_RGBA,GL_FLOAT,NULL);

	glRenderbufferStorage(GL_RENDERBUFFER,GL_DEPTH_COMPONENT,this->minimapWidth,this->minimapHeight);
	glFramebufferRenderbuffer(GL_FRAMEBUFFER,GL_DEPTH_ATTACHMENT,GL_RENDERBUFFER,this->miniRenderBuffer);


	this->disableMinimapBuffers();
};


void	DataManager::drawVertex( float x , float y , float z )
{
	glColor3f(x,y,z);
	glTexCoord3f(x,y,z);
	glVertex3f(x,y,z);
}

void	DataManager::drawVertex( float x , float y , float z , float r , float g , float b )
{
	glColor3f(r,g,b);
	glTexCoord3f(r,g,b);
	glVertex3f(x,y,z);
}

void	DataManager::drawSquare( float size , float zPos)
{
	glDisable(GL_DEPTH_TEST);
		glBegin(GL_QUADS);
			drawVertex(0,0,zPos);
			drawVertex(0,size,zPos);
			drawVertex(size,size,zPos);
			drawVertex(size,0,zPos);
		glEnd();
	glEnable(GL_DEPTH_TEST);
}

void	DataManager::drawCube( float cubeSize )
{
	this->drawCube(cubeSize,cubeSize,cubeSize,this->size);
}
void	DataManager::drawCube( float cubeWidth , float cubeHeight , float cubeDepth , float cubeSize )
{
	float	size;
	float	width , height , depth;



	size = cubeWidth;
	if ( cubeHeight > size )	size = cubeHeight;
	if ( cubeDepth > size )		size = cubeDepth;

	width = cubeSize*cubeWidth/size;
	height = cubeSize*cubeHeight/size;
	depth = cubeSize*cubeDepth/size;

	glBegin(GL_QUADS);

		// Back
		glNormal3f(0.0,0.0,-1.0);
		drawVertex(0,0,0,0,0,0);
		drawVertex(0,height,0,0,1,0);
		drawVertex(width,height,0,1,1,0);
		drawVertex(width,0,0,1,0,0);


		// Front
		glNormal3f(0.0,0.0,1.0);
		drawVertex(0,0,depth,0,0,1);
		drawVertex(width,0,depth,1,0,1);
		drawVertex(width,height,depth,1,1,1);
		drawVertex(0,height,depth,0,1,1);

		// Top
		glNormal3f(0.0,1.0,0.0);
		drawVertex(0,height,0,0,1,0);
		drawVertex(0,height,depth,0,1,1);
		drawVertex(width,height,depth,1,1,1);
		drawVertex(width,height,0,1,1,0);

		// Bottom
		glNormal3f(0.0,-1.0,0.0);
		drawVertex(0,0,0,0,0,0);
		drawVertex(width,0,0,1,0,0);
		drawVertex(width,0,depth,1,0,1);
		drawVertex(0,0,depth,0,0,1);

		// Left
		glNormal3f(-1.0,0.0,0.0);
		drawVertex(0,0,0,0,0,0);
		drawVertex(0,0,depth,0,0,1);
		drawVertex(0,height,depth,0,1,1);
		drawVertex(0,height,0,0,1,0);

		// Right
		glNormal3f(1.0,0.0,0.0);
		drawVertex(width,0,0,1,0,0);
		drawVertex(width,height,0,1,1,0);
		drawVertex(width,height,depth,1,1,1);
		drawVertex(width,0,depth,1,0,1);

	glEnd();
}

void	DataManager::drawBack()
{
	glFramebufferTexture2D(GL_FRAMEBUFFER,GL_COLOR_ATTACHMENT0,GL_TEXTURE_2D,this->backRender,0);
	glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT );

	glEnable(GL_CULL_FACE);
		glCullFace(GL_FRONT);
		this->drawCube(this->imageWidth,this->imageHeight,this->imageDepth,this->size);
	glDisable(GL_CULL_FACE);
}

void	DataManager::drawFront()
{
	glFramebufferTexture2D(GL_FRAMEBUFFER,GL_COLOR_ATTACHMENT0,GL_TEXTURE_2D,this->finalRender,0);
	glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);
	

	glEnable(GL_CULL_FACE);
		glCullFace(GL_BACK);
		this->drawCube(this->imageWidth,this->imageHeight,this->imageDepth,this->size);
	glDisable(GL_CULL_FACE);
}

void	DataManager::drawFinal()
{
	glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);

	glLoadIdentity();
	this->setOrtho();


	this->shader.ActivateProgram();
	
	glActiveTexture(GL_TEXTURE0+this->backRender);
	glBindTexture(GL_TEXTURE_2D,this->backRender);
	glUniform1i(this->backLoc,this->backRender);

	glActiveTexture(GL_TEXTURE0+this->finalRender);
	glBindTexture(GL_TEXTURE_2D,this->finalRender);
	glUniform1i(this->frontLoc,this->finalRender);

	glActiveTexture(GL_TEXTURE0+this->transferFunction);
	glBindTexture(GL_TEXTURE_1D,this->transferFunction);
	glUniform1i(this->transferLoc,this->transferFunction);

	glActiveTexture(GL_TEXTURE0+this->dicomData);
	glBindTexture(GL_TEXTURE_3D,this->dicomData);
	glUniform1i(this->volumeLoc,this->dicomData);

	glUniform1i(this->transparentLoc,this->transparent);

	glUniform2f(this->xCullingLoc,this->culling.getElement(0),this->culling.getElement(1));
	glUniform2f(this->yCullingLoc,this->culling.getElement(2),this->culling.getElement(3));
	glUniform2f(this->zCullingLoc,this->culling.getElement(4),this->culling.getElement(5));

	glUniform3f(this->cameraPosLoc,this->cameraPosition.x(),this->cameraPosition.y(),this->cameraPosition.z());
	glUniform3f(this->lightPosLoc,this->lightPosition.x(),this->lightPosition.y(),this->lightPosition.z());

	glUniform1f(this->widthLoc,this->imageWidth);
	glUniform1f(this->heightLoc,this->imageHeight);
	glUniform1f(this->depthLoc,this->dicomImages.size());


	glEnable(GL_TEXTURE_2D);
		this->drawSquare(size,0.0);
	glDisable(GL_TEXTURE_2D);
	this->shader.DeactivateProgram();
}


void	DataManager::updateCameraPosition()
{
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	glRotatef(this->pan,0,1,0);
	glRotatef(this->tilt,1,0,0);

	GLfloat	matrix[16];
	glGetFloatv(GL_MODELVIEW_MATRIX,matrix);

	GLfloat	w = matrix[3]*this->camera.x()+matrix[7]*this->camera.y()+matrix[8]*this->camera.z() + matrix[15];
	this->cameraPosition.X((matrix[0]*this->camera.x()+matrix[4]*this->camera.y()+matrix[8]*this->camera.z()+matrix[12])/w);
	this->cameraPosition.Y((matrix[1]*this->camera.x()+matrix[5]*this->camera.y()+matrix[9]*this->camera.z()+matrix[13])/w);
	this->cameraPosition.Z((matrix[2]*this->camera.x()+matrix[6]*this->camera.y()+matrix[10]*this->camera.z()+matrix[14])/w);

	GLfloat l = matrix[3]*this->light.x()+matrix[7]*this->light.y()+matrix[8]*this->light.z() + matrix[15];
	this->lightPosition.X((matrix[0]*this->light.x()+matrix[4]*this->light.y()+matrix[8]*this->light.z()+matrix[12])/l);
	this->lightPosition.Y((matrix[1]*this->light.x()+matrix[5]*this->light.y()+matrix[9]*this->light.z()+matrix[13])/l);
	this->lightPosition.Z((matrix[2]*this->light.x()+matrix[6]*this->light.y()+matrix[10]*this->light.z()+matrix[14])/l);
}

void	DataManager::setCamera()
{
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(45.0,this->winWidth/this->winHeight,0.001,9999);

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	gluLookAt(this->camera.x(),this->camera.y(),this->camera.z() - this->zoom,0.0,0.0,0.0,0.0,1.0,0.0);
}

void	DataManager::setMinimapCamera()
{
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(45.0,this->winWidth/this->winHeight,0.001,9999);

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	gluLookAt(this->camera.x(),this->camera.y(),this->camera.z(),0.0,0.0,0.0,0.0,1.0,0.0);
}

void	DataManager::setOrtho()
{
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluOrtho2D(0,1,0,1);
	glMatrixMode(GL_MODELVIEW);
}


void	DataManager::draw()
{
	this->updateCameraPosition();
	this->setCamera();


	glRotatef(this->pan,0,1,0);
	glRotatef(this->tilt,1,0,0);
	glTranslatef(-(this->size*this->imageWidth)/(2*this->imageSize),-(this->size*this->imageHeight)/(2*this->imageSize),-(this->size*this->imageDepth)/(2*this->imageSize));

	this->enableBuffers();
		this->drawBack();
		this->drawFront();
	this->disableBuffers();
	this->drawFinal();
}

void	DataManager::drawMinimap()
{
	this->setMinimapCamera();


	glRotatef(this->pan,0,1,0);
	glRotatef(this->tilt,1,0,0);
	glTranslatef(-(this->size*this->imageWidth)/(2*this->imageSize),-(this->size*this->imageHeight)/(2*this->imageSize),-(this->size*this->imageDepth)/(2*this->imageSize));

	this->enableMinimapBuffers();

		glFramebufferTexture2D(GL_FRAMEBUFFER,GL_COLOR_ATTACHMENT0,GL_TEXTURE_2D,this->miniBackRender,0);
		glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT );

		glEnable(GL_CULL_FACE);
			glCullFace(GL_FRONT);
			this->drawCube(this->imageWidth,this->imageHeight,this->imageDepth,this->size);
		glDisable(GL_CULL_FACE);	


		glFramebufferTexture2D(GL_FRAMEBUFFER,GL_COLOR_ATTACHMENT0,GL_TEXTURE_2D,this->miniFinalRender,0);
		glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);
	
		glEnable(GL_CULL_FACE);
			glCullFace(GL_BACK);
			this->drawCube(this->imageWidth,this->imageHeight,this->imageDepth,this->size);
		glDisable(GL_CULL_FACE);

	this->disableMinimapBuffers();


	glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);

	glLoadIdentity();
	this->setOrtho();


	this->miniShader.ActivateProgram();
	
	glActiveTexture(GL_TEXTURE0+this->miniBackRender);
	glBindTexture(GL_TEXTURE_2D,this->miniBackRender);
	glUniform1i(this->miniBackLoc,this->miniBackRender);

	glActiveTexture(GL_TEXTURE0+this->miniFinalRender);
	glBindTexture(GL_TEXTURE_2D,this->miniFinalRender);
	glUniform1i(this->miniFrontLoc,this->miniFinalRender);

	glActiveTexture(GL_TEXTURE0+this->miniTransferFunction);
	glBindTexture(GL_TEXTURE_1D,this->miniTransferFunction);
	glUniform1i(this->miniTransferLoc,this->miniTransferFunction);

	glActiveTexture(GL_TEXTURE0+this->miniDicomData);
	glBindTexture(GL_TEXTURE_3D,this->miniDicomData);
	glUniform1i(this->miniVolumeLoc,this->miniDicomData);

	glUniform1i(this->miniTransparentLoc,this->transparent);

	glUniform2f(this->miniXCullingLoc,this->culling.getElement(0),this->culling.getElement(1));
	glUniform2f(this->miniYCullingLoc,this->culling.getElement(2),this->culling.getElement(3));
	glUniform2f(this->miniZCullingLoc,this->culling.getElement(4),this->culling.getElement(5));

	glUniform3f(this->miniCameraPosLoc,this->cameraPosition.x(),this->cameraPosition.y(),this->cameraPosition.z());
	glUniform3f(this->miniLightPosLoc,this->lightPosition.x(),this->lightPosition.y(),this->lightPosition.z());

	glUniform1f(this->miniWidthLoc,this->imageWidth);
	glUniform1f(this->miniHeightLoc,this->imageHeight);
	glUniform1f(this->miniDepthLoc,this->dicomImages.size());

	glEnable(GL_TEXTURE_2D);
		this->drawSquare(size,0.0);
	glDisable(GL_TEXTURE_2D);
	this->miniShader.DeactivateProgram();


	glBegin(GL_LINES);
		glColor3f(1,0,0);
		glVertex2f( -((1.3*size-abs(zoom))/(1.3*size))*0.4+0.4002  , ((1.3*size-abs(zoom))/(1.3*size))*0.4+0.5998 );
		glVertex2f( ((1.3*size-abs(zoom))/(1.3*size))*0.4+0.5998  , ((1.3*size-abs(zoom))/(1.3*size))*0.4+0.5998 );

		glColor3f(1,0,0);
		glVertex2f( -((1.3*size-abs(zoom))/(1.3*size))*0.4+0.4002  , -((1.3*size-abs(zoom))/(1.3*size))*0.4+0.4002 );
		glVertex2f( ((1.3*size-abs(zoom))/(1.3*size))*0.4+0.5998  , -((1.3*size-abs(zoom))/(1.3*size))*0.4+0.4002 );

		glColor3f(1,0,0);
		glVertex2f( -((1.3*size-abs(zoom))/(1.3*size))*0.4+0.4003  , -((1.3*size-abs(zoom))/(1.3*size))*0.4+0.4002 );
		glVertex2f( -((1.3*size-abs(zoom))/(1.3*size))*0.4+0.4003  , ((1.3*size-abs(zoom))/(1.3*size))*0.4+0.5998 );

		glColor3f(1,0,0);
		glVertex2f( ((1.3*size-abs(zoom))/(1.3*size))*0.4+0.5999  , -((1.3*size-abs(zoom))/(1.3*size))*0.4+0.4002 );
		glVertex2f( ((1.3*size-abs(zoom))/(1.3*size))*0.4+0.5999  , ((1.3*size-abs(zoom))/(1.3*size))*0.4+0.5998 );

	glEnd();
}

void	DataManager::drawTransfer()
{
	glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);

	glEnable(GL_TEXTURE_1D);
		this->drawSquare(size,0.0);
	glDisable(GL_TEXTURE_1D);
}


bool	DataManager::init()
{
	GLenum error = glewInit();
	if ( error != GLEW_OK )
	{
		cerr	<< "Error:\tCannot initialize GLEW library. Terminating!" << endl
				<< "Error Detals:\t" << glewGetErrorString(error) << endl;
		return false;
	}

	if ( !GLEW_VERSION_2_1)
	{
		cerr << "GLEW: OpenGL version 3.1 not supported" << endl;
		return false;
	}


	this->createTextures();
	this->createBuffers();
	this->createTransferFunction(0);
	

	this->shader.CreateAndCompileShaders();
	this->shader.CreateAndLinkProgram();
	this->shader.printProgramLog();

	this->backLoc = glGetUniformLocation(this->shader.getProgram(),"backside");
	this->frontLoc = glGetUniformLocation(this->shader.getProgram(),"frontside");
	this->transferLoc = glGetUniformLocation(this->shader.getProgram(),"transferfunc");
	this->volumeLoc = glGetUniformLocation(this->shader.getProgram(),"volume");
	this->transparentLoc = glGetUniformLocation(this->shader.getProgram(),"transparent");
	this->xCullingLoc = glGetUniformLocation(this->shader.getProgram(),"xculling");
	this->yCullingLoc = glGetUniformLocation(this->shader.getProgram(),"yculling");
	this->zCullingLoc = glGetUniformLocation(this->shader.getProgram(),"zculling");
	this->cameraPosLoc = glGetUniformLocation(this->shader.getProgram(),"eyeposition");
	this->lightPosLoc = glGetUniformLocation(this->shader.getProgram(),"lightposition");
	this->widthLoc = glGetUniformLocation(this->shader.getProgram(),"width");
	this->heightLoc = glGetUniformLocation(this->shader.getProgram(),"height");
	this->depthLoc = glGetUniformLocation(this->shader.getProgram(),"depth");



	return true;
}

void	DataManager::setup()
{
	glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_BLEND);
	

	glClearColor(this->background.r(),this->background.g(),this->background.b(),this->background.a());
	glClearStencil(0.0);
	glClearDepth(1.0);

	glBlendFunc (GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
}

void	DataManager::minimapSetup()
{
	glewInit();

	this->createMinimapTextures();
	this->createMinimapBuffers();
	this->createTransferFunction(1);

	this->miniShader.CreateAndCompileShaders();
	this->miniShader.CreateAndLinkProgram();
	this->miniShader.printProgramLog();

	this->miniBackLoc = glGetUniformLocation(this->miniShader.getProgram(),"backside");
	this->miniFrontLoc = glGetUniformLocation(this->miniShader.getProgram(),"frontside");
	this->miniTransferLoc = glGetUniformLocation(this->miniShader.getProgram(),"transferfunc");
	this->miniVolumeLoc = glGetUniformLocation(this->miniShader.getProgram(),"volume");
	this->miniTransparentLoc = glGetUniformLocation(this->miniShader.getProgram(),"transparent");
	this->miniXCullingLoc = glGetUniformLocation(this->miniShader.getProgram(),"xculling");
	this->miniYCullingLoc = glGetUniformLocation(this->miniShader.getProgram(),"yculling");
	this->miniZCullingLoc = glGetUniformLocation(this->miniShader.getProgram(),"zculling");
	this->miniCameraPosLoc = glGetUniformLocation(this->miniShader.getProgram(),"eyeposition");
	this->miniLightPosLoc = glGetUniformLocation(this->miniShader.getProgram(),"lightposition");
	this->miniWidthLoc = glGetUniformLocation(this->miniShader.getProgram(),"width");
	this->miniHeightLoc = glGetUniformLocation(this->miniShader.getProgram(),"height");
	this->miniDepthLoc = glGetUniformLocation(this->miniShader.getProgram(),"depth");


	this->setup();
}

void	DataManager::transferSetup()
{
	this->createTransferTextures();
	this->createTransferFunction(2);
	this->setup();
	this->setOrtho();
}

void	DataManager::destroy()
{
	this->deleteTextures();
	this->deleteBuffers();
	this->shader.DeactivateProgram();
	this->shader.DetachShaders();
	this->shader.DeleteShaders();
	this->shader.DeleteProgram();
}

void	DataManager::minimapDestroy()
{
	this->deleteMinimapTextures();
	this->deleteMinimapBuffers();
	this->miniShader.DeactivateProgram();
	this->miniShader.DetachShaders();
	this->miniShader.DeleteShaders();
	this->miniShader.DeleteProgram();
}

void	DataManager::transferDestroy()
{
	this->deleteTransferTextures();
}


void	DataManager::resize( int width , int height )
{
	if ( width < 1 )	width = 1;
	if ( height < 1 )	height = 1;

	this->winWidth = width;
	this->winHeight = height;

	this->setViewport();
	
	if ( this->frameBuffer > 0 )
		this->updateBuffers();
};

void	DataManager::resizeMinimap( int width , int height )
{
	if ( width < 1 )	width = 1;
	if ( height < 1 )	height = 1;

	this->minimapWidth = width;
	this->minimapHeight = height;

	this->setMinimapViewport();

	if ( (GLint)this->miniFrameBuffer > 0 )
		this->updateMinimapBuffers();
};

void	DataManager::resizeTransfer( int width , int height )
{
	if ( width < 1 )	width = 1;
	if ( height < 1 )	height = 1;

	this->transferWidth = width;
	this->transferHeight = height;

	this->setTransferViewport();
}

void	DataManager::setViewport()
{
	glViewport(0,0,this->winWidth,this->winHeight);
	this->setCamera();
};

void	DataManager::setMinimapViewport()
{
	glViewport(0,0,this->minimapWidth,this->minimapHeight);
	this->setMinimapCamera();
};

void	DataManager::setTransferViewport()
{
	glViewport(0,0,this->transferWidth,this->transferHeight);
	this->setOrtho();
}


void	DataManager::tiltCamera( float amount )
{
	this->tilt += amount;

	if ( this->tilt > 360 )		this->tilt -= 360;
	else if ( this->tilt < 0 )	this->tilt += 360;

}
void	DataManager::panCamera( float amount )
{
	this->pan += amount;

	if ( this->pan > 360 )	this->pan -= 360;
	else if ( this->pan < 0 )	this->pan += 360;
}

void	DataManager::zoomCamera( float amount )
{
	this->zoom += amount;

	if ( this->zoom > 1.3*size )	this->zoom = 1.3*size;
	else if ( this->zoom < 0 )	this->zoom = 0;
}

void	DataManager::zoomToCamera( int position )
{
	if ( position > 130 )	position = 130;
	else if ( position < 0 )	position = 0;


	this->zoom = this->size*position/100;
}

float	DataManager::getZoom()	{ return this->zoom/this->size; };


void	DataManager::keyUp( unsigned char key , int x , int y)
{
	switch ( key )
	{
		case ' ' :	
					this->transparent = !this->transparent;
					break;
	}
};

void	DataManager::mouseMove( int x , int y )
{
	if ( this->cull ) 
	{
		if ( this->cullingState[4] )
			this->moveCullingLine(4,0.0025*(this->mouse.x() - x + this->mouse.y() - y));
		else if ( this->cullingState[5] )
			this->moveCullingLine(5,0.0025*(this->mouse.x() - x + this->mouse.y() - y));

		this->mouse.X(x);
		this->mouse.Y(y);
	}
	else if ( this->mouseState[1] )
	{
		this->panCamera(0.25*(x - this->mouse.x()));
		this->tiltCamera(0.25*(this->mouse.y() - y));
		this->mouse.X(x);
		this->mouse.Y(y);
	}
	else if ( this->mouseState[0] )
	{
		if ( abs(this->mouse.x() - x) > abs(this->mouse.y() - y) )
		{
			if ( this->cullingState[0] )
				this->moveCullingLine(0,(x - this->mouse.x())*0.001 );
			else if ( this->cullingState[1] )
				this->moveCullingLine(1,(x - this->mouse.x())*0.001 );
		}
		else
		{
			if ( this->cullingState[2] )
				this->moveCullingLine(2,(this->mouse.y() - y)*0.001 );
			else if ( this->cullingState[3] )
				this->moveCullingLine(3,(this->mouse.y() - y)*0.001 );
		}

		this->mouse.X(x);
		this->mouse.Y(y);
	}
};

void	DataManager::mouseDown( int button , int state , int x , int y )
{
	if ( button == GLUT_LEFT_BUTTON )
	{
		if ( state == GLUT_UP )
		{
			if ( !this->mouseState[1] )
				this->cull = false;

			for ( int i = 0;  i < 6;  i++ )
				this->cullingState[i] = false;

			this->mouseState[0] = false;
		}
		else if ( state == GLUT_DOWN )
		{
			if ( this->mouseState[1] )
			{
				this->cull = true;
			
				if ( y < this->winHeight/2 )
					this->cullingState[5] = true;
				else
					this->cullingState[4] = true;

			}

			if ( y < this->winHeight/2 )
				this->cullingState[3] = true;
			else
				this->cullingState[2] = true;

			if ( x > this->winWidth/2 )
				this->cullingState[1] = true;
			else
				this->cullingState[0] = true;

			this->mouse.X(x);
			this->mouse.Y(y);
			
			this->mouseState[0] = true;
		}
	}
	else if ( button == GLUT_RIGHT_BUTTON )
	{
		if ( state == GLUT_UP )
		{
			if ( !this->mouseState[0] )
				this->cull = false;

			this->mouseState[1] = false;
		}
		else if ( state == GLUT_DOWN )
		{
			if ( this->mouseState[0] )
			{
				this->cull = true;
			
				if ( y < this->winHeight/2 )
					this->cullingState[5] = true;
				else
					this->cullingState[4] = true;

			}
			
			this->mouse.X(x);
			this->mouse.Y(y);

			this->mouseState[1] = true;
		}
	}
}

void	DataManager::getMouseStatus( bool& left , bool& right )
{
	left = mouseState[0];
	right = mouseState[1];
}


void	DataManager::setTransparency( bool transparent )
{
	this->transparent = transparent;
}

bool	DataManager::getTransparency()	{ return this->transparent; };

void	DataManager::moveCullingLine( int line , float amount )
{
	float	current;



	if ( line >= 0  &&  line < 6 )
	{
		current = this->culling.getElement(line);
		current += amount;

		if ( line == 0  ||  line == 2  ||  line == 4 )
		{
			if ( current < -0.01 )	current = -0.01;
			else if ( current > 0.5 )	current = 0.5;
		}
		else
		{
			if ( current < 0.5 )	current =  0.5;
			else if ( current > 1.01 )	current = 1.01;
		}

		culling.setElement(line,current);
	}
}

void	DataManager::moveCullingLineTo( int line , int position )
{
	if ( line >= 0  && line < 6 )
	{
		if ( position > 51 )	position = 51;
		else if ( position < 0 )	position = 0;

		if ( (line%2) == 0 )
			culling.setElement(line,(float)position/100 - 0.01);
		else
			culling.setElement(line,(float)position/100 + 0.5);
	}
};

void	DataManager::getCullingLines( float& left , float& right , float& bottom , float& top , float& near , float& far )
{
	left = this->culling.getElement(0);
	right = this->culling.getElement(1);
	bottom = this->culling.getElement(2);
	top = this->culling.getElement(3);
	near = this->culling.getElement(4);
	far = this->culling.getElement(5);
}

