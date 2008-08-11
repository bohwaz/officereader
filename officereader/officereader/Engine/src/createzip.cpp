/*************************************************************************
 *
 * DO NOT ALTER OR REMOVE COPYRIGHT NOTICES OR THIS FILE HEADER.
 * 
 * Copyright 2008,2009,2010 by ODENDAHL SEPT-Solutions
 *
 * Office Reader - a OpenDocument viewer for Symbian OS
 *
 * $Id:$
 *
 * This file is part of Office Reader
 *
 * Office Reader is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 3 or GNU 
 * General Lesser Public License version 3 , as published by the Free Software 
 * Foundation.
 *
 * Office Reader is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General (Lesser) Public License version 3 for more details
 * (a copy is included in the LICENSE file that accompanied this code).
 *
 * You should have received a copy of the GNU (Lesser) General Public License
 * version 3 along with Office Reader.  If not, 
 * see <http://www.gnu.org/licenses/>.
 *
 ************************************************************************/

#include <BAUTILS.H>
#include <S32file.H>
#include "crc32.h"
#include "CreateZip.h"

//TInt CCreateZip::iDirectoryLevel = 0;

// -----------------------------------------------------------------------------
// CCreateZip::ConstructL()
// -----------------------------------------------------------------------------
//
void CCreateZip::ConstructL(RFs &aRfs, const TDesC &aZipFileName, TInt aBufferSize)
{
	ResetL(aRfs,aZipFileName,aBufferSize);

}
// -----------------------------------------------------------------------------
// CCreateZip::CCreateZip()
// -----------------------------------------------------------------------------
//
CCreateZip::CCreateZip()
{
}
// -----------------------------------------------------------------------------
// CCreateZip::~CCreateZip()
// -----------------------------------------------------------------------------
//
CCreateZip::~CCreateZip()
{
	iFs.Close();
	delete iZipFileName;
	iZipFileName = 0;
	//delete iBuffer;
	//iBuffer = 0;
	delete iFileList;
	iFileList = 0;
	delete iCompressor;
	iCompressor = 0;

}
// -----------------------------------------------------------------------------
// CCreateZip::NewLC()
// -----------------------------------------------------------------------------
//
CCreateZip* CCreateZip::NewLC(RFs &aRfs, const TDesC &aZipFileName, TInt aBufferSize)
{
    CCreateZip* self = new (ELeave) CCreateZip();
    CleanupStack::PushL(self);
    self->ConstructL(aRfs,aZipFileName,aBufferSize);
    return self;

}
// -----------------------------------------------------------------------------
// CCreateZip::NewL()
// -----------------------------------------------------------------------------
//
CCreateZip* CCreateZip::NewL(RFs &aRfs, const TDesC &aZipFileName, TInt aBufferSize)
{
    CCreateZip* self = CCreateZip::NewLC(aRfs,aZipFileName,aBufferSize);
    CleanupStack::Pop(self);
    return self;
}
// -----------------------------------------------------------------------------
// CCreateZip::AddFile()
// -----------------------------------------------------------------------------
//
void CCreateZip::AddFile(const TDesC &aGzFileName, TBool compress)
{
	//Attributes of aGzFileName
	TUint attr = 0;
	static TInt directory_level = 0;
	TBool isDirectory = FALSE;
	HBufC8* tempBuffer = 0;
	//Is aGzFileName a directory??
	if(iFs.Att(aGzFileName,attr) == KErrNone){
		isDirectory = attr & KEntryAttDir;
	}

	//aGzFileName is a Directory
	if(isDirectory) {
		//iDirectoryLevel++;
		//TPtrC dirNameTrailer = aGzFileName.Right(1);
		//_LIT(back_slash, "\\");
		//TBuf<KMaxFileName> dirPath(aGzFileName);
		//If back slash not present, then append
		//if(dirNameTrailer.Compare(back_slash) != 0) {
		//	dirPath.Append(back_slash);
		//}
		//Adding Directory first to linked list
		//tempBuffer = HBufC8::NewL(0);
		//Calling Overloaded function AddFile()
		//AddFile(*tempBuffer,dirPath,compress);

		//Deleting tempBuffer as it's not required any more
    	//delete tempBuffer;
		//tempBuffer = 0;
		////////Directory Added in LinkedList
		CDir* dirList;
		//Get the list of files and directories in aGzFileName path
		User::LeaveIfError(iFs.GetDir(aGzFileName,KEntryAttMaskSupported,ESortByName,dirList));
		CleanupStack::PushL(dirList);
		TInt fileCount = 0;
		directory_level++;
		//iDirectoryLevel++;
		for (fileCount=0;fileCount<dirList->Count();fileCount++)
		{
			TBuf<KMaxFileName> fileName((*dirList)[fileCount].iName);
			// fileName = (*dirList)[i].iName;
			TBuf<KMaxFileName> totalPath(aGzFileName);
			//totalPath = KDirName;
			totalPath.Append(fileName);
			//Check if total path is a dirctory
			//If yes, then append it with backslash, if it's not having it at trailing end
			TBool isTotalPathDir = FALSE;
			if(iFs.Att(totalPath,attr) == KErrNone){
				isTotalPathDir = attr & KEntryAttDir;
			}
			if(isTotalPathDir) {
				//Check for trailing backslash for Directories
				TPtrC fileNameTrailer = totalPath.Right(1);
				_LIT(back_slash, "\\");
				//If back slash not present, then append
				if(fileNameTrailer.Compare(back_slash) != 0) {
					totalPath.Append(back_slash);
				}
			}
			//Recursive Call
			AddFile(totalPath,compress);
		}
		directory_level--;
		//iDirectoryLevel--;
		CleanupStack::Pop(dirList);//dirList
		delete dirList;
		dirList = 0;
		//isDirectory = TRUE;
		//Returning if the directory is not empty, no need to write directory in zip file
		if(fileCount>0)
			return;

	}


	//if aGzFileName is a file
	if(!isDirectory){
		RFile	file;
	   	User::LeaveIfError(file.Open(iFs, aGzFileName, EFileRead));
	    CleanupClosePushL(file);
		TInt size;
	    User::LeaveIfError(file.Size(size));
	    //tempBuffer is a descriptor class which provides a buffer of fixed length,
	    //allocated on the heap, for containing and accessing data.
		tempBuffer = HBufC8::NewLC(size);
		//Creates and returns an 8-bit modifiable pointer descriptor for the data
		//represented by this 8-bit heap descriptor
		TPtr8 bufPtr(tempBuffer->Des());
	   	User::LeaveIfError(file.Read(bufPtr));
		file.Close();
    	CleanupStack::Pop(tempBuffer); // tempBuffer
    	CleanupStack::PopAndDestroy(1); // file
	}
	//if aGzFileName is a Directory
	else
		tempBuffer = HBufC8::NewL(0);



	//Calling Overloaded function AddFile()
	AddFile(*tempBuffer,aGzFileName,compress,directory_level);

	//Deleting tempBuffer as it's not required any more
    	delete tempBuffer;
		tempBuffer = 0;

}
// -----------------------------------------------------------------------------
// CCreateZip::AddFile()
// -----------------------------------------------------------------------------
//
void CCreateZip::AddFile(const TDesC8 &aContent, const TDesC &aGzFileName, TBool compress, TInt aDirLevel)
{
	if(aDirLevel<0)
		aDirLevel = 0;
	TInt uncompressedSize = 0;
	TInt compressedSize = 0;
	TInt intCrc =0;
	TInt size = aContent.Length();
	HBufC8* tempBuffer = 0;
	//Allocate memory only if size >0, else it will result in memory leak
	if(size>0){
		tempBuffer = HBufC8::NewL(size);
		*tempBuffer = aContent;
		// CRC is taken before compressing
		intCrc = GetCrc(*tempBuffer);
	}
	//Compressing
	if(compress == TRUE && size>0) {
		//Use NewL????
		iCompressInputBuffer = HBufC8::NewL(size);
		iCompressOutputBuffer = HBufC8::NewL(size);
		*iCompressInputBuffer = *tempBuffer;
		iCompressor->ResetL(*this);
		//TPtr8 t1 = iCompressOutputBuffer->Des();
		//TPtr8 t2 = iCompressInputBuffer->Des();
		//iCompressor->CompressL(t1,t2);
		TInt temperr;
		do{
			TRAPD(err,iCompressor->DeflateL());
			temperr = err;
		}while(temperr>=0);

		uncompressedSize = size;
		compressedSize = iCompressOutputBuffer->Des().Length();
		//Deleting tempBuffer and iCompressInputBuffer as they are no more required
		delete tempBuffer;
		tempBuffer = 0;
		delete iCompressInputBuffer;
		iCompressInputBuffer = 0;
	}
	else if(size>0){
		uncompressedSize = compressedSize = size;
	}
	//////////////////////////
	//Parsing the path to be written in Zip File
	TBuf<KMaxFileName> writePath(aGzFileName);

	TInt backslashCount = 0;
	TInt count = 0;
	//Traversing the filename backwards, looking for backslashes
	for( count = aGzFileName.Length()-1;count>=0;count--) {
		//0x5C corresponds to '\'
		if(writePath[count]	== 0x5C){
			backslashCount++;
			//0x2F corresponds to '/'. Replacing '\' with '/' for Open Office
			writePath[count] = 0x2F;
		}
		if(backslashCount>=aDirLevel+1)
			break;
	}
	//Adjusting the count
	count++;
	//Actual Path which will be written to Zip File
	TPtr actualWritePath = writePath.MidTPtr(count);

	//////////////////////////

	//Calculating DateTime Start
	TUint16 mod_date = 0;
	TUint16 mod_time = 0;
	GetLastModifiedDateTime(aGzFileName,mod_date,mod_time);
	//Attributes of aGzFileName
	TUint attr = 0;
	TBool isdir = FALSE;
	if(iFs.Att(aGzFileName,attr) == KErrNone){
			isdir = attr & KEntryAttDir;
	}

	//Creating Linked List Node for this file
	TZipNodeEntry* node = new (ELeave) TZipNodeEntry(actualWritePath,aGzFileName,compress,isdir);
	//Calculating values and assigning to node
	//iHeader values
	//node->iSignature = 0x04034b50 //(32 bits) already assigned in Constructor
	//node->iHeader->iVersionNeeded = 0x000A; //(16 bits) TODO already assigned in Constructor
	//node->iHeader->iFlags = 0x0000; //(16bits)
	//node->iHeader->iCompressionMethod = 0x0000; //(16 bits)
	node->iHeader->iLastModifiedFileTime = mod_time;  //(16 bits) TODO
	node->iHeader->iLastModifiedFileDate = mod_date;  //(16 bits) TODO
	//No Data Descriptor, so uncommenting nxt 3 lines
	node->iHeader->iCRC32 = intCrc;  //(32 bits) TODO
	node->iHeader->iCompressedSize = compressedSize;  //(32 bits)
	node->iHeader->iUncompressedSize = uncompressedSize; //(32 bits)
//For version 2 PKZIP header values for CRC Compressed size etc are 0
//	node->iHeader->iCRC32 = 0;  //(32 bits) TODO
//	node->iHeader->iCompressedSize = 0;  //(32 bits)
//	node->iHeader->iUncompressedSize = 0; //(32 bits)
	//node->iHeader->iFileNameLength = TInt16(aGzFileName.Length()); //(16 bits)
	//node->iHeader->iExtraFieldLength = 0x0000;  //(16 bits)

	//iDataDescr Values
	// Write Data Descriptor for version 2 of PKZIP
	node->iDataDescr->iCrc = intCrc;
	node->iDataDescr->iCompressedSize = compressedSize;
	node->iDataDescr->iUncompressedSize = uncompressedSize;

	//File Data
	//Only is size>0, ie case of a file not directory
	if(size>0) {
		if(compress) {
			node->iData = iCompressOutputBuffer;
		}
		else {
			node->iData = tempBuffer;
		}
	}

	//iCentralDirHeader Values
	//Commented ones are already populated in Constructor

	//node->iCentralDirHeader->iSignature = 0x02014b50;  //(32 bits)already assigned in Constructor
	//node->iCentralDirHeader->iMadeBy = 0x0014;  //(16 bits)
	//node->iCentralDirHeader->iRequired = 0x000A;  //(16 bits)
	//node->iCentralDirHeader->iFlags = 0x0000;  //(16 bits)
	//node->iCentralDirHeader->iCompressionMethod = 0x0000;  //(16 bits)
	node->iCentralDirHeader->iLastModifiedFileTime = mod_time;  //(16 bits)
	node->iCentralDirHeader->iLastModifiedFileDate = mod_date;  //(16 bits)
	node->iCentralDirHeader->iCRC32 = intCrc;  //(32 bits)
	node->iCentralDirHeader->iCompressedSize = compressedSize;  //(32 bits)
	node->iCentralDirHeader->iUncompressedSize = uncompressedSize;  //(32 bits)
	//node->iCentralDirHeader->iFileNameLength = TInt16(aGzFileName.Length());  //(16 bits)
	//node->iCentralDirHeader->iExtraFieldLength = 0x0000;  //(16 bits)
	//node->iCentralDirHeader->iFileCommentLength = 0x0000;  //(16 bits)
	//node->iCentralDirHeader->iDiskNumberStart = 0x0000;  //(16 bits)
	//node->iCentralDirHeader->iInternalFileAttributes = 0x0000;  //(16 bits)
	//node->iCentralDirHeader->iExternalFileAttributes = 0x00000002;  //(32 bits)
	//node->iCentralDirHeader->iLocalHeaderOffset = 0x00000000;  //(32 bits)

	//File Name

	//(node->iInputFileName)(aGzFileName);

	//Adding this node to linked list
	iFileList->AddToList(*node);
}
// -----------------------------------------------------------------------------
// CCreateZip::RemoveFile()
// -----------------------------------------------------------------------------
//
TBool CCreateZip::RemoveFile(const TDesC &aGzFileName)
{

	return iFileList->RemoveFromList(aGzFileName);


}
// -----------------------------------------------------------------------------
// CCreateZip::ReplaceFile()
// -----------------------------------------------------------------------------
//
TBool CCreateZip::ReplaceFile(const TDesC &aGzOldFileName, const TDesC &aGzNewFileName, TBool compress)
{
	TBool rc = FALSE;
	if(RemoveFile(aGzOldFileName)) {
		AddFile(aGzNewFileName,compress);
		rc = TRUE;
	}
	return rc;
}
// -----------------------------------------------------------------------------
// CCreateZip::FlushToZip()
// -----------------------------------------------------------------------------
//
TBool CCreateZip::FlushToZip()
{
	TBool rc = FALSE;
    RFile file;
    //User::LeaveIfError(rfs.Connect());
    //CleanupClosePushL(rfs);
    if(BaflUtils::FileExists(iFs,*iZipFileName)){
		BaflUtils::DeleteFile(iFs,*iZipFileName);
	}
	User::LeaveIfError(file.Create(iFs,*iZipFileName,EFileWrite));

	CleanupClosePushL(file);

	//Writing to file from Linked List
	iFileList->WriteToFile(file);
	rc =TRUE;
	file.Close();
	CleanupStack::PopAndDestroy(1);//file
	return rc;
}
// -----------------------------------------------------------------------------
// CCreateZip::ResetL()
// -----------------------------------------------------------------------------
//
void CCreateZip::ResetL(RFs &aRfs, const TDesC &aFileName, TInt aBufferSize)
{
	//RFs is handle to file server session.
	//Closing previous session, if it exists
	iFs.Close();
	iFs = aRfs;
	//Connecting this session
	iFs.Connect();

	//Deleting old file name if it exists
	if(iZipFileName)
		delete iZipFileName;
	//Saving final output zip file's name
	iZipFileName = HBufC::NewL(aFileName.Length());
	//Copy data into this 16-bit heap descriptor
	*iZipFileName = aFileName;

	iBufferSize = aBufferSize;
	//Deleting Old Link List, if it exists
	if(iFileList)
		delete iFileList;

	//Instantiating Linked List
	iFileList = CZipNodeLinkedList::NewL();

	if(iCompressor) {
		delete iCompressor;
		iCompressor = 0;
	}

	iCompressor = CEZCompressor::NewL(*this,CEZCompressor::EDefaultCompression,-CEZCompressor::EMaxWBits);

}
// -----------------------------------------------------------------------------
// CCreateZip::InitializeL()
// -----------------------------------------------------------------------------
//
void CCreateZip::InitializeL(CEZZStream &aZStream)
{
	if(iCompressInputBuffer&&iCompressOutputBuffer){
		TPtr8 iCompressOutputBufferPtr = iCompressOutputBuffer->Des();
		aZStream.SetInput(*iCompressInputBuffer);
		aZStream.SetOutput(iCompressOutputBufferPtr);
	}
}
// -----------------------------------------------------------------------------
// CCreateZip::NeedInputL()
// -----------------------------------------------------------------------------
//
void CCreateZip::NeedInputL(CEZZStream &aZStream)
{
	//TPtr8 iCompressOutputBufferPtr = iCompressOutputBuffer->Des();
	//aZStream.SetInput(iCompressInputBuffer->Des());
	//aZStream.SetOutput(iCompressOutputBufferPtr);
}
// -----------------------------------------------------------------------------
// CCreateZip::NeedOutputL()
// -----------------------------------------------------------------------------
//
void CCreateZip::NeedOutputL(CEZZStream &aZStream)
{


}
// -----------------------------------------------------------------------------
// CCreateZip::FinalizeL()
// -----------------------------------------------------------------------------
//
void CCreateZip::FinalizeL(CEZZStream &aZStream)
{
	TPtrC8 od = aZStream.OutputDescriptor();
	//iOutput.InsertL(iOutput.Size(), od);
	*iCompressOutputBuffer = od;

}

