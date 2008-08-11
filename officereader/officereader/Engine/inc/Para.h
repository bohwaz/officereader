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

#ifndef PARA_H
#define PARA_H

// INCLUDES
#include <e32std.h>
#include <e32base.h>

#include <txtfrmat.h>
#include <GDI.H>

#include "CellFormat.h"
#include "Bullet.h"


//#define _log_

#ifdef _log_
	#include <flogger.h>	
#endif



// CLASS DECLARATION

/**
*  CPara
* 
*/
class CPara : public CBase
{
public: // Constructors and destructor

	~CPara();
	static CPara* NewL();
	static CPara* NewLC();

	//Text
	void AddTextItem(TBool aInit,HBufC* aName);

	void SetColor(TRgb aColor)	;
	void SetColor(const TDesC& aColor)	;

	void SetParaHighlightColor(TRgb aColor);
	void SetParaHighlightColor(const TDesC& aColor);

	void SetHighlightColor(TRgb aColor);
	void SetHighlightColor(const TDesC& aColor);	
	
	void SetHeight(TInt aPoints);
	void SetHeight(const TDesC& aHeight);

	void SetIndent(TInt aPt);
	void SetIndent(const TDesC& aIndent)	;

	void SetWeight(TFontStrokeWeight aWeight);
	void SetWeight(const TDesC& aWeight);

	void SetStyle(TFontPosture aStyle);
	void SetStyle(const TDesC& aStyle);
	
	void SetUnderline(TFontUnderline aUnderline);
	void SetUnderline(const TDesC& aUnderline);

	void SetStrikeThrough(TFontStrikethrough aStrike);
	void SetStrikeThrough(const TDesC& aUnderline);

	void SetTextPos(TFontPrintPosition aPos);
	void SetTextPos(const TDesC& aPos);

	void SetAlign(CParaFormat::TAlignment aAlign);
	void SetAlign(const TDesC& aAlign);

	void SetLeftMargin(const TDesC& aLeft);
	void SetRightMargin(const TDesC& aRight);
	
	void SetHighlightStyle(TFontPresentation::TFontHighlightStyle aStyle);
	void SetBorder(const TDesC& aLocation,const TDesC& aBorder);
	void SetPadding(const TDesC &aPadding);
	
	TBool GetIndexByName(const TDesC& aName,TBool aGlobal);
	
	void SetParent(const TDesC& aName);
	void SetBullet(void);
	
	#ifdef _log_
		void StyleEnd();
	#endif

	TCharFormat CharFormat(TInt aPos);
	TCharFormatMask CharFormatMask(TInt aPos);
	CParaFormat* ParaFormat(TInt aPos);
	TParaFormatMask ParaFormatMask(TInt aPos);

	CCellFormat* CellFormat(TInt aPos);

	TInt GetPos(void);
	TInt GetGlobalPos(void);
	void SetPosToGlobal(void);

	TPtrC Name(TInt);

	TReal GetPtByDesC(const TDesC& aLength);
	TReal GetTwipsByDesC(const TDesC& aLength);

	void ClearParas(void);
	TBool IsText(void);
	//

	//Cell
	void AddCellItem(HBufC* aName);
	void SetCellHighlightColor(const TDesC& aColor);
	void SetCellFontColor(const TDesC& aColor);
	void SetCellAlign(const TDesC& aAlign);

	//Bullets
	void VerifyBullets(void);
	CBullets* CPara::Bullet(void);
	void AddBulletItem(const TDesC& aName);
	void AddBullet(TBulletType aType);
	void SetChar(TChar aChar);
	void GetBulletIndex(const TDesC& aName);
	void IncBulletLevel();
	void DecBulletLevel();
	void SetBulletChar();
	void SetBulletIndent(const TDesC& aIndent);
	void IncChar();
	void ResetBullet(const TDesC& aValue);

private:
	CPara();
	void ConstructL();
	
	TInt Exists(const TDesC& aName);
	void Initialise(void);

	TInt iBulletPos;

	TInt iPos;
	TInt iPosGlobal;
	TInt iLast;

	
	RArray< TBuf<100> >* iName;

	//Cell
	RPointerArray<CCellFormat>* iCellFormat;
	//Cell


	//Text
	RArray<TCharFormat>* iArrayFormat;
	RArray<TCharFormatMask>* iArrayFormatMask;
	RPointerArray<CParaFormat>* iParaFormat;
	RArray<TParaFormatMask>* iParaFormatMask;
	//Text

	//Bullets
	RPointerArray<CBullets>* iBullets;
	#ifdef _log_
		RFileLogger iFileLogger;
	#endif

};

#endif // PARA_H
