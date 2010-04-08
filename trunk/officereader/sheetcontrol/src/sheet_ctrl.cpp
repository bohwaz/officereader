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

#include <eikappui.h>
#include <eikenv.h>
#include <akndef.h>

#include "sheet_trace.h"
#include "sheet_ctrl.h"
#include "sheet_data.h"
#include "sheet_model.h"
#include "sheet_view.h"

CSheetCtrl::CSheetCtrl():
	m_pData( NULL ),
	m_pModel( NULL ),
	m_pDrawer( NULL ),
	m_pView( NULL ),
	m_pColumnHeaderWnd( NULL ),
	m_pRowHeaderWnd( NULL ),
	m_pSheetWnd( NULL ),
	m_pItemViewWnd( NULL ),
#ifdef __UIQ__	
	m_pVScrollbar( NULL ),
	m_pHScrollbar( NULL ),
#endif
	m_lItemViewWndOnTop( EFalse )
{
}

CSheetCtrl::~CSheetCtrl()
{
	if( m_pData )
	{
		delete m_pData;
		m_pData = NULL;
	}

	if( m_pModel )
	{
		delete m_pModel;
		m_pModel = NULL;
	}

	if( m_pView )
	{
		delete m_pView;
		m_pView = NULL;
	}

	if( m_pDrawer )
	{
		delete m_pDrawer;
		m_pDrawer = NULL;
	}

	if( m_pColumnHeaderWnd )
	{
		delete m_pColumnHeaderWnd;
		m_pColumnHeaderWnd = NULL;
	}

	if( m_pRowHeaderWnd )
	{
		delete m_pRowHeaderWnd;
		m_pRowHeaderWnd = NULL;
	}

	if( m_pSheetWnd )
	{
		CEikonEnv::Static()->EikAppUi()->RemoveFromStack( m_pSheetWnd );
		delete m_pSheetWnd;
		m_pSheetWnd = NULL;
	}

	if( m_pItemViewWnd )
	{
		delete m_pItemViewWnd;
		m_pItemViewWnd = NULL;
	}

#ifdef __UIQ__
	if( m_pHScrollbar )
	{
		delete m_pHScrollbar;
		m_pHScrollbar = NULL;
	}

	if( m_pVScrollbar )
	{
		delete m_pVScrollbar;
		m_pVScrollbar = NULL;
	}
#endif

	CEikonEnv::Static()->EikAppUi()->RemoveFromStack( this );
}

CSheetCtrl* CSheetCtrl::NewL()
{
	CSheetCtrl* pSelf = CSheetCtrl::NewLC();
	CleanupStack::Pop( pSelf );
	return pSelf;
}

CSheetCtrl* CSheetCtrl::NewLC()
{
	CSheetCtrl* pSelf = new ( ELeave ) CSheetCtrl;
	CleanupStack::PushL( pSelf );
	pSelf->ConstructL();
	return pSelf;
}

