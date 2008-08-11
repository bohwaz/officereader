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

#include "CZip.h"

CCZip::CCZip()
{
}


CCZip::~CCZip()
{
}

CCZip* CCZip::NewLC()
{
	CCZip* self = new (ELeave)CCZip();
	CleanupStack::PushL(self);
	self->ConstructL();
	return self;
}

CCZip* CCZip::NewL()
{
	CCZip* self=CCZip::NewLC();
	CleanupStack::Pop(); // self;
	return self;
}

void CCZip::ConstructL()
{

}


HBufC* ConvertTo16BitL(const TDesC8& aDes8)
{
   HBufC* myBuf = HBufC::NewL(aDes8.Length());
   myBuf->Des().Copy(aDes8);
   return myBuf;
}


HBufC8* CCZip::GetFileText(RFs aRFs,const TDesC& aContainer, const TDesC& aFile)
{
	CZipFile* zipFile = CZipFile::NewL(aRFs, aContainer);
	CleanupStack::PushL(zipFile);
	CZipFileMember* member = zipFile->CaseInsensitiveMemberL(aFile);
	if (member)
	{
		CleanupStack::PushL(member);
		RZipFileMemberReaderStream* stream;
		zipFile->GetInputStreamL(member, stream);
		CleanupStack::PushL(stream);

		HBufC8* buffer = HBufC8::NewLC(member->UncompressedSize());
		TPtr8 bufferPtr(buffer->Des());
		User::LeaveIfError(stream->Read(bufferPtr, member->UncompressedSize()));

		CleanupStack::Pop();	//buffer
		CleanupStack::PopAndDestroy(3);
		return buffer;
	}
	else {
		CleanupStack::PopAndDestroy();
		return NULL;
	}
}
