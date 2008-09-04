/*************************************************************************
 *
 * DO NOT ALTER OR REMOVE COPYRIGHT NOTICES OR THIS FILE HEADER.
 * 
 * Copyright 2008,2009,2010 by ODENDAHL SEPT-Solutions
 *
 * Office Reader - a OpenDocument viewer for Symbian OS
 *
 * $Id : XXXXXX $
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

#include "sheet_trace.h"
#include "sheet_data.h"

#include <eikenv.h>

#include "cell.h"
#include "sheet_model.h"


CSheetData::CSheetData():
	m_enLongTextMode( enRightMost ),
	m_pColumnHeaderFont( NULL ),
	m_pRowHeaderFont( NULL ),
	m_pCellFont( NULL ),
	m_pItemWndFont( NULL ),
	m_nZoomDivident( 1 ),
	m_nZoomDivisor( 1 ),
	m_pColumnSizes( NULL ),
	m_pRowSizes( NULL ),
	m_enFont( enAnnotation )
{
}

CSheetData::~CSheetData()
{
	ReleaseFonts();

	if( m_pColumnSizes )
	{
		delete m_pColumnSizes;
		m_pColumnSizes = NULL;
	}

	if( m_pRowSizes )
	{
		delete m_pRowSizes;
		m_pRowSizes = NULL;
	}
}

CSheetData* CSheetData::NewL()
{
	CSheetData* pSelf = CSheetData::NewLC();
	CleanupStack::Pop( pSelf );
	return pSelf;
}

CSheetData* CSheetData::NewLC()
{
	CSheetData* pSelf = new ( ELeave ) CSheetData;
	CleanupStack::PushL( pSelf );
	pSelf->ConstructL();
	return pSelf;
}

void CSheetData::ConstructL()
{
	m_enLongTextMode = enLeftMost;
	m_nCursorWidthInPixels = 3;

	UpdateFonts();

	m_BackgroundColor = KRgbWhite;
	m_GridColor = KRgbGray;
	m_CursorColor = KRgbBlack;

	
	TSize MinCellSize( CellFont()->TextWidthInPixels( _L("sometxt") ) + 4, CellFont()->HeightInPixels() + 4 );


	//column header properties
	m_ColumnHeaderColor = TRgb( 0x00ffaea5 );
	m_ColumnHeaderTextColor = KRgbBlack;
	m_ColumnHeaderSize = TSize( MinCellSize.iWidth, MinCellSize.iHeight );

	//row header properties
	m_RowHeaderColor = TRgb( 0x00ffaea5 );
	m_RowHeaderTextColor = KRgbBlack;
	m_RowHeaderSize = TSize( CellFont()->TextWidthInPixels( _L("99") ) + 4, CellFont()->HeightInPixels() + 4 );

	//cell properties
	m_CellTextColor = KRgbBlack;

	//item wnd properties
	m_ItemWndColor = KRgbYellow;
	m_ItemWndTextColor = KRgbBlack;
	m_ItemWndMargins.iTop = 0;
	m_ItemWndMargins.iLeft = 10;
	m_ItemWndMargins.iBottom = 0;
	m_ItemWndMargins.iRight = 10;

	CCellWidth* pColumnSizes = CCellWidth::NewLC();
	CCellHeight* pRowSizes = CCellHeight::NewLC();
	CleanupStack::Pop( pRowSizes );
	CleanupStack::Pop( pColumnSizes );
	m_pColumnSizes = pColumnSizes;
	m_pRowSizes = pRowSizes;
}

TInt CSheetData::ZoomedValue( TInt nValue ) const
{
	return (nValue*m_nZoomDivident)/m_nZoomDivisor;
}

TInt CSheetData::ColumnHeaderWidthL( TInt nIndex ) const 
{
/*
	TInt nWidth = m_ColumnHeaderSize.iWidth;
	m_pColumnSizes->AtL( nIndex, nWidth );
	return (nWidth*m_nZoomDivident)/m_nZoomDivisor;
*/
/*
	- Cell/Row resizing works only in normal mode, not when it is zoomed to 
	medium or large, e.g. pressing the right arrow makes the height smaller 
	or nothing happens
*/
	TInt nWidth = m_ColumnHeaderSize.iWidth;
	m_pColumnSizes->AtL( nIndex, nWidth );
	TInt nRes = (nWidth*m_nZoomDivident)/m_nZoomDivisor;

	if( TInt( (nWidth*m_nZoomDivident)%m_nZoomDivisor ) )
		nRes++;

	return nRes;
}

void CSheetData::SetColumnHeaderWidthL( TInt nIndex, TInt nWidth )
{
	m_pColumnSizes->InsertL( nIndex, (nWidth*m_nZoomDivisor)/m_nZoomDivident );
}

