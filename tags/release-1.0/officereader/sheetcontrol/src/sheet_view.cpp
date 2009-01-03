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
#include "sheet_view.h"
#include "sheet_model.h"
#include "sheet_data.h"
#include "sheet_drawer.h"

#include <eikenv.h>

CSheetView::CSheetView( CSheetModel* pModel, CSheetData* pData, CSheetDrawer* pDrawer ):
	m_pModel( pModel ),
	m_pData( pData ),
	m_pDrawer( pDrawer ),
	m_lCellAjustmentMode( EFalse ),
	m_nCellAjustmentStepInPixels( 2 ),
	m_enZoomMode( enZoomSmall )
{
}

CSheetView::~CSheetView()
{
}

CSheetView* CSheetView::NewL( CSheetModel* pModel, CSheetData* pData, CSheetDrawer* pDrawer )
{
	CSheetView* pSelf = CSheetView::NewLC( pModel, pData, pDrawer );
	CleanupStack::Pop( pSelf );
	return pSelf;
}

CSheetView* CSheetView::NewLC( CSheetModel* pModel, CSheetData* pData, CSheetDrawer* pDrawer )
{
	CSheetView* pSelf = new ( ELeave ) CSheetView( pModel, pData, pDrawer );
	CleanupStack::PushL( pSelf );
	pSelf->ConstructL();
	return pSelf;
}

void CSheetView::ConstructL()
{
	m_Current = TPoint( 0, 0 );
	m_TopLeft = m_Current;
	SetCellAjustmentLimits( TSize( 30, 12 ), TSize( 100, 30 ) );
}

void CSheetView::SetZoomModeL( EZoomMode enMode )
{
#ifdef SHEET_TRACE_ENABLED
	CSheetTrace::SetZoomModeL( enMode );
#endif

	/*
		shortly about zooming:

		there are 
			TInt CShhetData::m_nZoomDivident;
			TInt CShhetData::m_nZoomDivisor;

		to calculate some size I do the next
		SizeInPixels = (SomeSize*m_nZoomDivident)/m_nZoomDivisor;

		before doing this I wrote a test to get to know how many time
		take such operations: +, -, *, / - on device thay take approximately
		the same time. So calculating (SomeSize*m_nZoomDivident)/m_nZoomDivisor is fast enough.
	*/

	switch( enMode )
	{
	case enZoomSmall:
	case enZoomMedium:
	case enZoomLarge:
		break;
	default:
		User::Leave( KErrArgument );
	}

	if( m_enZoomMode != enMode )
	{
		m_enZoomMode = enMode;
		switch( m_enZoomMode )
		{
		case enZoomSmall:
			m_pData->UpdateFontsSize( CSheetData::enAnnotation );
			//m_pData->SetColumnHeaderFont( CEikonEnv::Static()->AnnotationFont() );
			//m_pData->SetRowHeaderFont( CEikonEnv::Static()->AnnotationFont() );
			//m_pData->SetCellFont( CEikonEnv::Static()->AnnotationFont() );
			break;
		case enZoomMedium:
			m_pData->UpdateFontsSize( CSheetData::enNormal );
			//m_pData->SetColumnHeaderFont( CEikonEnv::Static()->NormalFont() );
			//m_pData->SetRowHeaderFont( CEikonEnv::Static()->NormalFont() );
			//m_pData->SetCellFont( CEikonEnv::Static()->NormalFont() );
			break;
		case enZoomLarge:
			m_pData->UpdateFontsSize( CSheetData::enTitle );
			//m_pData->SetColumnHeaderFont( CEikonEnv::Static()->TitleFont() );
			//m_pData->SetRowHeaderFont( CEikonEnv::Static()->TitleFont() );
			//m_pData->SetCellFont( CEikonEnv::Static()->TitleFont() );
			break;
		}

		m_enZoomMode = enMode;
		m_pData->m_nZoomDivident = m_enZoomMode;
		m_pData->m_nZoomDivisor = enZoomSmall;

		m_pDrawer->m_pContainerWnd->SetExtent( m_pDrawer->m_pContainerWnd->Position(), 
			m_pDrawer->m_pContainerWnd->Size() );

		//set new top left cell to show current cell
		ScrollToMakeCellVisibleL( m_Current );

		m_pDrawer->Invalidate();
	}
}

