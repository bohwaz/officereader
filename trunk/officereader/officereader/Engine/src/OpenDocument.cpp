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

#include "OpenDocument.h"
#include "OpenDocument.pan"

#include <txtetext.h>  //ELineBreak
#include <txtrich.h>  //Richtext
#include <eikmenup.h> //menubar

#include <utf.h>

#include "Xml.h"
#include "CZip.h"
#include "Para.h"
#include "MyPicture.h"
#include "CImageEngine.h"
#include "Encryption.h"

#include "createzip.h"

#include "XMLTags.h"

#include <EZDecompressor.h>
#include <hash.h>
#include <IMCVCODC.H> //base64

#include "EncryptDecrypt.h"


//Series80: techview\eikmenup.h 

#include "sheet_view.h"
#include "sheet_ctrl.h"
#include "cell.h"
#include "sheet_model.h"
#include "sheet_data.h"

/*
#ifdef __SERIES60_30__
	#define NOTE_SIZE  300
#else
	#define NOTE_SIZE  120
#endif
*/

#define NOTE_SIZE 200


CGraphicsContext::TTextAlign GetGraphicAlign(TCellAlign aAlign)
{
	switch (aAlign)
	{
			case ECellLeft: {  return CGraphicsContext::ELeft;}
			case ECellRight: {  return CGraphicsContext::ERight;}
			case ECellCenter: {  return CGraphicsContext::ECenter;}
			default: {  return CGraphicsContext::ECenter;}
	}
}

COpenDocument::COpenDocument()
{
	// No implementation required
}


EXPORT_C COpenDocument::~COpenDocument()
{

	iRfs.Close();

	delete iPassword;

	//General
	delete iLocation;
	delete iSize;
	delete iCreated;
	delete iModified;

	//Description
	delete iTitle;
	delete iSubject;
	delete iKeywords;
	delete iDescription;

	//Statistics
	delete iPages;
	delete iWords;
	delete iParagraphs;

	delete iPara;
	delete iZip;
	delete iEncryptData;

	iBitmaps->ResetAndDestroy();
	delete iBitmaps;

	iNotes->ResetAndDestroy();
	delete iNotes;

	iTables->ResetAndDestroy();
	delete iTables;

	delete iFloating;
	
	//delete iBitmap;	 ??
	// iBitmapData is deleted in RunL function
}

EXPORT_C COpenDocument* COpenDocument::NewLC()
{
	COpenDocument* self = new (ELeave)COpenDocument();
	CleanupStack::PushL(self);
	self->ConstructL();
	return self;
	
}

EXPORT_C COpenDocument* COpenDocument::NewL()
{
	COpenDocument* self=COpenDocument::NewLC();
	CleanupStack::Pop(); // self;
	return self;
}

void COpenDocument::ConstructL()
{
	 iZip = CCZip::NewL();
	 iPara = CPara::NewL();
	 iEncryptData = CEncryptionData::NewL();

	 iRfs.Connect();

	 iPos = 300;
	 iNoteIndex = 0;
	 iPosition = TPoint(0,0);
	 iEncrypted = EFalse;
	 iEditable = EFalse;
	 iFloating = NULL;

	 xml = NULL;
	 xml2 = NULL;
	 xml3 = NULL;
	 iPassword = NULL;

	 iPages =NULL;
	 iWords=NULL;
	 iParagraphs=NULL;
	 iDescription=NULL;
	 iTitle=NULL;
	 iSubject=NULL;
	 iKeywords=NULL;
	 iLocation=NULL;
	 iSize=NULL;
	 iCreated=NULL;					
	 iModified=NULL;

	 iBitmaps = new (ELeave) RPointerArray<CCImageEngine>;
	 iNotes = new (ELeave) RPointerArray<HBufC>;
	 iTables = new (ELeave) RPointerArray<HBufC>;

   	 ClearMeta();	
}

void COpenDocument::ClearContent()
{
	// TODO: nur loeschen wenn neue Datei, sonst nur text neu laden, in xml neuer Modus, alle Styöes rausnehmen
	iEditable = EFalse;
	
	iPara->ClearParas();

	iNoteIndex = 0;

	iPosition = TPoint(0,0);

	iBitmaps->ResetAndDestroy();
	iNotes->ResetAndDestroy();
}