TInt CSheetData::ColumnHeaderHeight() const
{
	return (m_ColumnHeaderSize.iHeight*m_nZoomDivident)/m_nZoomDivisor;
}

void CSheetData::SetColumnHeaderHeight( TInt nHeight )
{
	m_ColumnHeaderSize.iHeight = (nHeight*m_nZoomDivisor)/m_nZoomDivident;
}

TInt CSheetData::RowHeaderHeightL( TInt nIndex ) const
{
/*
	TInt nHeight = m_RowHeaderSize.iHeight;
	m_pRowSizes->AtL( nIndex, nHeight );
	return (nHeight*m_nZoomDivident)/m_nZoomDivisor;
*/

/*
	- Cell/Row resizing works only in normal mode, not when it is zoomed to 
	medium or large, e.g. pressing the right arrow makes the height smaller 
	or nothing happens
*/

	TInt nHeight = m_RowHeaderSize.iHeight;
	m_pRowSizes->AtL( nIndex, nHeight );
	TInt nRes = (nHeight*m_nZoomDivident)/m_nZoomDivisor;

	if( TInt( (nHeight*m_nZoomDivident)%m_nZoomDivisor ) )
		nRes++;

	return nRes;
}

void CSheetData::SetRowHeaderHeightL( TInt nIndex, TInt nHeight )
{
	m_pRowSizes->InsertL( nIndex, (nHeight*m_nZoomDivisor)/m_nZoomDivident );
}

TInt CSheetData::RowHeaderWidth() const 
{
	return (m_RowHeaderSize.iWidth*m_nZoomDivident)/m_nZoomDivisor;
}

void CSheetData::SetRowHeaderWidth( TInt nWidth )
{
	m_RowHeaderSize.iWidth = (nWidth*m_nZoomDivisor)/m_nZoomDivident;
}

void CSheetData::ResetCellSizeL( const TSize& Size )
{
#ifdef SHEET_TRACE_ENABLED
	CSheetTrace::ResetCellSizeL( Size );
#endif

	m_pColumnSizes->RemoveAllL();
	m_pRowSizes->RemoveAllL();
	m_ColumnHeaderSize.iWidth = (Size.iWidth*m_nZoomDivisor)/m_nZoomDivident;
	m_RowHeaderSize.iHeight = (Size.iHeight*m_nZoomDivisor)/m_nZoomDivident; 
}

TSize CSheetData::CellSizeL( const TPoint& Position ) const
{
	return TSize( ColumnHeaderWidthL( Position.iX ), 
		RowHeaderHeightL( Position.iY ) );
}

void CSheetData::SetCellSizeL( const TPoint& Position, const TSize& Size ) 
{
#ifdef SHEET_TRACE_ENABLED
	CSheetTrace::SetCellSizeL( Position, Size );
#endif

	SetColumnHeaderWidthL( Position.iX, Size.iWidth );
	SetRowHeaderHeightL( Position.iY, Size.iHeight );
}

TRgb CSheetData::CellTextColorL( const TPoint& Position ) const
{
	TRgb CellColor = m_pModel->AtL( Position )->m_MetaData.m_TextColor;
	return CellColor == KDefaultCellTextColor ? m_CellTextColor : CellColor;
}

void CSheetData::SetCellTextColorL( const TPoint& Position, const TRgb& Color )
{
#ifdef SHEET_TRACE_ENABLED
	CSheetTrace::SetCellTextColorL( Position, Color );
#endif

	m_pModel->AtL( Position )->m_MetaData.m_TextColor = Color;
}

TRgb CSheetData::CellColorL( const TPoint& Position ) const
{
	TRgb CellColor = m_pModel->AtL( Position )->m_MetaData.m_Color;
	return CellColor == KDefaultCellColor ? m_BackgroundColor : CellColor;
}

void CSheetData::SetCellColorL( const TPoint& Position, const TRgb& Color )
{
#ifdef SHEET_TRACE_ENABLED
	CSheetTrace::SetCellColorL( Position, Color );
#endif

	m_pModel->AtL( Position )->m_MetaData.m_Color = Color;
}

CGraphicsContext::TTextAlign CSheetData::CellAlignmentL( const TPoint& Position ) const
{
	return m_pModel->AtL( Position )->m_MetaData.m_Align;
}

void CSheetData::SetCellAlignmentL( const TPoint& Position, CGraphicsContext::TTextAlign Alignment )
{
#ifdef SHEET_TRACE_ENABLED
	CSheetTrace::SetCellAlignmentL( Position, Alignment );
#endif

	m_pModel->AtL( Position )->m_MetaData.m_Align = Alignment;
}

