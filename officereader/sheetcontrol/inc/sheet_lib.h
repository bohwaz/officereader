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

// This file defines the API for sheet_lib.dll

#ifndef __SHEET_LIB_H__
#define __SHEET_LIB_H__


//  Include Files

#include <e32base.h>    // CBase
#include <e32std.h>     // TBuf


//  Constants

const TInt Ksheet_libBufferLength = 15;
typedef TBuf<Ksheet_libBufferLength> Tsheet_libExampleString;


//  Class Definitions

class Csheet_lib : public CBase
    {
    public:     // new functions
        IMPORT_C static Csheet_lib* NewL();
        IMPORT_C static Csheet_lib* NewLC();
        IMPORT_C ~Csheet_lib();

    public:     // new functions, example API
        IMPORT_C TVersion Version() const;
        IMPORT_C void ExampleFuncAddCharL(const TChar& aChar);
        IMPORT_C void ExampleFuncRemoveLast();
        IMPORT_C const TPtrC ExampleFuncString() const;

    private:    // new functions
        Csheet_lib();
        void ConstructL();

    private:    // data
        Tsheet_libExampleString* iString;
    };


#endif  // __SHEET_LIB_H__