EXPORT_C void COpenDocument::GetContentText(CEikRichTextEditor* aEditor)
{
	iEditor =  aEditor;
	iRichText = aEditor->RichText();

	if (!iEditable)
	{
		ClearContent();
		// read out styles from styles.xml
		GetContent(styles,XMLModeCompleteText);
		// read out styles and text from content.xml
		GetContent(content,XMLModeCompleteText);
	}
	
}

EXPORT_C void COpenDocument::GetTables(CEikMenuPane *aMenuPane)
{

	
	for (TInt i=iTables->Count()-1;i>=0;i--)
	{
		CEikMenuPaneItem::SData	item;
		item.iText = *(*iTables)[i];
		item.iCommandId = ETableIndex + i;
		item.iFlags = 0;
		item.iCascadeId = 0;
#ifdef __UIQ__
		aMenuPane->InsertMenuItemAtL(item,0);
#else
		aMenuPane->InsertMenuItemL(item,0);
#endif	
	}

}

EXPORT_C RPointerArray<HBufC>*COpenDocument::GetTableArray()
{
	return iTables;
}

EXPORT_C void COpenDocument::GetContentSheet(CSheetCtrl* aSheet, TInt aTable)
{
	iSheet =  aSheet;
	
	ClearContent(); // richtig?

	// read out styles from styles.xml
	GetContent(styles, XMLModeCompleteText);

	// read out styles from content.xml and text
	if ( (aTable - ETableIndex) < 0)
	{
		//new sheet or table, so we can also delete this
		iTables->ResetAndDestroy();
		GetContent(content, XMLModeCompleteText);
	}
	else
	{
		HBufC* temp = (*iTables)[aTable-ETableIndex]->Alloc();		
		iTables->ResetAndDestroy();
		GetContent(content, XMLModeCompleteText,temp);
		delete temp;
	}
}

EXPORT_C TOpenDocumentTyp COpenDocument::GetType(void)
{
	TOpenDocumentTyp aType;

	if (iEditable)
	{
		aType = ODT;
	}
	else
	{
		HBufC8* xml = iZip->GetFileText(iRfs,iFileName,mimetype);
		
		if (*xml == writer)
		{
			aType = ODT;
		}
		else
		if (*xml == text_template)
		{
			aType = OTT;
		}
		else
		if (*xml==spread)
		{
			aType = ODS;
		}
		else
		if (*xml==spread_template)
		{
			aType = OTS;
		}
		else
		if (*xml==presentation)
		{
			aType = ODP;
		}
		else
		if (*xml==presentation_template)
		{
			aType = OTP;
		}
		else
		{
			aType = ODFUnknown;	
		}

		delete xml;
		iODFType = aType;

		
		iEncrypted = EFalse;

		
		HBufC8* manifest = iZip->GetFileText(iRfs,iFileName,manifest_xml);
		CXML* parser = CXML::NewL(this); 
		if (parser)
		{
			parser->SetParseDataL(*manifest);
			parser->GetContent();
			delete parser;
		}	
		delete manifest;
	}

	return aType;
}

