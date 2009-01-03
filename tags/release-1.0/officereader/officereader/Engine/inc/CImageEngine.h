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

#ifndef CIMAGEENGINE_H
#define CIMAGEENGINE_H

#include <e32base.h>	// For CActive, link against: euser.lib
#include <e32std.h>		// For RTimer, link against: euser.lib

#include <ImageConversion.h> 
#include <BitmapTransforms.h>
#include <fbs.h>

#include "Opendocument.h"

enum TDecodeMode
{
	TDMThubnail = 0,
	TDMImage
};

class CCImageEngine : public CActive
{
public:
	// C++ constructor
	CCImageEngine(RFs aRf);
	
	// Second-phase constructor
	void ConstructL();
	
	// Cancel and destroy
	~CCImageEngine();

public: // New functions
	
	 TBool GetImage(COpenDocument* aDoc,TDecodeMode aMode,TSize aSize, TInt aPos,const TDesC& aFileName);
	 CFbsBitmap* Bitmap(void);
	 TInt Pos(void);

private: // From CActive
	// Handle completion
	void RunL();
	
	// How to cancel me
	void DoCancel();
	
	TBool StartDecode(const TDesC& aFile, TSize aSize);
	void Scale(void);

	// Override to handle leaves from RunL(). Default implementation causes
	// the active scheduler to panic.
	//void RunError(TInt aError);


private:

		TInt status;
		RFs iRfs;	// use from opendoucment

		CFbsBitmap* iBitmap;

		TDecodeMode iDM;
    	TSize iBitSize;
		TInt iPos;
		
		HBufC8* iBitmapData;
		CImageDecoder* iImageDecoder;
		CBitmapScaler* iImageScaler;

		COpenDocument* iDoc;


};

#endif
