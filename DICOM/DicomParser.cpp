#ifndef	WIN32_LEAN_AND_MEAN
	#define	WIN32_LEAN_AND_MEAN
#endif
#include	<windows.h>
#include	<intrin.h>
#include	<fstream>
#include	<sstream>
#include	<cstdlib>
#include	"DicomHeaderDeclarations.h"
#include	"DicomParserUtilityFunctions.h"
#include	"DicomParser.h"
#include	<fstream>



void	parseFile( DicomFileImage& fileInfo )
{
	wstring			filePath;
	ifstream		input;
	unsigned long	offset;
	char			value[5];
	


	value[4] = '\0';
	filePath = fileInfo.getFilePath() + L"\\" + fileInfo.getFileName();
	input.exceptions(ifstream::failbit|ifstream::badbit);
	input.open(filePath.c_str(),ifstream::binary|ifstream::in);
	// pass the first 128 bytes
	input.seekg(128,ifstream::beg);
	// read 4 bytes
	input.read(value,4);

	// if the 4 bytes read are not equal to the "DICM" string, the file is corrupted
	if ( strcmp(value,"DICM") != 0 )
		throw exception("Corrupted DICOM File!");
	else
	{
		// Parse the file header. The header is always in explicit little endian syntax
		offset = input.tellg();
		offset = readExplicitFile(input,offset,false,FILEMETAINFO,fileInfo);

		// if the transfer syntax is in explicit mode parse the file explicitly
		if ( fileInfo.isExplicitMode() )
			readExplicitFile(input,offset,fileInfo.isBigEndian(),0,fileInfo);
		else	// else parse the file implicitly
			readImplicitFile(input,offset,fileInfo.isBigEndian(),0,fileInfo);
	}
	input.close();
}



list<DicomFileImage>	dicomParser( wstring filePath )
{
	wstring						searchFor;
	wstring						command;
	list<wstring>				dicomFiles;
	WIN32_FIND_DATAW			resultFileData;
	HANDLE						searchHandle;
	list<DicomFileImage>		returnValue;
	


	// search for all the files in the specified folder
	searchFor = filePath + L"\\*";
	// add all found files to the return list
	searchHandle = FindFirstFileW(searchFor.c_str(),&resultFileData);
	if	( searchHandle == INVALID_HANDLE_VALUE  ||  GetLastError() == ERROR_FILE_NOT_FOUND )
		throw exception("FindFirstFile error occured!");
	else
	{
		command = resultFileData.cFileName;
		if ( command != L"."  &&  command != L".." )
			dicomFiles.push_back(command);
	}

	while( FindNextFileW(searchHandle,&resultFileData) > 0   &&  GetLastError() != ERROR_NO_MORE_FILES )
	{	
		command = resultFileData.cFileName;
		if ( command != L"."  &&  command != L".." )
			dicomFiles.push_back(command);
	}
	FindClose(searchHandle);
	

	// sort the found files in ascending order
	dicomFiles.sort(compareListStrings);
	for( list<wstring>::iterator it = dicomFiles.begin();  it != dicomFiles.end();  it++ )
	{
		DicomFileImage	temp(*it,filePath);



		parseFile(temp);
		returnValue.push_back(temp);
	}



	return returnValue;
};

