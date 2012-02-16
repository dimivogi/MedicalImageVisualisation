#include	"DicomFileImage.h"
#include	<fstream>



#ifndef	DICOM_PARSER_UTILITY_FUNCTIONS
#define	DICOM_PARSER_UTILITY_FUNCTIONS


// Sort function used in list sorting
bool			compareListStrings( wstring a , wstring b );
// Read a DICOM file with implicit transfer syntax
unsigned long	readImplicitFile( ifstream& input , unsigned long offset , bool bigEndian , unsigned short target , DicomFileImage& fileInfo );
// Read a DICOM file with explicit transfer syntax
unsigned long	readExplicitFile( ifstream& input, unsigned long offset , bool bigEndian , unsigned short target , DicomFileImage& fileInfo );


#endif	/* DICOM_PARSER_UTILITY_FUNCTIONS */