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
#include <AknQueryDialog.h>  // findquery
#include <aknnotewrappers.h>
#include <aknViewAppUi.h> 
#include <stringloader.h>

#include  <MobileOffice.rsg>
#include  "MobileOffice.hrh"
#include  "EditorViewView.h"
#include  "EditorViewContainer.h"
#include "MobileOfficeAppUi.h"
#include "sheet_view.h"


// ================= MEMBER FUNCTIONS =======================

// ---------------------------------------------------------
// CEditorViewView::ConstructL(const TRect& aRect)
// EPOC two-phased constructor
// ---------------------------------------------------------
//
void CEditorViewView::ConstructL()
    {
    BaseConstructL( R_MOBILEOFFICE_EDITORVIEW_VIEW );
	iLength = 0;
    }

// ---------------------------------------------------------
// CEditorViewView::~CEditorViewView()
// destructor
// ---------------------------------------------------------
//
CEditorViewView::~CEditorViewView()
    {
    if ( iContainer )
        {
        AppUi()->RemoveFromViewStack( *this, iContainer );
        }

    delete iContainer;
    }

// ---------------------------------------------------------
// TUid CEditorViewView::Id()
//
// ---------------------------------------------------------
//
TUid CEditorViewView::Id() const
    {
    return KEditorViewViewId;
    }

// ---------------------------------------------------------
// CEditorViewView::HandleCommandL(TInt aCommand)
// takes care of view command handling
// ---------------------------------------------------------
//
void CEditorViewView::HandleCommandL(TInt aCommand)
    {   

	CMobileOfficeAppUi* app = (CMobileOfficeAppUi*)AppUi();			
	app->OpenDocument()->SetNextViewID(KEditorViewViewId);	
	app->OpenDocument()->HideNote();
	app->CheckDemo();		

	switch ( aCommand)
        {
        case EMobileOfficeCmdFindFirst:
			{
					TBuf<50> text;
					CAknTextQueryDialog* FindQuery = CAknTextQueryDialog::NewL(text);
					CleanupStack::PushL(FindQuery);
					
					
					HBufC* prompt;
					prompt = StringLoader::LoadLC(R_FIND_QUERY);	// Pushes prompt onto the Cleanup Stack.
					FindQuery->SetPromptL(*prompt);
					CleanupStack::PopAndDestroy(prompt);
					CleanupStack::Pop(FindQuery);
					if (FindQuery->ExecuteLD(R_DATAQUERY_DATA_QUERY))
					{
						iLength = text.Length();
						
						if (iContainer->Editor())
						{
							TInt aPos =  iContainer->Editor()->FindTextL(&text,0,0);
							if (aPos > -1)
							{
								iContainer->Editor()->SetCursorPosL(aPos,EFalse);
								iContainer->Editor()->SetCursorPosL(aPos + text.Length(),ETrue);
							}
						}
						
						if (iContainer->Sheet())
						{
							TPoint aPoint;
							if ( iContainer->Sheet()->Model()->FindFirstL(text,aPoint,CSheetModel::enFindSubStr | CSheetModel::enFindOrderRows | CSheetModel::enFindCompareNoCase))
							{
								iContainer->Sheet()->View()->SetCurrentCell(aPoint);
								iLength = 10; // any variable so findnext is shown
							}
						
							
						}
				
					}		
				break;
			}
		case EMobileOfficeCmdFindAgain:
			{
				if (iContainer->Editor())
				{
						TInt aPos = -1;
						aPos = iContainer->Editor()->FindTextL(NULL,iContainer->Editor()->CursorPos(),0);
						if (aPos<0)
							aPos = iContainer->Editor()->FindTextL(NULL,0,0);
						if (aPos > -1)
						{
							iContainer->Editor()->SetCursorPosL(aPos,EFalse);
							iContainer->Editor()->SetCursorPosL(aPos + iLength,ETrue);
						}
				}

				if (iContainer->Sheet())
				{
					TPoint aPoint;
					if (iContainer->Sheet()->Model()->FindNextL(aPoint))
							iContainer->Sheet()->View()->SetCurrentCell(aPoint);
				}

				break;
			}
		case EMobileOfficeCmdStart:
		{
			if (iContainer->Editor())
			{
				iContainer->Editor()->SetCursorPosL(0, EFalse);
			}
			else
			{
				iContainer->Sheet()->View()->SetCurrentCell(TPoint(0,0));
			}

			break;
		}
		case EMobileOfficeCmdEnd:
		{
			if (iContainer->Editor())
			{
				iContainer->Editor()->SetCursorPosL(iContainer->Editor()->RichText()->DocumentLength(),EFalse);
			}
			else
			{
				iContainer->Sheet()->View()->SetCurrentCell(TPoint(0,app->OpenDocument()->CellEnd()));
			}
			break;
		}
		case EMobileOfficeCmdZoomNormal:
		{
			if (iContainer->Editor())
			{
			}
			else
			{
				iContainer->Sheet()->View()->SetZoomModeL( CSheetView::enZoomSmall );
			}
			break;
		}
		case EMobileOfficeCmdZoomMedium:
		{
			if (iContainer->Editor())
			{
			}
			else
			{
				iContainer->Sheet()->View()->SetZoomModeL( CSheetView::enZoomMedium );
			}
			break;
		}
		case EMobileOfficeCmdZoomLarge:
		{
			if (iContainer->Editor())
			{
			}
			else
			{
				iContainer->Sheet()->View()->SetZoomModeL( CSheetView::enZoomLarge );
			}
			break;
		}
		case EMobileOfficeCmdSheet:
		{
			if (iContainer->Sheet())
			{
				iContainer->Sheet()->View()->SetCellAjustmentMode( ETrue );
			}
			
			break;
		}
		case EMobileOfficeCmdFull:
		{
			// TODO: rect=rect not working??
			TRect r1 = iContainer->Rect();
			TRect r2 = ClientRect();

			TInt i = r1.Height();
			TInt s = r1.Width();

			TInt w = r2.Height();
			TInt y = r2.Width();

			if ( (i==w) && (s==y))
			{
				iContainer->SetExtentToWholeScreen();
			}
			else
			{
				iContainer->SetRect( ClientRect() );
			}
			break;
		}
		case EMobileOfficeCmdSave:
			{
				app->OpenDocument()->WriteFile(_L("c:\\wasgeht.odt"));
			}
        case EAknSoftkeyBack:
            { 

				//app->OpenDocument()->CancelOutstanding();
				app->OpenDocument()->ClearNote();
				app->HandleCommandL(EMobileOfficeCmdList);	
            break;
            }
		
		case EEikCmdExit:
			{
				app->OpenDocument()->ClearNote();
				app->HandleCommandL( EEikCmdExit );
				break;
			}
		case 10000:
		case 10001:
		case 10002:
		case 10003:
		case 10004:
		case 10005:
		case 10006:
		case 10007:
		case 10008:
		case 10009:
		case 10010:
			{
				iContainer->Sheet()->Model()->RemoveAllL();
				//iContainer->Sheet()->View()->ResetCellSizeL( const TSize& Size );
				//iContainer->Sheet()->View()->ResetCellTextColorL( KRgbBlack );
				//iContainer->Sheet()->View()->ResetCellColorL( KRgbWhite );
				User::Heap().Compress();
				app->OpenDocument()->GetContentSheet(iContainer->Sheet(),aCommand);	
				iContainer->Sheet()->View()->SetCurrentCell(TPoint(0,0));
				break;
			}
			
        default:
            {
            app->HandleCommandL( aCommand );
            break;
            }
        }
    }

