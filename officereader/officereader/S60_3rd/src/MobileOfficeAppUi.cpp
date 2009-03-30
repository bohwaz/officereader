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
#include "MobileOfficeAppui.h"
#include "MobileOfficeApp.h"
#include "MobileOfficeDocument.h"
#include <MobileOffice.rsg>
#include "MobileOffice.hrh"

#include "MobileOfficeAppui.h"
#include "MobileOfficeView.h"
#include "MobileOfficeView2.h"
#include "EditorViewView.h"
#include "ViewImageView.h"

#include <eikapp.h>
#include <stringloader.h>
#include <aknmessagequerydialog.h> 
#include <avkon.hrh>
#include <HLPLCH.H>
#include <APGCLI.H> 
#include <APMSTD.H>
#include <APMREC.H>
#include <apparc.h>
#include <EIKAPPUI.H>


#ifndef FREEVERSION
	#include "CIMEI.h"
#endif

/*
#include <rsendasmessage.h>
#include <rsendas.h>
#include <senduiconsts.h>

void SendFileL(const TDesC& aFilename)
    {
   // 1. Open session
   RSendAs session;
   User::LeaveIfError(session.Connect());
   CleanupClosePushL(session);

   // 2. Create message
   RSendAsMessage message;
   message.CreateL(session,	KSenduiMtmBtUid);
   CleanupClosePushL(message);

    // 3. Add attachment
    TRequestStatus status;
    message.AddAttachment(aFilename, status);
    User::WaitForRequest(status);

    // 4. Send message
    if (status.Int()== KErrNone)
        {  
        CleanupStack::Pop(&message);
        message.LaunchEditorAndCloseL();
        }
   else
       CleanupStack::PopAndDestroy(&message); 

   CleanupStack::PopAndDestroy(&session);
}
*/

// ================= MEMBER FUNCTIONS =======================
//
// ----------------------------------------------------------
// CMobileOfficeAppUi::ConstructL()
// 
// ----------------------------------------------------------
//
void CMobileOfficeAppUi::ConstructL()
{
	BaseConstructL(EAknEnableSkin);

	iOpenDocument = COpenDocument::NewL();
	
	#ifndef FREEVERSION
		TBuf<50> nein;
		#ifndef __WINSCW__
			CIMEIRetriever::GetIMEIL(nein);	
		#endif

		licMan.SetIMEI(nein);
		licMan.Start(this);
	#endif	

	//SetOrientationL(EAppUiOrientationLandscape);

    // Show tabs for main views from resources
    CEikStatusPane* sp = StatusPane();

    // Fetch pointer to the default navi pane control
    iNaviPane = (CAknNavigationControlContainer*)sp->ControlL( 
        TUid::Uid(EEikStatusPaneUidNavi));

    // Tabgroup has been read from resource and it were pushed to the navi pane. 
    // Get pointer to the navigation decorator with the ResourceDecorator() function. 
    // Application owns the decorator and it has responsibility to delete the object.
    iDecoratedTabGroup = iNaviPane->ResourceDecorator();
    if (iDecoratedTabGroup)
        {
        iTabGroup = (CAknTabGroup*) iDecoratedTabGroup->DecoratedControl();
		iTabGroup->SetObserver( this );
		}

		
    CMobileOfficeView* view1 = new (ELeave) CMobileOfficeView;

    CleanupStack::PushL( view1 );
    view1->ConstructL();
    AddViewL( view1 );      // transfer ownership to CAknViewAppUi
    CleanupStack::Pop();    // view1

    CMobileOfficeView2* view2 = new (ELeave) CMobileOfficeView2;

    CleanupStack::PushL( view2 );
    view2->ConstructL();
    AddViewL( view2 );      // transfer ownership to CAknViewAppUi
    CleanupStack::Pop();    // view2

	
	CEditorViewView* view3 = new (ELeave) CEditorViewView;

    CleanupStack::PushL( view3 );
    view3->ConstructL();
    AddViewL( view3 );      // transfer ownership to CAknViewAppUi
    CleanupStack::Pop();    // view2

	
	CViewImageView* view4 = new (ELeave) CViewImageView;

    CleanupStack::PushL( view4 );
    view4->ConstructL();
    AddViewL( view4 );      // transfer ownership to CAknViewAppUi
    CleanupStack::Pop();    // view2

    SetDefaultViewL(*view1);
	CheckDemo();
}

