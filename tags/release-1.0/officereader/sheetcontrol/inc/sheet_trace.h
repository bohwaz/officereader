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

#ifndef __SHEET_TRACE_H__
#define __SHEET_TRACE_H__

#include "sheet_view.h"
#include <gdi.h>

class CCell;

//#define SHEET_TRACE_ENABLED 

class CSheetTrace
{
public:
	static void InitL();

	//CSheetModel
	static void InsertL( const TPoint& Position, const CCell& Cell );
	static void RemoveL( const TPoint& Position );
	static void RemoveAllL();

	//CSheetView
	static void SetCellAjustmentLimitsL( const TSize& MinSize, const TSize& MaxSize ); 

	static void MoveL( CSheetView::EMoveDirection enDirection );
	static void SetZoomModeL( CSheetView::EZoomMode enMode );

	//CSheetData
	static void ResetCellSizeL( const TSize& Size );
	static void SetCellSizeL( const TPoint& Position, const TSize& Size );
	static void SetCellTextColorL( const TPoint& Position, const TRgb& Color );
	static void SetCellColorL( const TPoint& Position, const TRgb& Color );
	static void SetCellAlignmentL( const TPoint& Position, CGraphicsContext::TTextAlign Alignment );
};

#endif
