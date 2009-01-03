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

#ifndef __CELL_H__
#define __CELL_H__

#include <e32base.h> 
#include <gdi.h>


class CCell : public CBase
{
public:

	static const TInt KMaxTextRepresentation = 128;
	
	enum EType
	{
		enUnknown = 0,
		enInteger,
		enReal,
		enString,
		enWrong
	};

	CCell();
	virtual ~CCell();

	CCell& operator=( const CCell& Other );
	CCell& operator=( TInt nValue );
	CCell& operator=( TReal dValue );
	CCell& operator=( const TDesC& sValue );

	EType Type() const { return m_enType; }

	TInt  IntL() const;
	TReal RealL() const;
	const TDesC& DesL() const;
	void AppendL( const TDesC& sText );

	TInt MaxRepresentationLengthL() const;
	void FormatL( TDes& sDst ) const;

private:
	void Reset();

	EType m_enType;

	union CValue
	{
		TInt   m_nInteger;
		TReal  m_dReal;
		HBufC* m_sString;
	};

	CValue m_Value;

	//now cell contain some meta data
	struct CMetaData
	{
		#define KDefaultCellColor     TRgb(0xffffffff)
		#define KDefaultCellTextColor TRgb(0xfffffffe)

		CMetaData():
			m_Color( KDefaultCellColor ),
			m_TextColor( KDefaultCellTextColor ),
			m_Align( CGraphicsContext::ERight )
		{
		}

		TRgb     m_Color;
		TRgb     m_TextColor;
		CGraphicsContext::TTextAlign m_Align;
	};
	CMetaData m_MetaData;

	TRgb Color() const { return m_MetaData.m_Color; }
	TRgb TextColor() const { return m_MetaData.m_TextColor; }
	CGraphicsContext::TTextAlign Align() const { return m_MetaData.m_Align; }

	friend class CSheetData;
	friend class CSheetDrawer;
	friend class CSheetTrace;
};

#endif
