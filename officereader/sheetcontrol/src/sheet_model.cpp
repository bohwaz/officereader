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
#include "sheet_model.h"
#include "cell_array.h"
#include "cell.h"

CSheetModel::CSheetModel():
	m_pCells( NULL ),
	m_sTextToFind( NULL ),
	m_nFindFlags( 0 ),
	m_lSearchFromBegin( ETrue )
{
}

CSheetModel::~CSheetModel()
{
	if( m_pCells )
	{
		delete m_pCells;
		m_pCells = NULL;
	}

	if( m_sTextToFind )
	{
		delete m_sTextToFind;
		m_sTextToFind = NULL;
	}
}

CSheetModel* CSheetModel::NewL()
{
	CSheetModel* pSelf = CSheetModel::NewLC();
	CleanupStack::Pop( pSelf );
	return pSelf;
}

CSheetModel* CSheetModel::NewLC()
{
	CSheetModel* pSelf = new ( ELeave ) CSheetModel;
	CleanupStack::PushL( pSelf );
	pSelf->ConstructL();
	return pSelf;
}

void CSheetModel::ConstructL()
{
	m_pCells = CCellMapArray::NewL();
}

TInt CSheetModel::Count() const
{
	//return not implemented yet
	return -1;
}

void CSheetModel::InsertL( const TPoint& Position, const CCell& Cell )
{
#ifdef SHEET_TRACE_ENABLED
	CSheetTrace::InsertL( Position, Cell );
#endif
	m_pCells->InsertL( Position, Cell );
}

const CCell* CSheetModel::AtL( const TPoint& Position ) const
{
	return m_pCells->AtL( Position );
}

CCell* CSheetModel::AtL( const TPoint& Position )
{
	return m_pCells->AtL( Position );
}

TBool CSheetModel::IsExistL( const TPoint& Position ) const
{
	return m_pCells->IsExistL( Position );
}

void CSheetModel::RemoveL( const TPoint& Position )
{
#ifdef SHEET_TRACE_ENABLED
	CSheetTrace::RemoveL( Position );
#endif
	m_pCells->DeleteL( Position );
}

void CSheetModel::RemoveAllL()
{
#ifdef SHEET_TRACE_ENABLED
	CSheetTrace::RemoveAllL();
#endif
	return m_pCells->RemoveAllL();
}

TBool CSheetModel::FindFirstL( const TDesC& sText, TPoint& Pos, TInt nFlags )
{
	if( &sText != NULL )
	{
		if( ( nFlags == 0 ) ||
			( ( nFlags & enFindExact ) && ( nFlags & enFindSubStr ) ) ||
			( ( nFlags & enFindCompareCase ) && ( nFlags & enFindCompareNoCase ) ) || 
			( ( nFlags & enFindOrderRows ) && ( nFlags & enFindOrderColumns ) && ( nFlags & enFindOrderFast ) ) )
		{
			User::Leave( KErrArgument );
		}

		if( m_sTextToFind )
		{
			delete m_sTextToFind;
			m_sTextToFind = NULL;
		}

		m_sTextToFind = sText.AllocL();
		if( nFlags & enFindCompareNoCase )
			m_sTextToFind->Des().LowerCase();

		m_nFindFlags = nFlags;
	}

	if( m_pCells->Count() == 0 )
		return EFalse;

	m_lSearchFromBegin = ETrue;
	return FindNextL( Pos );
}