void CSheetView::ResetCellSizeL( const TSize& CellSize )
{
	TSize MinSize, MaxSize;
	CellAjustmentLimits( MinSize, MaxSize );

	TSize Size( CellSize );
/*
	Size.iWidth = (Size.iWidth*m_pData->m_nZoomDivisor)/m_pData->m_nZoomDivident;
	Size.iHeight = (Size.iHeight*m_pData->m_nZoomDivisor)/m_pData->m_nZoomDivident;

	Size.iWidth = (Size.iWidth*m_pData->m_nZoomDivisor)/m_pData->m_nZoomDivident;
	Size.iHeight = (Size.iHeight*m_pData->m_nZoomDivisor)/m_pData->m_nZoomDivident;
*/
	if( ( Size.iWidth < MinSize.iWidth ) ||
	    ( Size.iWidth > MaxSize.iWidth ) ||
	    ( Size.iHeight < MinSize.iHeight ) ||
	    ( Size.iHeight > MaxSize.iHeight ) )
	{
		User::Leave( KErrArgument );
	}

	m_pData->ResetCellSizeL( Size );
	ScrollToMakeCellVisibleL( m_Current );
	m_pDrawer->m_pContainerWnd->SetExtent( m_pDrawer->m_pContainerWnd->Position(), 
		m_pDrawer->m_pContainerWnd->Size() );
	m_pDrawer->Invalidate();
}

//void CSheetView::ResetCellTextColorL( const TRgb& Color )
//{
//	m_pData->ResetCellTextColorL( Color );
//	m_pDrawer->Invalidate();
//}

//void CSheetView::ResetCellColorL( const TRgb& Color )
//{
//	m_pData->ResetCellColorL( Color );
//	m_pDrawer->Invalidate();
//}

void CSheetView::CellAjustmentLimits( TSize& MinSize, TSize& MaxSize ) const
{
	MinSize.iWidth = (m_MinCellSize.iWidth*m_pData->m_nZoomDivident)/m_pData->m_nZoomDivisor;
	MinSize.iHeight = (m_MinCellSize.iHeight*m_pData->m_nZoomDivident)/m_pData->m_nZoomDivisor;

	MaxSize.iWidth = (m_MaxCellSize.iWidth*m_pData->m_nZoomDivident)/m_pData->m_nZoomDivisor;
	MaxSize.iHeight = (m_MaxCellSize.iHeight*m_pData->m_nZoomDivident)/m_pData->m_nZoomDivisor;
}

void CSheetView::SetCellAjustmentLimits( const TSize& MinSize, const TSize& MaxSize )
{
#ifdef SHEET_TRACE_ENABLED
	CSheetTrace::SetCellAjustmentLimitsL( MinSize, MaxSize );
#endif

	m_MinCellSize.iWidth = (MinSize.iWidth*m_pData->m_nZoomDivisor)/m_pData->m_nZoomDivident;
	m_MinCellSize.iHeight = (MinSize.iHeight*m_pData->m_nZoomDivisor)/m_pData->m_nZoomDivident;

	m_MaxCellSize.iWidth = (MaxSize.iWidth*m_pData->m_nZoomDivisor)/m_pData->m_nZoomDivident;
	m_MaxCellSize.iHeight = (MaxSize.iHeight*m_pData->m_nZoomDivisor)/m_pData->m_nZoomDivident;
}

