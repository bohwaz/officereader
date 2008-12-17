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

#include "Para.h"

#include <eikenv.h>
#include <gulutil.h>

#include <bitdev.h>

#define scalefactor 10
#define bulletscalefactor 5
#define heigthreduction 4

/*
	Formmating not yet working but there:

		- iBorderSize
	
*/

TRgb GetColorByDesC(const TDesC& aColor)
{
    TUint8  aRed;
	TUint8  aGreen;
	TUint8  aBlue;

	TLex(aColor.Left(2)).Val(aRed,EHex);
	TLex(aColor.Mid(2,2)).Val(aGreen,EHex);
	TLex(aColor.Right(2)).Val(aBlue,EHex);
	
	return TRgb(aRed,aGreen,aBlue);
}

TInt CPara::GetTwipsByDesC(const TDesC& aLength)
{
	return FontUtils::TwipsFromPoints( TInt(GetPtByDesC(aLength)) );
}
TReal CPara::GetPtByDesC(const TDesC& aLength)
{
	_LIT(in,"in");
	_LIT(mm,"mm");
	_LIT(cm,"cm");
	_LIT(px,"px");
	_LIT(pc,"pc");
	_LIT(pt,"pt");
	_LIT(pro,"%");
	
	TPtrC aType;

	aType.Set( aLength.Right(1) );
	TReal len;

	if (aType==pro)
	{
		#ifdef _log_
			iFileLogger.Write(aLength.Left( aLength.Length()- 1 ) );
		#endif
		__ASSERT_DEBUG( KErrNone  == TLex(aLength.Left( aLength.Length()- 1) ).Val(len), User::Panic( aLength.Left( aLength.Length()- 1) ,0) );
		TLex(aLength.Left( aLength.Length()- 1) ).Val(len);
		return (len / 100) *  ( FontUtils::PointsFromTwips( (*iArrayFormat)[iPos].iFontSpec.iHeight) + heigthreduction );

	}
	else
	{

		 aType.Set( aLength.Right(2) );
		 __ASSERT_DEBUG( KErrNone  == TLex(aLength.Left( aLength.Length()- 2) ).Val(len), User::Panic( aLength.Left( aLength.Length()- 2) ,1) );
		TLex(aLength.Left( aLength.Length()- 2) ).Val(len);

		if (aType==in)
		{
			return len * 72;
		}
		else if (aType==pt)
		{
			return len;
		}
		else if (aType==cm)
		{
			return len  * 10 * (72/25.4);
		}
		else if (aType==mm)
		{
			return len * (72/25.4);	//(25.40 mm = 1 Inch)
		}
		else if (aType==px)
		{
			
		}
		else if (aType==pc)
		{
			return len * 12;		// (6pc = 1 inch)
		}
	
		return 0;
	}
}




CPara::CPara()
{
	// No implementation required
}



CPara::~CPara()
{

	ClearParas();

	delete iArrayFormat;
	delete iArrayFormatMask;
	delete iParaFormat;
	delete iParaFormatMask;

	delete iCellFormat;

	delete iBullets;

	delete iName;

	#ifdef _log_
	  iFileLogger.CloseLog();
	  iFileLogger.Close();
	#endif

}


void CPara::ClearParas()
{
	iArrayFormat->Close();
	iArrayFormatMask->Close();
	iParaFormatMask->Close();
	iParaFormat->ResetAndDestroy();
	iCellFormat->ResetAndDestroy();
	iBullets->ResetAndDestroy();
	iName->Close();

	iBulletPos = -1;
	iPos = -1;
	iPosGlobal = -1;
	iLast = -1;
}

CPara* CPara::NewLC()
{
	CPara* self = new (ELeave)CPara();
	CleanupStack::PushL(self);
	self->ConstructL();
	return self;
}

CPara* CPara::NewL()
{
	CPara* self=CPara::NewLC();
	CleanupStack::Pop(); // self;
	return self;
}

