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

#include "sheet_drawer.h"
#include "sheet_data.h"
#include "sheet_model.h"
#include "sheet_view.h"

CSheetDrawer::CSheetDrawer( CSheetModel* pModel, CSheetData* pData ):
	m_pModel( pModel ),
	m_pData( pData ),
	m_pView( NULL ),
	m_pSheetGc( NULL ),
	m_pContainerWnd( NULL ),
	m_pColumnWnd( NULL ),
	m_pRowWnd( NULL ),
	m_pSheetWnd( NULL ),
	m_pCellViewWnd( NULL )
{
}

CSheetDrawer::~CSheetDrawer()
{
}

CSheetDrawer* CSheetDrawer::NewL( CSheetModel* pModel, CSheetData* pData )
{
	CSheetDrawer* pSelf = CSheetDrawer::NewLC( pModel, pData );
	CleanupStack::Pop( pSelf );
	return pSelf;
}

CSheetDrawer* CSheetDrawer::NewLC( CSheetModel* pModel, CSheetData* pData )
{
	CSheetDrawer* pSelf = new ( ELeave ) CSheetDrawer( pModel, pData );
	CleanupStack::PushL( pSelf );
	pSelf->ConstructL();
	return pSelf;
}

void CSheetDrawer::ConstructL()
{
}

void CSheetDrawer::SetWindows( CCoeControl* pContainerWnd, CSheetChildWindow* pColumnWnd,
	CSheetChildWindow* pRowWnd, CSheetChildWindow* pSheetWnd, 
	CSheetChildWindow* pCellViewWnd )
{
	m_pContainerWnd = pContainerWnd;
	m_pColumnWnd = pColumnWnd;
	m_pRowWnd = pRowWnd;
	m_pSheetWnd = pSheetWnd;
	m_pCellViewWnd = pCellViewWnd;
}

void CSheetDrawer::MakeCellRepresentationL( const CCell& Cell ) const
{
	Cell.FormatL( m_sCellTextRepresentation );
}

TPoint CSheetDrawer::CellbyPositionL( const TPoint& Position) const
{
	TPoint Pos = m_pView->TopLeftCell();

	TPoint PositionInPixels( 1, 1 );
	while( PositionInPixels.iX <= Position.iX )
	{
		PositionInPixels.iX += m_pData->CellSizeL( Pos ).iWidth + 1 /* 1 - grid line width */;
		Pos.iX++;
	}

	while( PositionInPixels.iY <= Position.iY )
	{
		PositionInPixels.iY += m_pData->CellSizeL( Pos ).iHeight + 1 /* 1 - grid line width */;
		Pos.iY++;
	}

	return Pos-TPoint(1,1);
}



TPoint CSheetDrawer::CellPositionL( const TPoint& Position ) const
{
	TPoint Pos = m_pView->TopLeftCell();
	if( Position.iX < Pos.iX || Position.iY < Pos.iY )
		User::Leave( KErrArgument );

	TPoint PositionInPixels( 1, 1 );
	while( Pos.iX != Position.iX )
	{
		PositionInPixels.iX += m_pData->CellSizeL( Pos ).iWidth + 1 /* 1 - grid line width */;
		Pos.iX++;
	}

	while( Pos.iY != Position.iY )
	{
		PositionInPixels.iY += m_pData->CellSizeL( Pos ).iHeight + 1 /* 1 - grid line width */;
		Pos.iY++;
	}

	return PositionInPixels;
}

void CSheetDrawer::Invalidate() const
{
	//m_pContainerWnd->DrawNow();
	m_pColumnWnd->DrawNow();
	m_pRowWnd->DrawNow();
	m_pSheetWnd->DrawNow();
	m_pCellViewWnd->DrawNow();
}

void CSheetDrawer::InvalidateCellView() const
{
	m_pCellViewWnd->DrawNow();
}