// -----------------------------------------------------------------------------
// CCreateZip::InitialiseBufManL()
// -----------------------------------------------------------------------------
//
void CCreateZip::InitialiseBufManL(RFs &aRfs, const TDesC &aZipFileName, TInt aBufferSize)
{


}
// -----------------------------------------------------------------------------
// CCreateZip::GetLastModifiedDateTime()
// -----------------------------------------------------------------------------
//
void CCreateZip::GetLastModifiedDateTime(const TDesC &aGzFileName,TUint16& mod_date,TUint16& mod_time)
{
	mod_date = mod_time = 0;
	TTime time_obj;

	if(iFs.Modified(aGzFileName, time_obj)== KErrNone ) {
		TDateTime date_time = time_obj.DateTime();
		//Calculating Date in MS-Dos Format
		TUint16 year = date_time.Year()-1980;
		if(year < 0){
			year = 0;
		}
		TUint16 month = date_time.Month() + 1;
		TUint16 day = date_time.Day() +1;
		//mod_date = (year*0x100)+(month*0x10)+day;
		mod_date = (year<<9) | (month<<5) | day;

		//Calculating Time in MS-Dos Format
		TUint16 hour = date_time.Hour() +1;
		TUint16 minute = date_time.Minute()+1;
		TUint16 second = date_time.Second()+1;

		mod_time = (hour<<11) | (minute<<5) | (second>>1);

	}
}

