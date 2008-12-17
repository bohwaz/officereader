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
#include  <aknnotewrappers.h>
#include <BAUTILS.H>

#include  <MobileOffice.rsg>
#include "MobileOffice.hrh"
#include  "MobileOfficeView.h"
#include  "MobileOfficeContainer.h" 

#include "MobileOfficeAppui.h"


// ================= MEMBER FUNCTIONS =======================

// ---------------------------------------------------------
// CMobileOfficeView::ConstructL(const TRect& aRect)
// EPOC two-phased constructor
// ---------------------------------------------------------
//
void CMobileOfficeView::ConstructL()
    {
    BaseConstructL( R_MOBILEOFFICE_VIEW1 );
    }

// ---------------------------------------------------------
// CMobileOfficeView::~CMobileOfficeView()
// destructor
// ---------------------------------------------------------
//
CMobileOfficeView::~CMobileOfficeView()
    {
    if ( iContainer )
        {
        AppUi()->RemoveFromViewStack( *this, iContainer );
        }

    delete iContainer;
    }

// ---------------------------------------------------------
// TUid CMobileOfficeView::Id()
//
// ----------------------	-----------------------------------
//
TUid CMobileOfficeView::Id() const
    {
    return KViewId;
    }

// ---------------------------------------------------------
// CMobileOfficeView::HandleCommandL(TInt aCommand)
// takes care of view command handling
// ---------------------------------------------------------
//
void CMobileOfficeView::HandleCommandL(TInt aCommand)
    {   
		
	TInt ListCount;
	CTextListBoxModel* model =  iContainer->iListbox->Model();  // not taking ownership
	model->SetOwnershipType (ELbmOwnsItemArray);
	CDesCArray* itemArray = STATIC_CAST(CDesCArray*, model->ItemTextArray());
	ListCount=itemArray->Count();

	CMobileOfficeAppUi* app = (CMobileOfficeAppUi*)AppUi();			

	if(ListCount>0 && (iContainer->iListbox->CurrentItemIndex() >-1))
	{
		app->OpenDocument()->SetFileName(iContainer->ListboxText());
		app->OpenDocument()->SetNextViewID(KViewId);
	}
	
	switch ( aCommand )
        {    
 
		case EMobileOfficeCmdNewText:
			{
				AppUi()->HandleCommandL(EMobileOfficeCmdNewText);
				break;
			}
		case EMobileOfficeCmdRefresh:
			{
				app->CheckDemo();	
				iContainer->ClearListbox();
				iContainer->Search();
				break;
			}
		case EMobileOfficeCmdDelete:
			{
				app->CheckDemo();
				if(ListCount>0)
				{
					iContainer->Delete();
				}
				else
				{
					_LIT(mesg,"No file selected");
					CAknInformationNote* informationNote = new (ELeave) CAknInformationNote;
					informationNote->ExecuteLD(mesg);
				}
				break;
			}
        case EAknSoftkeyBack:
            {
            AppUi()->HandleCommandL(EEikCmdExit);
            break;
            }
        default:
            {
				if ( (aCommand==EMobileOfficeCmdAppAbout) || (aCommand==EMobileOfficeCmdRegister) || 
					 (aCommand==EMobileOfficeCmdHelp) )
				{
					AppUi()->HandleCommandL( aCommand );
				}
				else
				if ( ((ListCount==0) || (iContainer->iListbox->CurrentItemIndex()==-1)) && ( (aCommand==EMobileOfficeCmdPicture) | (aCommand==EMobileOfficeCmdProp) |(aCommand==EMobileOfficeCmdOpen) ) )
				{
						_LIT(mesg,"No file selected");
						CAknInformationNote* informationNote = new (ELeave) CAknInformationNote;
						informationNote->ExecuteLD(mesg);
				}
				else
				{
					if (BaflUtils::FileExists(iCoeEnv->FsSession(), iContainer->ListboxText()))
					{
						AppUi()->HandleCommandL( aCommand );
					}
					else
					{
						iContainer->ClearListbox();
						iContainer->Search();
					}
				}
            break;
            }
        }
	}

// ---------------------------------------------------------
// CMobileOfficeView::HandleClientRectChange()
// ---------------------------------------------------------
//
void CMobileOfficeView::HandleClientRectChange()
    {
    if ( iContainer )
        {
        iContainer->SetRect( ClientRect() );
        }
    }

// ---------------------------------------------------------
// CMobileOfficeView::DoActivateL(...)
// 
// ---------------------------------------------------------
//
void CMobileOfficeView::DoActivateL(
   const TVwsViewId& /*aPrevViewId*/,TUid /*aCustomMessageId*/,
   const TDesC8& /*aCustomMessage*/)
    {
    if (!iContainer)
        {
        iContainer = new (ELeave) CMobileOfficeContainer;
        iContainer->SetMopParent(this);
        iContainer->ConstructL( ClientRect() );
		iContainer->ShowTab();
        AppUi()->AddToStackL( *this, iContainer );
        } 
   }

// ---------------------------------------------------------
// CMobileOfficeView::DoDeactivate()
// 
// ---------------------------------------------------------
//
void CMobileOfficeView::DoDeactivate()
    {
    if ( iContainer )
        {
			iContainer->HideTab();
			AppUi()->RemoveFromViewStack( *this, iContainer );
        }
    
    delete iContainer;
    iContainer = NULL;
    }

void CMobileOfficeView::DynInitMenuPaneL(TInt aResourceId, CEikMenuPane *aMenuPane)
{
	CAknView::DynInitMenuPaneL(aResourceId, aMenuPane);

	if(aResourceId == R_MOBILEOFFICE_VIEW1_MENU)
	{
		#ifndef FREEVERSION
			if(!((CMobileOfficeAppUi*)AppUi())->Registered())
			{	
				aMenuPane->DeleteMenuItem( EMobileOfficeCmdRegister );
			}
		#else
				aMenuPane->DeleteMenuItem( EMobileOfficeCmdRegister );
		#endif
	}

}