void CSheetDrawer::DrawColumnWnd( CWindowGc& gc ) const
{
	gc.SetBrushColor( m_pData->ColumnHeaderColor() );
	gc.Clear( m_pColumnWnd->Rect() );

	gc.UseFont( m_pData->ColumnHeaderFont() );

	TBuf<32> sText;
	TPoint CurPos( 0, 0 );
	TInt nCellIndex = 0;
	TInt nBaseLine = m_pData->ColumnHeaderFont()->HeightInPixels() + 
		( m_pColumnWnd->Size().iHeight - m_pData->ColumnHeaderFont()->HeightInPixels() ) / 2;

	while( CurPos.iX < m_pColumnWnd->Size().iWidth )
	{
		//TSize ColumnSize = m_pData->ColumnHeaderSize( m_pView->TopLeftCell().iX + nCellIndex );
		TSize ColumnSize( m_pData->ColumnHeaderWidthL( m_pView->TopLeftCell().iX + nCellIndex ),
			m_pData->ColumnHeaderHeight() );

		TPoint EndPos( CurPos.iX, CurPos.iY + ColumnSize.iHeight );

		gc.SetPenColor( m_pData->GridColor() );
		gc.DrawLine( CurPos, EndPos );
		
		m_pModel->ColumnTextL( m_pView->TopLeftCell().iX + nCellIndex, sText );
		gc.SetPenColor( m_pData->ColumnHeaderTextColor() );
		gc.DrawText( sText, TRect( CurPos, TPoint( EndPos.iX + ColumnSize.iWidth, EndPos.iY ) ), nBaseLine, CGraphicsContext::ECenter );

		CurPos.iX += ColumnSize.iWidth + 1 /* 1 - grid line width */ ;
		nCellIndex++;
	}
}

void CSheetDrawer::DrawRowWnd( CWindowGc& gc ) const
{
	gc.SetBrushColor( m_pData->RowHeaderColor() );
	gc.Clear( m_pRowWnd->Rect() );

	gc.UseFont( m_pData->RowHeaderFont() );

	TBuf<32> sText;
	TPoint CurPos( 0, 0 );
	TInt nCellIndex = 0;


	while( CurPos.iY < m_pRowWnd->Size().iHeight )
	{
		TSize RowSize( m_pData->RowHeaderWidth(), 
			m_pData->RowHeaderHeightL( m_pView->TopLeftCell().iY + nCellIndex ) );

		TInt nBaseLine = m_pData->RowHeaderFont()->HeightInPixels() + 
			( RowSize.iHeight - m_pData->RowHeaderFont()->HeightInPixels() ) / 2;

		TPoint EndPos( CurPos.iX + RowSize.iWidth, CurPos.iY );
		gc.SetPenColor( m_pData->GridColor() );
		gc.DrawLine( CurPos, EndPos );

		m_pModel->RowTextL( m_pView->TopLeftCell().iY + nCellIndex, sText );
		gc.SetPenColor( m_pData->RowHeaderTextColor() );
		gc.DrawText( sText, TRect( CurPos, TPoint( EndPos.iX, EndPos.iY + RowSize.iHeight) ), nBaseLine, CGraphicsContext::ECenter );

		CurPos.iY += RowSize.iHeight + 1 /* 1 - grid line width */ ;
		nCellIndex++;
	}
}

void CSheetDrawer::DrawSheetWnd( CWindowGc& gc ) const
{
	gc.SetBrushColor( m_pData->BackgroundColor() );
	gc.Clear( m_pSheetWnd->Rect() );

	//draw grid
	{
		TInt nCellIndex = 0;
		TPoint CurPos( 0, 0 );
		gc.SetPenColor( m_pData->GridColor() );
		while( CurPos.iX < m_pSheetWnd->Size().iWidth )
		{
			gc.DrawLine( CurPos, TPoint( CurPos.iX,
				CurPos.iY + m_pSheetWnd->Size().iHeight ) );

			CurPos.iX += m_pData->ColumnHeaderWidthL( m_pView->TopLeftCell().iX + nCellIndex ) 
				+ 1 /* 1 - grid line width */ ;

			nCellIndex++;
		}

		nCellIndex = 0;
		CurPos = TPoint( 0, 0 );
		gc.SetPenColor( m_pData->GridColor() );
		while( CurPos.iY < m_pSheetWnd->Size().iHeight )
		{
			gc.DrawLine( CurPos, TPoint( CurPos.iX + m_pSheetWnd->Size().iWidth,
				CurPos.iY ) );

			CurPos.iY += m_pData->RowHeaderHeightL( m_pView->TopLeftCell().iY + nCellIndex )
				+ 1 /* 1 - grid line width */ ;
			nCellIndex++;
		}
	}

	//draw all cells
   	if( m_pData->LongTextShowMode() == CSheetData::enOverNeighbour )
	{
		TPoint CellNumber( NumberOfCellsThatFitInRectL() );
		CellNumber.iX++; /* 1 - to draw partially visible cell */
		CellNumber.iY++; /* 1 - to draw partially visible cell */

		DrawItems2( m_pView->TopLeftCell(), m_pView->TopLeftCell() + CellNumber, EFalse );
	}
	else
	{
		TPoint CellNumber = NumberOfCellsThatFitInRectL();

		CellNumber.iX++; /* 1 - to draw partially visible cell */
		CellNumber.iY++; /* 1 - to draw partially visible cell */

		for( TInt x = 0; x < CellNumber.iX; x++ )
			for( TInt y = 0; y < CellNumber.iY; y++ )
			{
				TPoint CellPosition( x + m_pView->TopLeftCell().iX, y + m_pView->TopLeftCell().iY );
				TBool lCurrent = ( CellPosition == m_pView->CurrentCell() );
				DrawItem( CellPosition, lCurrent, EFalse /* fixme - item can be selected */, EFalse );
			}
	}
}

