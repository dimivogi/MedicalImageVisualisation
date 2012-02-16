#include	"DicomFileImage.h"





DicomFileImage::DicomFileImage()
	: fileName(L"") , filePath(L"") , explicitMode(false) , bigEndian(false) , sliceThickness(0) , sliceSpacing(0) , pixelDataSize(0) , pixelData(NULL) ,
	  samplesPerPixel(0) , photometricInterpretation("UNDEFINED") , numberOfFrames(1) , rows(0) , columns(0) , 
	  pixelSpacing("UNDEFINED") , bitsAllocated(0) , bitsStored(0) , highBit(0) , pixelRepresentation(0) , smallestValue(0) , 
	  largestValue(0) , windowCenter(0) , windowWidth(0) {};

DicomFileImage::DicomFileImage( wstring filename , wstring filepath ) 
	: fileName(filename) , filePath(filepath) , explicitMode(false) , bigEndian(false) , sliceThickness(0) , sliceSpacing(0) , pixelDataSize(0) , pixelData(NULL) ,
	  samplesPerPixel(0) , photometricInterpretation("UNDEFINED") , numberOfFrames(1) , rows(0) , columns(0) , 
	   pixelSpacing("UNDEFINED") , bitsAllocated(0) , bitsStored(0) , highBit(0) , pixelRepresentation(0) , smallestValue(0) , 
	  largestValue(0) , windowCenter(0) , windowWidth(0) {};

DicomFileImage::DicomFileImage( const DicomFileImage& dfi )
{
	try
	{	
		this->fileName = dfi.getFileName();
		this->filePath = dfi.getFilePath();
		this->explicitMode = dfi.isExplicitMode();
		this->bigEndian = dfi.isBigEndian();
		this->sliceThickness = dfi.getSliceThickness();
		this->sliceSpacing = dfi.getSliceSpacing();
		this->samplesPerPixel = dfi.getSamplesPerPixel();
		this->photometricInterpretation = dfi.getPhotometricInterpretation();
		this->numberOfFrames = dfi.getNumberOfFrames();
		this->columns = dfi.getColumns();
		this->rows = dfi.getRows();
		this->pixelSpacing = dfi.getPixelSpacing();
		this->bitsAllocated = dfi.getBitsAllocated();
		this->bitsStored = dfi.getBitsStored();
		this->highBit = dfi.getHighBit();
		this->pixelRepresentation = dfi.getPixelRepresentation();
		this->smallestValue = dfi.getSmallestValue();
		this->largestValue = dfi.getLargestValue();
		this->windowCenter = dfi.getWindowCenter();
		this->windowWidth = dfi.getWindowWidth();

		if ( dfi.getPixelData() != NULL )
		{
			this->pixelDataSize = dfi.getPixelDataSize();
			this->pixelData = new unsigned char[dfi.getPixelDataSize()];
			memcpy(this->pixelData,dfi.getPixelData(),this->pixelDataSize);
		}
		else
		{
			this->pixelDataSize = 0;
			this->pixelData = NULL;
		}
	}
	catch ( exception& e )
	{
		if ( this->pixelData != NULL )
			delete[] this->pixelData;

		cerr << "An exception has occured in DicomFileImage::DicomFileImage() :\t" << e.what() << endl;
		throw exception("DICOM File Image Constructor");
	}
};

DicomFileImage::~DicomFileImage()
{
	if ( pixelData != NULL )
		delete[] pixelData;
};


DicomFileImage	DicomFileImage::operator=( const DicomFileImage& dfi )
{
	DicomFileImage	temp(dfi);


	return temp;
};


void	DicomFileImage::setFileName( std::wstring name )						{ this->fileName = name; };
void	DicomFileImage::setFilePath( std::wstring path )						{ this->filePath = path; };
void	DicomFileImage::setExplicitMode( bool mode )							{ this->explicitMode = mode; };
void	DicomFileImage::setBigEndian( bool isBig )								{ this->bigEndian = isBig; };
void	DicomFileImage::setSliceThickness( unsigned int thickness )				{ this->sliceThickness = thickness; };
void	DicomFileImage::setSliceSpacing( unsigned int spacing )					{ this->sliceSpacing = spacing; };
void	DicomFileImage::setSamplesPerPixel( unsigned long samples )				{ this->samplesPerPixel = samples; };
void	DicomFileImage::setPhotometricInterpretation( string interpretation )	{ this->photometricInterpretation = interpretation; };
void	DicomFileImage::setNumberOfFrames( unsigned long number )				{ this->numberOfFrames = number; };
void	DicomFileImage::setRows( unsigned long rows )							{ this->rows = rows; };
void	DicomFileImage::setColumns( unsigned long columns )						{ this->columns = columns; };
void	DicomFileImage::setPixelSpacing( string spacing )						{ this->pixelSpacing = spacing; };
void	DicomFileImage::setBitsAllocated( unsigned long bits)					{ this->bitsAllocated = bits; };
void	DicomFileImage::setBitsStored( unsigned long bits )						{ this->bitsStored = bits; };
void	DicomFileImage::setHighBit( unsigned long bit )							{ this->highBit = bit; };
void	DicomFileImage::setPixelRepresentation( unsigned long representation )	{ this->pixelRepresentation = representation; };
void	DicomFileImage::setSmallestValue( unsigned long value )					{ this->smallestValue = value; };
void	DicomFileImage::setLargestValue( unsigned long value )					{ this->largestValue = value; };
void	DicomFileImage::setWindowCenter( unsigned long value )					{ this->windowCenter = value; };
void	DicomFileImage::setWindowWidth( unsigned long value )					{ this->windowWidth = value; };
void	DicomFileImage::setPixelData( unsigned long size , const unsigned char* data )
{
	try
	{
		this->pixelDataSize = size;

		if ( this->pixelData != NULL )
			delete[] this->pixelData;

		this->pixelData = new unsigned char[this->pixelDataSize];
		memcpy(this->pixelData,data,this->pixelDataSize);
	}
	catch ( exception& e )
	{
		if ( this->pixelData != NULL )
			delete[] this->pixelData;

		cerr << "An exception has occured in DicomFileImage::setPixelData() :\t" << e.what() << endl;
		throw exception("DICOM File Image set pixel data");
	}
};


