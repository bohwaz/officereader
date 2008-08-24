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

#ifndef __OPENDOCUMENT_H__
#define __OPENDOCUMENT_H__


//  Include Files

#include <e32base.h>    // CBase
#include <e32std.h>     // TBuf
#include <f32file.h>
#include <txtrich.h>
#include <eikrted.h>
#include <COECNTRL.H>
#include <coemain.h>
#include <frmtlay.h> //tcursorselection

#include "EZBufman.h" 

#include "hint.h"


//#define _log_

#ifdef _log_
	#include <flogger.h>	
#endif


// CLASS DECLARATION
class CCZip;
class CXML;
class CPara;
class CCImageEngine;
class CSheetCtrl;
class CEncryptionData;
class CCreateZip;

enum TXMLMode
{
	XMLModeCompleteText,
	XMLModeMeta
};

enum TOpenDocumentTyp
{
	ODFUnknown=-1,
	ODT,
	ODS,
	ODP,
	OTT,
	OTS,
	OTP,
};

#define ETableIndex 10000
	

//  Class Definitions	 
class COpenDocument: public MEZBufferManager
    {
    public:     // new functions
        IMPORT_C static COpenDocument* NewL();
        IMPORT_C static COpenDocument* NewLC();
        IMPORT_C ~COpenDocument();

    public:     
        IMPORT_C TVersion Version() const;
        
		IMPORT_C TOpenDocumentTyp GetType(void);
		IMPORT_C void GetContentText(CEikRichTextEditor* aEditor);
		IMPORT_C void GetContentSheet(CSheetCtrl* aSheet, TInt aTable);
		IMPORT_C TBool Editable();
		IMPORT_C void SetEditable(TBool aEditable);
		IMPORT_C TDes& Filename();
		IMPORT_C void SetFileName(const TDesC& aFileName);
		IMPORT_C void SetPassword(const TDesC& aPassword);
		IMPORT_C void LoadMetaInfomation();
		IMPORT_C void GetThubnail(MCoeControlObserver* aContainer,TSize aSize);
		IMPORT_C void GetPicture(const TDesC& aFile,TSize aSize);
		IMPORT_C CFbsBitmap* Bitmap();
		IMPORT_C void SetNextViewID(TUid aViewID);
		IMPORT_C TUid NextViewID();
		IMPORT_C void GetTables(CEikMenuPane *aMenuPane);
		IMPORT_C RPointerArray<HBufC>*GetTableArray();
	public:


		//Sheet
		void AddTableName(HBufC* aName);
		void AddCellText(const TDesC& aText);
		void AddCellWhiteSpace(TInt aNumber);
		void AddColumnCell(TInt aAmount = 1);
		void AddRowCell(TInt aAmount = 1);
		void AddCellFormat(void);
		void CellPositionReset();
		void SetValueType(const TDesC& aType, TBool aName);
		IMPORT_C TInt CellEnd();

		//Text
		void AddTextL(const TDesC& aText,TBool aIsList);
		void AddCarriageReturnL();
		void AddWhiteSpace(TInt aNumber);
		void AddTabL();
		void AddNewPageL();
		void AppendParagraphL(void);
		void DeleteL();
		void AddFootnoteLink(const TDesC& aLink);
		void AddTrackedChangeInsertion(const TDesC& aInsert);

		//Writing ODF files
		IMPORT_C void WriteFile(const TDesC& aFileName);
		void WriteMIME(CCreateZip* aZip);
		void WriteContentAndStyles(CCreateZip* aZip);
		void WriteManifest(CCreateZip* aZip);
		//

		void HandleEventL(TInt aEvent,CCImageEngine* aEngine);

		CPara* Para();
		CCZip* Zip();
		CEncryptionData* EncryptData();
		RFs RF();

		//IMPORT_C void CancelOutstanding();

		void AddPictureL(); //CCoeEnv* aCoe,const TDesC& aFile,TCursorSelection aPos);
		
		void ClearContent();
		void ClearMeta();
			
		void SetPages(const TDesC& aValue);
		void SetWords(const TDesC& aValue);
		void SetParagraphs(const TDesC& aValue);
		void SetDescription(const TDesC& aValue);
		void SetTitle(const TDesC& aValue);
		void SetSubject(const TDesC& aValue);
		void SetKeywords(const TDesC& aValue);

		IMPORT_C TDesC& GetPages(void);
		IMPORT_C TDesC& GetWords(void);
		IMPORT_C TDesC& GetParagraphs(void);
		IMPORT_C TDesC& GetDescription(void);
		IMPORT_C TDesC& GetTitle(void);
		IMPORT_C TDesC& GetSubject(void);
		IMPORT_C TDesC& GetKeywords(void);


		IMPORT_C  void GetCurrentNote(CEikonEnv* aEnv,TBool aExternal);
		IMPORT_C void GetNextNote(CEikonEnv* aEnv);
		IMPORT_C void GetPreviousNote(CEikonEnv* aEnv);
		IMPORT_C void ClearNote(void);
		IMPORT_C void HideNote(void);
		RPointerArray<HBufC>* Notes(void);

		IMPORT_C TBool Encrypted();
		void SetEncrypted(TBool aEncrypted);

	private:    // new functions
        COpenDocument();
        void ConstructL();
		void GetContent(const TDesC& aFile, TXMLMode aMode, HBufC* aTable=NULL);

		//MEZBufferManager
		void InitializeL(CEZZStream &aZStream);
		void NeedInputL(CEZZStream &aZStream);
		void NeedOutputL(CEZZStream &aZStream);
		void FinalizeL(CEZZStream &aZStream);

		HBufC8* xml;
		HBufC8* xml2;
		HBufC8* xml3;

		CTextBox* iFloating;

		TUid iNextViewID;
		RFs iRfs;
		CRichText* iRichText;
		CEikRichTextEditor* iEditor;
		CSheetCtrl* iSheet;
		TInt iPos;

		TInt iCellEnd;

		TPoint iPosition;

		TFileName iFileName;
		TBool iEditable;
		TBool iFileModified;
		HBufC8* iPassword;
		TOpenDocumentTyp iODFType;
		TBool iEncrypted;

		CCZip* iZip;
		CXML* iXml;
		CPara* iPara;
		CEncryptionData* iEncryptData;

		MCoeControlObserver* iBitmapContainer;

		CFbsBitmap* iBitmap;


		/****	META **********************************/
		//General
		HBufC* iLocation;
		HBufC* iSize;
		HBufC* iCreated;
		HBufC* iModified;

		//Description
		HBufC* iTitle;
		HBufC* iSubject;
		HBufC* iKeywords;
		HBufC* iDescription;

		//Statistics
		HBufC* iPages;
		HBufC* iWords;
		HBufC* iParagraphs;

		RPointerArray<CCImageEngine>* iBitmaps;
		RPointerArray<HBufC>* iNotes;
		RPointerArray<HBufC>* iTables;
		TInt iNoteIndex;	
		/***********************************************/

		#ifdef _log_
			RFileLogger iFileLogger;
		#endif
    };

#endif  // __OPENDOCUMENT_H__