void CSheetView::HandleCellAjustmentEventL( ECellAjustmentEvent enEvent )
{
	if( !m_lCellAjustmentMode )
		return;

	TSize MinCellSize, MaxCellSize;
	CellAjustmentLimits( MinCellSize, MaxCellSize );
	TSize CellSize = m_pData->CellSizeL( m_Current );

	TInt nCellAjustmentStepInPixels = m_nCellAjustmentStepInPixels;
//	TInt nCellAjustmentStepInPixels = 
//		(m_nCellAjustmentStepInPixels*m_pData->m_nZoomDivident)/m_pData->m_nZoomDivisor;

	TBool lChanged = EFalse;
	switch( enEvent )
	{
	case enCellDecreaseHeight:
		if( CellSize.iHeight - nCellAjustmentStepInPixels >= MinCellSize.iHeight )
		{
			CellSize.iHeight -= nCellAjustmentStepInPixels;
			lChanged = ETrue;
		}
		break;
	case enCellInreaseHeight:
		if( CellSize.iHeight + nCellAjustmentStepInPixels <= MaxCellSize.iHeight )
		{
			CellSize.iHeight += nCellAjustmentStepInPixels;
			lChanged = ETrue;
		}
		break;
	case enCellDecreaseWidth:
		if( CellSize.iWidth - nCellAjustmentStepInPixels >= MinCellSize.iWidth )
		{
			CellSize.iWidth -= nCellAjustmentStepInPixels;
			lChanged = ETrue;
		}
		break;
	case enCellInreaseWidth:
		if( CellSize.iWidth + nCellAjustmentStepInPixels <= MaxCellSize.iWidth )
		{
			CellSize.iWidth += nCellAjustmentStepInPixels;
			lChanged = ETrue;
		}
		break;
	default:
		ASSERT( FALSE );
	}

	if( lChanged )
	{
		m_pData->SetCellSizeL( m_Current, CellSize );
		m_pDrawer->Invalidate();
	}
}

TPoint CSheetView::CurrentCell() const
{
	return m_Current;
}

void CSheetView::SetCurrentCell( TPoint Position)
{
	if( Position.iX < 0 || Position.iY < 0 )
		User::Leave( KErrArgument );

	TPoint Num = m_pDrawer->NumberOfCellsThatFitInRectL();
	if( Position.iX < m_TopLeft.iX || 
	    Position.iY < m_TopLeft.iY ||
	    Position.iX > m_TopLeft.iX + Num.iX || 
	    Position.iY > m_TopLeft.iY + Num.iY )
	{
		m_TopLeft = Position;
		m_Current = Position;
		m_pDrawer->Invalidate();
	}
	else
	{
		m_pDrawer->DrawItem( m_Current, EFalse, EFalse /* fixme - it can be selected item*/ );
		m_Current = Position;
		m_pDrawer->DrawItem( m_Current, ETrue, EFalse /* fixme - it can be selected item*/ );
	}

#ifdef __UIQ__
	m_pHScrollbar->SetModelThumbPosition(Position.iX);
	m_pVScrollbar->SetModelThumbPosition(Position.iY);
#endif
}

TBool CSheetView::ScrollToMakeCellVisibleL( const TPoint& Position )
{
	TPoint Size = m_pDrawer->NumberOfCellsThatFitInRectL();
	if( ( Position.iX < m_TopLeft.iX || Position.iX > m_TopLeft.iX + Size.iX ) ||
		( Position.iY < m_TopLeft.iY || Position.iY > m_TopLeft.iY + Size.iY ) )
	{
		if( Position.iX < m_TopLeft.iX )
			m_TopLeft.iX = Position.iX;
		else if( Position.iX > m_TopLeft.iX + Size.iX )
			m_TopLeft.iX = Position.iX - Size.iX + ( Size.iX > 1 ? 1 : 0 ) ;

		if( Position.iY < m_TopLeft.iY )
			m_TopLeft.iY = Position.iY;
		else if( Position.iY > m_TopLeft.iY + Size.iY )
			m_TopLeft.iY = Position.iY - Size.iY + ( Size.iY > 1 ? 1 : 0 ) ;

		return ETrue;
	}
	else
	{
		return EFalse;
	}
}

