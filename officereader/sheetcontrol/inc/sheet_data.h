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

#ifndef __SHEET_DATA_H__
#define __SHEET_DATA_H__

#include <gdi.h>
#include <s32btree.h>


class CSheetModel;

/**
*	stores sheet properties: colors, fonts, sizes, etc.
*/

class CSheetData : public CBase
{
private:
	CSheetData();

public:
	virtual ~CSheetData();

	static CSheetData* NewL();
	static CSheetData* NewLC();

	void SetModel( CSheetModel* pModel ) { m_pModel = pModel; }

	//sheet properties
	enum ELongTextMode
	{
		enLeftMost = 0,
		enRightMost,
		enOverNeighbour
	};

	ELongTextMode LongTextShowMode() const { return m_enLongTextMode; }
	void SetLongTextShowMode( ELongTextMode enMode ) { m_enLongTextMode = enMode;}
	
	TInt CursorWidthInPixels() const { return m_nCursorWidthInPixels; }
	void SetCursorWidthInPixels( TInt nWidth ) { m_nCursorWidthInPixels = nWidth; }

	TRgb BackgroundColor() const { return m_BackgroundColor; }
	void SetBackgroundColor( TRgb Color ) { m_BackgroundColor = Color; }

	TRgb GridColor() const { return m_GridColor; }
	void SetGridColor( TRgb Color ) { m_GridColor = Color; }

	TRgb CursorColor() const { return m_CursorColor; }
	void SetCursorColor( TRgb Color ){ m_CursorColor = Color; }

	//column header properties
	TRgb ColumnHeaderColor() const { return m_ColumnHeaderColor; }
	void SetColumnHeaderColor( TRgb Color ) { m_ColumnHeaderColor = Color; }

	TRgb ColumnHeaderTextColor() const { return m_ColumnHeaderTextColor; }
	void SetColumnHeaderTextColor( TRgb Color ) { m_ColumnHeaderTextColor = Color; }

	TInt ColumnHeaderWidthL( TInt nIndex ) const;
	void SetColumnHeaderWidthL( TInt nIndex, TInt nWidth );

	TInt ColumnHeaderHeight() const;
	void SetColumnHeaderHeight( TInt nHeight );

	const CFont* ColumnHeaderFont() const { return m_pColumnHeaderFont; }
	void SetColumnHeaderFont( const CFont* pFont ) { m_pColumnHeaderFont = pFont; }

	//row header properties
	TRgb RowHeaderColor() const { return m_RowHeaderColor; }
	void SetRowHeaderColor( TRgb Color ) { m_RowHeaderColor = Color; }

	TRgb RowHeaderTextColor() const { return m_RowHeaderTextColor; }
	void SetRowHeaderTextColor( TRgb Color ) { m_RowHeaderTextColor = Color; }

	TInt RowHeaderHeightL( TInt nIndex ) const;
	void SetRowHeaderHeightL( TInt nIndex, TInt nHeight );

	TInt RowHeaderWidth() const;
	void SetRowHeaderWidth( TInt nWidth );

	const CFont* RowHeaderFont() const { return m_pRowHeaderFont; }
	void SetRowHeaderFont( const CFont* pFont ){ m_pRowHeaderFont = pFont; }

	//cell properties

	/*
	*  This method frees all resources relevant to individual cell size
	*  and sets default cell size. 
	*  You should use CSheetView::ResetCellSizeL( const TSize& ).
	*  and then User::Heap().Compress();
	*/
	void ResetCellSizeL( const TSize& Size );

	TSize CellSizeL( const TPoint& Position ) const;
	void SetCellSizeL( const TPoint& Position, const TSize& Size );
	const CFont* CellFont() const { return m_pCellFont; }
	void SetCellFont( const CFont* pFont ){ m_pCellFont = pFont; }

	TRgb CellTextColor() const { return m_CellTextColor; }
	void SetCellTextColor( const TRgb& Color ){ m_CellTextColor = Color; }
	TRgb CellTextColorL( const TPoint& Position ) const;
	void SetCellTextColorL( const TPoint& Position, const TRgb& Color );