void COpenDocument::GetContent(const TDesC& aFile, TXMLMode aMode, HBufC* aTable)
{
	CXML* parser = NULL;
	CEncryptDecrypt* object = NULL;

	xml =iZip->GetFileText(iRfs,iFileName,aFile);

	switch ( iODFType )
	{
		case ODP: 
		case OTP: 
		case OTT: 
		case ODT: 
			{ 
				parser = CTextXML::NewL(this);
				break;
			
			}
		case ODS: 
		case OTS: 
			{ 
				parser = CTableXML::NewL(this); 
				((CTableXML*)parser)->iTable = aTable;
				break;
			}
		default:{ 
				break;
			} 
	}


	if (iEncrypted)
	{

		// todo: dynamisch		
		TBuf8<200> salt;
		TBuf8<200> init;
		
		TImCodecB64 base64;
		base64.Initialise();
		if (aFile==content)
		{
			base64.Decode(iEncryptData->SaltContent(),salt);
			base64.Decode(iEncryptData->InitContent(),init);
			xml3 = HBufC8::NewL(iEncryptData->ContentSize());	
			xml3->Des().SetLength(iEncryptData->ContentSize());		
		}
		else
		{
			base64.Decode(iEncryptData->SaltStyle(),salt);
			base64.Decode(iEncryptData->InitStyle(),init);
			xml3 = HBufC8::NewL(iEncryptData->StyleSize());	
			xml3->Des().SetLength(iEncryptData->StyleSize());		
			
		}


		object = CEncryptDecrypt::NewL();				
		xml2 = object->Decrypt(*iPassword,*xml,salt	,init);
		
		CEZDecompressor* decompress = CEZDecompressor::NewL(*this,-CEZDecompressor::EMaxWBits);
		TRAPD(err,decompress->InflateL());				
		delete decompress;
	}

	if (parser)
	{
		if (iEncrypted)
			parser->SetParseDataL(*xml3);
		else
			parser->SetParseDataL(*xml);
		parser->SetMode(aMode);
		parser->GetContent();
		delete parser;
	}	


	if (xml)
	{
		delete xml;
		xml = NULL;
	}

	if (xml2)
	{
		delete xml2;
		xml2 = NULL;
	}
	if (xml3)
	{
		delete xml3;
		xml3 = NULL;
	}

	if (object)
	{
		delete object;
		object = NULL;
	}

}
CCZip* COpenDocument::Zip()
{
	return iZip;
}


EXPORT_C TBool COpenDocument::Encrypted()
{
	return iEncrypted;
}

void COpenDocument::SetEncrypted(TBool aEncrypted)
{
	iEncrypted = aEncrypted;
}

EXPORT_C TDes& COpenDocument::Filename()
{
	return iFileName;
}

EXPORT_C void COpenDocument::SetFileName(const TDesC& aFileName)
{
	iFileName = aFileName;

}


EXPORT_C TBool COpenDocument::Editable()
{
	return iEditable;
}

EXPORT_C void COpenDocument::SetEditable(TBool aEditable)
{
	iEditable = aEditable;
}

EXPORT_C TVersion COpenDocument::Version() const
{
		const TInt KMajor = 1;
		const TInt KMinor = 0;
		const TInt KBuild = 0;
		return TVersion(KMajor, KMinor, KBuild);
}

EXPORT_C void COpenDocument::LoadMetaInfomation()
{
	_LIT(meta,"meta.xml");
	ClearMeta();
	iODFType = GetType();
	GetContent(meta,XMLModeMeta);
}

void COpenDocument::AddTextL(const TDesC& aText,TBool aIsList)
{
	TInt documentLength = iRichText->DocumentLength();

	iRichText->InsertL (documentLength, aText);

	if (iPara->GetGlobalPos()>-1)	// no formatting at all
	{
		if (iPara->IsText())
		{
			iRichText->ApplyCharFormatL(iPara->CharFormat(iPara->GetGlobalPos()), iPara->CharFormatMask(iPara->GetGlobalPos()),documentLength,aText.Length());
			
		}
		iRichText->ApplyParaFormatL(iPara->ParaFormat(iPara->GetGlobalPos()), iPara->ParaFormatMask(iPara->GetGlobalPos()),documentLength,aText.Length());
		iRichText->ApplyCharFormatL(iPara->CharFormat(iPara->GetPos()), iPara->CharFormatMask(iPara->GetPos()),documentLength,aText.Length());
	}
	else
	{
		if (aIsList)
		{
			CParaFormat* aParaFormat = CParaFormat::NewL();
			TParaFormatMask aMask;

			aMask.SetAttrib(EAttBullet);
			aMask.SetAttrib(EAttIndent);
			aMask.SetAttrib(EAttLeftMargin);

			aParaFormat->iBullet = new (ELeave) TBullet();
			aParaFormat->iBullet->iHangingIndent = ETrue;
			aParaFormat->iBullet->iCharacterCode = Para()->Bullet()->Char();
			aParaFormat->iIndentInTwips = FontUtils::TwipsFromPoints(10);	
			aParaFormat->iLeftMarginInTwips = Para()->Bullet()->Indent();
			aParaFormat->iBullet->iHeightInTwips = 100;
			aParaFormat->iBullet->iColor =KRgbBlack;

			TCharFormat aFormat;
			TCharFormatMask aFormatMask;

			aFormatMask.SetAttrib(EAttColor);
			aFormat.iFontPresentation.iTextColor = KRgbBlack;

			iRichText->ApplyCharFormatL(aFormat, aFormatMask,documentLength,aText.Length());				
			iRichText->ApplyParaFormatL(aParaFormat,aMask,documentLength,aText.Length());
			
			delete  aParaFormat;
		}
	}
}