void CPara::ConstructL()
{
	iBulletPos = -1;
	iPos = -1;
	iPosGlobal = -1;
	iLast = -1;

	iName = new (ELeave) RArray< TBuf<100> >;

	iArrayFormat = new (ELeave) RArray<TCharFormat>;
	iArrayFormatMask = new (ELeave) RArray<TCharFormatMask>;

	iParaFormat = new (ELeave) RPointerArray<CParaFormat>;
	iParaFormatMask = new (ELeave) RArray<TParaFormatMask>;

	iCellFormat = new (ELeave) RPointerArray<CCellFormat>;

	iBullets = new (ELeave) RPointerArray<CBullets>;

	#ifdef _log_
		User::LeaveIfError(iFileLogger.Connect());
		iFileLogger.CreateLog(_L("Office"),_L("para.txt"),EFileLoggingModeOverwrite);
	#endif	

}

void CPara::AddCellItem(HBufC* aName)
{
	TBuf<100> Name;

	// TODO: Exists

	if (aName)
	{
		Name.Append(*aName);
	}

	CCellFormat* aFormat = CCellFormat::NewL();
	iCellFormat->Append( aFormat );
	iName->Append(Name);

	iPos++;

	#ifdef _log_
		iFileLogger.Write(Name);
	#endif

}

//Insert at the correct position, so all globals are first
void CPara::AddTextItem(TBool aInit,HBufC* aName)//,TCharFormat aFormat,TCharFormatMask aFormatMask,TBool aInit)
{
	
	// neu allokieren, falls kein Name vorhanden ist (aName nicht definiert)
	TBuf<100> Name;

	if (aName)
	{
		Name.Append( *aName );
	}

	TInt aPos;
	aPos = Exists(Name);


	if (aPos > -1)
	{
		iPos = aPos;
		Initialise();
	}
	else
	{
		
   TCharFormat     aFormat;
   TCharFormatMask aFormatMask;

   /*
	TFontSpec* fontSpec=new TFontSpec(_L("Times New Roman"),FontUtils::TwipsFromPoints(12));
   
   CleanupStack::PushL(fontSpec);
 
   CFont*  editorFont = CCoeEnv::Static()->CreateScreenFontL(*fontSpec);
   CleanupStack::PushL(editorFont);


   aFormat.iFontSpec = editorFont->FontSpecInTwips();
   aFormatMask.SetAttrib(EAttFontTypeface);

   CCoeEnv::Static()->ReleaseScreenFont(editorFont);
   CleanupStack::Pop();                    // editorFont

   CleanupStack::Pop();  // fontSpec
   delete fontSpec;    // Delete Font
  */ 


   // oder


   /*

	// Set the font
	TCharFormat cf;
	TCharFormatMask cfm;

	cf.iFontPresentation.iTextColor=KRgbWhite;
	cfm.SetAttrib(EAttColor);

	cfm.SetAttrib(EAttFontTypeface);

	_LIT(KFontName, "LatinPlain12");
	CFont *myFont;
	TFontSpec tfs(KFontName, 1);
	iCoeEnv->ScreenDevice()->GetNearestFontInTwips(myFont, tfs);

	cf.iFontSpec = myFont->FontSpecInTwips();
	CCharFormatLayer *pCharFL = CCharFormatLayer::NewL(cf,cfm);
	iConsole->SetCharFormatLayer(pCharFL);
   */

	//TCharFormat aFormat;//(_L("LatinBold19"),357);
	//TCharFormatMask aFormatMask;
	
	//TBuf<50> aName;


	if (!aInit)
	{
		iArrayFormatMask->Append(aFormatMask);
		iArrayFormat->Append(aFormat);
		iPos = iLast + 1;
		iName->Append(Name);
	}
	else
	{
		CParaFormat* aParaFormat = CParaFormat::NewL();
		TParaFormatMask aMask;

		
/*
		TTabStop tab1;
		tab1.iTwipsPosition = FontUtils::TwipsFromPoints(20);

		TTabStop tab3;
		tab1.iTwipsPosition = FontUtils::TwipsFromPoints(35);

		aParaFormat->StoreTabL(tab1);
		aParaFormat->StoreTabL(tab3);

		aMask.SetAttrib(EAttTabStop);
*/		

		if (iLast==iPosGlobal)
		{
			/*
			_LIT(hi,"LatinBold19");
			TBuf<50> nein;
			nein.Append(hi);
			aFormat.iFontSpec.iTypeface.iName = nein;
			aFormatMask.SetAttrib(EAttFontTypeface);
			*/

				iArrayFormatMask->Append(aFormatMask);
				iArrayFormat->Append(aFormat);
				iPos++;	
				iName->Append(Name);
					
				iParaFormat->Append(aParaFormat);
				iParaFormatMask->Append(aMask);
				iPosGlobal++;
				Initialise();

		}
		else
		{
				TInt aPos = iPosGlobal + 1;
				iArrayFormatMask->Insert(aFormatMask,aPos);
				iArrayFormat->Insert(aFormat,aPos);
				iName->Insert(Name,aPos);
				iPos = aPos;	
					
				iParaFormat->Insert(aParaFormat,aPos);
				iParaFormatMask->Insert(aMask,aPos);
				iPosGlobal = aPos;
				Initialise();

		
		}
	}

	iLast++;

	}

}

