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

#include  <aknviewappui.h>
#include  <avkon.hrh>
#include  <MobileOffice.rsg>
#include  "ViewImageView.h"
#include  "ViewImageContainer.h"
#include "MobileOfficeAppUi.h"

#include  "MobileOffice.hrh"

// ================= MEMBER FUNCTIONS =======================

// ---------------------------------------------------------
// CViewImageView::ConstructL(const TRect& aRect)
// EPOC two-phased constructor
// ---------------------------------------------------------
//
void CViewImageView::ConstructL()
    {
    BaseConstructL( R_MOBILEOFFICE_VIEWIMAGE_VIEW );
    }

// ---------------------------------------------------------
// CViewImageView::~CViewImageView()
// destructor
// ---------------------------------------------------------
//
CViewImageView::~CViewImageView()
    {
    if ( iContainer )
        {
        AppUi()->RemoveFromViewStack( *this, iContainer );
        }

    delete iContainer;
    }

// ---------------------------------------------------------
// TUid CViewImageView::Id()
//
// ---------------------------------------------------------
//
TUid CViewImageView::Id() const
    {
    return KViewImageViewId;
    }

// ---------------------------------------------------------
// CViewImageView::HandleCommandL(TInt aCommand)
// takes care of view command handling
// ---------------------------------------------------------
//
void CViewImageView::HandleCommandL(TInt aCommand)
    {   

    switch ( aCommand )
        {
        case EAknSoftkeyOk:
            {
            iEikonEnv->InfoMsg( _L("view1 ok") );
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
// CViewImageView::HandleClientRectChange()
// ---------------------------------------------------------
//
void CViewImageView::HandleClientRectChange()
    {
    if ( iContainer )
        {
        iContainer->SetRect( ClientRect() );
        }
    }

// ---------------------------------------------------------
// CViewImageView::DoActivateL(...)
// 
// ---------------------------------------------------------
//
void CViewImageView::DoActivateL(
   const TVwsViewId& /*aPrevViewId*/,TUid /*aCustomMessageId*/,
   const TDesC8& /*aCustomMessage*/)
    {
    if (!iContainer)
        {
        iContainer = new (ELeave) CViewImageContainer;
        iContainer->SetMopParent(this);
        iContainer->ConstructL( ClientRect() );
        AppUi()->AddToStackL( *this, iContainer );
        } 
   }

// ---------------------------------------------------------
// CViewImageView::DoDeactivate()
// 
// ---------------------------------------------------------
//
void CViewImageView::DoDeactivate()
    {
    if ( iContainer )
        {
        AppUi()->RemoveFromViewStack( *this, iContainer );
        }
    
    delete iContainer;
    iContainer = NULL;
    }

// End of File

