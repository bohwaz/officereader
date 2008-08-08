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

#include "cell_array.h"
#include "cell.h"

CCellMapArray::CCellMapArray():
	m_pTree( NULL ),
	m_pMemPool( NULL ),
	m_nCount( 0 )
{
}

CCellMapArray::~CCellMapArray()
{
	TBtreeMark i;
	TBool more = m_pTree->ResetL( i );
	TEntry Entry;
	while( more )
	{
		m_pTree->ExtractAtL( i, Entry );
		delete Entry.pData;
		more = m_pTree->NextL( i );
	}

	delete m_pTree;
	delete m_pMemPool;
}

CCellMapArray* CCellMapArray::NewL()
{
	CCellMapArray* pSelf = CCellMapArray::NewLC();
	CleanupStack::Pop( pSelf );
	return pSelf;
}

CCellMapArray* CCellMapArray::NewLC()
{
	CCellMapArray* pSelf = new ( ELeave ) CCellMapArray;
	CleanupStack::PushL( pSelf );
	pSelf->ConstructL();
	return pSelf;
}

void CCellMapArray::InsertL( const TPoint& Position, const CCell& Cell )
{
	TBtreePos TreePos;
	TEntry Entry;
	Entry.Key = PositionToKey( Position ); 
	CCell* pCell = new ( ELeave ) CCell;
	CleanupStack::PushL( pCell );
	(*pCell) = Cell;
	Entry.pData = pCell;
	TBool no_dup = m_pTree->InsertL( TreePos, Entry );
	if( ! no_dup )
	{
		delete pCell;
	}
	else
	{
		//update range
		{
			if( Position.iX > m_Range.iBr.iX )
				m_Range.iBr.iX = Position.iX;

			if( Position.iY > m_Range.iBr.iY )
				m_Range.iBr.iY = Position.iY;

			if( Position.iX < m_Range.iTl.iX )
				m_Range.iTl.iX = Position.iX;

			if( Position.iY < m_Range.iTl.iY )
				m_Range.iTl.iY = Position.iY;
		}

		m_nCount++;
	}
	CleanupStack::Pop( pCell );
}

const CCell* CCellMapArray::AtL( const TPoint& Position ) const
{
	TBtreePos TreePos;
	TBool lFound = m_pTree->FindL( TreePos, PositionToKey( Position ) );
	if( lFound )
	{
		TEntry Entry;
		m_pTree->ExtractAtL( TreePos, Entry );
		return Entry.pData;
	}
	else
	{
		return NULL;
	}
}

CCell* CCellMapArray::AtL( const TPoint& Position )
{
	TBtreePos TreePos;
	TBool lFound = m_pTree->FindL( TreePos, PositionToKey( Position ) );
	if( lFound )
	{
		TEntry Entry;
		m_pTree->ExtractAtL( TreePos, Entry );
		return Entry.pData;
	}
	else
	{
		return NULL;
	}
}

void CCellMapArray::DeleteL( const TPoint& Position )
{
	TBtreePos TreePos;
	TBool lFound = m_pTree->FindL( TreePos, PositionToKey( Position ) );
	if( lFound )
	{
		TEntry Entry;
		m_pTree->ExtractAtL( TreePos, Entry );
		delete Entry.pData;
		m_pTree->DeleteAtL( TreePos );

		m_nCount--;
		ASSERT( m_nCount >= 0 );
	}
}

void CCellMapArray::RemoveAllL()
{
	TBtreeMark i;
	TBool more = m_pTree->ResetL( i );
	TEntry Entry;
	while( more )
	{
		m_pTree->ExtractAtL( i, Entry );
		delete Entry.pData;
		more = m_pTree->NextL( i );
	}
	m_pTree->ClearL();
	m_Range = TRect( TPoint( 1000000, 1000000 ), TPoint( 0, 0 ) );
	m_nCount = 0;
}

TBool CCellMapArray::IsExistL( const TPoint& Position ) const
{
	TBtreePos TreePos;
	return m_pTree->FindL( TreePos, PositionToKey( Position ) );
}

CCell* CCellMapArray::GetFirstL( TPoint& Position ) const
{
	if( !m_pTree->FirstL( m_Pos ) )
		return NULL;

	TEntry Entry;
	m_pTree->ExtractAtL( m_Pos, Entry );
	Position = KeyToPosition( Entry.Key );
	return Entry.pData;
}

CCell* CCellMapArray::GetNextL( TPoint& Position ) const
{
	if( !m_pTree->NextL( m_Pos ) )
		return NULL;

	TEntry Entry;
	m_pTree->ExtractAtL( m_Pos, Entry );
	Position = KeyToPosition( Entry.Key );
	return Entry.pData;
}

void CCellMapArray::ConstructL()
{
	m_pTree = new ( ELeave ) TBtreeFix<TEntry,TUint32>( EBtreeFast );
	m_pMemPool = CMemPagePool::NewL();
	m_pTree->Connect( m_pMemPool, &m_Key );

	m_Range = TRect( TPoint( 1000000, 1000000 ), TPoint( 0, 0 ) );
}

void CCellMapArray::CPointKey::Between( const TAny* Left,const TAny* Right, TBtreePivot& Pivot ) const
{
	TUint32 left  = *( TUint32* )Left;
	TUint32 right = *( TUint32* )Right;

	TUint32 mid = left + ( right - left )/2;
	Pivot.Copy( (TUint8*)&mid, sizeof(mid) );
}

TInt CCellMapArray::CPointKey::Compare( const TAny* Left, const TAny* Right ) const
{
	TUint32 left = *(TUint32*)Left;
	TUint32 right = *(TUint32*)Right;
	if( left < right ) return -1;
	if( left > right ) return 1;
	return 0;
}

const TAny* CCellMapArray::CPointKey::Key( const TAny* Entry ) const
{
	TEntry* e = (TEntry*)Entry;
	return (TAny*)&(e->Key);
}