// -----------------------------------------------------------------------------
// CCreateZip::GetCrc()
// -----------------------------------------------------------------------------
//
TUint32 CCreateZip::GetCrc(const TDesC8& aData)
{
	TUint32 intCrc = 0;
	CCrc32* crc32 = CCrc32::NewLC();
	crc32->ResetHash();
	crc32->AppendToHashL(aData);
	HBufC8* crcOfFileData = crc32->GetHashAndResetL();
	CleanupStack::Pop(crc32);
	delete crc32;
	crc32 = 0;
	//Calculating TInt32 from HBufC8*
	TPtrC8 des = crcOfFileData->Des();
	//TLex8 lex(des);
	//lex.Val(intCrc);
	for (TInt i = 0; i < 4; i++ ) {
       	 intCrc *= 0x100;
        	intCrc += des[i];
	}
	delete crcOfFileData;
	crcOfFileData = 0;
	return intCrc;
}
//-----------------------------------------------------------------------------
//---------------TZipDataDescriptor Implementation ------------------------------------
//-----------------------------------------------------------------------------

// -----------------------------------------------------------------------------
// TZipDataDescriptor::TZipDataDescriptor()
// -----------------------------------------------------------------------------
//
TZipDataDescriptor::TZipDataDescriptor()
{
	iHeadersign = 0x08074b50;
	iCrc = 0;//Will be populated later.
	iCompressedSize = 0;//Will be populated later.
	iUncompressedSize = 0;//Will be populated later.
}
// -----------------------------------------------------------------------------
// TZipDataDescriptor::~TZipDataDescriptor()
// -----------------------------------------------------------------------------
//
TZipDataDescriptor::~TZipDataDescriptor()
{
	iHeadersign =0;
	iCrc = 0;
	iCompressedSize = 0;
	iUncompressedSize = 0;
}
//-----------------------------------------------------------------------------
//---------------TZipNodeEntry Implementation ------------------------------------
//-----------------------------------------------------------------------------
const TInt TZipNodeEntry::iOffset = _FOFF(TZipNodeEntry,iSlink);
// -----------------------------------------------------------------------------
// TZipNodeEntry::TZipNodeEntry()
// -----------------------------------------------------------------------------
//
TZipNodeEntry::TZipNodeEntry(const TDesC &aGzFileName, const TDesC &aGzFullFileName, TBool compress,TBool isDirectory)
{
	iIsDirectory = isDirectory;
	//Is aGzFullFileName a directory??

	iHeader = new (ELeave) TLocalHeader();

	//Initializing iHeader(TLocalHeader)
	iHeader->iSignature = 0x04034b50;
	//Version 2 by default
	iHeader->iVersionNeeded = 0x0014;
/*	if(compress) {
		//0x0014--> 2.0 --> 2.0 - File is compressed using Deflate compression
		iHeader->iVersionNeeded = 0x0014;
	}
	else {
		//0x000A--> 1.0 --> 1.0 - Default value
		iHeader->iVersionNeeded = 0x000A;
	}*/
	//Bit 2  Bit 1
       // 0      0    Normal (-en) compression option was used.
	if(compress && !iIsDirectory){
		iHeader->iFlags = 0x0008;
	}
	else {
		iHeader->iFlags = 0x0000;
	}
	//iHeader->iFlags =0x0008;
	//For non encrypted file
	//iHeader->iFlags =0x0000;
	if(compress && !iIsDirectory) {
		//8 - The file is Deflated
		iHeader->iCompressionMethod = 0x0008;
	}
	else {
		//0 - The file is stored (no compression)
		iHeader->iCompressionMethod = 0x0000;
	}
	iHeader->iLastModifiedFileDate = 0; //Will be populated later.
	iHeader->iLastModifiedFileTime = 0;//Will be populated later.
	iHeader->iCRC32 = 0;//Will be populated later.
	iHeader->iCompressedSize = 0;//Will be populated later.
	iHeader->iUncompressedSize = 0;//Will be populated later.
	iHeader->iFileNameLength = TUint16(aGzFileName.Length());
	iHeader->iExtraFieldLength = 0;

	iDataDescr = new (ELeave) TZipDataDescriptor();
	//Initializing iDataDescr(TZipDataDescriptor)
	iDataDescr->iHeadersign = 0x08074b50;
	iDataDescr->iCrc = 0;
	iDataDescr->iCompressedSize = 0;
	iDataDescr->iUncompressedSize = 0;

	iCentralDirHeader = new (ELeave) TCentralDirectoryHeader();
	//Initializing iCentralDirHeader(TCentralDirectoryHeader)
	iCentralDirHeader->iSignature= 0x02014b50;
	iCentralDirHeader->iMadeBy = 0x0014;
	//Same as iHeader->iVersionNeeded
	iCentralDirHeader->iRequired = iHeader->iVersionNeeded;
	//Same as iHeader->iFlags
	iCentralDirHeader->iFlags = iHeader->iFlags;
	//Same as iHeader->iCompressionMethod
	iCentralDirHeader->iCompressionMethod = iHeader->iCompressionMethod;
	iCentralDirHeader->iLastModifiedFileDate = 0;//Will be populated later.
	iCentralDirHeader->iLastModifiedFileTime = 0;//Will be populated later.
	iCentralDirHeader->iCRC32 = 0;//Will be populated later.
	iCentralDirHeader->iCompressedSize = 0;//Will be populated later.
	iCentralDirHeader->iUncompressedSize = 0;//Will be populated later.
	//Same as iHeader->iFileNameLength
	iCentralDirHeader->iFileNameLength = iHeader->iFileNameLength;
	iCentralDirHeader->iExtraFieldLength = 0x0000;
	iCentralDirHeader->iFileCommentLength = 0x0000;
	iCentralDirHeader->iDiskNumberStart = 0x0000;
	//Set to 0. All files will be treated as binary
	iCentralDirHeader->iInternalFileAttributes = 0x0000;
	iCentralDirHeader->iExternalFileAttributes = 0x00000002;
	iCentralDirHeader->iLocalHeaderOffset = 0;//Will be populated later.

	//Initializing iData(HBufC8)
	iData = 0;//Will be populated later.

	//Initializing iInputFileName
	iInputFileName = HBufC::NewL(aGzFileName.Length());
	*iInputFileName = aGzFileName;

	//Initializing iFullFileName
	iFullFileName = HBufC::NewL(aGzFullFileName.Length());
	*iFullFileName = aGzFullFileName;



}
// -----------------------------------------------------------------------------
// TZipNodeEntry::~TZipNodeEntry()
// -----------------------------------------------------------------------------
//
TZipNodeEntry::~TZipNodeEntry()
{
	delete iFullFileName;
	iFullFileName = 0;
	delete iInputFileName;
	iInputFileName = 0;
	delete iHeader;
	iHeader = 0;
	delete iDataDescr;
	iDataDescr =0;
	delete iCentralDirHeader;
	iCentralDirHeader = 0;
	delete iData;
	iData = 0;
}

