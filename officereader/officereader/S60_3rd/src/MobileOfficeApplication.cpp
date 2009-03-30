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

// INCLUDE FILES
#include "MobileOfficeDocument.h"
#include "MobileOfficeApplication.h"
#include "MobileOfficeApp.h"

// ============================ MEMBER FUNCTIONS ===============================
// -----------------------------------------------------------------------------
// CMobileOfficeApplication::CreateDocumentL()
// Creates CApaDocument object
// -----------------------------------------------------------------------------
//
CApaDocument* CMobileOfficeApplication::CreateDocumentL()
    {
    // Create an MobileOffice document, and return a pointer to it
    return (static_cast<CApaDocument*>
                    ( CMobileOfficeDocument::NewL( *this ) ) );
    }

// -----------------------------------------------------------------------------
// CMobileOfficeApplication::AppDllUid()
// Returns application UID
// -----------------------------------------------------------------------------
//
TUid CMobileOfficeApplication::AppDllUid() const
    {
    // Return the UID for the MobileOffice application
    return KUidMobileOffice;
    }

void CMobileOfficeApplication::PreDocConstructL()
   {
   CEikonEnv* env = CEikonEnv::Static();
   
   // Check that this app is started as stand-alone
   if (!env->StartedAsServerApp() && !env->EikAppUi())
     {
     RWsSession& ws = env->WsSession();
     const TInt myWgId = env->RootWin().Identifier();
     TInt wgId = 0;  
     TUid uid(AppDllUid());
   
     // Look for another instance of this app
     while (wgId >= 0)
       {
       if (wgId && wgId != myWgId) // Another instance found -> close it
         {
         TApaTask other(ws);
         other.SetWgId(wgId);
         other.EndTask();          // Requires SwEvent capability
         }
       CApaWindowGroupName::FindByAppUid(uid, ws, wgId);
       }
     }
   
   // call PreDocConstructL from base class
   CEikApplication::PreDocConstructL();
   }



// End of File