void CPara::Initialise(void)
{
	_LIT(length, "0cm");

	//TODO: default-style auslesen, Standard erbt davon


	// TODO:   wann genau brauche ich das, welche Funktionen ja, wann nein
	//paragraph properties
	SetAlign(CParaFormat::ELeftAlign);
	
	//text-properties
	SetColor(TRgb(0,0,0));
	SetHeight(12);
	SetWeight(EStrokeWeightNormal);
	SetStyle(EPostureUpright);
	SetUnderline(EUnderlineOff);
	SetStrikeThrough(EStrikethroughOff);
	SetTextPos(EPrintPosNormal);
	SetHighlightStyle(TFontPresentation::EFontHighlightNone);
	
	
	// TODO: wiso geht das nicht??
	SetLeftMargin(length);
	SetRightMargin(length);
	
	//SetHighlightColor(TRgb(255,255,255));   geht nicht so....... 
	//SetParaHighlightColor(TRgb(0,0,0));
	SetIndent(0);
	
	(*iParaFormat)[iPosGlobal]->RemoveAllBorders();


}


void CPara::SetParaHighlightColor(TRgb aColor)
{
    (*iParaFormatMask)[iPosGlobal].SetAttrib(EAttFillColor);
	(*iParaFormat)[iPosGlobal]->iFillColor =  aColor ;
}

void CPara::SetParaHighlightColor(const TDesC& aColor)	
{
	SetParaHighlightColor(GetColorByDesC(aColor));
}
		

		
void CPara::SetColor(TRgb aColor)
{
    (*iArrayFormatMask)[iPos].SetAttrib(EAttColor);
	(*iArrayFormat)[iPos].iFontPresentation.iTextColor = aColor;

}

void CPara::SetColor(const TDesC& aColor)	
{
	SetColor( GetColorByDesC(aColor));
}

void CPara::SetIndent(TInt aPt)
{

	if (!(*iParaFormat)[iPosGlobal]->iBullet)
	{
		(*iParaFormatMask)[iPosGlobal].SetAttrib(EAttIndent);
		(*iParaFormat)[iPosGlobal]->iIndentInTwips = FontUtils::TwipsFromPoints(aPt / scalefactor);
	}
}

void CPara::SetIndent(const TDesC& aIndent)	
{
	SetIndent( TInt(GetPtByDesC(aIndent)) );
}


void CPara::SetHighlightStyle(TFontPresentation::TFontHighlightStyle aStyle)
{
    (*iArrayFormatMask)[iPos].SetAttrib(EAttFontHighlightStyle);
	(*iArrayFormat)[iPos].iFontPresentation.iHighlightStyle = aStyle;
}

void CPara::SetHighlightColor(TRgb aColor)
{
    (*iArrayFormatMask)[iPos].SetAttrib(EAttFontHighlightColor);
	(*iArrayFormat)[iPos].iFontPresentation.iHighlightColor = aColor;

	SetHighlightStyle(TFontPresentation::EFontHighlightNormal);
}