//-----------------------------------------------------------------------------
//---------------CZipNodeLinkedList Implementation ------------------------------------
//-----------------------------------------------------------------------------

// -----------------------------------------------------------------------------
// CZipNodeLinkedList::NewLC()
// -----------------------------------------------------------------------------
//
CZipNodeLinkedList* CZipNodeLinkedList::NewLC()
{
    CZipNodeLinkedList* self = CZipNodeLinkedList::NewL();
    CleanupStack::PushL(self);
    return self;
}
// -----------------------------------------------------------------------------
// CZipNodeLinkedList::NewL()
// -----------------------------------------------------------------------------
//
CZipNodeLinkedList* CZipNodeLinkedList::NewL()
{
    CZipNodeLinkedList* self = new (ELeave) CZipNodeLinkedList;
    return self;
}
// -----------------------------------------------------------------------------
// CZipNodeLinkedList::CZipNodeLinkedList()
// -----------------------------------------------------------------------------
//
CZipNodeLinkedList::CZipNodeLinkedList()
    : iZipNodeLinkedList(TZipNodeEntry::iOffset),iZipNodeLinkedListIter(iZipNodeLinkedList)
{
}
// -----------------------------------------------------------------------------
// CZipNodeLinkedList::~CZipNodeLinkedList()
// -----------------------------------------------------------------------------
//
CZipNodeLinkedList::~CZipNodeLinkedList()
{
    TZipNodeEntry* item;

    iZipNodeLinkedListIter.SetToFirst();
    while ((item = iZipNodeLinkedListIter++) != NULL)
        {
        iZipNodeLinkedList.Remove(*item);
        delete item;
        };
}
// -----------------------------------------------------------------------------
// CZipNodeLinkedList::AddToList()
// -----------------------------------------------------------------------------
//
void CZipNodeLinkedList::AddToList(TZipNodeEntry& anItem)
{
    iZipNodeLinkedList.AddLast(anItem);
}
// -----------------------------------------------------------------------------
// CZipNodeLinkedList::RemoveFromList()
// -----------------------------------------------------------------------------
//
TBool CZipNodeLinkedList::RemoveFromList(const TDesC &aZipFileName)
{
	TBool rc(FALSE);
	//Attributes of aGzFileName
	TUint attr = 0;
	RFs rfs;
	rfs.Connect();
	TBool isDirectory = FALSE;
	//Is aGzFileName a directory??
	if(rfs.Att(aZipFileName,attr) == KErrNone){
		isDirectory = attr & KEntryAttDir;
	}
	rfs.Close();
	//If List is empty, or aZipFileName is a directory, then return FALSE
	if(iZipNodeLinkedList.IsEmpty()||isDirectory)
   		return rc;


    TZipNodeEntry* item;

    iZipNodeLinkedListIter.SetToFirst();
    while ((item = iZipNodeLinkedListIter++) != NULL)
        {
       		//Check here for the given file name and remove that node from Linked list
       		if(*(item->iFullFileName) == aZipFileName) {
       			iZipNodeLinkedList.Remove(*item);
       			delete item;
       			rc = TRUE;
       			break;
			}

        };

   return rc;
}
// -----------------------------------------------------------------------------
// CZipNodeLinkedList::ReplaceInList()
// -----------------------------------------------------------------------------
//
//TBool CZipNodeLinkedList::ReplaceInList(const TDesC &aGzOldFileName, TZipNodeEntry& anItem)
//{
//   TBool rc = FALSE;
//    TZipNodeEntry* item;
//
//    iZipNodeLinkedListIter.SetToFirst();
//    while ((item = iZipNodeLinkedListIter++) != NULL)
//        {
//       //Check here for the given file name and replace that node with new one(anItem) in Linked list
//        };
//   return rc;
//}
// -----------------------------------------------------------------------------
// CZipNodeLinkedList::WriteToFile()
// -----------------------------------------------------------------------------
//
TBool CZipNodeLinkedList::WriteToFile(RFile& iZipFile)
{
    TBool rc = FALSE;
    TZipNodeEntry* item;
    RFileWriteStream fwriteStream(iZipFile);
    fwriteStream.PushL();
    HBufC8* inputFileNameAscii;
    TBool writeDataDescriptor = FALSE;

    TInt32 centralDirectoryOffSet = 0;
    TInt32 localHeaderOffSet = 0;

    iZipNodeLinkedListIter.SetToFirst();

    //Writing Local File Header, File Data and Data Descriptor
    while ((item = iZipNodeLinkedListIter++) != NULL)
    {
        if(item->iHeader->iFlags & 0x0008) {
		writeDataDescriptor = TRUE;
	}
    //Write to file
    //Writing local file header
	fwriteStream.WriteUint32L(item->iHeader->iSignature);
	fwriteStream.WriteUint16L(item->iHeader->iVersionNeeded);
	fwriteStream.WriteUint16L(item->iHeader->iFlags);
	fwriteStream.WriteUint16L(item->iHeader->iCompressionMethod);
	fwriteStream.WriteUint16L(item->iHeader->iLastModifiedFileTime);
	fwriteStream.WriteUint16L(item->iHeader->iLastModifiedFileDate);
	fwriteStream.WriteUint32L(item->iHeader->iCRC32);
	fwriteStream.WriteUint32L(item->iHeader->iCompressedSize);
	fwriteStream.WriteUint32L(item->iHeader->iUncompressedSize);
	fwriteStream.WriteUint16L(item->iHeader->iFileNameLength);
	fwriteStream.WriteUint16L(item->iHeader->iExtraFieldLength);


	//File Name(variable size)
	inputFileNameAscii = HBufC8::NewL(item->iInputFileName->Length());
	TPtr8 temp = inputFileNameAscii->Des();
	//*inputFileNameAscii = *(item->inputFileNameAscii);
	//Change all BackSlashes to forward slashes....

	temp.Copy(*(item->iInputFileName));
	fwriteStream.WriteL(temp);
	//TODO
	//extra field (variable size)

	//Writing File Data
	//if(!(item->iIsDirectory) )
	//If item->iData == 0, then this item corresponds to a directory
	if(item->iData){
		fwriteStream.WriteL(item->iData->Des());
	}

	//Writing Data Descriptor
	if(writeDataDescriptor) {
		fwriteStream.WriteUint32L(item->iDataDescr->iHeadersign);
		fwriteStream.WriteUint32L(item->iDataDescr->iCrc);
		fwriteStream.WriteUint32L(item->iDataDescr->iCompressedSize);
		fwriteStream.WriteUint32L(item->iDataDescr->iUncompressedSize);
	}


	//const item1 = item;
	//const TZipNodeEntry* t1 = item;
	//TPckgC<TZipNodeEntry> structpkg(*t1);
	//iZipFile.Write(structpkg);

	//Don't know why sizeof is giving 2 bytes etc.
  	//centralDirectoryOffSet += sizeof(*(item->iHeader)) -2;
    //centralDirectoryOffSet += inputFileNameAscii.Size();
    //centralDirectoryOffSet += item->iData->Size();

	//Calculating Local Header Offset for Central Directory
    item->iCentralDirHeader->iLocalHeaderOffset = localHeaderOffSet;
	//Don't know why sizeof is giving 2 bytes etc.
	//localHeaderOffSet = 0;
    localHeaderOffSet += (sizeof(*(item->iHeader)) -2);
    localHeaderOffSet += inputFileNameAscii->Size();

    //if(!item->iIsDirectory)
    	//If item->iData == 0, then this item corresponds to a directory
    	if(item->iData) {
   		 localHeaderOffSet += item->iData->Size();
    	}
    	if(writeDataDescriptor) {
  			localHeaderOffSet += (sizeof(*(item->iDataDescr)));
		}

    centralDirectoryOffSet = localHeaderOffSet;

    delete inputFileNameAscii;
	inputFileNameAscii = 0;

    };


	TInt16 noOfEntries = 0;
	TInt32 centralDirectorySize = 0;
    iZipNodeLinkedListIter.SetToFirst();
    //Writing Central Directory Structure
    while ((item = iZipNodeLinkedListIter++) != NULL)
    {
    //Write to file
    //WritingCentral Directory Structure
	fwriteStream.WriteUint32L(item->iCentralDirHeader->iSignature);
	fwriteStream.WriteUint16L(item->iCentralDirHeader->iMadeBy);
	fwriteStream.WriteUint16L(item->iCentralDirHeader->iRequired);
	fwriteStream.WriteUint16L(item->iCentralDirHeader->iFlags);
	fwriteStream.WriteUint16L(item->iCentralDirHeader->iCompressionMethod);
	fwriteStream.WriteUint16L(item->iCentralDirHeader->iLastModifiedFileTime);
	fwriteStream.WriteUint16L(item->iCentralDirHeader->iLastModifiedFileDate);
	fwriteStream.WriteUint32L(item->iCentralDirHeader->iCRC32);
	fwriteStream.WriteUint32L(item->iCentralDirHeader->iCompressedSize);
	fwriteStream.WriteUint32L(item->iCentralDirHeader->iUncompressedSize);
	fwriteStream.WriteUint16L(item->iCentralDirHeader->iFileNameLength);
	fwriteStream.WriteUint16L(item->iCentralDirHeader->iExtraFieldLength);
	fwriteStream.WriteUint16L(item->iCentralDirHeader->iFileCommentLength);
	fwriteStream.WriteUint16L(item->iCentralDirHeader->iDiskNumberStart);
	fwriteStream.WriteUint16L(item->iCentralDirHeader->iInternalFileAttributes);
	fwriteStream.WriteUint32L(item->iCentralDirHeader->iExternalFileAttributes);
	//iLocalHeaderOffset has to be calculated here
	fwriteStream.WriteUint32L(item->iCentralDirHeader->iLocalHeaderOffset);

	//Write file name    -- variable length
	//Convert TDesC to TDesC8 in case unicode support is not there
	//inputFileNameAscii is 8 bit descriptor
	//inputFileNameAscii.Copy(item->iInputFileName);
	//fwriteStream.WriteL(inputFileNameAscii);
	inputFileNameAscii = HBufC8::NewL(item->iInputFileName->Length());
	TPtr8 temp = inputFileNameAscii->Des();
	//*inputFileNameAscii = *(item->iInputFileName);

	temp.Copy(*(item->iInputFileName));
	fwriteStream.WriteL(temp);

	//TODO
	//Write extra field  -- variable length

	//file comment -- variable length
	//centralDirectorySize += sizeof(*(item->iCentralDirHeader)) ;
	//Don't know why sizeof is giving 2 bytes etc.
	centralDirectorySize += sizeof(*(item->iCentralDirHeader)) -2 ;
	centralDirectorySize += inputFileNameAscii->Size();

	noOfEntries++;

	delete inputFileNameAscii;
	inputFileNameAscii = 0;
    };

	//TODO:
	//Write end of central dir signature
	//End of Central Directory Signature (4 bytes)
	fwriteStream.WriteUint32L(0x06054b50);
	//number of this disk (2 bytes)
	fwriteStream.WriteUint16L(0x0000);
	//number of the disk with the start of the central directory (2 bytes)
	fwriteStream.WriteUint16L(0x0000);
	//total number of entries in the central directory on this disk(2 bytes)
	fwriteStream.WriteUint16L(noOfEntries);
	//total number of entries in the central directory (2 bytes)
	fwriteStream.WriteUint16L(noOfEntries);
	//Size of Central Directory (4 bytes)
	fwriteStream.WriteUint32L(centralDirectorySize);
	//offset of start of central directory with respect to the starting disk number(4 bytes)
	fwriteStream.WriteUint32L(centralDirectoryOffSet);
	//.ZIP file comment length (2 bytes)
	fwriteStream.WriteUint16L(0x0000);
	//.ZIP file comment
	//None

    fwriteStream.CommitL();
    fwriteStream.Release();
    fwriteStream.Pop();
   	return rc;
}