	TRgb CellColor() const { return m_BackgroundColor; }
	void SetCellColor( const TRgb& Color ){ m_BackgroundColor = Color; }
	TRgb CellColorL( const TPoint& Position ) const;
	void SetCellColorL( const TPoint& Position, const TRgb& Color );

	CGraphicsContext::TTextAlign CellAlignmentL( const TPoint& Position ) const;
	void SetCellAlignmentL( const TPoint& Position, CGraphicsContext::TTextAlign Alignment );

//	TMargins CellMarginsL( const TPoint& Position ) const;
//	void SetCellMarginsL( const TPoint& Position, TMargins Margins );

	//item wnd properties
	const CFont* ItemWndFont() const { return m_pItemWndFont; }
	void SetItemWndFont( const CFont* pFont ){ m_pItemWndFont = pFont; }

	TRgb ItemWndColor() const { return m_ItemWndColor; }
	void SetItemWndColor( TRgb Color ){ m_ItemWndColor = Color; }

	TRgb ItemWndTextColor() const { return m_ItemWndTextColor; }
	void SetItemWndTextColor( TRgb Color ){ m_ItemWndTextColor = Color; }

	TMargins ItemWndMargins() const { return m_ItemWndMargins; }
	void SetItemWndMargins( const TMargins& Margins ){ m_ItemWndMargins = Margins; }

	void UpdateFonts();

	TInt ZoomedValue( TInt nValue ) const;

private:
	void ConstructL();
	void ReleaseFonts();

	CSheetModel* m_pModel;

	//sheet properties
	ELongTextMode m_enLongTextMode;
	TInt m_nCursorWidthInPixels;

	TRgb m_BackgroundColor;
	TRgb m_GridColor;
	TRgb m_CursorColor;

	//column header properties
	TRgb  m_ColumnHeaderColor;
	TRgb  m_ColumnHeaderTextColor;
	mutable TSize m_ColumnHeaderSize;
	const CFont* m_pColumnHeaderFont;

	//row header properties
	TRgb  m_RowHeaderColor;
	TRgb  m_RowHeaderTextColor;
	mutable TSize m_RowHeaderSize;
	const CFont* m_pRowHeaderFont;

	//cell properties
	const CFont* m_pCellFont;
	TRgb m_CellTextColor;

	//item wnd properties
	const CFont* m_pItemWndFont;
	TRgb m_ItemWndColor;
	TRgb m_ItemWndTextColor;
	TMargins m_ItemWndMargins;

	//zoom properties
	TInt m_nZoomDivident;
	TInt m_nZoomDivisor;

	template<class _Value>
	class CCellAttribute : public CBase
	{
	private:
		CCellAttribute();

	public:
		virtual ~CCellAttribute();

		static CCellAttribute* NewL();
		static CCellAttribute* NewLC();

		void InsertL( TInt nPosition, const _Value& nValue );
		TBool AtL( TInt nPosition, _Value& nValue ) const;
		void DeleteL( TInt nPosition );
		void RemoveAllL();
		static TUint32 PositionToKey( const TPoint& Position );

	private:
		void ConstructL();

		struct TEntry
		{
			TInt	Key;
			_Value	Data;
		};

		class CPointKey : public MBtreeKey
		{
		public:
			void Between( const TAny* Left, const TAny* Right, TBtreePivot& Pivot ) const;
			TInt Compare( const TAny* Left, const TAny* Right ) const;
			const TAny* Key( const TAny* Entry ) const;
		};

		TBtreeFix<TEntry,TInt>* m_pTree;
		CPointKey     m_Key;
		CMemPagePool* m_pMemPool;
	};

	typedef CCellAttribute<TInt> CCellWidth;
	typedef CCellAttribute<TInt> CCellHeight;

	CCellWidth*  m_pColumnSizes;
	CCellHeight* m_pRowSizes;

	enum EFont 
	{
		enAnnotation = 0,
		enNormal,
		enTitle
	};

