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

#ifndef CELLFORMAT_H
#define CELLFORMAT_H

// INCLUDES
#include <e32std.h>
#include <e32base.h>


#include <gdi.h>

enum TCellAlign
{
	ECellNone = 0,
	ECellLeft,
	ECellRight,
	ECellCenter
};

class CCellFormat : public CBase
{
public: // Constructors and destructor

	~CCellFormat();
	CCellFormat();  // no allocation, so need for 2-phase construction

	static CCellFormat* NewL();
	static CCellFormat* NewLC();

	void SetAlign(TCellAlign aAlign);
	TCellAlign Align();

	void SetColor(TRgb aColor);
	TRgb Color();

	void SetFontColor(TRgb aColor);
	TRgb FontColor();

	TBool Changed();

private:

	void ConstructL();

	TCellAlign iAlign;
	TRgb iFontColor;
	TRgb iColor;
	TBool iChanged;

};

#endif // CELLFORMAT_H