void COpenDocument::AddCarriageReturnL()
{
		iRichText->InsertL(iRichText->DocumentLength(), CEditableText::ELineBreak);
	}

void COpenDocument::DeleteL()
	{
		if (iRichText->DocumentLength()>0)
		{
		iRichText->DeleteL(iRichText->DocumentLength()-1,1);
		}
	}

void COpenDocument::AddTabL()
{
	TInt documentLength = iRichText->DocumentLength();

	iRichText->InsertL(iRichText->DocumentLength(), CEditableText::ETabCharacter);
	iRichText->ApplyCharFormatL(iPara->CharFormat(iPara->GetGlobalPos()), iPara->CharFormatMask(iPara->GetGlobalPos()),documentLength,1);
}


void COpenDocument::AddWhiteSpace(TInt aNumber)
{
	for(TInt i=0;i<aNumber;i++)
		iRichText->InsertL(iRichText->DocumentLength(), CEditableText::ESpace);
	//TODO: warum hier nciht auch applycharformat???
	// TODO: AddText... ???
}

void COpenDocument::AddNewPageL()
{
	iRichText->InsertL(iRichText->DocumentLength(), CEditableText::EPageBreak);
}

void COpenDocument::AppendParagraphL()
{
	if (iRichText->DocumentLength()>0)
		iRichText->AppendParagraphL();
}


EXPORT_C CFbsBitmap* COpenDocument::Bitmap()
{
	return iBitmap;
}

EXPORT_C void COpenDocument::SetNextViewID(TUid aViewID)
{
	iNextViewID = aViewID;
}

EXPORT_C TUid COpenDocument::NextViewID()
{
	return iNextViewID;
}

CPara* COpenDocument::Para()
{
	return iPara;
}

CEncryptionData* COpenDocument::EncryptData()
{
	return iEncryptData;
}


void COpenDocument::SetPages(const TDesC& aValue)
{
	delete iPages;
	iPages = aValue.Alloc();
}

void COpenDocument::SetWords(const TDesC& aValue)
{
	delete iWords;
	iWords = aValue.Alloc();
}


void COpenDocument::SetParagraphs(const TDesC& aValue)
{
	delete iParagraphs;
	iParagraphs = aValue.Alloc();
}


void COpenDocument::SetTitle(const TDesC& aValue)
{
	delete iTitle;
	iTitle = aValue.Alloc();
}

void COpenDocument::SetDescription(const TDesC& aValue)
{
	delete iDescription;
	iDescription = aValue.Alloc();
}

void COpenDocument::SetSubject(const TDesC& aValue)
{
	delete iSubject;
	iSubject = aValue.Alloc();
}

void COpenDocument::SetKeywords(const TDesC& aValue)
{
	delete iKeywords;
	iKeywords = aValue.Alloc();
}


EXPORT_C TDesC& COpenDocument::GetDescription(void)
{
		return *iDescription;
}


EXPORT_C TDesC& COpenDocument::GetPages(void)
{
		return *iPages;
}


EXPORT_C TDesC& COpenDocument::GetWords(void)
{
		return *iWords;
}


EXPORT_C TDesC& COpenDocument::GetParagraphs(void)
{
		return *iParagraphs;
}

EXPORT_C TDesC& COpenDocument::GetTitle(void)
{
		return *iTitle;
}

EXPORT_C TDesC& COpenDocument::GetSubject(void)
{

		return *iSubject;
}

EXPORT_C TDesC& COpenDocument::GetKeywords(void)
{

		return *iKeywords;
}

