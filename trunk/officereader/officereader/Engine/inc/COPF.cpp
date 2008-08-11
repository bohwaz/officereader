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

#include "COPF.h"

 CODF::CODF()
{
}

 CODF::~CODF()
{
}

void CODF::GetODFTextL(const TDesC& aFileName,CEikEdwin* aEdwin) {

	iEdwin = aEdwin;
	RFs aRFSession;
	aRFSession.Connect();
	CZipFile* zipFile = CZipFile::NewL(aRFSession, aFileName);
	CleanupStack::PushL(zipFile);
	CZipFileMember* member = zipFile->CaseInsensitiveMemberL(content);
	CleanupStack::PushL(member);
	RZipFileMemberReaderStream* stream;
	zipFile->GetInputStreamL(member, stream);
	CleanupStack::PushL(stream);
	HBufC8* aBuffer = HBufC8::NewL(member->UncompressedSize());
	TPtr8 aBufferPtr(aBuffer->Des());
	User::LeaveIfError(stream->Read(aBufferPtr, member->UncompressedSize()));		
	CleanupStack::PopAndDestroy(3);
	aRFSession.Close();
	
	CleanupStack::PushL(aBuffer);
	_LIT8 (KMimeType, "text/xml");
	CParser* aParser=CParser::NewL(KMimeType,*this);
	CleanupStack::PushL(aParser);
	aParser->ParseBeginL();
	aParser->ParseL(aBufferPtr);
	aParser->ParseEndL();
	CleanupStack::PopAndDestroy( aParser);
	CleanupStack::PopAndDestroy( aBuffer);

};

void CODF::OnContentL(const TDesC8& aBytes, TInt aErrorCode)
{
	HBufC* aBuf = HBufC::NewL(aBytes.Length());
	CleanupStack::PushL(aBuf);
	aBuf->Des().Copy(aBytes);
	iEdwin->Text()->InsertL(iEdwin->TextLength(),*aBuf);
	CleanupStack::PopAndDestroy( aBuf);

};
	
void CODF::OnStartDocumentL(const RDocumentParameters& aDocParam, TInt aErrorCode){};
void CODF::OnEndDocumentL(TInt aErrorCode){};
void CODF::OnStartElementL(const RTagInfo& aElement, const RAttributeArray& aAttributes,TInt aErrorCode){};
void CODF::OnEndElementL(const RTagInfo& aElement, TInt aErrorCode){};
void CODF::OnStartPrefixMappingL(const RString& aPrefix, const RString& aUri, TInt aErrorCode){};
void CODF::OnEndPrefixMappingL(const RString& aPrefix, TInt aErrorCode){};
void CODF::OnIgnorableWhiteSpaceL(const TDesC8& aBytes, TInt aErrorCode){};
void CODF::OnSkippedEntityL(const RString& aName, TInt aErrorCode){};
void CODF::OnProcessingInstructionL(const TDesC8& aTarget, const TDesC8& aData, TInt aErrorCode){};
void CODF::OnError(TInt aErrorCode){};
TAny *CODF::GetExtendedInterface(const long auid){};