void CSheetCtrl::ConstructL()
{
#ifdef SHEET_TRACE_ENABLED
	CSheetTrace::InitL();
#endif

	CSheetData* pData = CSheetData::NewLC();
	CSheetModel* pModel = CSheetModel::NewLC();
	CSheetDrawer* pDrawer = CSheetDrawer::NewLC( pModel, pData );
	CSheetView* pView = CSheetView::NewLC( pModel, pData, pDrawer );

	
	CColumnHeaderWnd* pColumnHeaderWnd = new ( ELeave ) CColumnHeaderWnd( pView );
	CleanupStack::PushL( pColumnHeaderWnd );
	pColumnHeaderWnd->SetContainerWindowL( *this );
	pColumnHeaderWnd->SetMopParent( this );
	pColumnHeaderWnd->ConstructL();	

	CRowHeaderWnd* pRowHeaderWnd = new ( ELeave ) CRowHeaderWnd( pView );
	CleanupStack::PushL( pRowHeaderWnd );
	pRowHeaderWnd->SetContainerWindowL( *this );
	pRowHeaderWnd->SetMopParent( this );
	pRowHeaderWnd->ConstructL();	

	CSheetWnd* pSheetWnd = new ( ELeave ) CSheetWnd( pView );
	CleanupStack::PushL( pSheetWnd );
	pSheetWnd->SetContainerWindowL( *this );
	pSheetWnd->SetMopParent( this );
	pSheetWnd->ConstructL();	

	CItemViewWnd* pItemViewWnd = new ( ELeave ) CItemViewWnd( pView );
	CleanupStack::PushL( pItemViewWnd );
	pItemViewWnd->SetContainerWindowL( *this );
	pItemViewWnd->SetMopParent( this );
	pItemViewWnd->ConstructL();	

	CleanupStack::Pop( pItemViewWnd );
	CleanupStack::Pop( pSheetWnd );
	CleanupStack::Pop( pRowHeaderWnd );
	CleanupStack::Pop( pColumnHeaderWnd );
	CleanupStack::Pop( pView );
	CleanupStack::Pop( pDrawer );
	CleanupStack::Pop( pModel );
	CleanupStack::Pop( pData );

	m_pColumnHeaderWnd = pColumnHeaderWnd;
	m_pRowHeaderWnd = pRowHeaderWnd;
	m_pSheetWnd = pSheetWnd;
	m_pItemViewWnd = pItemViewWnd;


	CEikonEnv::Static()->EikAppUi()->AddToStackL( m_pSheetWnd );
	CEikonEnv::Static()->EikAppUi()->AddToStackL( this );

	m_pData = pData;
	m_pData->SetModel( pModel );
	m_pModel = pModel;
	m_pDrawer = pDrawer;
	m_pDrawer->SetView( pView );
	m_pDrawer->SetWindows( this, m_pColumnHeaderWnd, m_pRowHeaderWnd, m_pSheetWnd, m_pItemViewWnd );
	m_pView = pView;

	#ifdef __UIQ__
		m_pVScrollbar = new ( ELeave ) CEikScrollBar();
		CleanupStack::PushL( m_pVScrollbar );
		m_pVScrollbar->SetContainerWindowL( *this );
		m_pVScrollbar->SetMopParent( this );
		m_pVScrollbar->ConstructL(this,CEikScrollBar::EVertical,100);	
		CleanupStack::Pop( m_pVScrollbar );

		m_pHScrollbar = new ( ELeave ) CEikScrollBar();
		CleanupStack::PushL( m_pHScrollbar );
		m_pHScrollbar->SetContainerWindowL( *this );
		m_pHScrollbar->SetMopParent( this );
		m_pHScrollbar->ConstructL(this,CEikScrollBar::EHorizontal,100);	
		CleanupStack::Pop( m_pHScrollbar );

		TEikScrollBarModel verticalModel;
		verticalModel.iThumbSpan = 5;
		verticalModel.iThumbPosition = 0;
		verticalModel.iScrollSpan = 1000;
		m_pVScrollbar->SetModelL(&verticalModel);
		m_pHScrollbar->SetModelL(&verticalModel);

		m_pView->SetScrollbars(m_pHScrollbar,m_pVScrollbar);
	#endif


}

CSheetModel* CSheetCtrl::Model() const
{
	return m_pModel;
}

CSheetData* CSheetCtrl::Data() const
{
	return m_pData;
}

CSheetView* CSheetCtrl::View() const
{
	return m_pView;
}

CSheetChildWindow* CSheetCtrl::SheetWnd()
{
	return m_pSheetWnd;
}


