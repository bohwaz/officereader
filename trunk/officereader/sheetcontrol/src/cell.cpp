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

#include "cell.h"

CCell::CCell():
	m_enType( enUnknown )
{
}

CCell::~CCell()
{
	Reset();
}


CCell& CCell::operator=( const CCell& Other )
{
	if( this == &Other )
		return *this;

	Reset();

	switch( m_enType = Other.m_enType )
	{
	case enInteger:
		m_Value.m_nInteger = Other.m_Value.m_nInteger;
		break;
	case enReal:
		m_Value.m_dReal = Other.m_Value.m_dReal;
		break;
	case enString:
		m_Value.m_sString = Other.m_Value.m_sString->AllocL();
		break;
	case enUnknown:
	case enWrong:
	default:
		ASSERT( FALSE );
	}

	return *this;
}

CCell& CCell::operator=( TInt nValue )
{
	Reset();
	m_enType = enInteger;
	m_Value.m_nInteger = nValue;
	return *this;
}

CCell& CCell::operator=( TReal dValue )
{
	Reset();
	m_enType = enReal;
	m_Value.m_dReal = dValue;
	return *this;
}

CCell& CCell::operator=( const TDesC& sValue )
{
	Reset();
	m_enType = enString;
	m_Value.m_sString = sValue.AllocL();
	return *this;
}

TInt CCell::IntL() const
{
	if( m_enType != enInteger )
		User::Leave( KErrGeneral );

	return m_Value.m_nInteger;
}

TReal CCell::RealL() const
{
	if( m_enType != enReal )
		User::Leave( KErrGeneral );

	return m_Value.m_dReal;
}

const TDesC& CCell::DesL() const
{
	if( m_enType != enString )
		User::Leave( KErrGeneral );

	return *m_Value.m_sString;
}

void CCell::AppendL( const TDesC& sText )
{
	if( m_enType != enString )
		User::Leave( KErrGeneral );

	m_Value.m_sString = m_Value.m_sString->ReAllocL( m_Value.m_sString->Length() + sText.Length() );
	m_Value.m_sString->Des().Append( sText );
}

void CCell::Reset()
{
	switch( m_enType )
	{
	case enInteger:
		m_Value.m_nInteger = 0;
		break;
	case enReal:
		m_Value.m_dReal = 0;
		break;
	case enString:
		delete m_Value.m_sString;
		m_Value.m_sString = NULL;
		break;
	case enUnknown:
	case enWrong:
	default:
		break;
	}
	m_enType = enUnknown;
}

TInt CCell::MaxRepresentationLengthL() const
{
	switch( m_enType )
	{
	case enString:
		return m_Value.m_sString->Length();
	case enInteger:
	case enReal:
	case enUnknown:
	case enWrong:
		return 32;
	default: return 0;
	}
}

void CCell::FormatL( TDes& sDst ) const
{
	if( sDst.MaxLength() <= 32 )
		User::Leave( KErrArgument );

	switch( m_enType )
	{
	case enInteger:
		sDst.Num( m_Value.m_nInteger );
		break;
	case enReal:
		sDst.Num( m_Value.m_dReal, TRealFormat() );
		break;
	case enString:
		//sDst.Copy( *m_Value.m_sString );
		sDst.Copy( m_Value.m_sString->Ptr(), m_Value.m_sString->Length() <= sDst.MaxLength() ? 
			m_Value.m_sString->Length() : sDst.MaxLength() );
		break;
	case enUnknown:
	case enWrong:
		//like in MS Excel
		sDst.Copy( _L("####################") );
		break;
	}
}

/*
CGraphicsContext::TTextAlign CCell::Align() const
{
	switch( m_enType )
	{
	case enInteger:
	case enReal:
	case enString:
		return CGraphicsContext::ERight;
	case enUnknown:
	case enWrong:
		return CGraphicsContext::ERight;
	default:
		ASSERT( FALSE );
	}
	return CGraphicsContext::ERight;
}
*/
