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

#ifndef __CREATEZIP_H__
#define __CREATEZIP_H__

#include <e32std.h>
#include <s32file.h>
#include <s32file.h>
#include <EZBufman.h>
#include <ziparchive.h>
#include <EZGzip.h>
/**
 * @publishedAll
 * @released
 */
 //Structure for Data Descriptors
class TZipDataDescriptor
{
public:
	//Constructor
	IMPORT_C TZipDataDescriptor();
	//Destructor
	IMPORT_C ~TZipDataDescriptor();

public:
	TInt32 iHeadersign; //Header Signature
	TInt32  iCrc; //Crc
	TInt32  iCompressedSize; //Compressed Size
	TInt32  iUncompressedSize; //Uncompressed Size
};
/**
 * @publishedAll
 * @released
 */
 //Structure for Node in the LinkList
class TZipNodeEntry : public CZipArchive
{
public:
	//Constructor
	IMPORT_C TZipNodeEntry(const TDesC &aGzFileName, const TDesC &aGzFullFileName, TBool compress,TBool isDirectory);
	//Destructor
	IMPORT_C ~TZipNodeEntry();

public:
   	static const TInt iOffset; //Offset of TSglQueLink used by TSglQue internally
	TLocalHeader* iHeader; //Local Header of each file
	HBufC8* iData; //Data of each file
	TZipDataDescriptor* iDataDescr; //Data Descriptor of each file
	TCentralDirectoryHeader* iCentralDirHeader; //Central Directory Header for each file
	HBufC* iInputFileName; //File name to be zipped
	HBufC* iFullFileName; //Full File name to be zipped
	TBool iIsDirectory;	//Whether this file is a directory

private:
	TSglQueLink iSlink; //TSglQueLink used by TSglQue internally
	friend class CZipNodeLinkedList;

};
/**
 * @publishedAll
 * @released
 */
 //Class to access LinkedList
  typedef TSglQue<TZipNodeEntry> TZipNodeLinkedList;
  typedef TSglQueIter<TZipNodeEntry> TZipNodeLinkedListIter;
class CZipNodeLinkedList : public CBase
{
public :
	//NewL()
	IMPORT_C static CZipNodeLinkedList* NewL();
	//NewLC()
    	IMPORT_C static CZipNodeLinkedList* NewLC();
	//Constructor
    	CZipNodeLinkedList();
 	//Destructor
	virtual ~CZipNodeLinkedList();
	/**Adds a node at the last of the link list
	*@param  anItem Node to be added
	*/
	IMPORT_C void AddToList(TZipNodeEntry& anItem);
	/**Removes a node from the link list
	*@param  aZipFileName Filename in the node, which has to be removed
	*@return TRUE if successful, FALSE otherwise.
	*/
	IMPORT_C TBool RemoveFromList(const TDesC &aZipFileName);
	/**Writes all the nodes to a file
	*@param  iZipFile Filename of the file in which all nodes are to be written
	*@return TRUE if successful, FALSE otherwise.
	*/
	IMPORT_C TBool WriteToFile(RFile& iZipFile);

private :
    	TZipNodeLinkedList iZipNodeLinkedList; //Linked List
    	TZipNodeLinkedListIter iZipNodeLinkedListIter;//Iterator for Linked List
};
/**
 * @publishedAll
 * @released
 */

class CCreateZip : public CBase, public MEZBufferManager
	{
public:
	~CCreateZip();

	/**NewLC
	*@param  aRfs 				File Session
	*@param aZipFileName 		Zip file name(output file)
	*@param aBufferSize 		Buffer Size used for Compression
	*/
	IMPORT_C static CCreateZip* NewLC(RFs &aRfs, const TDesC &aZipFileName, TInt aBufferSize = 0x8000);
	/**NewL
	*@param  aRfs 				File Session
	*@param aZipFileName 		Zip file name(output file)
	*@param aBufferSize 		Buffer Size used for Compression
	*/
	IMPORT_C static CCreateZip* NewL(RFs &aRfs, const TDesC &aZipFileName, TInt aBufferSize = 0x8000);

	/**AddFile()--> Adds the file in the output Zip file
	*@param aGzFileName 		File Name of the file to be added
	*@param compress 			Whether to Compress or Not
	*/
	IMPORT_C void AddFile(const TDesC &aGzFileName, TBool compress = FALSE);

	/**AddFile()--> Adds the content and filename in the output Zip file
	*@param aContent 			Content which is to be written to Zip Archive
	*@param aGzFileName 		File Name to be written for this content in Zip Archive
	*@param compress 			Whether to Compress or Not
	*@param aDirLevel 			This tells about the relative path to be written in the zip file
	*/
	IMPORT_C void AddFile(const TDesC8 &aContent, const TDesC &aGzFileName, TBool compress = FALSE, TInt aDirLevel = 0);

	/**RemoveFile()--> Removes the file from the output Zip file
	*@param aGzFileName 		File Name of the file to be removed
	*/
	IMPORT_C TBool RemoveFile(const TDesC &aGzFileName);

	/**ReplaceFile() ---> Replaces an old file with a new one in output zip file
	*@param  aGzOldFileName 			File Name of the file to be removed
	*@param aGzNewFileName 				File Name of the file to be added
	*@param compress 					Whether to Compress or Not(for new file)
	*/
	IMPORT_C TBool ReplaceFile(const TDesC &aGzOldFileName, const TDesC &aGzNewFileName, TBool compress = FALSE);

	/**FlushToZip() -->Finally flush all the data to zip and clears all the buffer
	*/
	IMPORT_C TBool FlushToZip();

	/**ResetL --> Resets the class to a new Zip
	*@param  aRfs 				File Session
	*@param aZipFileName 		Zip file name(output file)
	*@param aBufferSize 		Buffer Size used for Compression
	*/
	IMPORT_C void ResetL(RFs &aRfs, const TDesC &aFileName, TInt aBufferSize=0x8000);

    ///MEZBufferManager API Implementation
	virtual void InitializeL(CEZZStream &aZStream);
	virtual void NeedInputL(CEZZStream &aZStream);
	virtual void NeedOutputL(CEZZStream &aZStream);
	virtual void FinalizeL(CEZZStream &aZStream);\

private:
	//Private Constructor
	CCreateZip();
	/**ConstructL --> Constructs this object
	*@param  aRfs 			File Session
	*@param aZipFileName 		Zip file name(output file)
	*@param aBufferSize 		Buffer Size used for Compression
	*/
	void ConstructL(RFs &aRfs, const TDesC &aZipFileName, TInt aBufferSize);
	void InitialiseBufManL(RFs &aRfs, const TDesC &aZipFileName, TInt aBufferSize);
	void GetLastModifiedDateTime(const TDesC &aGzFileName,TUint16& mod_date,TUint16& mod_time);
//	TUint16 GetLastModifiedTime(const TDesC &aGzFileName);
	TUint32 GetCrc(const TDesC8& aData);
private:
	CEZCompressor *iCompressor; //Compressor used for Compressing input files data
	//TInt iUncompressedDataSize; //May be required when compressing
	CZipNodeLinkedList* iFileList; //Linked list of the files
	//CEZCompressor* icompressor;
	HBufC8* iCompressInputBuffer;
	HBufC8* iCompressOutputBuffer;
//	TPtr8 iCompressOutputBufferPtr;
	RFs iFs; //Holds file session
	HBufC*  iZipFileName; // Output file name to be wrriten as zip file
	TInt iBufferSize; //Buffer Size used for Compression
	//static TInt iDirectoryLevel;
};


#endif