void CSheetCtrl::SizeChanged()
{
	if( m_lItemViewWndOnTop )
	{
		#ifdef __UIQ__
			TPoint Offset = PositionRelativeToScreen();

			m_pItemViewWnd->SetExtent( TPoint(0,0),
					TSize( Rect().Width(), /*m_pData->ZoomedValue(*/ m_pItemViewWnd->MinimumSize().iHeight/* ) */));
			m_pColumnHeaderWnd->SetExtent( TPoint( Offset.iX + m_pRowHeaderWnd->MinimumSize().iWidth, 
				/* Max Änderung: m_pItemViewWnd->Position().iY */ Offset.iY + m_pItemViewWnd->Size().iHeight ), 
				TSize( Rect().Width() - m_pRowHeaderWnd->MinimumSize().iWidth -15 , m_pColumnHeaderWnd->MinimumSize().iHeight ) );

			m_pRowHeaderWnd->SetExtent( 
				TPoint( Offset.iX , m_pColumnHeaderWnd->Position().iY + m_pColumnHeaderWnd->Size().iHeight ),
				TSize( m_pRowHeaderWnd->MinimumSize().iWidth, 
				Rect().Height() - m_pColumnHeaderWnd->Size().iHeight - 
				m_pItemViewWnd->Size().iHeight -15 ) ); 

			m_pSheetWnd->SetExtent( TPoint( m_pColumnHeaderWnd->Position().iX, m_pRowHeaderWnd->Position().iY ),
				TSize( m_pColumnHeaderWnd->Size().iWidth, m_pRowHeaderWnd->Size().iHeight) );	

				m_pVScrollbar->SetExtent( TPoint( Rect().Width()-15, m_pItemViewWnd->MinimumSize().iHeight) ,TSize(15,Rect().Height() - m_pItemViewWnd->Size().iHeight -15));
				m_pHScrollbar->SetExtent( TPoint( 0, Rect().Height() - 15) ,TSize(Rect().Width()-15,15));
		#else
			TPoint Offset = PositionRelativeToScreen();
				//m_pItemViewWnd->SetExtent( TPoint( Offset.iX, Offset.iY ),
				//	TSize( Rect().Width(), /*m_pData->ZoomedValue(*/ m_pItemViewWnd->MinimumSize().iHeight/* ) */));
			m_pItemViewWnd->SetExtent( TPoint( 0,0),TSize( Rect().Width(), /*m_pData->ZoomedValue(*/ m_pItemViewWnd->MinimumSize().iHeight/* ) */));

			m_pColumnHeaderWnd->SetExtent( TPoint( Offset.iX + m_pRowHeaderWnd->MinimumSize().iWidth, 
				/* Max Änderung: m_pItemViewWnd->Position().iY */ Offset.iY + m_pItemViewWnd->Size().iHeight ), 
				TSize( Rect().Width() - m_pRowHeaderWnd->MinimumSize().iWidth , m_pColumnHeaderWnd->MinimumSize().iHeight ) );

			m_pRowHeaderWnd->SetExtent( 
				TPoint( Offset.iX , m_pColumnHeaderWnd->Position().iY + m_pColumnHeaderWnd->Size().iHeight ),
				TSize( m_pRowHeaderWnd->MinimumSize().iWidth, 
				Rect().Height() - m_pColumnHeaderWnd->Size().iHeight - 
				m_pItemViewWnd->Size().iHeight ) ); 

			m_pSheetWnd->SetExtent( TPoint( m_pColumnHeaderWnd->Position().iX, m_pRowHeaderWnd->Position().iY ),
				TSize( m_pColumnHeaderWnd->Size().iWidth, m_pRowHeaderWnd->Size().iHeight) );	
			
		#endif

	}
	else
	{
		// TODO: funktioneirt nicht
		TPoint Offset = PositionRelativeToScreen();

		m_pColumnHeaderWnd->SetExtent( TPoint( Offset.iX + m_pRowHeaderWnd->MinimumSize().iWidth, Offset.iY ), 
			TSize( Rect().Width() - m_pRowHeaderWnd->MinimumSize().iWidth, m_pColumnHeaderWnd->MinimumSize().iHeight ) );

		m_pRowHeaderWnd->SetExtent( 
			TPoint( Offset.iX , Offset.iY + m_pColumnHeaderWnd->Size().iHeight ),
			TSize( m_pRowHeaderWnd->MinimumSize().iWidth, 
			Rect().Height() - m_pColumnHeaderWnd->Size().iHeight - 
			/*m_pData->ZoomedValue( */m_pItemViewWnd->MinimumSize().iHeight /*)*/ ) ); 

		m_pSheetWnd->SetExtent( TPoint( m_pColumnHeaderWnd->Position().iX, m_pRowHeaderWnd->Position().iY ),
			TSize( m_pColumnHeaderWnd->Size().iWidth, m_pRowHeaderWnd->Size().iHeight ) );
	
		m_pItemViewWnd->SetExtent( TPoint( m_pRowHeaderWnd->Position().iX,
			m_pColumnHeaderWnd->Position().iY + m_pColumnHeaderWnd->Size().iHeight +
			m_pRowHeaderWnd->Size().iHeight ),
			TSize( Rect().Width(), /*m_pData->ZoomedValue(*/ m_pItemViewWnd->MinimumSize().iHeight /*)*/ ) );

		#ifdef __UIQ__
			m_pVScrollbar->SetExtent( TPoint(100,100),TSize(10,100));
			m_pHScrollbar->SetExtent( TPoint(100,100),TSize(10,100));
		#endif
	}
}

