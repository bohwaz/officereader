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
#include  <aknviewappui.h>
#include  <avkon.hrh>
#include  <MobileOffice.rsg>
#include  "MobileOfficeView2.h"
#include  "MobileOfficeContainer2.h" 
#include "MobileOffice.hrh"

// ================= MEMBER FUNCTIONS =======================

// ---------------------------------------------------------
// CMobileOfficeView2::ConstructL(const TRect& aRect)
// EPOC two-phased constructor
// ---------------------------------------------------------
//
void CMobileOfficeView2::ConstructL()
    {
    BaseConstructL( R_MOBILEOFFICE_VIEW2 );
    }

// ---------------------------------------------------------
// CMobileOfficeView2::~CMobileOfficeView2()
// destructor
// ---------------------------------------------------------
//
CMobileOfficeView2::~CMobileOfficeView2()
    {
    if ( iContainer )
        {
        AppUi()->RemoveFromViewStack( *this, iContainer );
        }

    delete iContainer;
    }

// ---------------------------------------------------------
// TUid CMobileOfficeView2::Id()
// 
// ---------------------------------------------------------
//
TUid CMobileOfficeView2::Id() const
    {
    return KView2Id;
    }

// ---------------------------------------------------------
// CMobileOfficeView2::HandleCommandL(TInt aCommand)
// takes care of view command handling
// ---------------------------------------------------------
//
void CMobileOfficeView2::HandleCommandL(TInt aCommand)
{
	switch ( aCommand )
        {
        case EAknSoftkeyOk:
            {
            iEikonEnv->InfoMsg( _L("view2 ok") );
            break;
            }
        case EAknSoftkeyBack:
            {
				AppUi()->HandleCommandL(EMobileOfficeCmdBack);
			break;
            }
        default:
            {
            AppUi()->HandleCommandL( aCommand );
            break;
            }
        } 
    }

// ---------------------------------------------------------
// CMobileOfficeView2::HandleClientRectChange()
// ---------------------------------------------------------
//
void CMobileOfficeView2::HandleClientRectChange()
    {
    if ( iContainer )
        {
        iContainer->SetRect( ClientRect() );
        }
    }

// ---------------------------------------------------------
// CMobileOfficeView2::DoActivateL(...)
// 
// ---------------------------------------------------------
//
void CMobileOfficeView2::DoActivateL(
   const TVwsViewId& /*aPrevViewId*/,TUid /*aCustomMessageId*/,
   const TDesC8& /*aCustomMessage*/)
    {
    if (!iContainer)
        {
        iContainer = new (ELeave) CMobileOfficeContainer2;
        iContainer->SetMopParent(this);
        iContainer->ConstructL( ClientRect() );
        AppUi()->AddToStackL( *this, iContainer );
        }
   }

// ---------------------------------------------------------
// CMobileOfficeView2::DoDeactivate()
// 
// ---------------------------------------------------------
//
void CMobileOfficeView2::DoDeactivate()
    {
    if ( iContainer )
        {
        AppUi()->RemoveFromViewStack( *this, iContainer );
        }
    
    delete iContainer;
    iContainer = NULL;
    }



