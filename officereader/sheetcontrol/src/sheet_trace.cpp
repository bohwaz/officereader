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
#include "cell.h"

#include <f32file.h>
#include <coemain.h>

_LIT( KFile, "c:\\sheet_ctrl_trace.txt" );

void CSheetTrace::InitL()
{
	TUint nAttValue;
	RFs& Fs = CCoeEnv::Static()->FsSession();
	TInt nRes = Fs.Att( KFile, nAttValue );
	if( KErrNone == nRes )
	{
		//delete
		User::LeaveIfError( Fs.Delete( KFile ) );
	}

	//create
	RFile File;
	User::LeaveIfError( File.Create( Fs, KFile, EFileWrite ) );
	File.Close();
}

//CSheetModel

_LIT8( KInsert, "InsertL" );
void CSheetTrace::InsertL( const TPoint& Position, const CCell& Cell )
{
/*
	RFile File;
	User::LeaveIfError( File.Open( CCoeEnv::Static()->FsSession(), KFile, EFileWrite ) );
	TBuf8<512> Buf( KInsert );
	_LIT8( KFormat, " (%d:%d) %d\r\n" );
	Buf.AppendFormat( KFormat, Position.iX, Position.iY, Cell.m_enType );
	File.Write( Buf );
	File.Close();
*/
	RFile File;
	User::LeaveIfError( File.Open( CCoeEnv::Static()->FsSession(), KFile, EFileWrite ) );
	TBuf8<2048> Buf( KInsert );
	_LIT8( KFormat, " (%d:%d) %d %s\r\n" );

	TBuf<2048>* sRepresentation = new ( ELeave ) TBuf<2048>;
	CleanupStack::PushL( sRepresentation );
	Cell.FormatL( *sRepresentation );

	TBuf8<2048>* sRepresentation8 = new ( ELeave ) TBuf8<2048>;
	CleanupStack::PushL( sRepresentation8 );
	sRepresentation8->Copy( *sRepresentation );

	Buf.AppendFormat( KFormat, Position.iX, Position.iY, Cell.m_enType, sRepresentation8->PtrZ() );

	CleanupStack::PopAndDestroy( sRepresentation8 );
	CleanupStack::PopAndDestroy( sRepresentation );

	TInt Pos = 0;
	File.Seek( ESeekEnd , Pos );
	File.Write( Buf );
	File.Flush();
	File.Close();
}

_LIT8( KRemove, "RemoveL" );
void CSheetTrace::RemoveL( const TPoint& Position )
{
	RFile File;
	User::LeaveIfError( File.Open( CCoeEnv::Static()->FsSession(), KFile, EFileWrite ) );
	TBuf8<512> Buf( KRemove );
	_LIT8( KFormat, " (%d:%d)\r\n" );
	Buf.AppendFormat( KFormat, Position.iX, Position.iY );

	TInt Pos = 0;
	File.Seek( ESeekEnd , Pos );
	File.Write( Buf );
	File.Flush();
	File.Close();
}

_LIT8( KRemoveAll, "RemoveAllL" );
void CSheetTrace::RemoveAllL()
{
	RFile File;
	User::LeaveIfError( File.Open( CCoeEnv::Static()->FsSession(), KFile, EFileWrite ) );
	TBuf8<128> Buf( KRemoveAll );
	Buf.Append( _L8("\r\n") );

	TInt Pos = 0;
	File.Seek( ESeekEnd , Pos );
	File.Write( Buf );
	File.Flush();
	File.Close();
}

//CSheetView

_LIT8( KSetCellAjustmentLimits, "SetCellAjustmentLimitsL" );
void CSheetTrace::SetCellAjustmentLimitsL( const TSize& MinSize, const TSize& MaxSize ) 
{
	RFile File;
	User::LeaveIfError( File.Open( CCoeEnv::Static()->FsSession(), KFile, EFileWrite ) );
	TBuf8<128> Buf( KSetCellAjustmentLimits );
	_LIT8( KFormat, " %d:%d %d:%d\r\n" );
	Buf.AppendFormat( KFormat, MinSize.iWidth, MinSize.iHeight, MaxSize.iWidth, MaxSize.iHeight );

	TInt Pos = 0;
	File.Seek( ESeekEnd , Pos );
	File.Write( Buf );
	File.Flush();
	File.Close();
}

