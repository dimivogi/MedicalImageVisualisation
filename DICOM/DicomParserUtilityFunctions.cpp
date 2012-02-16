#include	"DicomHeaderDeclarations.h"
#include	"DicomParserUtilityFunctions.h"
#include	<fstream>
#include	<sstream>
#include	<list>
#include	<exception>
#include	<cmath>



// Function for sorting lists in ascending order
bool	compareListStrings( wstring a , wstring b )
{
	list<int>	aValueList;
	list<int>	bValueList;
	int			aValue = 0;
	int			bValue = 0;
	int			counter;

	

	try
	{
		for( unsigned int i = 0;  i < a.length();  i++ )
		{
			if ( a[i] >= '0' && a[i] <= '9' )
				aValueList.push_back((int)(a[i]-'0'));
		}

		for( unsigned int i = 0;  i < b.length();  i++ )
		{
			if ( b[i] >= '0' && b[i] <= '9' )
				bValueList.push_back((int)(b[i]-'0'));
		}


		counter = aValueList.size();
		for( list<int>::iterator it = aValueList.begin();  it != aValueList.end();  it++ )
		{
			aValue += (*it) * (int)pow((double)10,counter);
			counter--;
		}

		counter = bValueList.size();
		for( list<int>::iterator it = bValueList.begin();  it != bValueList.end();  it++ )
		{
			bValue += (*it) * (int)pow((double)10,counter);
			counter--;
		}

	

		if ( aValue < bValue )
			return true;
		else
			return false;
	}
	catch( exception& e )
	{
		cerr << "An exception has occured while sorting the image files:\t" << e.what() << endl;
		throw exception("Image File Sorting");
	}
};



// Save the wanted fields of a DICOM file in a DicomFileImage class
void	saveInformation( unsigned short group , unsigned short element , int size , char* value , DicomFileImage& fileInfo )
{
	string			temp;
	unsigned long	temp2;



	temp = value;
	switch ( group )
	{
		case FILEMETAINFO :
									// Transfer Syntax Code
									if ( element == 0x10 )
									{
										if ( temp == DCM_EXPLICITMODE  ||  temp == DCM_EXPLICITMODE_DEFLATED )
											fileInfo.setExplicitMode(true);
										else if ( temp == DCM_EXPLICITMODE_BIGENDIAN )
										{
											fileInfo.setExplicitMode(true);
											fileInfo.setBigEndian(true);
										}
										else if ( temp != DCM_NORMALMODE )
											throw exception("Unsupported Transfer Syntax");
									}
									break;

		case AQUISITIONGROUPINFO :	
									// Slice Thickness Code
									if ( element == 0x0050 )
									{
										temp = value;
										temp = temp.substr(0,temp.find_first_of(' '));
										temp2 = atoi(temp.c_str());
										fileInfo.setSliceThickness(temp2);
									}
									else if ( element == 0x0088 )	// Slice Spacing Code
									{
										temp = value;
										temp = temp.substr(0,temp.find_first_of(' '));
										temp2 = atoi(temp.c_str());
										fileInfo.setSliceSpacing(temp2);
									}
									break;

		case IMAGEINFO :
									if ( size > sizeof(unsigned short) )
										temp2 = *((unsigned int*)value);
									else
										temp2 = *((unsigned short*)value);


									switch ( element )
									{
										// Samples Per Pixel
										case	0x0002 :	fileInfo.setSamplesPerPixel(temp2);
															break;
										// Photometric Representation
										case	0x0004 :	temp = value;
															temp = temp.substr(0,temp.find_first_of(' '));
															fileInfo.setPhotometricInterpretation(temp);
															break;
										// Number of Frames
										case	0x0008 :	fileInfo.setNumberOfFrames(temp2);
															break;
										// Rows
										case	0x0010 :	fileInfo.setRows(temp2);
															break;
										// Colums
										case	0x0011 :	fileInfo.setColumns(temp2);
															break;
										// Pixel Spacing
										case	0x0030 :	temp = value;
															temp = temp.substr(0,temp.find_first_of(' '));
															fileInfo.setPixelSpacing(temp);
															break;
										// Bits Allocated
										case	0x0100 :	fileInfo.setBitsAllocated(temp2);
															break;
										// Bits Stored
										case	0x0101 :	fileInfo.setBitsStored(temp2);
															break;
										// High Bits
										case	0x0102 :	fileInfo.setHighBit(temp2);
															break;
										// Pixel Representation
										case	0x0103 :	fileInfo.setPixelRepresentation(temp2);
															break;
										// Smallest Value
										case	0x0106 :	fileInfo.setSmallestValue(temp2);
															break;
										// Largest Value
										case	0x0107 :	fileInfo.setLargestValue(temp2);
															break;
										// Window Center
										case	0x1050 :	temp2 = atol(value);
															fileInfo.setWindowCenter(temp2);
															break;
										// Window Width
										case	0x1051 :	temp2 = atol(value);
															fileInfo.setWindowWidth(temp2);
															break;
									}
									break;

		case PIXELDATA :
									// Pixel Data
									if ( element == 0x0010)	fileInfo.setPixelData(size,(unsigned char*)value);
									break;
	};
};