void CSheetDrawer::DrawItemViewWnd( CWindowGc& gc ) const
{
	gc.SetBrushColor( m_pData->ItemWndColor() );
	gc.Clear( m_pCellViewWnd->Rect() );

	CCell* pCell = NULL;
	if( NULL != ( pCell = m_pModel->AtL( m_pView->CurrentCell() ) ) )
	{
		MakeCellRepresentationL( *pCell );
		gc.UseFont( m_pData->ItemWndFont() );

		TInt nBaseLine = m_pData->ItemWndFont()->HeightInPixels() + 
			( m_pCellViewWnd->Size().iHeight - m_pData->ItemWndFont()->HeightInPixels() ) / 2;

		TRect r( m_pCellViewWnd->Rect() );
		TMargins m = m_pData->ItemWndMargins();
		r.iTl.iY += m.iTop;
		r.iTl.iX += m.iLeft;
		r.iBr.iY -= m.iBottom;
		r.iBr.iX -= m.iRight;

		gc.DrawText( m_sCellTextRepresentation, r, nBaseLine, CGraphicsContext::ELeft );
	}
}

void CSheetDrawer::DrawItems( const TPoint& Old, const TPoint& New ) const
{
	if( m_pData->LongTextShowMode() == CSheetData::enOverNeighbour )
	{
		TPoint CellNumber( NumberOfCellsThatFitInRectL() );
		CellNumber.iX++; /* 1 - to draw partially visible cell */
		CellNumber.iY++; /* 1 - to draw partially visible cell */
		
		TPoint Begin( m_pView->TopLeftCell().iX, Old.iY < New.iY ? Old.iY : New.iY );
		TPoint End( m_pView->TopLeftCell().iX + CellNumber.iX, New.iY > Old.iY ? New.iY : Old.iY );
		DrawItems2( Begin, End, ETrue );
	}
	else
	{
		DrawItem( Old, EFalse, EFalse /* fixme - it can be selected item*/ );
		DrawItem( New, ETrue, EFalse /* fixme - it can be selected item*/ );
	}
}

