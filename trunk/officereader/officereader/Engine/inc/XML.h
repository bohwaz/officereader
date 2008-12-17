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
 * but WITHOUT ANY WARRANTY; w	ithout even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General (Lesser) Public License version 3 for more details
 * (a copy is included in the LICENSE file that accompanied this code).
 *
 * You should have received a copy of the GNU (Lesser) General Public License
 * version 3 along with Office Reader.  If not, 
 * see <http://www.gnu.org/licenses/>.
 *
 ************************************************************************/

#ifndef XML_H
#define XML_H

#include <e32std.h>
#include <e32base.h>
#include <f32file.h>
#include <txtetext.h>  //ELineBreak
#include <txtrich.h>  //Richtext

#include "OpenDocument.h"

#include <SyExpat.h>

//#define _log_

#ifdef _log_
	#include <flogger.h>
#endif

class CTags: public CBase
{
	public:
	
		~CTags();

		static CTags* NewL();
		static CTags* NewLC();

		void PushL(HBufC* aName);
#ifdef _log_
		void Pop(TPtrC aName);
#else
		void Pop();
#endif
		TBool IsNote();
		TBool IsText();
		TBool IsNoteCitation();
		TBool IsFooter();
		TBool IsHeader();
		TBool IsAnnotation();
		TBool IsTrackedChanges();
		TBool IsTable();
		TBool IsTitle();
		TBool IsDescription();
		TBool IsSubject();
		TBool IsKeyword();
		TBool IsList();

	private:	
		RPointerArray<HBufC>* iTags;
	protected:
		CTags();
		void ConstructL();
};


class CXML : public CBase, MSyDocHandler
{
	public: // Constructors and destructor

		~CXML();
		static CXML* NewL(COpenDocument* aParent);
		static CXML* NewLC(COpenDocument* aParent);

		void SetParseDataL(const TDesC8& aData);
		void GetContent();
		void SetMode(TXMLMode aInfo);

		//HBufC* GetValue(const TDesC8& aData,const TDesC& aName, const TDesC&aPrefix);
		//HBufC* GetAttributeValue(const TDesC& aAttribute);

	protected:	
		CXML();
		virtual void ConstructL();
	

		virtual void StartElement(const TQualified& aName, const RArray< ::TAttribute>& aAttributes);
		virtual void EndElement(const TQualified& aName);
		virtual void CharacterData(const TDesC& aName);
		virtual void ProcessingInstruction(const TDesC& aTarget, const TDesC& aData);
		virtual void Comment(const TDesC& aData) ;
		virtual void StartCDataSection();
		virtual void EndCDataSection();
		virtual void Default(const TDesC& aData);
		virtual void SetParser(CExpatParserBase* aParser);
			 	
		CTags* iTags;	// stackclass to hold XML tags
		HBufC* temp;	// temp buffer for storing note fragments
		
		TBool iChange;
		TXMLMode iMode;
		
		TPtrC8 iData;
		RFs iRFs;		
		CSyParser* iReader;
		COpenDocument* iOpenDocument;

		#ifdef _log_
			RFileLogger iFileLogger;
		#endif

};

class CTableXML: public CXML
{
	public: 
		static CTableXML* NewL(COpenDocument* aParent);
		static CTableXML* NewLC(COpenDocument* aParent);
		TInt iColumnRepeated;//TODO: in private
		TInt iColumnSpanned;
		TInt iRowRepeated;
		HBufC *iTable;
		TBool iCorrectTable;
	protected:
		virtual void StartElement(const TQualified& aName, const RArray< ::TAttribute>& aAttributes);
		virtual void EndElement(const TQualified& aName);
		virtual void CharacterData(const TDesC& aName);
	private:
		TBool iAddFormat;
};

class CTextXML: public CXML
{
	public: 
		static CTextXML* NewL(COpenDocument* aParent);
		static CTextXML* NewLC(COpenDocument* aParent);
	protected:
		virtual void ConstructL();
		virtual void StartElement(const TQualified& aName, const RArray< ::TAttribute>& aAttributes);
		virtual void EndElement(const TQualified& aName);
		virtual void CharacterData(const TDesC& aName);
	private:
		HBufC* iWidth;
		HBufC* iHeight;
};
#endif // XML_H