void CSheetView::Move( EMoveDirection enDirection )
{
#ifdef SHEET_TRACE_ENABLED
	CSheetTrace::MoveL( enDirection );
#endif

	TBool lNewItem = EFalse;
	TBool lScroll = EFalse;
	TPoint NewPosition( m_Current );

	switch( enDirection )
	{
	case enMoveUp:
		if( NewPosition.iY > 0 )
		{
			NewPosition.iY--;
			if( NewPosition.iY < m_TopLeft.iY )
			{
				m_TopLeft.iY--;
				m_Current.iY--;
				lScroll = ETrue;
			}
			else
				lNewItem = ETrue;

			#ifdef __UIQ__
				m_pVScrollbar->SetModelThumbPosition(m_pVScrollbar->ThumbPosition()-1);
			#endif
		}
		break;
	case enMoveDown:
		if( NewPosition.iY + 1 > 0 &&
			m_pModel->IsPositionInGoodRange( TPoint( NewPosition.iX, NewPosition.iY + 1) ) )
		{
			NewPosition.iY++;
			if( ( NewPosition.iY - m_TopLeft.iY ) > 
				( m_pDrawer->NumberOfCellsThatFitInRectL().iY - 1 ) )
			{
				m_TopLeft.iY++;
				m_Current.iY++;
				lScroll = ETrue;
			}
			else
				lNewItem = ETrue;

			#ifdef __UIQ__
			m_pVScrollbar->SetModelThumbPosition(m_pVScrollbar->ThumbPosition()+1);
			#endif
		}
		break;
	case enMoveLeft:
		if( NewPosition.iX > 0 )
		{
			NewPosition.iX--;
			if( NewPosition.iX < m_TopLeft.iX )
			{
				m_TopLeft.iX--;
				m_Current.iX--;
				lScroll = ETrue;
			}
			else
				lNewItem = ETrue;
			#ifdef __UIQ__
			m_pHScrollbar->SetModelThumbPosition(m_pHScrollbar->ThumbPosition()-1);
			#endif
		}
		break;
	case enMoveRight:
		if( NewPosition.iX + 1 > 0 &&
			m_pModel->IsPositionInGoodRange( TPoint( NewPosition.iX + 1, NewPosition.iY ) ) )
		{
			NewPosition.iX++;
			if( ( NewPosition.iX - m_TopLeft.iX ) > 
				( m_pDrawer->NumberOfCellsThatFitInRectL().iX - 1 ) )
			{
				m_TopLeft.iX++;
				m_Current.iX++;
				lScroll = ETrue;
			}
			else
				lNewItem = ETrue;
			#ifdef __UIQ__
				m_pHScrollbar->SetModelThumbPosition(m_pHScrollbar->ThumbPosition()+1);
			#endif
		}
		break;
	}

	if( lScroll )
	{
		m_pDrawer->Invalidate();
	}

	if( lNewItem )
	{
		TPoint Old( m_Current );
		m_Current = NewPosition;
		m_pDrawer->DrawItems( Old, m_Current );
		m_pDrawer->InvalidateCellView();

//		m_pDrawer->DrawItems( m_Current, NewPosition );
//		m_Current = NewPosition;
//		m_pDrawer->InvalidateCellView();

//		m_pDrawer->DrawItem( m_Current, EFalse, EFalse /* fixme - it can be selected item*/ );
//		m_pDrawer->DrawItem( NewPosition, ETrue, EFalse /* fixme - it can be selected item*/ );
//		m_Current = NewPosition;
//		m_pDrawer->InvalidateCellView();
	}
}

void CSheetView::SetScrollbars(CEikScrollBar* aHorizontal,CEikScrollBar* aVertical)
{
	m_pVScrollbar = aVertical;
	m_pHScrollbar = aHorizontal;
}