void CPara::SetHighlightColor(const TDesC& aColor)	
{
	  SetHighlightColor( GetColorByDesC(aColor) );
}

void CPara::SetUnderline(TFontUnderline aUnderline)
{
	(*iArrayFormatMask)[iPos].SetAttrib(EAttFontUnderline);
	(*iArrayFormat)[iPos].iFontPresentation.iUnderline = aUnderline;
}

void CPara::SetUnderline(const TDesC& aUnderline)
{
	_LIT(none,"none");
	TFontUnderline under;

	if (aUnderline==none)
	{
		under = EUnderlineOff;
	}
	else
	{
		under = EUnderlineOn;
	}

	SetUnderline(under);
}

void CPara::SetStrikeThrough(TFontStrikethrough aStrike)
{

	(*iArrayFormatMask)[iPos].SetAttrib(EAttFontStrikethrough);
	(*iArrayFormat)[iPos].iFontPresentation.iStrikethrough = aStrike;
}

void CPara::SetStrikeThrough(const TDesC& aUnderline)
{
	TFontStrikethrough strike;
	_LIT(none,"none");

	if (aUnderline==none)
	{
		strike = EStrikethroughOff;
	}
	else
	{
		strike = EStrikethroughOn;
	}
	
	SetStrikeThrough(strike);
}


void CPara::SetStyle(const TDesC& aStyle)
{
	_LIT(italic,"italic");
	TFontPosture style;

	if (aStyle==italic)
	{
		style = EPostureItalic;		
	}
	else
	{
		style = EPostureUpright;
	}
	

	SetStyle(style);
}

void CPara::SetStyle(TFontPosture aStyle)
{
    (*iArrayFormatMask)[iPos].SetAttrib(EAttFontPosture);
	(*iArrayFormat)[iPos].iFontSpec.iFontStyle.SetPosture(aStyle);
}

void CPara::SetWeight(TFontStrokeWeight aWeight)
{

	(*iArrayFormatMask)[iPos].SetAttrib(EAttFontStrokeWeight);
	 // (*iArrayFormatMask)[iPos].SetAttrib(EAttFontTypeface);
	(*iArrayFormat)[iPos].iFontSpec.iFontStyle.SetStrokeWeight(aWeight);


	/*
  CFont*  editorFont = ->CreateScreenFontL((*iArrayFormat)[iPos].iFontSpec);
   CleanupStack::PushL(editorFont);


   (*iArrayFormat)[iPos].iFontSpec = editorFont->FontSpecInTwips();

   CCoeEnv::Static()->ReleaseScreenFont(editorFont);
   CleanupStack::Pop();                    // editorFont
   */
 
}

void CPara::SetWeight(const TDesC& aWeight)
{
	TFontStrokeWeight weight;
	_LIT(normal,"normal");

	if (aWeight==normal)
	{
		weight = EStrokeWeightNormal;
	}
	else
	{
		weight = EStrokeWeightBold;
	}
	SetWeight(weight);
}

void CPara::SetTextPos(TFontPrintPosition aPos)
{
    (*iArrayFormatMask)[iPos].SetAttrib(EAttFontPrintPos);
	(*iArrayFormat)[iPos].iFontSpec.iFontStyle.SetPrintPosition(aPos);

}

void CPara::SetTextPos(const TDesC& aPos)
{
	_LIT(sup,"super");
	_LIT(sub,"sub");
	TFontPrintPosition aFontPos;

	TLex aItem(aPos);		// TDOD: besser find? oder auch nexttoken bei italic??
	TPtrC Pos = aItem.NextToken();

	if (Pos==sup)
	{
		aFontPos = EPrintPosSuperscript;
	}
	else if (Pos==sub)
	{
		aFontPos = EPrintPosSubscript;
	}
	else
	{
		aFontPos = EPrintPosNormal;
	}
	
	SetTextPos(aFontPos);
}