// Read an unlimited text field
char*	readUnlimitedText( ifstream& input , string VR , bool implicit , bool bigEndian , int* size = NULL )
{
	unsigned int	lengthInt;
	char*			returnValue;



	lengthInt = 0;
	returnValue = NULL;
	// If the transfer syntax is explicit
	if ( !implicit )
	{
		// read Value Length
		input.read((char*)&lengthInt,4);
		// If the transfer syntax is Big Endian do a byte swap
		if ( bigEndian )	lengthInt = _byteswap_ulong(lengthInt);

		// If the Value Representation  is "UT" and Value Length field is of UNDEFINED_LENGTH the file is corrupted
		if ( VR == "UT"  &&  lengthInt == UNDEFINED_LENGTH )
			throw exception("Corrupted DICOM file!");
	}

	// if the transfer syntax is implicit or if the value length is of UNDEFINED_LENGTH
	if ( implicit  ||  lengthInt == UNDEFINED_LENGTH )
	{
		unsigned short	tempGroup;
		unsigned short	tempElement;
		char*			temp;
		unsigned int	totalData;
		bool			done;



		if ( size != NULL )	*size = sizeof(unsigned int);
		tempGroup = 0;
		tempElement = 0;
		totalData = 0;
		done = false;
		temp = NULL;
		// as long as we haven't reached a sequence deliminator field
		while( done == false ) 
		{
			// if the value representation field is "SQ" (Sequence field)
			if ( VR == "SQ" )
			{
				bool	itemDone;



				itemDone = false;
				// while we haven't found a item deliminator field
				while ( itemDone == false )
				{
					// read group and element info
					input.read((char*)tempGroup,2);
					input.read((char*)tempElement,2);

					// if the group tag is not one of the DELIMINATOR_GROUP tags, the file is corrupt
					if ( tempGroup != DELIMINATOR_GROUP )
					{
						if ( returnValue != NULL )	delete[] returnValue;
						if ( temp != NULL )	delete[] temp;
						throw exception("Corrupted DICOM File");
					}
					else
					{
						// if the tag is not a sequence deliminator tag, we haven't reached the end of the sequence
						if ( tempElement != SEQUENCE_DELIMINATOR_TAG )
						{
							// if the tag is not the ITEM_TAG the file is corrupted
							if ( tempElement != ITEM_TAG )
							{
								if ( returnValue != NULL )	delete[] returnValue;
								if ( temp != NULL )	delete[] temp;
								throw exception("Corrupted DICOM File!");
							}
							else
							{
								unsigned int	tempLength = 0;

												
								// read the item's length
								input.read((char*)tempLength,4);
								// if the length is undefined
								if ( tempLength == UNDEFINED_LENGTH )
								{
									bool			itemSpecificFound;



									itemSpecificFound = false;
									// keep reading until we reach the item deliminator field
									// item fields are always implicit
									while ( itemSpecificFound == false )
									{
										// read the group tag
										input.read((char*)tempGroup,2);

										// if the group tag is not the DELIMINATOR_GROUP tag
										if ( tempGroup != DELIMINATOR_GROUP )
										{
											
											// add the field's data, to the total data acquired
											if ( totalData > 0 )
											{
												temp = new char[totalData+2];
												memcpy(temp,returnValue,totalData);
												delete[] returnValue;
												returnValue = new char[totalData+2];
												memcpy(returnValue,temp,totalData);
												memcpy(returnValue+totalData,(char*)&tempGroup,2);
												totalData += 2;
												delete[] temp;
												temp = NULL;
											}
											else
											{
												totalData = 2;
												returnValue = new char[2];
												memcpy(returnValue,(char*)&tempGroup,2);
											}
										}
										else // if the tag is the DELIMINATOR_GROUP
										{
											// read the element tag
											input.read((char*)tempElement,2);

											// if the tag is not the item deliminator tag
											if ( tempElement != ITEM_DELIMINATOR_TAG )
											{
												// add the field's data to the total data acquired
												if ( totalData > 0 )
												{
													temp = new char[totalData+4];
													memcpy(temp,returnValue,totalData);
													delete[] returnValue;
													returnValue = new char[totalData+4];
													memcpy(returnValue,temp,totalData);
													memcpy(returnValue+totalData,(char*)&tempGroup,2);
													memcpy(returnValue+totalData+2,(char*)&tempElement,2);
													totalData += 4;
													delete[] temp;
													temp = NULL;
												}
												else
												{
													totalData = 4;
													returnValue = new char[4];
													memcpy(returnValue,(char*)&tempGroup,2);
													memcpy(returnValue+2,(char*)&tempElement,2);
												}
											}
											else // if the tag is the item deliminator tag
											{
												// read the rest of the data contained in the tag
												input.read((char*)lengthInt,4);
												itemSpecificFound = true;
											}
										}
									}
								}
								else
								{
									if ( tempLength > 0 )
									{
										if ( totalData > 0 )
										{
											temp = new char[totalData+tempLength+4];
											memcpy(temp,returnValue,totalData);
											delete[] returnValue;
											returnValue = new char[totalData+tempLength+4];
											memcpy(returnValue,temp,totalData);
											input.read(returnValue+totalData,tempLength);
											memcpy(returnValue+totalData+tempLength," :: ",4);
											totalData += (tempLength +4);
											delete[] temp;
											temp = NULL;
										}
										else
										{
											totalData = tempLength+4;
											returnValue = new char[tempLength+4];
											input.read(returnValue,tempLength);
											memcpy(returnValue+tempLength," :: ",4);
										}
									}
								}
							}
						}
						else
						{
							input.read((char*)lengthInt,4);
							itemDone = true;
						}
					}
				}
			}
			else // if the value representation is not "SQ"
			{
				// read the group tag
				input.read((char*)tempGroup,2);

				// if the group tag is not the DELIMINATOR_GROUP tag
				if ( tempGroup != DELIMINATOR_GROUP )
				{
					// add the field's data to the total data acquired
					if ( totalData > 0 )
					{
						temp = new char[totalData+2];
						memcpy(temp,returnValue,totalData);
						delete[] returnValue;
						returnValue = new char[totalData+2];
						memcpy(returnValue,temp,totalData);
						memcpy(returnValue+totalData,(char*)&tempGroup,2);
						totalData += 2;
						delete[] temp;
						temp = NULL;
					}
					else
					{
						totalData = 2;
						returnValue = new char[2];
						memcpy(returnValue,(char*)&tempGroup,2);
					}
				}
				else // if it is the DELIMINATOR_GROUP tag
				{
					// read the element tag
					input.read((char*)tempElement,2);

					// if the element tag is not the SEQUENCE_DELIMINATOR tag
					if ( tempElement != SEQUENCE_DELIMINATOR_TAG )
					{
						// add the field's data to the total data acquired
						if ( totalData > 0 )
						{
							temp = new char[totalData+4];
							memcpy(temp,returnValue,totalData);
							delete[] returnValue;
							returnValue = new char[totalData+4];
							memcpy(returnValue,temp,totalData);
							memcpy(returnValue+totalData,(char*)&tempGroup,2);
							memcpy(returnValue+totalData+2,(char*)&tempElement,2);
							totalData += 4;
							delete[] temp;
							temp = NULL;
						}
						else
						{
							totalData = 4;
							returnValue = new char[4];
							memcpy(returnValue,(char*)&tempGroup,2);
							memcpy(returnValue+2,(char*)&tempElement,2);
						}
					}
					else // if the element tag is the SEQUENCE_DELIMINATOR tag
					{
						// read the rest of the fields data
						input.read((char*)lengthInt,4);
						done = true;
					}
				}
			}
		}

		if ( size != NULL )	*size = totalData;
	}
	else // if it is not of undefined length
	{
		if ( size != NULL )	*size = sizeof(unsigned int);

		// read data equal to the specified length
		if ( lengthInt > 0 )
		{
			if ( size != NULL )	*size = lengthInt;
			returnValue = new char[lengthInt];
			input.read(returnValue,lengthInt);
		}
	}



	return returnValue;
};