TInt CSheetCtrl::CountComponentControls() const
{
	#ifdef __UIQ__
		return 6;
	#else
		return 4;
	#endif
}

CCoeControl* CSheetCtrl::ComponentControl( TInt nIndex ) const
{
	switch( nIndex )
	{
	case 0: return m_pItemViewWnd;
	case 1: return m_pRowHeaderWnd;
	case 2: return m_pSheetWnd;
	case 3: return m_pColumnHeaderWnd;
	#ifdef __UIQ__
		case 4: return m_pVScrollbar;
		case 5: return m_pHScrollbar;
	#endif
	default: return NULL;
	}
}

void CSheetCtrl::Draw( const TRect& r ) const
{
	SystemGc().Clear( r );
}

TKeyResponse CSheetCtrl::OfferKeyEventL( const TKeyEvent& KeyEvent, TEventCode Type )
{
	if( Type != EEventKey )
		return EKeyWasNotConsumed;

	if( m_pView->CellAjustmentMode() )
	{

		#ifdef __UIQ__
		TChar charCode( KeyEvent.iCode );
		switch( charCode )
		{
		case EKeyEnter:
		case EKeyDevice8:
			m_pView->SetCellAjustmentMode( EFalse );
			return EKeyWasConsumed;
		case EKeyUpArrow:
		case EKeyDevice4:
		case EKeyDevice1:
			m_pView->HandleCellAjustmentEventL( CSheetView::enCellDecreaseHeight );
			return EKeyWasConsumed;
		case EKeyDownArrow:
		case EKeyDevice5:
		case EKeyDevice2:
			m_pView->HandleCellAjustmentEventL( CSheetView::enCellInreaseHeight );
			return EKeyWasConsumed;
		case EKeyLeftArrow:
		case EKeyDevice6:
			m_pView->HandleCellAjustmentEventL( CSheetView::enCellDecreaseWidth );
			return EKeyWasConsumed;
		case EKeyRightArrow:
		case EKeyDevice7:
			m_pView->HandleCellAjustmentEventL( CSheetView::enCellInreaseWidth );
			return EKeyWasConsumed;
		default:
			return EKeyWasNotConsumed;
		}
		#else
		TChar charCode( KeyEvent.iCode );

		switch( charCode )
		{
		case EKeyEnter:
		case EKeyDevice3:
			m_pView->SetCellAjustmentMode( EFalse );
			return EKeyWasConsumed;
		case EKeyUpArrow:
			m_pView->HandleCellAjustmentEventL( CSheetView::enCellDecreaseHeight );
			return EKeyWasConsumed;
		case EKeyDownArrow:
			m_pView->HandleCellAjustmentEventL( CSheetView::enCellInreaseHeight );
			return EKeyWasConsumed;
		case EKeyLeftArrow:
			m_pView->HandleCellAjustmentEventL( CSheetView::enCellDecreaseWidth );
			return EKeyWasConsumed;
		case EKeyRightArrow:
			m_pView->HandleCellAjustmentEventL( CSheetView::enCellInreaseWidth );
			return EKeyWasConsumed;
		default:
			return EKeyWasNotConsumed;
		}
		#endif
	}
	else
	{
		return EKeyWasNotConsumed;
	}
}