void CPara::SetHeight(TInt aPoints)
{
	(*iArrayFormatMask)[iPos].SetAttrib(EAttFontHeight);
	//TODO TODO ???? (*iArrayFormatMask)[iPos].SetAttrib(EAttFontTypeface);
	(*iArrayFormat)[iPos].iFontSpec.iHeight = FontUtils::TwipsFromPoints(aPoints - heigthreduction);
}

void CPara::SetHeight(const TDesC& aHeight)
{
	#ifdef _log_
		iFileLogger.Write(aHeight);
		TBuf<10> nein;
		nein.Num(GetPtByDesC(aHeight));
		iFileLogger.Write(nein);
	#endif
	SetHeight( TInt(GetPtByDesC(aHeight)) );
}

void CPara::SetAlign(CParaFormat::TAlignment aAlign)
{
	(*iParaFormatMask)[iPosGlobal].SetAttrib(EAttAlignment);
	(*iParaFormat)[iPosGlobal]->iHorizontalAlignment = aAlign;
}

void CPara::SetLeftMargin(const TDesC& aLeft)
{	
	(*iParaFormatMask)[iPosGlobal].SetAttrib(EAttLeftMargin);
	(*iParaFormat)[iPosGlobal]->iLeftMarginInTwips =  GetTwipsByDesC( aLeft ) / scalefactor;
}

void CPara::SetRightMargin(const TDesC& aRight)
{
	(*iParaFormatMask)[iPosGlobal].SetAttrib(EAttRightMargin);
	(*iParaFormat)[iPosGlobal]->iRightMarginInTwips = GetTwipsByDesC( aRight ) / scalefactor;
}


void CPara::SetAlign(const TDesC& aAlign)
{
	_LIT(end,"end");
	_LIT(center,"center");

	CParaFormat::TAlignment align;

	if (aAlign==end)
	{
		align = CParaFormat::ERightAlign;
	}
	else if (aAlign==center)
	{
		align = CParaFormat::ECenterAlign;
	}
	else
	{
		align = CParaFormat::ELeftAlign;
	}

	SetAlign(align);	
}

TBool CPara::GetIndexByName(const TDesC& aName,TBool aGlobal)
{
	// if we are at the same global paragraph as before, return true to delete the inserted enter char
	if  ( aGlobal && ((*iName)[iPosGlobal]== aName) )
		return ETrue;
	
	for (TInt i=0;i<iName->Count();i++)
	{
		if ((*iName)[i].Compare(aName)==0)
		{
			iPos = i;
			if (aGlobal)
				iPosGlobal = i;
			return EFalse;
			break;
		}
	}

	return EFalse;

	#ifdef _log_
		iFileLogger.Write(_L("start"));
		iFileLogger.Write(aName);
		iFileLogger.Write((*iName)[iPos]);
		iFileLogger.Write(_L("end"));
	#endif

}

TBool CPara::IsText(void)
{
	return (iPos!=iPosGlobal);
}

TCharFormat CPara::CharFormat(TInt aPos)
{
	return (*iArrayFormat)[aPos];
}

TCharFormatMask CPara::CharFormatMask(TInt aPos)
{
	return (*iArrayFormatMask)[aPos];
}


CParaFormat* CPara::ParaFormat(TInt aPos)
{
	return (*iParaFormat)[aPos];
}


TParaFormatMask CPara::ParaFormatMask(TInt aPos)
{
	return (*iParaFormatMask)[aPos];
}

CCellFormat* CPara::CellFormat(TInt aPos)
{
	return (*iCellFormat)[aPos];
}

TInt CPara::GetPos(void)
{
	return iPos;
}
TInt CPara::GetGlobalPos(void)
{
	return iPosGlobal;
}

void CPara::SetPosToGlobal(void)
{
	iPos = iPosGlobal;
}

void CPara::SetBullet(void)
{
	(*iParaFormatMask)[iPosGlobal].SetAttrib(EAttBullet);
	(*iParaFormatMask)[iPosGlobal].SetAttrib(EAttIndent);

	// could be already copied by the SetParent function
	if (!(*iParaFormat)[iPosGlobal]->iBullet)
	{	
		(*iParaFormat)[iPosGlobal]->iBullet = new (ELeave) TBullet();
		(*iParaFormat)[iPosGlobal]->iBullet->iHangingIndent = ETrue;
		(*iParaFormat)[iPosGlobal]->iIndentInTwips = FontUtils::TwipsFromPoints(10);
	}
}