TBool CSheetModel::FindNextL( TPoint& Pos )
{
	if( !m_sTextToFind || m_nFindFlags == 0 )
	{
		User::Leave( KErrGeneral );
	}

	if( m_nFindFlags & enFindOrderFast )
	{
		CCell* pCell = NULL;
		
		if( m_lSearchFromBegin == 2 )
		{
			m_lSearchFromBegin = ETrue;
			return EFalse;
		}

		if( m_lSearchFromBegin )
		{
			if( NULL == ( pCell = m_pCells->GetFirstL( Pos ) ) )
				return EFalse;

			m_lSearchFromBegin = EFalse;
		}

		TBool lFounded = EFalse;
		HBufC* sCellText = HBufC::NewMaxL( CCell::KMaxTextRepresentation );
		CleanupStack::PushL( sCellText );

		for(;;)
		{
			if( pCell == NULL )
				if( NULL == ( pCell = m_pCells->GetNextL( Pos ) ) )
				{
					m_lSearchFromBegin = ETrue;
					break;
				}

			if( pCell->MaxRepresentationLengthL() > sCellText->Des().MaxLength() )
			{
				CleanupStack::PopAndDestroy( sCellText );
				sCellText = NULL;

				sCellText = HBufC::NewMaxL( pCell->MaxRepresentationLengthL() );
				CleanupStack::PushL( sCellText );
			}

			TPtr sText = sCellText->Des();
			pCell->FormatL( sText );

			if( m_nFindFlags & enFindCompareNoCase )
				sText.LowerCase();

			if( m_nFindFlags & enFindExact )
			{
				if( (*sCellText) == *(m_sTextToFind) )
				{
					lFounded = ETrue;
					break;
				}
			}
			else if( m_nFindFlags & enFindSubStr )
			{
				if( KErrNotFound != sCellText->Find( *m_sTextToFind ) )
				{
					lFounded = ETrue;
					break;
				}
			}
			else
			{
				User::Leave( KErrArgument );
			}
			pCell = NULL;
		}

		if( lFounded & Pos == m_pCells->Range().iBr )
			m_lSearchFromBegin = 2;

		CleanupStack::PopAndDestroy( sCellText );
		return lFounded;
	}
	else if( ( m_nFindFlags & enFindOrderRows ) || 
	         ( m_nFindFlags & enFindOrderColumns ) )
	{
		if( m_lSearchFromBegin == 2 )
		{
			m_lSearchFromBegin = ETrue;
			return EFalse;
		}

		TBool lFounded = EFalse;
		HBufC* sCellText = HBufC::NewMaxL( CCell::KMaxTextRepresentation );
		CleanupStack::PushL( sCellText );

		TRect rRange = m_pCells->Range();

		TInt xBegin = ( m_nFindFlags & enFindOrderRows ) ? rRange.iTl.iX : rRange.iTl.iY;
		TInt xEnd = ( m_nFindFlags & enFindOrderRows ) ? rRange.iBr.iX : rRange.iBr.iY;

		TInt yBegin = ( m_nFindFlags & enFindOrderRows ) ? rRange.iTl.iY : rRange.iTl.iX;
		TInt yEnd = ( m_nFindFlags & enFindOrderRows ) ? rRange.iBr.iY : rRange.iBr.iX;

		if( m_lSearchFromBegin )
		{
			m_OrderSearchPosition = TPoint( 0, 0 );
			m_lSearchFromBegin = EFalse;
		}
		else
		{
			xBegin = ( m_nFindFlags & enFindOrderRows ) ? m_OrderSearchPosition.iX : m_OrderSearchPosition.iY;
			yBegin = ( m_nFindFlags & enFindOrderRows ) ? m_OrderSearchPosition.iY : m_OrderSearchPosition.iX;
		}

		TInt y = yBegin;
		TInt x = xBegin;
		for( y = yBegin; y <= yEnd; y++ )
		{
			if( lFounded )
				break;

			for( x = xBegin; x <= xEnd; x++ )
			{
				CCell* pCell = NULL;
				m_OrderSearchPosition = TPoint( 
					( m_nFindFlags & enFindOrderRows ) ? x : y , 
					( m_nFindFlags & enFindOrderRows ) ? y : x );

				if( NULL == ( pCell = AtL( m_OrderSearchPosition ) ) )
					continue;

				if( pCell->MaxRepresentationLengthL() > sCellText->Des().MaxLength() )
				{
					CleanupStack::PopAndDestroy( sCellText );
					sCellText = NULL;

					sCellText = HBufC::NewMaxL( pCell->MaxRepresentationLengthL() );
					CleanupStack::PushL( sCellText );
				}

				TPtr sText = sCellText->Des();
				pCell->FormatL( sText );

				if( m_nFindFlags & enFindCompareNoCase )
					sText.LowerCase();

				if( m_nFindFlags & enFindExact )
				{
					if( (*sCellText) == *(m_sTextToFind) )
						lFounded = ETrue;
				}
				else if( m_nFindFlags & enFindSubStr )
				{
					if( KErrNotFound != sCellText->Find( *m_sTextToFind ) )
						lFounded = ETrue;
				}
				else
				{
					User::Leave( KErrArgument );
				}

				if( x == xEnd )
					xBegin = 0;

				if( lFounded )
				{
					if( m_nFindFlags & enFindOrderRows )
					{
						if( m_OrderSearchPosition.iX < rRange.iBr.iX )
							m_OrderSearchPosition.iX++;
						else
						{
							if( m_OrderSearchPosition != rRange.iBr )
							{
								m_OrderSearchPosition.iX = 0;
								m_OrderSearchPosition.iY++;
							}
						}
					}
					else
					{
						if( m_OrderSearchPosition.iY < rRange.iBr.iY )
							m_OrderSearchPosition.iY++;
						else
						{
							if( m_OrderSearchPosition != rRange.iBr )
							{
								m_OrderSearchPosition.iX++;
								m_OrderSearchPosition.iY = 0;
							}
						}
					}

					Pos = TPoint( ( m_nFindFlags & enFindOrderRows ) ? x : y, 
					              ( m_nFindFlags & enFindOrderRows ) ? y : x );
					break;
				}
			}
		}

		TPoint Position( ( m_nFindFlags & enFindOrderRows ) ? x : y, 
		                 ( m_nFindFlags & enFindOrderRows ) ? y : x );

		if( m_OrderSearchPosition == rRange.iBr )
		{
			if( lFounded )
				m_lSearchFromBegin = 2;
			else
				m_lSearchFromBegin = ETrue;
		}

		CleanupStack::PopAndDestroy( sCellText );
		return lFounded;
	}
	else
	{
		ASSERT( FALSE );
	}
	return	EFalse;
}

void CSheetModel::ColumnTextL( TInt nIndex, TDes& sText ) const
{
	sText.Zero();

	if( nIndex > KLettersNumber - 1 )
		sText.Append( 'A' - 1 + nIndex / KLettersNumber );

	sText.Append( 'A' + nIndex % KLettersNumber );
}

void CSheetModel::RowTextL( TInt nIndex, TDes& sText  ) const
{
	//sText.Num( nIndex );
	/*
		>please start from A:1 instead of A:2
	*/
	sText.Num( nIndex + 1 );
}

TBool CSheetModel::IsPositionInGoodRange( const TPoint& Point )
{
	return ( Point.iX < ( KLettersNumber + 1 )*( KLettersNumber ) ) && ( Point.iY >= 0 );
}
