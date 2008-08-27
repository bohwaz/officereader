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

#ifndef __SHEET_CTRL_H__
#define __SHEET_CTRL_H__

#include <coecntrl.h>
#include <EIKSBOBS.H>
#include <EIKSCRLB.H>


#define __UIQ__

#include "sheet_drawer.h"

class CSheetModel;
class CSheetData;
//class CSheetDrawer;
class CSheetView;

#ifdef __UIQ__
	class CSheetCtrl : public CCoeControl,MEikScrollBarObserver
#else
	class CSheetCtrl : public CCoeControl
#endif
{

public:
	CSheetCtrl();	

public:
	virtual ~CSheetCtrl();

	static CSheetCtrl* NewL();
	static CSheetCtrl* NewLC();

	CSheetModel* Model() const;
	CSheetData* Data() const;
	CSheetView* View() const;

	void SetItemViewPos( TBool lOnTop = EFalse ) { m_lItemViewWndOnTop = lOnTop; }

	CSheetChildWindow* SheetWnd();
	void ConstructL();

	#ifdef __UIQ__
		void HandleScrollEventL(CEikScrollBar *aScrollBar, TEikScrollEvent aEventType);
	#endif
private:
//	void ConstructL();
//	void CreateDrawerL();

/**
*  Sheet windows
*/

	class CColumnHeaderWnd : public CSheetChildWindow
	{
	public:
		CColumnHeaderWnd( CSheetView* pView ):
			CSheetChildWindow( pView ) {}
		TSize MinimumSize(); //{ return TSize( 0, 15 ); }
		void Draw( const TRect& r ) const;
	};

	class CRowHeaderWnd : public CSheetChildWindow
	{
	public:
		CRowHeaderWnd( CSheetView* pView ):
			CSheetChildWindow( pView ) {}
		TSize MinimumSize();
		void Draw( const TRect& r ) const;
	};

	class CSheetWnd : public CSheetChildWindow
	{
	public:
		CSheetWnd( CSheetView* pView ):
			CSheetChildWindow( pView ) {}
		TSize MinimumSize();
		void Draw( const TRect& r ) const;
		TKeyResponse OfferKeyEventL( const TKeyEvent& KeyEvent, TEventCode Type );
		void HandlePointerEventL(const TPointerEvent &aPointerEvent);
	private:
		TPoint iOldPoint;
	};

	class CItemViewWnd : public CSheetChildWindow
	{
	public:
		CItemViewWnd( CSheetView* pView ):
			CSheetChildWindow( pView ) {}
		TSize MinimumSize();
		inline void ConstructL() { }
		void Draw( const TRect& r ) const;
	};

	void SizeChanged();
	TInt CountComponentControls() const;
	CCoeControl* ComponentControl( TInt nIndex ) const;
	void Draw( const TRect& r ) const;
	TKeyResponse OfferKeyEventL( const TKeyEvent& KeyEvent, TEventCode Type );
	void HandleResourceChange( TInt nType );
	CSheetData*   m_pData;
	CSheetModel*  m_pModel;
	CSheetDrawer* m_pDrawer;
	CSheetView*   m_pView;

	CColumnHeaderWnd* m_pColumnHeaderWnd;
	CRowHeaderWnd*    m_pRowHeaderWnd;
	CSheetWnd*        m_pSheetWnd;
	CItemViewWnd*     m_pItemViewWnd;

	#ifdef __UIQ__
	CEikScrollBar*    m_pVScrollbar;
	CEikScrollBar*    m_pHScrollbar;
	#endif

	TBool m_lItemViewWndOnTop;
};

#endif
