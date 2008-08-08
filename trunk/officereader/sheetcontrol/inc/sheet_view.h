/*************************************************************************
 *
 * DO NOT ALTER OR REMOVE COPYRIGHT NOTICES OR THIS FILE HEADER.
 * 
 * Copyright 2008,2009,2010 by ODENDAHL SEPT-Solutions
 *
 * Office Reader - a OpenDocument viewer for Symbian OS
 *
 * URL: 		$HeadURL$
 * Revision: 	$LastChangedRevision$
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

#ifndef __SHEET_VIEW_H__
#define __SHEET_VIEW_H__

#include <e32base.h>
#include <EIKSBOBS.H>
#include <EIKSCRLB.H>

class CCoeControl;
class CSheetModel;
class CSheetData;
class CSheetDrawer;
class TRgb;

/**
*	main sheet functionality
*/
class CSheetView : public CBase  
{
public:
	typedef CArrayFix<TPoint> CSelectionArray;

	enum EZoomMode
	{
		enZoomSmall = 1000,
		enZoomMedium = 1400,
		enZoomLarge = 1700
	};

private:
	CSheetView( CSheetModel* pModel, CSheetData* pData, CSheetDrawer* pDrawer );
public:

	virtual ~CSheetView();

	static CSheetView* NewL( CSheetModel* pModel, CSheetData* pData, CSheetDrawer* pDrawer );
	static CSheetView* NewLC( CSheetModel* pModel, CSheetData* pData, CSheetDrawer* pDrawer );

	CSheetModel* Model() const;
	CSheetData* Data() const { return m_pData; }
	CSheetDrawer* Drawer() const { return m_pDrawer; }

	//zoom fuctions
	EZoomMode ZoomMode() const { return m_enZoomMode; }
	void SetZoomModeL( EZoomMode enMode );

	//cell size ajustment
	void CellAjustmentLimits( TSize& MinSize, TSize& MaxSize ) const;
	void SetCellAjustmentLimits( const TSize& MinSize, const TSize& MaxSize ); 
	TBool CellAjustmentMode() const { return m_lCellAjustmentMode; }
	void SetCellAjustmentMode( TBool lMode ) { m_lCellAjustmentMode = lMode; }
	void ResetCellSizeL( const TSize& Size );
//	void ResetCellTextColorL( const TRgb& Color );
//	void ResetCellColorL( const TRgb& Color );

	enum ECellAjustmentEvent
	{
		enCellInreaseWidth = 0,
		enCellDecreaseWidth,
		enCellInreaseHeight,
		enCellDecreaseHeight,
	};

	void HandleCellAjustmentEventL( ECellAjustmentEvent enEvent );

	void SetScrollbars(CEikScrollBar* aHorizontal,CEikScrollBar* aVertical  );

	// misc. access functions for the main attributes
	TPoint CurrentCell() const;	
	void SetCurrentCell( TPoint Position);

	TPoint TopLeftCell() const { return m_TopLeft; }		
	void SetTopLeftCell( TPoint Position ) { m_TopLeft = Position; }

	TBool ScrollToMakeCellVisibleL( const TPoint& Position );

	// functions that support selection
	const CSelectionArray* SelectionCells() const;
	void GetSelectionL( CSelectionArray* pSelectionArray ) const;
	void SetSelectionL( const CSelectionArray* pSelectionArray );
	void ClearSelection();
	void ToggleCellL( TPoint Position );
	void SelectCellL( TPoint Position );
	void DeselectCell( TPoint Position );

	enum EMoveDirection
	{
		enMoveUp = 0,
		enMoveDown,
		enMoveLeft,
		enMoveRight
	};

	void Move( EMoveDirection enDirection );

private:
	void ConstructL();

	CSheetModel*  m_pModel;
	CSheetData*   m_pData;
	CSheetDrawer* m_pDrawer;

	TPoint m_Current;
	TPoint m_TopLeft;

	TBool m_lCellAjustmentMode;
	TInt  m_nCellAjustmentStepInPixels;
	TSize m_MinCellSize;
	TSize m_MaxCellSize;

	EZoomMode m_enZoomMode;
	
	CEikScrollBar* m_pVScrollbar;
	CEikScrollBar* m_pHScrollbar;
};

#endif