// ---------------------------------------------------------
// CEditorViewView::HandleClientRectChange()
// ---------------------------------------------------------
//
void CEditorViewView::HandleClientRectChange()
    {
    if ( iContainer )
        {
        iContainer->SetRect( ClientRect() );
        }
    }

// ---------------------------------------------------------
// CEditorViewView::DoActivateL(...)
// 
// ---------------------------------------------------------
//

void CEditorViewView::DoActivateL(const TVwsViewId& /*aPrevViewId*/,TUid /*aCustomMessageId*/, const TDesC8& aCustomMessage)
    {
    if (!iContainer)
        {
        iContainer = new (ELeave) CEditorViewContainer;
        iContainer->SetMopParent(this);
        iContainer->ConstructL( ClientRect() );
        AppUi()->AddToStackL( *this, iContainer );
        } 
   }

// ---------------------------------------------------------
// CEditorViewView::DoDeactivate()
// 
// ---------------------------------------------------------
//
void CEditorViewView::DoDeactivate()
    {
    if ( iContainer )
        {
        AppUi()->RemoveFromViewStack( *this, iContainer );
        }
    
    delete iContainer;
    iContainer = NULL;
    }


void CEditorViewView::DynInitMenuPaneL(TInt aResourceId, CEikMenuPane *aMenuPane)
{
	CAknView::DynInitMenuPaneL(aResourceId, aMenuPane);

	if (aResourceId == R_MOBILEOFFICE_EDITORVIEW_TABLE)
	{
			aMenuPane->EnableMarqueeL(ETrue);
			STATIC_CAST(CMobileOfficeAppUi*,iEikonEnv->EikAppUi())->OpenDocument()->GetTables(aMenuPane);
	}

	if(aResourceId == R_MOBILEOFFICE_EDITORVIEW_MENU)
	{
		if (iContainer->Editor())		
		{
			aMenuPane->DeleteMenuItem( EMobileOfficeCmdSheet );
			aMenuPane->DeleteMenuItem( EMobileOfficeCmdZoom );
			aMenuPane->DeleteMenuItem( EMobileOfficeCmdTable );

			if ( STATIC_CAST(CMobileOfficeAppUi*,iEikonEnv->EikAppUi())->OpenDocument()->Editable() )
			{
				aMenuPane->DeleteMenuItem( EMobileOfficeCmdProp );
				aMenuPane->DeleteMenuItem( EMobileOfficeCmdPicture );		
			}
			else
			{
				// it is not editable, so we do not want the save button
				aMenuPane->DeleteMenuItem( EMobileOfficeCmdSave );	
			}
		}		
	}

	if(aResourceId == R_MOBILEOFFICE_EDITORVIEW_FIND)
	{
		if (iLength==0)
			aMenuPane->DeleteMenuItem( EMobileOfficeCmdFindAgain  );
		
	}

}

// End of File

