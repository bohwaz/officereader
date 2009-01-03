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

#ifndef CZIP_H
#define CZIP_H

// INCLUDES
#include <e32std.h>
#include <e32base.h>

#include <zipfile.h>

// CLASS DECLARATION

/**
*  CCZip
* 
*/
class CCZip : public CBase
{
public: // Constructors and destructor

	/**
        * Destructor.
        */
	~CCZip();

        /**
        * Two-phased constructor.
        */
	static CCZip* NewL();

        /**
        * Two-phased constructor.
        */
	static CCZip* NewLC();
	
	HBufC8* GetFileText(RFs aRFs, const TDesC& aContainer,const TDesC& aFile);
private:

	/**
        * Constructor for performing 1st stage construction
        */
	CCZip();

	/**
        * EPOC default constructor for performing 2nd stage construction
        */
	void ConstructL();


};

#endif // CZIP_H
