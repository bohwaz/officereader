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

#ifndef __MOBILEOFFICEAPPLICATION_H__
#define __MOBILEOFFICEAPPLICATION_H__

// INCLUDES
#include <aknapp.h>


// CLASS DECLARATION

/**
* CMobileOfficeApplication application class.
* Provides factory to create concrete document object.
* An instance of CMobileOfficeApplication is the application part of the
* AVKON application framework for the MobileOffice example application.
*/
class CMobileOfficeApplication : public CAknApplication
    {
    public: // Functions from base classes

        /**
        * From CApaApplication, AppDllUid.
        * @return Application's UID (KUidMobileOfficeApp).
        */
        TUid AppDllUid() const;

    protected: // Functions from base classes

        /**
        * From CApaApplication, CreateDocumentL.
        * Creates CMobileOfficeDocument document object. The returned
        * pointer in not owned by the CMobileOfficeApplication object.
        * @return A pointer to the created document object.
        */
        CApaDocument* CreateDocumentL();
    };

#endif // __MOBILEOFFICEAPPLICATION_H__

// End of File