void CSheetDrawer::DrawItem( const TPoint& Position, TBool lItemIsCurrent, TBool /* lItemIsSelected */, TBool lActivate ) const
{
	TPoint CellPosition = CellPositionL( Position );
	TSize CellSize = m_pData->CellSizeL( Position );

	if( lActivate )
		Gc().Activate( m_pSheetWnd->Window() );

	TRgb CellColor( m_pData->CellColor() );

	CCell* pCell = NULL;
	if( NULL != ( pCell = m_pModel->AtL( Position ) ) &&
		pCell->m_MetaData.m_Color != KDefaultCellColor )
	{
		CellColor = pCell->m_MetaData.m_Color;
	}

	if( lItemIsCurrent )
	{
		Gc().SetBrushColor( CellColor );
		Gc().Clear( TRect( CellPosition, CellSize ) );

		//draw cursor
		Gc().SetPenColor( m_pData->CursorColor() );

		TPoint CurPosition( CellPosition.iX - 1, CellPosition.iY - 1 );
		TSize  CurSize( CellSize.iWidth + 2, CellSize.iHeight + 2 );

		for( TInt i = 0; i < m_pData->CursorWidthInPixels(); i++ )
		{
			Gc().DrawRect( TRect( CurPosition, CurSize ) );
			CurPosition.iX++;
			CurPosition.iY++;
			CurSize.iWidth -= 2;
			CurSize.iHeight -= 2;
		}
	}
	else
	{
		Gc().SetBrushColor( CellColor );
		Gc().Clear( TRect( CellPosition, CellSize ) );

		//draw recrangle around cell
		{
			Gc().SetPenColor( m_pData->GridColor() );
			Gc().DrawRect( TRect( TPoint( CellPosition.iX - 1, CellPosition.iY - 1 ),
				TSize( CellSize.iWidth + 2, CellSize.iHeight + 2 ) ) );
		}
	}

	//draw cell text
	if( pCell )
	{
		TRgb CellTextColor( m_pData->CellTextColor() );
		if( pCell->m_MetaData.m_TextColor != KDefaultCellTextColor )
			CellTextColor = pCell->m_MetaData.m_TextColor;

		MakeCellRepresentationL( *pCell );
		Gc().UseFont( m_pData->CellFont() );
		Gc().SetPenColor( CellTextColor );

		TInt nBaseLine = m_pData->CellFont()->HeightInPixels() + 
			( CellSize.iHeight - m_pData->CellFont()->HeightInPixels() ) / 2;

		//	nBaseLine, pCell->Align() );
		if( m_pData->LongTextShowMode() == CSheetData::enLeftMost )
		{
			//left most text part
			TInt nTextCount = m_pData->CellFont()->TextCount( m_sCellTextRepresentation, CellSize.iWidth );

			//if text longer than cell width - align to left, like in MS Excel
			Gc().DrawText( TPtrC( m_sCellTextRepresentation.Ptr(), nTextCount ) , TRect( CellPosition, CellSize ), 
				nBaseLine, pCell->Align() );
		}
		else
		{
			Gc().DrawText( m_sCellTextRepresentation, TRect( CellPosition, CellSize ), 
				nBaseLine, pCell->Align() );
		}
	}

	if( lActivate )
		Gc().Deactivate();
}

TPoint CSheetDrawer::NumberOfCellsThatFitInRectL() const
{
	TPoint Cell = m_pView->TopLeftCell();
	TSize SheetWndSize( m_pSheetWnd->Size() );

	TPoint NumberOfCells( 0, 0 );
	TSize Size( m_pData->CellSizeL( Cell ) );
	Size.iWidth += 1; /* 1 - grid line width */
	while( Size.iWidth < SheetWndSize.iWidth )
	{
		NumberOfCells.iX++;
		Cell.iX++;
		Size.iWidth += m_pData->CellSizeL( Cell ).iWidth + 1 /* 1 - grid line width */;
	}

	Size.iHeight += 1; /* 1 - grid line width */
	while( Size.iHeight < SheetWndSize.iHeight )
	{
		NumberOfCells.iY++;
		Cell.iY++;
		Size.iHeight += m_pData->CellSizeL( Cell ).iHeight + 1 /* 1 - grid line width */;
	}

	return NumberOfCells;
}