void CPara::SetParent(const TDesC& aName)
{
	#ifdef _log_
		iFileLogger.Write(_L("SetParent:"));
		iFileLogger.Write(aName);
	#endif

	for (TInt aPos=0;aPos<=iName->Count()-1;aPos++)
	{
		if ((*iName)[aPos] == aName)
		{

			if (iPos < iParaFormat->Count())  // //Para Attribute
			{	
				// paragraph style, so we copy paragraph formmating as well as text formmating
				(*iParaFormatMask)[iPos] = (*iParaFormatMask)[aPos];
				(*iParaFormat)[iPos]->CopyL( (*(*iParaFormat)[aPos]) );

				(*iArrayFormat)[iPos] = (*iArrayFormat)[aPos];
				(*iArrayFormatMask)[iPos] = (*iArrayFormatMask)[aPos];
			}
			else	// Text attributes
			{	
				if (aPos > iParaFormat->Count())	// inherit from text attributes
				{
					(*iArrayFormat)[ iPos - iParaFormat->Count() ] = (*iArrayFormat)[aPos - iParaFormat->Count() ];
					(*iArrayFormatMask)[iPos - iParaFormat->Count() ] = (*iArrayFormatMask)[aPos - iParaFormat->Count() ];	
				}
				else	// inherit from para attributes
				{
					(*iArrayFormat)[iPos] = (*iArrayFormat)[aPos];
					(*iArrayFormatMask)[iPos] = (*iArrayFormatMask)[aPos];
				}
			}


			break;
		}
	}

}

TInt CPara::Exists(const TDesC& aName)
{
	
	TInt Pos = -1;

	if ((HBufC*)&aName)
	{

		for (TInt i=0;i<iName->Count();i++)
		{
			if ((*iName)[i].Compare(aName)==0)
			{
				Pos = i;
				break;
			}
		}
	}

	return Pos;
}

