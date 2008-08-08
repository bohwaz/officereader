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

#ifndef __SHEET_MODEL_H__
#define __SHEET_MODEL_H__

#include <e32base.h>


class CCellMapArray;
class CCell;
/**
*	main sheet functionality
*/

class CSheetModel : public CBase
{
private:
	CSheetModel();

public:
	virtual ~CSheetModel();

	static CSheetModel* NewL();
	static CSheetModel* NewLC();

	TInt Count() const;
	void InsertL( const TPoint& Position, const CCell& Cell );
	const CCell* AtL( const TPoint& Position ) const;
	CCell* AtL( const TPoint& Position );
	TBool IsExistL( const TPoint& Position ) const;

	void RemoveL( const TPoint& Position );
	void RemoveAllL();

	enum
	{
		enFindExact = 0x1,  //here I use TDesC::Compare
		enFindSubStr = 0x2, //here I use TDesC::Find
		enFindCompareCase = 0x04,
		enFindCompareNoCase = 0x08, 
		enFindOrderRows = 0x10,   //int this case I search row by row
		enFindOrderColumns = 0x20, //int this case I search column by column
		enFindOrderFast = 0x40     //
	};

	/*
	*	@param sText Text to find
	*	@param Pos first fosition to find,
	*		on return contains position
	*
	*   if sText points to NULL, I serch the previous text with previous flags
	*/

	TBool FindFirstL( const TDesC& sText, TPoint& Pos, TInt nFlags = enFindExact | enFindCompareCase );
	TBool FindNextL( TPoint& Pos );

private:
	TPoint m_OrderSearchPosition;

public:

	/*
	*   this is a bad idea to have some methods like:
	*	AddColumn( const TDesC& ), AddRow( const TDesC& ), 
	*	SetColumnsNumber( TInt ), SetRowsNumber( TInt ),
	*
	*	to get to know why - check MS Excel behaviour
	*   Excel does not have limitations on the number of rows and columns
	*/
	void ColumnTextL( TInt nIndex, TDes& sText ) const;
	void RowTextL( TInt nIndex, TDes& sText  ) const;

	TBool IsPositionInGoodRange( const TPoint& Point );

private:
	void ConstructL();
	
	static const TInt KLettersNumber = 26;// 26 - number of letters in english ABC

//	static const TInt KMaxColumnNameSize = 16;
//	static const TInt KMaxRowNameSize = 16;

//	TBuf<KMaxColumnNameSize> m_sColumnName;
//	TBuf<KMaxRowNameSize>    m_sRowName;

	CCellMapArray* m_pCells;

	HBufC* m_sTextToFind;
	TInt   m_nFindFlags;
	TBool  m_lSearchFromBegin;
};

#endif
