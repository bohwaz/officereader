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

#include <coemain.h>

#include "CImageEngine.h"
#include "CZip.h"




CCImageEngine::CCImageEngine(RFs aRf) : CActive(EPriorityStandard)	// Standard priority
{
	ConstructL();
	iRfs = aRf;
}

void CCImageEngine::ConstructL()
{
	CActiveScheduler::Add(this);				// Add to scheduler
}

CCImageEngine::~CCImageEngine()
{
	Cancel(); // Cancel any request, if outstanding

	if (iBitmap)
		delete iBitmap;

	if (iImageDecoder)
		delete iImageDecoder;

	if (iImageScaler)
		delete iImageScaler;

}

void CCImageEngine::DoCancel()
{
	if (iImageDecoder)
	{
		iImageDecoder->Cancel();
	}

	if (iImageScaler)
	{
		iImageScaler->Cancel();
	}

	if (iBitmapData)
	{
		delete iBitmapData;
	}
}

TBool CCImageEngine::GetImage(COpenDocument* aDoc,TDecodeMode aMode,TSize aSize, TInt aPos,const TDesC& aFileName)
{
	iDoc = aDoc;
	iPos = aPos;
	iBitSize = aSize;
	iDM = aMode;

	return StartDecode(aFileName,aSize);
}

void CCImageEngine::RunL()
{
	if( iStatus == KErrNone ) 
				{
				  /*
				  if (iDM== TDMThubnail)
				  {
					if (status== 0 ) 
					{
						Scale();
					} 
					else
					{
						
						iDoc->HandleEventL(1,this);
						delete iBitmapData;
					}
				  }
				  else  //Thubnail
				  {
						iDoc->HandleEventL(2,this);
						delete iBitmapData;
				  }
				  */
					if (status==0)
					{
						Scale();
					}
					else
					{
						if (iDM == TDMThubnail)
							iDoc->HandleEventL(1,this);
						else
							iDoc->HandleEventL(2,this);
						
						delete iBitmapData;
					}



				}//KERRNone
			else if( iStatus == KErrUnderflow ) 
				{
				iImageDecoder->ContinueConvert( &iStatus );
				SetActive();
				}
}


TBool CCImageEngine::StartDecode(const TDesC& aFile, TSize aSize)
{
    status = 0;
	iBitSize = aSize;

	delete iBitmap;
	iBitmap = NULL;

	delete iImageDecoder; 
	iImageDecoder = NULL;
	
	iBitmapData =iDoc->Zip()->GetFileText(iRfs,iDoc->Filename(),aFile);
	
	// create the decoder
	if (iBitmapData)
	{
		TBuf8<256> MIME;
		TRAPD(error,CImageDecoder::GetMimeTypeDataL(*iBitmapData,MIME));
		
		if (error== KErrNone)
		{
			iImageDecoder = CImageDecoder::DataNewL( iRfs,*iBitmapData,MIME);// ,CImageDecoder::EOptionAlwaysThread);
			
			// create the destination bitmap
			iBitmap = new (ELeave) CFbsBitmap();
			iBitmap->Create( iImageDecoder->FrameInfo().iOverallSizeInPixels,
							iImageDecoder->FrameInfo().iFrameDisplayMode ); 

			TRAPD(nein,iImageDecoder->Convert( &iStatus, *iBitmap ));

			SetActive();
			return ETrue;
		}
		else
		{
			delete iBitmapData;
			return EFalse;
		}
	}
	else
		return EFalse;
}

void CCImageEngine::Scale(void )
{
	status = 1;
	
	delete iImageScaler;
	iImageScaler = NULL;

	iImageScaler = CBitmapScaler::NewL();
	iImageScaler->Scale(&iStatus,*iBitmap,iBitSize);
	SetActive();
}

CFbsBitmap* CCImageEngine::Bitmap(void)
{
	return iBitmap;
}

TInt CCImageEngine::Pos(void)
{
	return iPos;
}
