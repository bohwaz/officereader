/*************************************************************************
 *
 * DO NOT ALTER OR REMOVE COPYRIGHT NOTICES OR THIS FILE HEADER.
 * 
 * Copyright 2008,2009,2010 by ODENDAHL SEPT-Solutions
 *
 * Office Reader - a OpenDocument viewer for Symbian OS
 *
 * $Id:$
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

#include "CellFormat.h"

CCellFormat::CCellFormat()
{
	// No implementation required
	ConstructL();
}


CCellFormat::~CCellFormat()
{
}

CCellFormat* CCellFormat::NewLC()
{
	CCellFormat* self = new (ELeave)CCellFormat();
	CleanupStack::PushL(self);
	self->ConstructL();
	return self;
}

CCellFormat* CCellFormat::NewL()
{
	CCellFormat* self=CCellFormat::NewLC();
	CleanupStack::Pop(); // self;
	return self;
}

void CCellFormat::ConstructL()
{
	iChanged = EFalse;
	iAlign = ECellNone;
}

void CCellFormat::SetAlign(TCellAlign aAlign)
{
	// if already set, do not set it again, e.g this would override previous settings when cell-style="string"
	if (iAlign == ECellNone)
		iAlign = aAlign;
	iChanged = ETrue;
}

void CCellFormat::SetColor(TRgb aColor)
{
	iColor = aColor;
	iChanged = ETrue;
}

void CCellFormat::SetFontColor(TRgb aColor)
{
	iFontColor = aColor;
	iChanged = ETrue;
}


TCellAlign CCellFormat::Align()
{
	return iAlign;
}

TRgb CCellFormat::Color()
{
	return iColor;
}

TRgb CCellFormat::FontColor()
{
	return iFontColor;
}

TBool CCellFormat::Changed()
{
	return iChanged;
}