void CSheetCtrl::HandleResourceChange( TInt nType )
{
	CCoeControl::HandleResourceChange( nType );

	if( nType == KEikDynamicLayoutVariantSwitch )
	{
		m_pData->UpdateFonts(); 
		m_pView->ScrollToMakeCellVisibleL( m_pView->CurrentCell() );
	}
}
//-------------------------- CSheetCtrl::CColumnHeaderWnd -------------------------

TSize CSheetCtrl::CColumnHeaderWnd::MinimumSize()
{ 
	return TSize( 0, m_pView->Data()->ColumnHeaderHeight() );
}

void CSheetCtrl::CColumnHeaderWnd::Draw( const TRect& /* r */ ) const
{
	m_pView->Drawer()->DrawColumnWnd( SystemGc() );
}

//------------------------ CSheetCtrl::CRowHeaderWnd -------------------------------

TSize CSheetCtrl::CRowHeaderWnd::MinimumSize()
{
	return TSize( m_pView->Data()->RowHeaderWidth(), 0 );
}

void CSheetCtrl::CRowHeaderWnd::Draw( const TRect& /* r */ ) const
{
	m_pView->Drawer()->DrawRowWnd( SystemGc() );
}

//-------------------------- CSheetCtrl::CSheetWnd --------------------------------


void CSheetCtrl::CSheetWnd::HandlePointerEventL(const TPointerEvent &aPointerEvent)
{
	
	if (aPointerEvent.iType== TPointerEvent::EButton1Down)
	{
		TPoint aNewPoint = m_pView->Drawer()->CellbyPositionL( aPointerEvent.iPosition );
		m_pView->SetCurrentCell(aNewPoint);	
		m_pView->Drawer()->Invalidate();
	}

	if (aPointerEvent.iType == TPointerEvent::EDrag)
	{
		m_pView->SetCellAjustmentMode(ETrue);

		if (aPointerEvent.iPosition.iX > iOldPoint.iX)
		{
			m_pView->HandleCellAjustmentEventL( CSheetView::enCellInreaseWidth );
		}
		if (aPointerEvent.iPosition.iX < iOldPoint.iX)
		{
			m_pView->HandleCellAjustmentEventL( CSheetView::enCellDecreaseWidth );
		}

		if (aPointerEvent.iPosition.iY > iOldPoint.iY)
		{
			m_pView->HandleCellAjustmentEventL( CSheetView::enCellInreaseHeight );
		}
		if (aPointerEvent.iPosition.iY < iOldPoint.iY)
		{
			m_pView->HandleCellAjustmentEventL( CSheetView::enCellDecreaseHeight );
		}
		m_pView->SetCellAjustmentMode(EFalse);
	}	
	iOldPoint = aPointerEvent.iPosition;
}

TSize CSheetCtrl::CSheetWnd::MinimumSize()
{
	return TSize( 0, 0 );
}

void CSheetCtrl::CSheetWnd::Draw( const TRect& /* r */ ) const
{
		m_pView->Drawer()->DrawSheetWnd( SystemGc() );
}

