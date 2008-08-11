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

#ifndef ENCRYPTION_H
#define ENCRYPTION_H

// INCLUDES
#include <e32std.h>
#include <e32base.h>

enum TODFFile
{
	EODFNone = 0,
	EODFContent,
	EODFStyles,
	EODFSettings
};

class CEncryptionData : public CBase
{
public: // Constructors and destructor

	~CEncryptionData();

	static CEncryptionData* NewL();
    static CEncryptionData* NewLC();

	void SetCurrentFile(const TDesC& aFile);
	void SetCurrentFileSize(TInt aSize);
	void SetInit(const TDesC& aInit);
	void SetSalt(const TDesC& aSalt);
	void SetCheckSum(const TDesC& aCheckSum);


	TInt ContentSize(void);
	TInt StyleSize(void);

	TPtr8 CheckSumStyle(void);
	TPtr8 CheckSumContent(void);
	
	TPtr8 InitStyle(void);
	TPtr8 InitContent(void);

	TPtr8 SaltStyle(void);
	TPtr8 SaltContent(void);

private:

	CEncryptionData();
	void ConstructL();

	TODFFile iCurrentFile;

	HBufC8* iContentSalt;
	HBufC8* iContentInit;
	HBufC8* iContentCheckSum;
	TInt iContentSize;

	HBufC8* iStyleSalt;
	HBufC8* iStyleInit;
	HBufC8* iStyleCheckSum;
	TInt iStyleSize;

};

#endif // ENCRYPTION_H