void COpenDocument::ClearMeta()
{
	_LIT(empty," ");
	 TBuf<1> leer;
	 leer.Append(empty);
	 			

	 SetPages(leer);
	 SetWords(leer);
	 SetParagraphs(leer);
	 SetDescription(leer);
	 SetTitle(leer);
	 SetSubject(leer);
	 SetKeywords(leer);
}


EXPORT_C void COpenDocument::GetThubnail(MCoeControlObserver* aContainer,TSize aSize)
{
	_LIT(thubnail,"Thumbnails\\thumbnail.png");
	iBitmapContainer = aContainer;

	CCImageEngine* aEngine = new (ELeave) CCImageEngine(iRfs);
	iBitmaps->Append(aEngine);
	aEngine->GetImage(this,TDMThubnail,aSize, 0 ,thubnail);
}

/*
EXPORT_C void COpenDocument::CancelOutstanding()
{
	for (TInt i=0;i<= iBitmaps->Count()-1;i++)
	{
		(*iBitmaps)[i]->Cancel();
	}
}
*/



EXPORT_C void COpenDocument::GetPicture(const TDesC& aFile,TSize aSize)
{
//	DOES NOT WOKR ON UIQ3
	if (aSize.iWidth > iEditor->Size().iWidth)
	{
		aSize.iWidth = iEditor->Size().iWidth - 20;
	}

	
	if (aSize.iHeight > iEditor->Size().iHeight)
	{
		aSize.iHeight = iEditor->Size().iHeight - 80;
	}

	//fix this bug, it should never be 0
	if (aSize.iWidth == 0)
	{
		aSize.iWidth = 50;
	}
	
	if (aSize.iHeight == 0)
	{
		aSize.iHeight = 50;
	}


	/*
	if (aSize.iWidth> 200)
	{
		aSize.iWidth = 200;
	}
	if (aSize.iHeight> 200)
	{
		aSize.iHeight = 200;
	}
	*/
	
	
	

	CCImageEngine* aEngine = new (ELeave) CCImageEngine(iRfs);
	if (!aEngine->GetImage(this,TDMImage,aSize, iRichText->DocumentLength() ,aFile))
	{
		//CleanupStack::PopAndDestroy();
		delete aEngine;
	}
	else
	{
		iBitmaps->Append(aEngine);
//		CleanupStack::Pop();
	}
}

RFs COpenDocument::RF()
{
	return iRfs;
}


void COpenDocument::AddPictureL()//CCoeEnv* aCoe,const TDesC& aFile,TCursorSelection aPos)
{

  // CFbsBitmap returns wrong size in twips!! Use this workaround
  TSize pixelSize = iBitmap->SizeInPixels();
  //TRect rect = TRect(TPoint(),TSize(100,100)); //pixelSize);
  TRect twipRect = (CCoeEnv::Static()->ScreenDevice()->PixelsToTwips(pixelSize));
  TSize sizeInTwips = twipRect.Size();

  // create new picture header object to insert into text
  CMyPicture* picture = new (ELeave) CMyPicture(sizeInTwips, *iBitmap);
  CleanupStack::PushL(picture);
  TPictureHeader header;
  header.iPicture = TSwizzle<CPicture>(picture);
			
  // insert picture into text
  if (iRichText && iEditor)
  {
	iRichText->InsertL(iPos , header);
	iEditor->HandleTextChangedL();
  }	
  else
  {

  }
 /*
	//TODO: formmatierungen hier anwenden?
	CParaFormat* aFormat = CParaFormat::NewL();
	TParaFormatMask aMask;

 
	aFormat->iLeftMarginInTwips = FontUtils::TwipsFromPoints(50);
	aMask.SetAttrib(EAttLeftMargin);
 

	aMask.SetAttrib(EAttAlignment);
	aFormat->iHorizontalAlignment = CParaFormat::ERightAlign;

 iRichText->ApplyParaFormatL(aFormat,aMask,iPos,1);
*/

	
 CleanupStack::Pop();
  
}

