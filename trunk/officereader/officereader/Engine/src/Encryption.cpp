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

#include "Encryption.h"


CEncryptionData::CEncryptionData()
{
	// No implementation required
}


CEncryptionData::~CEncryptionData()
{
	delete iContentCheckSum;
	delete iContentInit;
	delete iContentSalt;

	delete iStyleCheckSum;
	delete iStyleInit;
	delete iStyleSalt;
}

CEncryptionData* CEncryptionData::NewLC()
{
	CEncryptionData* self = new (ELeave)CEncryptionData();
	CleanupStack::PushL(self);
	self->ConstructL();
	return self;
}

CEncryptionData* CEncryptionData::NewL()
{
	CEncryptionData* self=CEncryptionData::NewLC();
	CleanupStack::Pop(); // self;
	return self;
}

void CEncryptionData::ConstructL()
{
	iContentCheckSum = NULL;
	iContentInit = NULL;
	iContentSalt = NULL;

	iStyleCheckSum = NULL;
	iStyleInit = NULL;
	iStyleSalt = NULL;
}


void CEncryptionData::SetCurrentFile(const TDesC& aFile)
{
	_LIT(styles_xml,"styles.xml");
	_LIT(content_xml,"content.xml");

	if (aFile==content_xml)
	{
		iCurrentFile = EODFContent;
	}
	else
	if (aFile==styles_xml)
	{
		iCurrentFile = EODFStyles;
	}
	else
	{
		iCurrentFile = EODFNone;
	}
}

void CEncryptionData::SetCurrentFileSize(TInt aSize)
{
	switch (iCurrentFile)
	{
		case EODFContent: 
				{
					iContentSize = aSize;
					break; 
				};
		case EODFStyles: 
				{
					iStyleSize = aSize;
					break; 
				};
		default: break;
	};

}

void CEncryptionData::SetInit(const TDesC& aInit)
{
	switch (iCurrentFile) 
	{
		case EODFContent: 
				{
					delete iContentInit;
					iContentInit = HBufC8::NewL(aInit.Length());
					iContentInit->Des().Copy(aInit);					
					break;
				};
		case EODFStyles: 
				{
					delete iStyleInit;
					iStyleInit = HBufC8::New(aInit.Length());
					iStyleInit->Des().Copy(aInit);
					break;
				};
		default: break;
	};
}

void CEncryptionData::SetSalt(const TDesC& aSalt)
{
	switch (iCurrentFile) 
	{
		case EODFContent: 
				{
					delete iContentSalt; 
					iContentSalt = HBufC8::New(aSalt.Length());
					iContentSalt->Des().Copy(aSalt);
					break; 
				};
		case EODFStyles: 
				{
					delete iStyleSalt;
					iStyleSalt = HBufC8::New(aSalt.Length());
					iStyleSalt->Des().Copy(aSalt);
					break; 
				};
		default: break;
	};
}

void CEncryptionData::SetCheckSum(const TDesC& aCheckSum)
{
	
	switch (iCurrentFile) 
	{
		case EODFContent: 
				{
					delete iContentCheckSum; 
					iContentCheckSum = HBufC8::NewL(aCheckSum.Length());
					iContentCheckSum->Des().Copy(aCheckSum);
					break; 
				};
		case EODFStyles: 
				{
					delete iStyleCheckSum;
					iStyleCheckSum = HBufC8::NewL(aCheckSum.Length());
					iStyleCheckSum->Des().Copy(aCheckSum);
					break; 
				};
		default: break;
	};
}


TInt CEncryptionData::ContentSize(void)
{
	return iContentSize;
}

TInt CEncryptionData::StyleSize(void)
{
	return iStyleSize;
}

TPtr8 CEncryptionData::CheckSumStyle(void)
{
	return iStyleCheckSum->Des();
}

TPtr8 CEncryptionData::CheckSumContent(void)
{
	return iContentCheckSum->Des();
}

TPtr8 CEncryptionData::InitStyle(void)
{
	return iStyleInit->Des();
}

TPtr8 CEncryptionData::InitContent(void)
{
	return iContentInit->Des();
}

TPtr8 CEncryptionData::SaltStyle(void)
{
	return iStyleSalt->Des();
}

TPtr8 CEncryptionData::SaltContent(void)
{
	return iContentSalt->Des();
}

