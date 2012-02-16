#include	<iostream>
#include	<string>
using namespace std;



#ifndef	DICOM_FILE_IMAGE_CLASS
#define	DICOM_FILE_IMAGE_CLASS


// A class that represents a DICOM file
class	DicomFileImage
{
	private:

		// filename
		wstring					fileName;
		// file path
		wstring					filePath;
		// whether the file has an explicit syntax
		bool					explicitMode;
		// whether the file is a bigEndian file
		bool					bigEndian;
		// image slice thickness
		unsigned int			sliceThickness;
		// image slice spacing
		unsigned int			sliceSpacing;
		// image samples per pixel
		unsigned long			samplesPerPixel;
		// image type: MONOCHROMATIC1, MONOCHROMATIC2 or RGB
		string					photometricInterpretation;
		// number of frames contained in the image data
		unsigned long			numberOfFrames;
		// image height
		unsigned long			rows;
		// image width
		unsigned long			columns;
		// spacing betweeen pixels
		string					pixelSpacing;
		// allocated bits per pixel
		unsigned long			bitsAllocated;
		// used bits per pixel
		unsigned long			bitsStored;
		// most significant bit
		unsigned long			highBit;
		// pixel representation
		unsigned long			pixelRepresentation;
		// smallest value in the image data
		unsigned long			smallestValue;
		// largest value in the image data
		unsigned long			largestValue;
		// the value representing the middle of the value range
		unsigned long			windowCenter; 
		// the value representing the maximum possible value
		unsigned long			windowWidth; 
		// size of the image data
		unsigned long			pixelDataSize;
		// the image data
		unsigned char*			pixelData;



	public:

		DicomFileImage();
		DicomFileImage( wstring filename , wstring filepath );
		DicomFileImage( const DicomFileImage& );
		~DicomFileImage();

		DicomFileImage			operator=( const DicomFileImage& );


		// Functions mutating the class
		void					setFileName( wstring name );
		void					setFilePath( wstring path );
		void					setExplicitMode( bool mode );
		void					setBigEndian( bool isBig );
		void					setSliceThickness( unsigned int thickness);
		void					setSliceSpacing( unsigned int spacing );
		void					setSamplesPerPixel( unsigned long samples );
		void					setPhotometricInterpretation( string interpretation );
		void					setNumberOfFrames( unsigned long number );
		void					setRows( unsigned long rows );
		void					setColumns( unsigned long columns );
		void					setPixelSpacing(string spacing );
		void					setBitsAllocated( unsigned long bits);
		void					setBitsStored( unsigned long bits );
		void					setHighBit( unsigned long bit );
		void					setPixelRepresentation( unsigned long representation );
		void					setSmallestValue( unsigned long value );
		void					setLargestValue( unsigned long value );
		void					setWindowCenter( unsigned long value );
		void					setWindowWidth( unsigned long value );
		void					setPixelData( unsigned long size , const unsigned char* data );


		// Functions returning the private parts of the class
		wstring					getFileName() const;
		wstring					getFilePath() const;
		bool					isExplicitMode() const;
		bool					isBigEndian() const;
		unsigned int			getSliceThickness() const;
		unsigned int			getSliceSpacing() const;
		unsigned long			getSamplesPerPixel() const;
		string					getPhotometricInterpretation() const;
		unsigned long			getNumberOfFrames() const;
		unsigned long			getRows() const;
		unsigned long			getColumns() const;
		string					getPixelSpacing() const;
		unsigned long			getBitsAllocated() const;
		unsigned long			getBitsStored() const;
		unsigned long			getHighBit() const;
		unsigned long			getPixelRepresentation() const;
		unsigned long			getSmallestValue() const;
		unsigned long			getLargestValue() const;
		unsigned long			getWindowCenter() const;
		unsigned long			getWindowWidth() const;
		unsigned long			getPixelDataSize() const;
		const unsigned char*	getPixelData() const;

		// Function printing the class
		void	printInfo();
};


#endif	/* DICOM_FILE_IMAGE_CLASS */