_LIT8( KMove, "MoveL" );
void CSheetTrace::MoveL( CSheetView::EMoveDirection enDirection )
{
	RFile File;
	User::LeaveIfError( File.Open( CCoeEnv::Static()->FsSession(), KFile, EFileWrite ) );
	TBuf8<128> Buf( KMove );
	_LIT8( KFormat, " %d\r\n" );
	Buf.AppendFormat( KFormat, enDirection );

	TInt Pos = 0;
	File.Seek( ESeekEnd , Pos );
	File.Write( Buf );
	File.Flush();
	File.Close();
}

_LIT8( KSetZoomMode, "SetZoomModeL" );
void CSheetTrace::SetZoomModeL( CSheetView::EZoomMode enMode )
{
	RFile File;
	User::LeaveIfError( File.Open( CCoeEnv::Static()->FsSession(), KFile, EFileWrite ) );
	TBuf8<128> Buf( KSetZoomMode );
	_LIT8( KFormat, " %d\r\n" );
	Buf.AppendFormat( KFormat, enMode );

	TInt Pos = 0;
	File.Seek( ESeekEnd , Pos );
	File.Write( Buf );
	File.Flush();
	File.Close();
}

//CSheetData

_LIT8( KResetCellSize, "ResetCellSizeL" );
void CSheetTrace::ResetCellSizeL( const TSize& Size )
{
	RFile File;
	User::LeaveIfError( File.Open( CCoeEnv::Static()->FsSession(), KFile, EFileWrite ) );
	TBuf8<128> Buf( KResetCellSize );
	_LIT8( KFormat, " %d:%d\r\n" );
	Buf.AppendFormat( KFormat, Size.iWidth, Size.iHeight );

	TInt Pos = 0;
	File.Seek( ESeekEnd , Pos );
	File.Write( Buf );
	File.Flush();
	File.Close();
}

_LIT8( KSetCellSize, "SetCellSizeL" );
void CSheetTrace::SetCellSizeL( const TPoint& Position, const TSize& Size )
{
	RFile File;
	User::LeaveIfError( File.Open( CCoeEnv::Static()->FsSession(), KFile, EFileWrite ) );
	TBuf8<128> Buf( KSetCellSize );
	_LIT8( KFormat, " (%d:%d) %d:%d\r\n" );
	Buf.AppendFormat( KFormat, Position.iX, Position.iY, Size.iWidth, Size.iHeight );

	TInt Pos = 0;
	File.Seek( ESeekEnd , Pos );
	File.Write( Buf );
	File.Flush();
	File.Close();
}

_LIT8( KSetCellTextColor, "SetCellTextColorL" );
void CSheetTrace::SetCellTextColorL( const TPoint& Position, const TRgb& Color )
{
	RFile File;
	User::LeaveIfError( File.Open( CCoeEnv::Static()->FsSession(), KFile, EFileWrite ) );
	TBuf8<128> Buf( KSetCellTextColor );
	_LIT8( KFormat, " (%d:%d) %d\r\n" );
	Buf.AppendFormat( KFormat, Position.iX, Position.iY, Color.Value() );

	TInt Pos = 0;
	File.Seek( ESeekEnd , Pos );
	File.Write( Buf );
	File.Flush();
	File.Close();
}

_LIT8( KSetCellColor, "SetCellColorL" );
void CSheetTrace::SetCellColorL( const TPoint& Position, const TRgb& Color )
{
	RFile File;
	User::LeaveIfError( File.Open( CCoeEnv::Static()->FsSession(), KFile, EFileWrite ) );
	TBuf8<128> Buf( KSetCellColor );
	_LIT8( KFormat, " (%d:%d) %d\r\n" );
	Buf.AppendFormat( KFormat, Position.iX, Position.iY, Color.Value() );

	TInt Pos = 0;
	File.Seek( ESeekEnd , Pos );
	File.Write( Buf );
	File.Flush();
	File.Close();
}

_LIT8( KSetCellAlignment, "SetCellAlignmentL" );
void CSheetTrace::SetCellAlignmentL( const TPoint& Position, CGraphicsContext::TTextAlign Alignment )
{
	RFile File;
	User::LeaveIfError( File.Open( CCoeEnv::Static()->FsSession(), KFile, EFileWrite ) );
	TBuf8<128> Buf( KSetCellAlignment );
	_LIT8( KFormat, " (%d:%d) %d\r\n" );
	Buf.AppendFormat( KFormat, Position.iX, Position.iY, Alignment );

	TInt Pos = 0;
	File.Seek( ESeekEnd , Pos );
	File.Write( Buf );
	File.Flush();
	File.Close();
}