void COpenDocument::HandleEventL(TInt aEvent,CCImageEngine* aEngine)
{
	//next  line was moved to getimage
	//iBitmaps->Append(aEngine);

  iBitmap = aEngine->Bitmap();
  //add the position of the bitmap in the array, because if we added 100 bitmaps before, our current documentlength is 100 positions longer
  iPos = aEngine->Pos() + iBitmaps->Count() - 1;
  
  if (iPos<0)
  {
	  iPos = 0;
  }
  
  switch (aEvent)
  {
	 //Thubnail
	case 1: iBitmapContainer->HandleControlEventL(NULL,MCoeControlObserver::EEventStateChanged ); break;
	//Image embedded into text
	case 2: AddPictureL();
  }

}

EXPORT_C void COpenDocument::GetCurrentNote(CEikonEnv* aEnv,TBool aExternal)
{
	if ( (iNotes->Count()>iNoteIndex) && (iNoteIndex > -1) )
	{
		if (aExternal && iFloating)
		{
			delete iFloating;
			iFloating = NULL;
		}
		else
		{
			delete iFloating;
			iFloating=new (ELeave) CTextBox();
			iFloating->ConstructL(aEnv, KRgbBlue, KRgbWhite, KRgbBlack, 1, 2);
			iFloating->SetText(NOTE_SIZE,  (*iNotes)[iNoteIndex] , CTextBox::ETopLeft, TPoint(5, 5));
			iFloating->ActivateL();
		}
	}
}

EXPORT_C void COpenDocument::GetNextNote(CEikonEnv* aEnv)
{
	TInt aTemp = iNoteIndex;
	TInt aHigh = iNotes->Count()-1;

	if (iNoteIndex+1 > aHigh)
	{
		iNoteIndex = 0;
	}
	else
	{
		iNoteIndex++;
	}

	if (aTemp != iNoteIndex)
	{
		GetCurrentNote(aEnv,EFalse);
	}
}

EXPORT_C void COpenDocument::GetPreviousNote(CEikonEnv* aEnv)
{
	TInt aTemp = iNoteIndex;

	if (iNoteIndex-1 < 0)
	{
		iNoteIndex = iNotes->Count()-1;
	}
	else
	{
		iNoteIndex--;
	}

	if (aTemp != iNoteIndex)
	{
		GetCurrentNote(aEnv,EFalse);
	}
}


RPointerArray<HBufC>* COpenDocument::Notes(void)
{
	return iNotes;
}

EXPORT_C void COpenDocument::HideNote(void)
{
	if (iFloating)
	{
		delete iFloating;
		iFloating = NULL;
	}
}

EXPORT_C void COpenDocument::ClearNote(void)
{
	if (iFloating)
	{
		delete iFloating;
		iFloating = NULL;
	}
	iNotes->ResetAndDestroy();
}

void COpenDocument::AddFootnoteLink(const TDesC& aLink)
{

	TCharFormat aFormat;
	TCharFormatMask aFormatMask;

	aFormatMask.SetAttrib(EAttFontPrintPos);
	aFormat.iFontSpec.iFontStyle.SetPrintPosition(EPrintPosSuperscript);	

	TInt documentLength = iRichText->DocumentLength();

	iRichText->InsertL (documentLength, aLink);
	iRichText->ApplyCharFormatL(aFormat, aFormatMask,documentLength,aLink.Length());	
}

void COpenDocument::AddTrackedChangeInsertion(const TDesC& aInsert)
{
	//TODO: list with deleted items to insert later
	TInt documentLength = iRichText->DocumentLength();
	
	AddTextL(aInsert,EFalse);
	
	TCharFormat aFormat;
	TCharFormatMask aFormatMask;

	aFormatMask.SetAttrib(EAttFontUnderline);
	aFormat.iFontPresentation.iUnderline = EUnderlineOn;	

	aFormatMask.SetAttrib(EAttColor);
	aFormat.iFontPresentation.iTextColor = KRgbRed;

	iRichText->ApplyCharFormatL(aFormat, aFormatMask,documentLength,aInsert.Length());	
}

void COpenDocument::AddCellWhiteSpace(TInt aNumber)
{
	HBufC* text = HBufC::NewL(aNumber);

	for (TInt i=0;i<aNumber;i++)
		text->Des().Append(CEditableText::ESpace);
	
	AddCellText(*text);
	delete text;
}