	void UpdateFontsSize( EFont enFont );
	EFont m_enFont;

	friend class CSheetView;
};

template<class _Value>
CSheetData::CCellAttribute<_Value>::CCellAttribute():
	m_pTree( NULL ),
	m_pMemPool( NULL )
{
}

template<class _Value>
CSheetData::CCellAttribute<_Value>::~CCellAttribute()
{
	delete m_pTree;
	delete m_pMemPool;
}

template<class _Value>
CSheetData::CCellAttribute<_Value>* CSheetData::CCellAttribute<_Value>::NewL()
{
	CCellAttribute<_Value>* pSelf = CCellAttribute<_Value>::NewLC();
	CleanupStack::Pop( pSelf );
	return pSelf;
}

template<class _Value>
CSheetData::CCellAttribute<_Value>* CSheetData::CCellAttribute<_Value>::NewLC()
{
	CCellAttribute<_Value>* pSelf = new ( ELeave ) CCellAttribute<_Value>;
	CleanupStack::PushL( pSelf );
	pSelf->ConstructL();
	return pSelf;
}

template<class _Value>
void CSheetData::CCellAttribute<_Value>::InsertL( TInt nPosition, const _Value& Value )
{
	TBtreePos TreePos;
	TEntry Entry;
	Entry.Key = nPosition; 
	Entry.Data = Value;
	/*TBool no_dup = */m_pTree->InsertL( TreePos, Entry, EAllowDuplicates );
}

template<class _Value>
TBool CSheetData::CCellAttribute<_Value>::AtL( TInt nPosition, _Value& Value ) const
{
	TBtreePos TreePos;
	TBool lFound = m_pTree->FindL( TreePos, nPosition );
	if( lFound )
	{
		TEntry Entry;
		m_pTree->ExtractAtL( TreePos, Entry );
		Value = Entry.Data;
		return ETrue;
	}
	else
	{
		return EFalse;
	}
}

template<class _Value>
void CSheetData::CCellAttribute<_Value>::DeleteL( TInt nPosition )
{
	TBtreePos TreePos;
	TBool lFound = m_pTree->FindL( TreePos, nPosition );
	if( lFound )
	{
		m_pTree->DeleteAtL( TreePos );
	}
}

template<class _Value>
void CSheetData::CCellAttribute<_Value>::RemoveAllL()
{
	m_pTree->ClearL();
}

template<class _Value>
void CSheetData::CCellAttribute<_Value>::ConstructL()
{
	m_pTree = new ( ELeave ) TBtreeFix<TEntry,TInt>( EBtreeFast );
	m_pMemPool = CMemPagePool::NewL();
	m_pTree->Connect( m_pMemPool, &m_Key );
}

template<class _Value>
void CSheetData::CCellAttribute<_Value>::CPointKey::Between( const TAny* Left,const TAny* Right, TBtreePivot& Pivot ) const
{
	TInt left  = *( TInt* )Left;
	TInt right = *( TInt* )Right;

	TInt mid = left + ( right - left )/2;
	Pivot.Copy( (TUint8*)&mid, sizeof(mid) );
}

template<class _Value>
TInt CSheetData::CCellAttribute<_Value>::CPointKey::Compare( const TAny* Left, const TAny* Right ) const
{
	TInt left = *(TInt*)Left;
	TInt right = *(TInt*)Right;
	if( left < right ) return -1;
	if( left > right ) return 1;
	return 0;
}

template<class _Value>
const TAny* CSheetData::CCellAttribute<_Value>::CPointKey::Key( const TAny* Entry ) const
{
	TEntry* e = (TEntry*)Entry;
	return (TAny*)&(e->Key);
}

template<class _Value>
inline TUint32 CSheetData::CCellAttribute<_Value>::PositionToKey( const TPoint& Position )
{
	// convert TPoint to TUint32 ro reduce memory usage & increase speed
	TUint32 Res = ( ( Position.iX & 0x0000ffff ) << 16 ) | ( Position.iY & 0x0000ffff );
	return Res;
}

#endif