// ----------------------------------------------------
// CMobileOfficeAppUi::~CMobileOfficeAppUi()
// Destructor
// Frees reserved resources
// ----------------------------------------------------
//
CMobileOfficeAppUi::~CMobileOfficeAppUi()
    {
    //delete iDecoratedTabGroup;
	delete iOpenDocument;
   }

// ------------------------------------------------------------------------------
// CMobileOfficeAppUi::DynInitMenuPaneL(TInt aResourceId,CEikMenuPane* aMenuPane)
//  This function is called by the EIKON framework just before it displays
//  a menu pane. Its default implementation is empty, and by overriding it,
//  the application can set the state of menu items dynamically according
//  to the state of application data.
// ------------------------------------------------------------------------------
//
void CMobileOfficeAppUi::DynInitMenuPaneL(
    TInt /*aResourceId*/,CEikMenuPane* /*aMenuPane*/)
    {
    }

// ----------------------------------------------------
// CMobileOfficeAppUi::HandleKeyEventL(
//     const TKeyEvent& aKeyEvent,TEventCode /*aType*/)
// takes care of key event handling
// ----------------------------------------------------
//
TKeyResponse CMobileOfficeAppUi::HandleKeyEventL(
    const TKeyEvent& /*aKeyEvent*/,TEventCode /*aType*/)
    {
		/*if ( iTabGroup == NULL )
        {
        return EKeyWasNotConsumed;
        }

    if ( aKeyEvent.iCode == EKeyLeftArrow || aKeyEvent.iCode == EKeyRightArrow )
        {
        return iTabGroup->OfferKeyEventL( aKeyEvent, aType );
        }
    else
        {*/
        return EKeyWasNotConsumed;
	        //}

    }

// ----------------------------------------------------
// CMobileOfficeAppUi::HandleCommandL(TInt aCommand)
// takes care of command handling
// ----------------------------------------------------
//
void CMobileOfficeAppUi::HandleCommandL(TInt aCommand)
{
		CheckDemo();
	
		switch ( aCommand )
        {
		case EAknSoftkeyExit:
		case EEikCmdExit:	
            {
				if (iDoorObserver)
					iDoorObserver->NotifyExit( MApaEmbeddedDocObserver::ENoChanges );
				#ifndef FREEVERSION
					licMan1.End();
				#endif
				Exit();
				break;
            }
        case EMobileOfficeCmdAppAbout:
            {
			
				HBufC* messageText = StringLoader::LoadLC( R_MOBILESEARCH_ABOUT );
				
				/*
				TBuf<500> nein;
				nein.Copy(*messageText);
				nein.Append(_L("\r\n"));
				nein.Append(_L("\r\n"));
				nein.Append(_L("Application: 1.1"));
				nein.Append(_L("\r\n"));
				*/
				CAknMessageQueryDialog* dlg = CAknMessageQueryDialog::NewL( *messageText );
				dlg->PrepareLC(R_MESSAGE_QUERY);
				HBufC* aboutTitleText = StringLoader::LoadLC( R_MOBILESEARCH_ABOUTTITLE );	
				dlg->QueryHeading()->SetTextL( *aboutTitleText );
				CleanupStack::PopAndDestroy( aboutTitleText );
				dlg->RunLD();
				CleanupStack::PopAndDestroy( messageText );
				break;
            }
		case EMobileOfficeCmdOpen:
            {
				TUid viewID = KEditorViewViewId;	
				ActivateLocalViewL(viewID);
				//SendFileL(iOpenDocument->Filename());

				
				break;
			
			}
	
		case EMobileOfficeCmdList:
            {
				TUid viewID = KViewId;
				ActivateLocalViewL(viewID);
				break;
			
			}
	
		case EMobileOfficeCmdProp:
            {
				TUid viewID = KView2Id;
				ActivateLocalViewL(viewID);
				break;
			
			}
	#ifndef FREEVERSION
		case EMobileOfficeCmdRegister:
			{
				Register();	
				break;		
			}
	#endif
		case EMobileOfficeCmdPicture:
            {
				TUid viewID = KViewImageViewId;
				ActivateLocalViewL(viewID);
				break;
			
			}
		case EMobileOfficeCmdNewText:
			{
				TUid viewID = KEditorViewViewId;	
				OpenDocument()->SetNextViewID(viewID);
				OpenDocument()->SetEditable(ETrue);
				ActivateLocalViewL(viewID);
				break;
			}
		case EMobileOfficeCmdBack:
            {
			
				ActivateLocalViewL(OpenDocument()->NextViewID());
				break;
			
			}
		case EMobileOfficeCmdHelp:
			{
				CArrayFix<TCoeHelpContext>* buf = AppHelpContextL();
				HlpLauncher::LaunchHelpApplicationL(iEikonEnv->WsSession(), buf);
			}
        default:
            break;      
        }
    }

 // ----------------------------------------------------
 // CMobileOfficeAppUi::TabChangedL(TInt aIndex)
 // This method gets called when CAknTabGroup active 
 // tab has changed.
 // ----------------------------------------------------
 //
 void CMobileOfficeAppUi::TabChangedL(TInt /*aIndex*/)
     {
    // ActivateLocalViewL(TUid::Uid(iTabGroup->TabIdFromIndex(aIndex)));
     }

 TFileName CMobileOfficeAppUi::GetAppPath()
{
      TParse parse;
    //On WINS the application is on the z drive
    #ifdef __WINSCW__
      TFileName appfullname = Application()->AppFullName();
      parse.Set(_L("c:"), &appfullname, NULL);
    #else
      parse.Set( Application()->AppFullName(), NULL, NULL);
    #endif
      return ( parse.DriveAndPath() ); //Base
}

