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

#ifndef __SHEET_DRAWER_H__
#define __SHEET_DRAWER_H__

#include <e32base.h>
#include <coecntrl.h>
#include "cell.h"

class CSheetModel;
class CSheetData;
class CSheetView;

class CSheetChildWindow : public CCoeControl
{	
public:
	CSheetChildWindow( CSheetView* pView ):
		m_pView( pView ){}
	
	virtual void ConstructL() { CreateWindowL(); EnableDragEvents();}
	CWindowGc& SystemGc() const { return CCoeControl::SystemGc(); }
	RWindow& Window() const { return CCoeControl::Window(); }
protected:
	CSheetView* m_pView;
};

/*
* sheet drawer
*/
class CSheetDrawer: public CBase
{
private:
	CSheetDrawer( CSheetModel* pModel, CSheetData* pData );

public:
	virtual ~CSheetDrawer();

	static CSheetDrawer* NewL( CSheetModel* pModel, CSheetData* pData );
	static CSheetDrawer* NewLC( CSheetModel* pModel, CSheetData* pData );

	void SetView( CSheetView* pView ){ m_pView = pView; }

	void SetWindows( CCoeControl* pContainerWnd, CSheetChildWindow* pColumnWnd,
		CSheetChildWindow* pRowWnd, CSheetChildWindow* pSheetWnd, 
		CSheetChildWindow* pCellViewWnd );

	void Invalidate() const;
	void InvalidateCellView() const;

	virtual void DrawColumnWnd( CWindowGc& gc ) const;
	virtual void DrawRowWnd( CWindowGc& gc ) const;
	virtual void DrawSheetWnd( CWindowGc& gc ) const;
	virtual void DrawItemViewWnd( CWindowGc& gc ) const;

	virtual void DrawItems( const TPoint& Old, const TPoint& New ) const;

	virtual void DrawItem( const TPoint& Position, TBool lItemIsCurrent, TBool lItemIsSelected, 
		TBool lActivate = ETrue ) const;

	CWindowGc& Gc() const { return m_pSheetWnd->SystemGc(); }

	//virtual TBool XYPosToCellIndex( const TPoint& Cell, TPoint& Position ) const; // returns false if no item
	virtual TPoint NumberOfCellsThatFitInRectL() const;
	TPoint CellbyPositionL( const TPoint& Position) const;

private:
	void ConstructL();
	TPoint CellPositionL( const TPoint& Position ) const;

protected:
	virtual void DrawItems2( const TPoint& Begin, const TPoint& End, TBool lActivate = ETrue ) const;

	CSheetModel* m_pModel;
	CSheetData*  m_pData;
	CSheetView*  m_pView;
	CWindowGc*   m_pSheetGc;

	CCoeControl*       m_pContainerWnd;
	CSheetChildWindow* m_pColumnWnd;
	CSheetChildWindow* m_pRowWnd;
	CSheetChildWindow* m_pSheetWnd;
	CSheetChildWindow* m_pCellViewWnd;

	mutable TBuf<CCell::KMaxTextRepresentation> m_sCellTextRepresentation;
	void MakeCellRepresentationL( const CCell& Cell ) const;

	friend class CSheetView;
};

#endif