void COpenDocument::AddCellText(const TDesC& aText)
{

	// TODO: cell formmatierung as well as column formatierung

	if (iSheet->Model()->IsExistL(iPosition))
	{
		CCell* aCell = iSheet->Model()->AtL(iPosition);
		aCell->AppendL(aText);
	}
	else
	{
		CCell aCell;
		aCell = aText;

		// formmatierung für text hier(aufruf von CharacterData), für celle bereits in startelement
		//iSheet->Data()->SetCellColorL(iPosition, Para()->CellFormat( Para()->GetPos() ).Color() );
		iSheet->Model()->InsertL( iPosition, aCell );
		//AddCellFormat();
	}

	if (iPosition.iY > iCellEnd)
	{
		iCellEnd = iPosition.iY;
	}
}

void COpenDocument::AddCellFormat(void)
{
	if (Para()->CellFormat( Para()->GetPos() )->Changed() )
	{
		if (!iSheet->Model()->IsExistL(iPosition))
		{
			CCell aCell;
			//aCell = _L("######");
			aCell = _L("");
			iSheet->Model()->InsertL( iPosition, aCell );

		}
		if (Para()->CellFormat( Para()->GetPos() )->Color() != TRgb(255,255,255))
			iSheet->Data()->SetCellColorL(iPosition, Para()->CellFormat( Para()->GetPos() )->Color() );
		if (Para()->CellFormat( Para()->GetPos() )->FontColor() != TRgb(255,255,255))
			iSheet->Data()->SetCellTextColorL(iPosition, Para()->CellFormat( Para()->GetPos() )->FontColor() );
		if (Para()->CellFormat( Para()->GetPos() )->Align() != ECellNone)  // Standardeinstellung
			iSheet->Data()->SetCellAlignmentL(iPosition, GetGraphicAlign( Para()->CellFormat( Para()->GetPos() )->Align()) );
	}
}

void COpenDocument::SetValueType(const TDesC& aType, TBool /* aName */)
{
		_LIT(string,"string");
//		_LIT(date,"date");
		_LIT(time,"time");

		if ((aType==string))// ||(aType==date))
		{
			if (!iSheet->Model()->IsExistL(iPosition))
			{
				CCell aCell;
				aCell = _L("");

				iSheet->Model()->InsertL( iPosition, aCell );
			}
			
			iSheet->Data()->SetCellAlignmentL(iPosition,CGraphicsContext::ELeft);
		}

		if ((aType==time))// ||(aType==date))
		{
			if (!iSheet->Model()->IsExistL(iPosition))
			{
				CCell aCell;
				aCell = _L("");

				iSheet->Model()->InsertL( iPosition, aCell );
			}
			
			iSheet->Data()->SetCellAlignmentL(iPosition,CGraphicsContext::ECenter);
		}

}

void COpenDocument::CellPositionReset()
{
	iPosition = TPoint(0,0);
	iCellEnd = 0;
}

void COpenDocument::AddColumnCell(TInt aAmount)
{
	iPosition.iX += aAmount; 
}

void COpenDocument::AddRowCell(TInt aAmount)
{
	iPosition.iY += aAmount;	
	iPosition.iX = 0;
}

void COpenDocument::AddTableName(HBufC* aName)
{
	iTables->Append(aName);
}

EXPORT_C void COpenDocument::SetPassword(const TDesC& aPassword)
{
		delete iPassword;
		iPassword = HBufC8::NewL(aPassword.Length());
		iPassword->Des().Copy(aPassword);
}

EXPORT_C TInt COpenDocument::CellEnd()
{
	return iCellEnd;
}



/*******	MEZBufferManager  ********************************/
void COpenDocument::InitializeL(CEZZStream &aZStream)
{
	aZStream.SetInput(*xml2);
	
	TPtr8 ptr_xml = xml3->Des();
	aZStream.SetOutput(ptr_xml);

}
void COpenDocument::NeedInputL(CEZZStream &/*aZStream*/)	{}
void COpenDocument::NeedOutputL(CEZZStream &/*aZStream*/) 	{}
void COpenDocument::FinalizeL(CEZZStream &/*aZStream*/)		{}
/*************************************************************/