wstring					DicomFileImage::getFileName() const						{ return this->fileName; };
wstring					DicomFileImage::getFilePath() const						{ return this->filePath; };
bool					DicomFileImage::isExplicitMode() const					{ return this->explicitMode; };
bool					DicomFileImage::isBigEndian() const						{ return this->bigEndian; };
unsigned int			DicomFileImage::getSliceThickness() const				{ return this->sliceThickness; };
unsigned int			DicomFileImage::getSliceSpacing() const					{ return this->sliceSpacing; };
unsigned long			DicomFileImage::getSamplesPerPixel() const				{ return this->samplesPerPixel; };
string					DicomFileImage::getPhotometricInterpretation() const	{ return this->photometricInterpretation; };
unsigned long			DicomFileImage::getNumberOfFrames() const				{ return this->numberOfFrames; };
unsigned long			DicomFileImage::getRows() const							{ return this->rows; };
unsigned long			DicomFileImage::getColumns() const						{ return this->columns; };
string					DicomFileImage::getPixelSpacing() const					{ return this->pixelSpacing; };
unsigned long			DicomFileImage::getBitsAllocated() const				{ return this->bitsAllocated; };
unsigned long			DicomFileImage::getBitsStored() const					{ return this->bitsStored; };
unsigned long			DicomFileImage::getHighBit() const						{ return this->highBit; };
unsigned long			DicomFileImage::getPixelRepresentation() const			{ return this->pixelRepresentation; };
unsigned long			DicomFileImage::getSmallestValue() const				{ return this->smallestValue; };
unsigned long			DicomFileImage::getLargestValue() const					{ return this->largestValue; };
unsigned long			DicomFileImage::getWindowCenter() const					{ return this->windowCenter; };
unsigned long			DicomFileImage::getWindowWidth() const					{ return this->windowWidth; };
unsigned long			DicomFileImage::getPixelDataSize() const				{ return this->pixelDataSize; };
const unsigned char*	DicomFileImage::getPixelData() const					{ return this->pixelData; };


void	DicomFileImage::printInfo()
{
	wcout	<< L"Filename:\t" << this->fileName << endl
			<< L"File Path:\t" << this->filePath << endl
			<< L"Is explicit syntax:\t";

	if ( this->explicitMode )
		cout << "Yes" << endl;
	else
		cout << "No" << endl;

	cout	<< "Is big-endian format:\t";

	if ( this->bigEndian )
		cout << "Yes" << endl;
	else
		cout << "No" << endl;


	cout	<< "Slice Thickness:\t" << this->sliceThickness << endl
			<< "Slice Spacing: \t" << this->sliceSpacing << endl
			<< "Samples Per Pixel:\t" << this->samplesPerPixel << endl
			<< "Photometric Interpretation:\t" << this->photometricInterpretation << endl
			<< "Number of Frames:\t" << this->numberOfFrames << endl
			<< "Rows:\t" << this->rows << endl
			<< "Columns:\t" << this->columns << endl
			<< "Pixel Spacing:\t" << this->pixelSpacing << endl
			<< "Bits Allocated:\t" << this->bitsAllocated << endl
			<< "Bits Stored:\t" << this->bitsStored << endl
			<< "High Bits:\t" << this->highBit << endl
			<< "Pixel Representation:\t" << this->pixelRepresentation << endl
			<< "Smallest Image Pixel Value:\t" << this->smallestValue << endl
			<< "Largest Image Pixel Value:\t" << this->largestValue << endl
			<< "Window Center:\t" << this->windowCenter << endl
			<< "Window Width:\t" << this->windowWidth << endl
			<< "Pixel Data Size:\t" << this->pixelDataSize << endl;
}