unsigned long	readImplicitFile( ifstream& input , unsigned long offset , bool bigEndian , unsigned short target , DicomFileImage& fileInfo )
{
	unsigned long	currentOffset;
	unsigned long	size;
	unsigned short	group;
	unsigned short	element;
	unsigned int	lengthInt;
	char*			value;
	bool			found;
	

	
	try
	{
		currentOffset = offset;
		found = false;
		value = NULL;
		input.seekg(0,ifstream::end);
		size = input.tellg();
		input.seekg(offset,ifstream::beg);
		while( !input.eof()  &&  ( currentOffset = input.tellg() ) < size )
		{
			input.read((char*)&group,2);	// read group code
			input.read((char*)&element,2);	// read element code
			input.read((char*)&lengthInt,4);	// read value length

			// if the transfer syntax is Big Endian perform a byte swap
			if ( bigEndian )
			{
				group = _byteswap_ushort(group);
				element = _byteswap_ushort(element);
				lengthInt = _byteswap_ulong(lengthInt);
			}

			// termination condition update
			if ( group == target  &&  !found )	found = true;
			else if ( group != target  &&  found )	break;


			// if the length is undefined
			if ( lengthInt == 0xFFFFFFFF )
			{
				int	valueSize;



				// read unlimited text 
				value = readUnlimitedText(input,"",true,bigEndian,&valueSize);
				// update DicomFileImage information
				saveInformation(group,element,valueSize,value,fileInfo);
				delete[] value;
				value = NULL;
			}
			else	// if the length is defined
			{
				if ( lengthInt > 0 )
				{
					// read data of size equal to the defined length
					value = new char[lengthInt];
					input.read(value,lengthInt);
					saveInformation(group,element,sizeof(unsigned short),value,fileInfo);
					delete[] value;
					value = NULL;
				}
			}
		}
	}
	catch ( exception &e )
	{
		if ( value != NULL )
			delete[] value;

		throw e;
	}



	return currentOffset;
};