void CPara::SetPadding(const TDesC &aPadding)
{
	(*iParaFormatMask)[iPosGlobal].SetAttrib(EAttBorderMargin);			
	
	if ( (*iParaFormat)[iPosGlobal]->iBorderMarginInTwips == 0)
		(*iParaFormat)[iPosGlobal]->iBorderMarginInTwips = GetTwipsByDesC(aPadding) / scalefactor;

}
void CPara::SetBorder(const TDesC& aLocation,const TDesC& aBorder)
{	

	_LIT(join_border,"join-border"); //TODO: was macht das???
	//border-line-width-bottom


	_LIT(none,"none");
	_LIT(doubl,"double");

	_LIT(border,"border");
	_LIT(border_left,"border-left");
	_LIT(border_right,"border-right");
	_LIT(border_top,"border-top");
	_LIT(border_bottom,"border-bottom");

	_LIT(almostzero,"2mm");
	
	TParaBorder aParaBorder;

	if (aBorder==none)
	{
		if (aLocation==border_left)
		{
			(*iParaFormatMask)[iPosGlobal].ClearAttrib(EAttLeftBorder);	
		}
		if (aLocation==border_right)
		{
			(*iParaFormatMask)[iPosGlobal].ClearAttrib(EAttRightBorder);	
		}
		if (aLocation==border_top)
		{
			(*iParaFormatMask)[iPosGlobal].ClearAttrib(EAttTopBorder);	
		}
		if (aLocation==border_bottom)
		{
			(*iParaFormatMask)[iPosGlobal].ClearAttrib(EAttBottomBorder);	
		}
		if (aLocation==border)
		{
			(*iParaFormatMask)[iPosGlobal].ClearAttrib(EAttTopBorder);	
			(*iParaFormatMask)[iPosGlobal].ClearAttrib(EAttBottomBorder);
			(*iParaFormatMask)[iPosGlobal].ClearAttrib(EAttLeftBorder);		
			(*iParaFormatMask)[iPosGlobal].ClearAttrib(EAttRightBorder);	
		}
	}
	else
	{
		if (aLocation!=join_border)
		{
			TPtrC aWidth;
			TPtrC aType;
			TPtrC aColor;	

			TLex aToken(aBorder);
			
			//PtrC
			aWidth.Set(aToken.NextToken()); 
			aType.Set(aToken.NextToken());
			aColor.Set(aToken.NextToken());
			
			aParaBorder.iThickness = 20; //FontUtils::TwipsFromPoints( GetPtByDesC(*aWidth) );
			if ((aColor.Length()>=6) && (aColor.Length()<=aBorder.Length()))
			{
				aParaBorder.iColor = GetColorByDesC( aColor.Right(6) );
				aParaBorder.iAutoColor = EFalse;
			}
			else
				aParaBorder.iAutoColor = ETrue;
			
			if (aType==doubl)
			{
				aParaBorder.iLineStyle = TParaBorder::EDouble;
			}
			else	//if single etc
			{
				aParaBorder.iLineStyle = TParaBorder::ESolid;
			}

			if (aLocation==border_left)
			{
				(*iParaFormatMask)[iPosGlobal].SetAttrib(EAttLeftBorder);	
				(*iParaFormat)[iPosGlobal]->SetParaBorderL(CParaFormat::EParaBorderLeft,aParaBorder);

				// fix, otherwise no border is shown
				if ( (*iParaFormat)[iPosGlobal]->iLeftMarginInTwips == 0)
					SetLeftMargin(almostzero);
				if ( (*iParaFormat)[iPosGlobal]->iRightMarginInTwips == 0)
					SetRightMargin(almostzero);
			}
			if (aLocation==border_right)
			{
				(*iParaFormatMask)[iPosGlobal].SetAttrib(EAttRightBorder);	
				(*iParaFormat)[iPosGlobal]->SetParaBorderL(CParaFormat::EParaBorderRight,aParaBorder);
			}
			if (aLocation==border_top)
			{
				(*iParaFormatMask)[iPosGlobal].SetAttrib(EAttTopBorder);	
				(*iParaFormat)[iPosGlobal]->SetParaBorderL(CParaFormat::EParaBorderTop,aParaBorder);
			}
			if (aLocation==border_bottom)
			{
				(*iParaFormatMask)[iPosGlobal].SetAttrib(EAttBottomBorder);	
				(*iParaFormat)[iPosGlobal]->SetParaBorderL(CParaFormat::EParaBorderBottom,aParaBorder);
			}
			if (aLocation==border)
			{
				(*iParaFormatMask)[iPosGlobal].SetAttrib(EAttTopBorder);
				(*iParaFormatMask)[iPosGlobal].SetAttrib(EAttBottomBorder);	
				(*iParaFormatMask)[iPosGlobal].SetAttrib(EAttRightBorder);	
				(*iParaFormatMask)[iPosGlobal].SetAttrib(EAttLeftBorder);			

				(*iParaFormat)[iPosGlobal]->SetParaBorderL(CParaFormat::EParaBorderTop,aParaBorder);
				(*iParaFormat)[iPosGlobal]->SetParaBorderL(CParaFormat::EParaBorderBottom,aParaBorder);
				(*iParaFormat)[iPosGlobal]->SetParaBorderL(CParaFormat::EParaBorderRight,aParaBorder);
				(*iParaFormat)[iPosGlobal]->SetParaBorderL(CParaFormat::EParaBorderLeft,aParaBorder);

				// fix, otherwise no border is shown
				if ( (*iParaFormat)[iPosGlobal]->iLeftMarginInTwips == 0)
					SetLeftMargin(almostzero);
				if ( (*iParaFormat)[iPosGlobal]->iRightMarginInTwips == 0)
					SetRightMargin(almostzero);
			}
		} // falls nicht join border

	}	

}

void CPara::SetCellHighlightColor(const TDesC& aColor)
{
	(*iCellFormat)[iPos]->SetColor( GetColorByDesC(aColor) );
}

void CPara::SetCellFontColor(const TDesC& aColor)
{
	(*iCellFormat)[iPos]->SetFontColor( GetColorByDesC(aColor) );
}