void CMobileOfficeAppUi::CheckDemo()
{
	#ifndef FREEVERSION
	if( licMan.Is_dm_1() )
	{
		HBufC* mobileSearchText = StringLoader::LoadLC( R_MOBILESEARCH_MOBILESEARCH );
	
			HBufC* demoText = StringLoader::LoadLC( R_MOBILESEARCH_DEMO );

			CEikonEnv::Static()->InfoWinL( *mobileSearchText, *demoText );

			CleanupStack::PopAndDestroy( demoText );

		CleanupStack::PopAndDestroy( mobileSearchText );
    }
#endif
}

#ifndef FREEVERSION
bool CMobileOfficeAppUi::Register()
{
    TBuf <64> buff;
    HBufC * propmtText = CEikonEnv::Static()->AllocReadResourceL( R_STR_INPUT_ACTIVATION_CODE );

    CAknTextQueryDialog* dlg = new(ELeave) CAknTextQueryDialog( buff, CAknQueryDialog::ENoTone );
    if (dlg->ExecuteLD(R_GNFIELDQUERY_BIGNUMBER_QUERY, *propmtText ))
					
    {
         int user_number = 0;
         TLex lx;
         lx.Assign( buff );
         lx.Val( user_number );

        licMan.SetUnlockCode( user_number );

         // Dummy code
         TBuf<64> aImei;
    }

    if( licMan.Is_dm_1() )
    {
		HBufC* errorText = StringLoader::LoadLC( R_MOBILESEARCH_ERROR );

			HBufC* invalidCodeText = StringLoader::LoadLC( R_MOBILESEARCH_INVALIDCODE );

				CEikonEnv::Static()->InfoWinL( *errorText, *invalidCodeText );
			
			CleanupStack::PopAndDestroy( invalidCodeText );

		CleanupStack::PopAndDestroy( errorText );
    }
	else 
	{
		HBufC* correctCodeText = StringLoader::LoadLC( R_MOBILESEARCH_CORRECTCODE );

			HBufC* thanksText = StringLoader::LoadLC( R_MOBILESEARCH_THANKS );
	
				CEikonEnv::Static()->InfoWinL( *correctCodeText, *thanksText );

			CleanupStack::PopAndDestroy( thanksText );	

		CleanupStack::PopAndDestroy( correctCodeText );
	}

    delete propmtText;
	return true;
}

TBool CMobileOfficeAppUi::Registered()
{
	return licMan.Is_dm_1();
}
#endif

COpenDocument* CMobileOfficeAppUi::OpenDocument()
{
	return iOpenDocument;
}

CArrayFix<TCoeHelpContext>* CMobileOfficeAppUi::HelpContextL() const
{
	CArrayFixFlat<TCoeHelpContext>* array = new(ELeave)CArrayFixFlat<TCoeHelpContext>(1);
	CleanupStack::PushL(array);
	array->AppendL(TCoeHelpContext(KUidMobileOffice, _L("Introduction")));
	CleanupStack::Pop(array);
	return array;
}

void CMobileOfficeAppUi::OpenFileL( const TDesC& aFileName )
   {
   CMobileOfficeDocument* doc = static_cast<CMobileOfficeDocument*>( Document() );
   doc->OpenFileL( ETrue, aFileName, iEikonEnv->FsSession() );
   }
