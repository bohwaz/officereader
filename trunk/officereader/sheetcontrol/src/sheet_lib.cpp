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

#include "sheet_lib.h"	// Csheet_lib
#include "sheet_lib.pan"      	// panic codes


//  Member Functions

EXPORT_C Csheet_lib* Csheet_lib::NewLC()
    {
    Csheet_lib* self = new (ELeave) Csheet_lib;
    CleanupStack::PushL(self);
    self->ConstructL();
    return self;
    }


EXPORT_C Csheet_lib* Csheet_lib::NewL()
    {
    Csheet_lib* self = Csheet_lib::NewLC();
    CleanupStack::Pop(self);
    return self;
    }


Csheet_lib::Csheet_lib()
// note, CBase initialises all member variables to zero
    {
    }


void Csheet_lib::ConstructL()
    {
    // second phase constructor, anything that may leave must be constructed here
    iString = new (ELeave) Tsheet_libExampleString;
    }


EXPORT_C Csheet_lib::~Csheet_lib()
    {
    delete iString;
    }


EXPORT_C TVersion Csheet_lib::Version() const
    {
    // Version number of example API
    const TInt KMajor = 1;
    const TInt KMinor = 0;
    const TInt KBuild = 1;
    return TVersion(KMajor, KMinor, KBuild);
    }


EXPORT_C void Csheet_lib::ExampleFuncAddCharL(const TChar& aChar)
    {
    //__ASSERT_ALWAYS(iString != NULL, Panic(Esheet_libNullPointer));

    if (iString->Length() >= Ksheet_libBufferLength)
        {
        User::Leave(KErrTooBig);
        }

    iString->Append(aChar);
    }


EXPORT_C void Csheet_lib::ExampleFuncRemoveLast()
    {
    //__ASSERT_ALWAYS(iString != NULL, Panic(Esheet_libNullPointer));

    if (iString->Length() > 0)
        {
        iString->SetLength(iString->Length() - 1);
        }
    }


EXPORT_C const TPtrC Csheet_lib::ExampleFuncString() const
    {
    //__ASSERT_ALWAYS(iString != NULL, Panic(Esheet_libNullPointer));
    return *iString;
    }