TKeyResponse CSheetCtrl::CSheetWnd::OfferKeyEventL( const TKeyEvent& KeyEvent, TEventCode Type )
{
	if( Type != EEventKey ) 
		return EKeyWasNotConsumed;
 
#ifdef __UIQ__
	TChar charCode( KeyEvent.iCode );

	switch( charCode )
	{
	case EKeyDevice4:
	case EKeyDevice1: //wheel up
		m_pView->Move( CSheetView::enMoveUp );
		return EKeyWasConsumed;
	case EKeyDevice5:
	case EKeyDevice2: //wheel down
		m_pView->Move( CSheetView::enMoveDown );
		return EKeyWasConsumed;
	case EKeyDevice6:
		m_pView->Move( CSheetView::enMoveLeft );
		return EKeyWasConsumed;
	case EKeyDevice7:
		m_pView->Move( CSheetView::enMoveRight );
		return EKeyWasConsumed;
	case EKeyUpArrow:
		m_pView->Move( CSheetView::enMoveUp );
		return EKeyWasConsumed;
	case EKeyDownArrow:
		m_pView->Move( CSheetView::enMoveDown );
		return EKeyWasConsumed;
 	case EKeyLeftArrow:
		m_pView->Move( CSheetView::enMoveLeft );
		return EKeyWasConsumed;
	case EKeyRightArrow:
		m_pView->Move( CSheetView::enMoveRight );
		return EKeyWasConsumed;
	}
#else
	TChar charCode( KeyEvent.iCode );

	switch( charCode )
	{
	case EKeyUpArrow:
		m_pView->Move( CSheetView::enMoveUp );
		return EKeyWasConsumed;
	case EKeyDownArrow:
		m_pView->Move( CSheetView::enMoveDown );
		return EKeyWasConsumed;
	case EKeyLeftArrow:
		m_pView->Move( CSheetView::enMoveLeft );
		return EKeyWasConsumed;
	case EKeyRightArrow:
		m_pView->Move( CSheetView::enMoveRight );
		return EKeyWasConsumed;
	}
#endif
	return EKeyWasNotConsumed;
}

//------------------------ CSheetCtrl::CItemViewWnd -----------------------------

TSize CSheetCtrl::CItemViewWnd::MinimumSize()
{
	//fixme - I have to get minimum size from sheet data
	//return TSize( 0, 15 );
	return TSize( 0, m_pView->Data()->ItemWndFont()->HeightInPixels() + 
		m_pView->Data()->ZoomedValue( 4 ) );
}

void CSheetCtrl::CItemViewWnd::Draw( const TRect& /* r */ ) const
{
	m_pView->Drawer()->DrawItemViewWnd( SystemGc() );
}

#ifdef __UIQ__
void CSheetCtrl::HandleScrollEventL(CEikScrollBar *aScrollBar, TEikScrollEvent aEventType)
{
	TPoint aCurrent = m_pView->CurrentCell();
	switch (aEventType)
	{
		case EEikScrollDown:
			m_pView->Move(CSheetView::enMoveDown);
			break;
		case EEikScrollUp:
			m_pView->Move(CSheetView::enMoveUp);
			break;
			case EEikScrollRight:
			m_pView->Move(CSheetView::enMoveRight);
			break;
		case EEikScrollLeft:
			m_pView->Move(CSheetView::enMoveLeft);
			break;
		case EEikScrollPageLeft:
			aCurrent.iX -= 5;
			m_pView->SetCurrentCell(aCurrent);
			break;
		case EEikScrollPageRight:
			aCurrent.iX += 5;
			m_pView->SetCurrentCell(aCurrent);
			break;
		case EEikScrollPageUp:
			aCurrent.iY -= 5;
			m_pView->SetCurrentCell(aCurrent);
			break; 
		case EEikScrollPageDown :
			aCurrent.iY += 5;
			m_pView->SetCurrentCell(aCurrent);
			break;
		default:
			m_pView->SetCurrentCell(TPoint(m_pHScrollbar->ThumbPosition(),m_pVScrollbar->ThumbPosition() ));
	}
}
#endif