unsigned long	readExplicitFile( ifstream& input, unsigned long offset , bool bigEndian , unsigned short target , DicomFileImage& fileInfo )
{
	unsigned long	currentOffset;
	unsigned long	size;
	unsigned short	group;
	unsigned short	element;
	char			type[3];
	unsigned short	lengthShort;
	char*			value;
	bool			found;



	try
	{
		currentOffset = offset;
		found = false;
		type[2] = '\0';
		value = NULL;
		input.seekg(0,ifstream::end);
		size = input.tellg();
		input.seekg(offset,ifstream::beg);
		while( !input.eof()  &&  ( currentOffset = input.tellg() ) < size )
		{
			input.read((char*)&group,2);	// read group code
			input.read((char*)&element,2);	// read element code
			input.read(type,2);	// read value representation
			input.read((char*)&lengthShort,2);	// for "OB","OW",“OF”,“SQ”,“UT”,"UN" unused , else read Value Length

			// if the transfer syntax is Big Endian perform a byte swap
			if ( bigEndian )
			{
				group = _byteswap_ushort(group);
				element = _byteswap_ushort(element);
				lengthShort = _byteswap_ulong(lengthShort);
			}

			if ( group == target  &&  !found )	found = true;
			else if ( group != target  &&  found )	break;
		

			// if the value representation is one of the following
			if ( strcmp(type,"OB") == 0  ||  strcmp(type,"OW") == 0  ||  
				 strcmp(type,"OF") == 0  ||  strcmp(type,"SQ") == 0  ||  
				 strcmp(type,"UN") == 0  ||  strcmp(type,"UT") == 0 )
			{
				int	valueSize;


				// read unlimited text
				value = readUnlimitedText(input,type,false,bigEndian,&valueSize);
				saveInformation(group,element,valueSize,value,fileInfo);
				delete[] value;
				value = NULL;
			}
			else
			{
				if ( lengthShort > 0 )
				{
					// read data equal to the defined length
					value = new char[lengthShort];
					input.read(value,lengthShort);
					saveInformation(group,element,sizeof(unsigned short),value,fileInfo);
					delete[] value;
					value = NULL;
				}
			}
		}
	}
	catch( exception& e )
	{
		if ( value != NULL )
			delete[] value;

		throw e;
	}



	return currentOffset;
};