void CSheetDrawer::DrawItems2( const TPoint& Begin, const TPoint& End, TBool lActivate ) const
{

	if( lActivate )
		Gc().Activate( m_pSheetWnd->Window() );

	for( TInt y = Begin.iY; y <= End.iY; y++ )
		for( TInt x = Begin.iX; x < End.iX; x++ )
		{
			TPoint Position( x, y );
			CCell* pCell = NULL;

			if( NULL != ( pCell = m_pModel->AtL( Position ) ) )
			{
				TPoint CellPosition = CellPositionL( Position );
				TSize CellSize = m_pData->CellSizeL( Position );
				TSize TotalSize( CellSize );

				TBool lDrawCursor = EFalse;
				TPoint CursorPosition( 0, 0 );
				if( Position == m_pView->CurrentCell() )
				{
					lDrawCursor = ETrue;
					CursorPosition = Position;
				}


				MakeCellRepresentationL( *pCell );
				TInt nTextCount = m_pData->CellFont()->TextCount( m_sCellTextRepresentation, CellSize.iWidth );
				if( nTextCount < m_sCellTextRepresentation.Length() )
				{
					for( TInt x2 = x + 1; x2 < End.iX/* - 1*/; x2++ )
					{
						TPoint SubPosition( x2, y );
						if( SubPosition == m_pView->CurrentCell() )
						{
							lDrawCursor = ETrue;
							CursorPosition = SubPosition;
						}

						if( ( m_pModel->IsExistL( SubPosition ) ) || 
						    ( m_pData->CellFont()->TextCount( m_sCellTextRepresentation, TotalSize.iWidth ) == 
							m_sCellTextRepresentation.Length() ) )
						{
							break;
						}
						x++;
						TotalSize.iWidth += m_pData->CellSizeL( SubPosition ).iWidth +
							1 /* 1 - grid line width */;
					}
				}

				//draw cells
				{
					//clear
					{
						TRgb CellColor( pCell->m_MetaData.m_Color == KDefaultCellColor ? 
							m_pData->CellColor() : pCell->m_MetaData.m_Color );

						Gc().SetBrushColor( CellColor );
						Gc().Clear( TRect( CellPosition, TotalSize ) );
					}

					//draw recrangle around cell
					{
						Gc().SetPenColor( m_pData->GridColor() );
						Gc().DrawRect( TRect( TPoint( CellPosition.iX - 1, CellPosition.iY - 1 ),
							TSize( TotalSize.iWidth + 2, TotalSize.iHeight + 2 ) ) );
					}

					if( lDrawCursor )
					{
						//Gc().SetBrushColor( CellColor );
						//Gc().Clear( TRect( CellPosition, CellSize ) );

						//draw cursor
						Gc().SetPenColor( m_pData->CursorColor() );

						TPoint CurPosition( CellPositionL( CursorPosition ) );
						CurPosition.iX -= 1;
						CurPosition.iY -= 1;
						
						TSize CurSize( m_pData->CellSizeL( CursorPosition ) );
						CurSize.iWidth += 2;
						CurSize.iHeight += 2;

						for( TInt i = 0; i < m_pData->CursorWidthInPixels(); i++ )
						{
							Gc().DrawRect( TRect( CurPosition, CurSize ) );
							CurPosition.iX++;
							CurPosition.iY++;
							CurSize.iWidth -= 2;
							CurSize.iHeight -= 2;
						}
					}
					else
					{
						//Gc().SetBrushColor( CellColor );
						//Gc().Clear( TRect( CellPosition, TotalSize ) );

						//draw recrangle around cell
						{
							Gc().SetPenColor( m_pData->GridColor() );
							Gc().DrawRect( TRect( TPoint( CellPosition.iX - 1, CellPosition.iY - 1 ),
								TSize( TotalSize.iWidth + 2, TotalSize.iHeight + 2 ) ) );
						}
					}

					//draw cell text
					if( pCell )
					{
						TRgb CellTextColor( pCell->m_MetaData.m_TextColor == KDefaultCellTextColor ? 
							m_pData->CellTextColor() : pCell->m_MetaData.m_TextColor );

						Gc().UseFont( m_pData->CellFont() );
						Gc().SetPenColor( CellTextColor );

						TInt nBaseLine = m_pData->CellFont()->HeightInPixels() + 
							( CellSize.iHeight - m_pData->CellFont()->HeightInPixels() ) / 2;

						//if text longer than cell width - align to left, like in MS Excel
						Gc().DrawText( m_sCellTextRepresentation, TRect( CellPosition, TotalSize ), 
							nBaseLine, ( ( CellSize.iWidth < TotalSize.iWidth ) || 
							( nTextCount < m_sCellTextRepresentation.Length() ) ) ? CGraphicsContext::ELeft : pCell->Align() );
					}
				}
			}
			else
			{
				TBool lCurrent = ( Position == m_pView->CurrentCell() );
				DrawItem( Position, lCurrent, EFalse /* fixme - item can be selected */, EFalse );
			}
		}

	if( lActivate )
		Gc().Deactivate();
}