//TMargins CSheetData::CellMarginsL( const TPoint& Position ) const
//{
//	return m_pModel->AtL( Position )->m_MetaData.m_Margins;
//}

//void CSheetData::SetCellMarginsL( const TPoint& Position, TMargins Margins )
//{
//	m_pModel->AtL( Position )->m_MetaData.m_Margins = Margins;
//}

void CSheetData::UpdateFontsSize( EFont enFont )
{
	m_enFont = enFont;
	UpdateFonts();
}

void CSheetData::UpdateFonts()
{
	ReleaseFonts();

#ifdef __SERIES60_30__
	static const TInt ANNOTATION_FONT_HEIGHT_176_X_208 = 10;
	static const TInt NORMAL_FONT_HEIGHT_176_X_208     = 14;
	static const TInt TITLE_FONT_HEIGHT_176_X_208      = 17;

	TInt nFontHeight = ANNOTATION_FONT_HEIGHT_176_X_208;
	switch( m_enFont )
	{
	case enAnnotation:
		nFontHeight = ANNOTATION_FONT_HEIGHT_176_X_208;
		break;
	case enNormal:
		nFontHeight = NORMAL_FONT_HEIGHT_176_X_208;
		break;
	case enTitle:
		nFontHeight = TITLE_FONT_HEIGHT_176_X_208;
		break;
	default:
		break;
	}
	
	TPoint FontSize = CEikonEnv::Static()->ScreenDevice()->PixelsToTwips( TPoint( 0, nFontHeight ) );

	_LIT( KFontName,"Arial" );
	TFontSpec FontSpec( KFontName, FontSize.iY );
	CFont* pFont = NULL;
	CEikonEnv::Static()->ScreenDevice()->GetNearestFontInTwips( pFont, FontSpec );
	m_pColumnHeaderFont = pFont;

	CEikonEnv::Static()->ScreenDevice()->GetNearestFontInTwips( pFont, FontSpec );
	m_pRowHeaderFont = pFont;

	CEikonEnv::Static()->ScreenDevice()->GetNearestFontInTwips( pFont, FontSpec );
	m_pCellFont = pFont;

	CEikonEnv::Static()->ScreenDevice()->GetNearestFontInTwips( pFont, FontSpec );
	m_pItemWndFont = pFont;

#elif defined __S60_2ND_FP2__
	m_pColumnHeaderFont = CEikonEnv::Static()->AnnotationFont();
	m_pRowHeaderFont = CEikonEnv::Static()->AnnotationFont();
	m_pCellFont = CEikonEnv::Static()->AnnotationFont();
	m_pItemWndFont = CEikonEnv::Static()->AnnotationFont();
#elif defined __UIQ__
	m_pColumnHeaderFont = CEikonEnv::Static()->AnnotationFont();
	m_pRowHeaderFont = CEikonEnv::Static()->AnnotationFont();
	m_pCellFont = CEikonEnv::Static()->AnnotationFont();
	m_pItemWndFont = CEikonEnv::Static()->AnnotationFont();
#elif defined __S80__
	m_pColumnHeaderFont = CEikonEnv::Static()->AnnotationFont();
	m_pRowHeaderFont = CEikonEnv::Static()->AnnotationFont();
	m_pCellFont = CEikonEnv::Static()->AnnotationFont();
	m_pItemWndFont = CEikonEnv::Static()->AnnotationFont();
#else         
	#error device not defined
#endif
}

void CSheetData::ReleaseFonts()
{
#ifdef __SERIES60_30__
	if( m_pColumnHeaderFont )
	{
		CEikonEnv::Static()->ScreenDevice()->ReleaseFont( CONST_CAST( CFont*, m_pColumnHeaderFont ) );
		m_pColumnHeaderFont = NULL;
	}

	if( m_pRowHeaderFont )
	{
		CEikonEnv::Static()->ScreenDevice()->ReleaseFont( CONST_CAST( CFont*, m_pRowHeaderFont ) );
		m_pRowHeaderFont = NULL;
	}

	if( m_pCellFont )
	{
		CEikonEnv::Static()->ScreenDevice()->ReleaseFont( CONST_CAST( CFont*, m_pCellFont ) );
		m_pCellFont = NULL;
	}

	if( m_pItemWndFont )
	{
		CEikonEnv::Static()->ScreenDevice()->ReleaseFont( CONST_CAST( CFont*, m_pItemWndFont ) );
		m_pItemWndFont = NULL;
	}
#elif defined __S60_2ND_FP2__
#elif defined __UIQ__
#elif defined __S80__
#else
	#error device not defined
#endif

}
