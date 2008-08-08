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

#ifndef __SHEET_CONTAINER_H__
#define __SHEET_CONTAINER_H__

#include <s32btree.h>

class CCell;

class CCellMapArray : public CBase
{
private:
	CCellMapArray();

public:
	virtual ~CCellMapArray();

	static CCellMapArray* NewL();
	static CCellMapArray* NewLC();
	
	TInt Count() const { return m_nCount; }
	void InsertL( const TPoint& Position, const CCell& Cell );
	const CCell* AtL( const TPoint& Position ) const;
	CCell* AtL( const TPoint& Position );
	void DeleteL( const TPoint& Position );
	void RemoveAllL();
	TBool IsExistL( const TPoint& Position ) const;

	CCell* GetFirstL( TPoint& Position ) const;
	CCell* GetNextL( TPoint& Position ) const;

	TRect Range() const { return m_Range; }

private:
	void ConstructL();

	struct TEntry
	{
		TUint32	Key;
		CCell*	pData;
	};

	class CPointKey : public MBtreeKey
	{
	public:
		void Between( const TAny* Left, const TAny* Right, TBtreePivot& Pivot ) const;
		TInt Compare( const TAny* Left, const TAny* Right ) const;
		const TAny* Key( const TAny* Entry ) const;
	};

	static TUint32 PositionToKey( const TPoint& Position );
	static TPoint KeyToPosition( TUint32 Key );
    
	TBtreeFix<TEntry,TUint32>* m_pTree;
	CPointKey     m_Key;
	CMemPagePool* m_pMemPool;

	/*
	* m_Range tores top-left(min) and bottom-right(max) element positions
	*/
	TRect         m_Range;

	mutable TBtreePos m_Pos;
	TInt m_nCount;
};

inline TUint32 CCellMapArray::PositionToKey( const TPoint& Position )
{
	// convert TPoint to TUint32 to reduce memory usage & increase speed
	TUint32 Res = ( ( Position.iX & 0x0000ffff ) << 16 ) | ( Position.iY & 0x0000ffff );
	return Res;
}

inline TPoint CCellMapArray::KeyToPosition( TUint32 Key )
{
	// convert TUint32 to TPoint to reduce memory usage & increase speed
	return TPoint( ( Key & 0xffff0000 ) >> 16, ( Key & 0x0000ffff ) );
}

#endif