void CPara::SetCellAlign(const TDesC& aAlign)
{

	_LIT(end,"end");
	_LIT(center,"center");

	TCellAlign align;

	if (aAlign==end)
	{
		align = ECellRight;
	}
	else if (aAlign==center)
	{
		align = ECellCenter;
	}
	else
	{
		align = ECellLeft;
	}


	(*iCellFormat)[iPos]->SetAlign(align);
}


TPtrC CPara::Name(TInt)
	{
		return (*iName)[iPosGlobal];
	};

void CPara::VerifyBullets()
{
	(*iBullets)[iBulletPos]->VerifyBullets();	
}

void CPara::AddBulletItem(const TDesC& aName)
{
	CBullets* aBullet = CBullets::NewL();
	iBullets->Append(aBullet);
	iBulletPos ++;
	(*iBullets)[iBulletPos]->SetName(aName);
}

CBullets* CPara::Bullet(void)
 {
	return (*iBullets)[iBulletPos];
 }

void CPara::AddBullet(TBulletType aType)
{
	(*iBullets)[iBulletPos]->AddBullet(aType);
}

void CPara::SetChar(TChar aChar)
{
	(*iBullets)[iBulletPos]->SetChar(aChar);
}

void CPara::GetBulletIndex(const TDesC& aName)
{
	for (TInt i=0; i <= iBullets->Count()-1;i++)
	{
		if (aName == (*iBullets)[i]->Name())
		{
			iBulletPos = i;
			break;
		}
	}
}

void CPara::IncBulletLevel()
{
	// list header does not have style
	if (iBulletPos != -1)
		(*iBullets)[iBulletPos]->IncLevel();
}

void CPara::DecBulletLevel()
{
	// list header does not have style
	if (iBulletPos != -1)
		(*iBullets)[iBulletPos]->DecLevel();
}

void CPara::SetBulletChar()
{	
	if ((*iParaFormat)[iPosGlobal]->iBullet)
	{
		(*iParaFormat)[iPosGlobal]->iBullet->iCharacterCode = (*iBullets)[iBulletPos]->Char();
		(*iParaFormat)[iPosGlobal]->iBullet->iHeightInTwips = (*iArrayFormat)[iPos].iFontSpec.iHeight;
		(*iParaFormat)[iPosGlobal]->iBullet->iColor = (*iArrayFormat)[iPos].iFontPresentation.iTextColor;
		
		(*iParaFormatMask)[iPosGlobal].SetAttrib(EAttLeftMargin);
		//GetMargin values
		(*iParaFormat)[iPosGlobal]->iLeftMarginInTwips = (*iBullets)[iBulletPos]->Indent();
	}
	else
	{
		SetBullet();

		(*iParaFormat)[iPosGlobal]->iBullet->iCharacterCode = (*iBullets)[iBulletPos]->Char();
		(*iParaFormat)[iPosGlobal]->iBullet->iHeightInTwips = (*iArrayFormat)[iPos].iFontSpec.iHeight;
		(*iParaFormat)[iPosGlobal]->iBullet->iColor = (*iArrayFormat)[iPos].iFontPresentation.iTextColor;
		
		(*iParaFormatMask)[iPosGlobal].SetAttrib(EAttLeftMargin);
		//GetMargin values
		(*iParaFormat)[iPosGlobal]->iLeftMarginInTwips = (*iBullets)[iBulletPos]->Indent();
	}
}

void CPara::IncChar()
{
	(*iBullets)[iBulletPos]->IncChar();
}

void CPara::SetBulletIndent(const TDesC& aIndent)
{
	(*iBullets)[iBulletPos]->SetIndent( GetTwipsByDesC(aIndent) /  bulletscalefactor);
}

void CPara::ResetBullet(const TDesC& aValue)
{
	if (aValue.Length()>1)
	{
		(*iBullets)[iBulletPos]->ResetBullet(0x2022);
	}
	else
	{
		(*iBullets)[iBulletPos]->ResetBullet(aValue[0]);
	}
}