/********	Writing of ODF files ******************************/
EXPORT_C void COpenDocument::WriteFile(const TDesC& aFileName)
{
	CCreateZip* aZip = CCreateZip::NewL(iRfs,aFileName);
	WriteMIME(aZip);	
	WriteContentAndStyles(aZip);
	WriteManifest(aZip);
	aZip->FlushToZip();
	delete aZip;
}

void COpenDocument::WriteManifest(CCreateZip* aZip)
{
	HBufC* aStyle = HBufC::NewLC(1000);
	TPtr pStyle = aStyle->Des();
	
	pStyle.Append(xml_encoding);
	pStyle.Append(xml_manifest);


	HBufC8* nein = HBufC8::NewL(pStyle.Size());
	nein->Des().Copy(pStyle);

	aZip->AddFile(*nein,manifest_xml,EFalse,1);

	delete nein;

	CleanupStack::PopAndDestroy(aStyle);
}

void COpenDocument::WriteMIME(CCreateZip* aZip)
{
	switch (iODFType)
	{
		case ODP:	{ aZip->AddFile(presentation,mimetype); break; }
		case OTP:	{ aZip->AddFile(presentation_template,mimetype); break; }
		case OTT:	{ aZip->AddFile(text_template,mimetype); break; }
		case ODT:	{ aZip->AddFile(writer,mimetype); break; }
		case ODS:	{ aZip->AddFile(spread,mimetype); break; }
		case OTS:	{ aZip->AddFile(spread_template,mimetype); break; }
		case ODFUnknown:{ break; }
	}
}
	
void COpenDocument::WriteContentAndStyles(CCreateZip* /*aZip*/)
{
	HBufC* aStyle = HBufC::NewLC(9000);
	TPtr pStyle = aStyle->Des();
	pStyle.Append(xml_encoding);
	pStyle.Append(start_xml_xml);
	pStyle.Append(start_xml_style);

	// xml_encoding.Öength().....
	HBufC* aContent = HBufC::NewLC( iRichText->DocumentLength() + 90000);
	TPtr pContent = aContent->Des();
	pContent.Append(xml_encoding);
	pContent.Append(start_xml);
	pContent.Append(start_xml_writer);

	TInt length;
	TInt Start;
	HBufC* aText;
	CParaFormat* aFormat = CParaFormat::NewLC();

	for (TInt i=0; i<=iRichText->ParagraphCount()-1;i++)
	{
		Start = iRichText->CharPosOfParagraph(length,i);
		iRichText->GetParagraphFormatL(aFormat,i);
		
		// add paragrah stuff zu styles
		// mit GetChars(TPtrC &aView, TCharFormat &aFormat, TInt aPos) bekommen wir die characterformatietung
		// UND CHARCTER selber, also nicht mehr extract
		// die aller rste fügen wir dem Paragrahen hinzu, ab dem 2. dan eigene mit T1, T2 etc.
		
		if (length-1==0)
		{
			pContent.Append(startend_xml_para);
		}
		else
		{
			aText = HBufC::NewL(length);
			TPtr text = aText->Des();
			iRichText->Extract(text,Start,length-1);
			pContent.Append(start_xml_para);
			pContent.Append(*aText);
			pContent.Append(end_xml_para);
			delete aText;
		}
	}
	
	CleanupStack::PopAndDestroy(aFormat);

	pContent.Append(end_xml_writer);
	pContent.Append(end_xml);

	/*
	HBufC8* pContent8 = HBufC8::NewL(pContent.Size());
	pContent8->Des().Copy(pContent);
	aZip->AddFile(*pContent8,content);
	delete pContent8;

	HBufC8* pStyle8 = HBufC8::NewL(pStyle.Size());
	pStyle->Des().Copy(pStyle);
	aZip->AddFile(*pStyle,styles);
	delete pStyle8;
	*/



	CleanupStack::PopAndDestroy(aContent);
	CleanupStack::PopAndDestroy(aStyle);
}
/****************************************************/		
