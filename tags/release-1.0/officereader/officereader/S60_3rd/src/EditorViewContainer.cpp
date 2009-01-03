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

#include <barsread.h>
#include <GDI.H> //zoomfactor
#include <FRMTVIEW.H> // cursor visibility
#include <aknnotewrappers.h> 
#include <AknQueryDialog.h>  // findquery
#include <stringloader.h>
#include <s32file.h>

#include "EditorViewContainer.h"
#include "MobileOfficeAppUi.h"
#include  "EditorViewView.h"
#include  "MobileOffice.hrh"
#include "MobileOfficeApp.h"
#include <MobileOffice.rsg> // resources


#define KEditorPosition TPoint(0,0) 


HBufC* ConvertTo16BitL1(const TDesC8& aDes8)
{
   HBufC* myBuf = HBufC::NewL(aDes8.Length());
   myBuf->Des().Copy(aDes8);
   return myBuf;
}

// ================= MEMBER FUNCTIONS =======================

// ---------------------------------------------------------
// CEditorViewContainer::ConstructL(const TRect& aRect)
// EPOC two phased constructor
// ---------------------------------------------------------
//
void CEditorViewContainer::ConstructL(const TRect& aRect)
{

	COpenDocument* iDoc = STATIC_CAST(CMobileOfficeAppUi*,iEikonEnv->EikAppUi())->OpenDocument();
    
	CreateWindowL();	
	SetRect(aRect);

	switch ( iDoc->GetType() )
	{
		
			case ODT:
			case OTT: { 
						SetupText();
						break; 
					}
			case ODP: 
			case OTP: { 
						//SetupPresentation();
						SetupText();
						break; 
					}
			case ODS: 
			case OTS: { 
						SetupSheet();
						break; 
					}
			case ODFUnknown:
				{
					CAknErrorNote* note = new (ELeave) CAknErrorNote();
					note->ExecuteLD(_L("This is not a known ODF file"));
					break;
				}
	}
	
	ActivateL();

	if (iEditor) 
	{
		iEditor->SetFocus(ETrue);
		iEditor->SetCursorPosL(0, EFalse);
	
		if (!iDoc->Editable())
		{
			iEditor->TextView()->SetCursorVisibilityL(TCursor::EFCursorInvisible,TCursor::EFCursorInvisible);
		}
	}
}

// Destructor
CEditorViewContainer::~CEditorViewContainer()
{
		if (iEditor)
		{
			delete iEditor;
			iEditor =NULL;
		}

		if (iSheet)
		{
			delete iSheet;
			iSheet = NULL;
		}
}


void CEditorViewContainer::SetupSheet(void)
{	
	iSheet = new ( ELeave ) CSheetCtrl;
	iSheet->SetContainerWindowL( *this );
	iSheet->SetMopParent( this );
	iSheet->ConstructL();
	iSheet->SetItemViewPos( ETrue );
	iSheet->Data()->SetLongTextShowMode( CSheetData::enOverNeighbour );
	iSheet->SetRect(Rect());

	COpenDocument* iDoc = STATIC_CAST(CMobileOfficeAppUi*,iEikonEnv->EikAppUi())->OpenDocument();

	if (iDoc->Encrypted())
	{
		TBuf<50> text;
		CAknTextQueryDialog* FindQuery = CAknTextQueryDialog::NewL(text);
		CleanupStack::PushL(FindQuery);
		HBufC* prompt = prompt = StringLoader::LoadLC(R_PASSWORD_QUERY);
		FindQuery->SetPromptL(*prompt);
		CleanupStack::PopAndDestroy(prompt);
		CleanupStack::Pop(FindQuery);
		if (FindQuery->ExecuteLD(R_DATAQUERY_DATA_QUERY))
		{
			iDoc->SetPassword(text);
			iDoc->GetContentSheet(iSheet,ETableIndex-1);
		}
	}
	else	
		iDoc->GetContentSheet(iSheet,ETableIndex-1);
	
}

void CEditorViewContainer::SetupText(void)
{
	COpenDocument* iDoc = STATIC_CAST(CMobileOfficeAppUi*,iEikonEnv->EikAppUi())->OpenDocument();

	iEditor = new (ELeave) CEikRichTextEditor();
	iEditor->SetContainerWindowL(*this);
	if (iDoc->Editable())
	{
		iEditor->ConstructL(this,1000,1000,CEikEdwin::EAllowPictures,EGulFontControlAll,EGulAllFonts);
	}
	else
	{
		iEditor->ConstructL(this,1000,1000,CEikEdwin::EAllowPictures | CEikEdwin::EReadOnly ,EGulFontControlAll,EGulAllFonts);
	}

	iEditor->SetRect(TRect(TSize(Rect().Width() - 10,Rect().Height())));	
	iEditor-> CreateScrollBarFrameL()->SetScrollBarVisibilityL( CEikScrollBarFrame::EOff, CEikScrollBarFrame::EOn );

	if (iDoc->Encrypted())
	{
		
		TBuf<50> text;
		CAknTextQueryDialog* FindQuery = CAknTextQueryDialog::NewL(text);
		CleanupStack::PushL(FindQuery);
		HBufC* prompt = prompt = StringLoader::LoadLC(R_PASSWORD_QUERY);
		FindQuery->SetPromptL(*prompt);
		CleanupStack::PopAndDestroy(prompt);
		CleanupStack::Pop(FindQuery);
		if (FindQuery->ExecuteLD(R_DATAQUERY_DATA_QUERY))
		{
			 iDoc->SetPassword(text);
			 iDoc->GetContentText( iEditor);
		}
		
					
	}
	else
	{
			iDoc->GetContentText( iEditor);
	}
	

	iEditor->ActivateL();
}

// ---------------------------------------------------------
// CEditorViewContainer::SizeChanged()
// Called by framework when the view size is changed
// ---------------------------------------------------------
//
void CEditorViewContainer::SizeChanged()
    {
	if (iEditor)
			iEditor->SetRect(TRect(TSize(Rect().Width() - 10,Rect().Height())));	
				
	/*
		if (iSheet)
			iSheet->SetExtent( KEditorPosition , Rect().Size() );
		*/
	if (iSheet)
		iSheet->SetRect(Rect());

    }

// ---------------------------------------------------------
// CEditorViewContainer::CountComponentControls() const
// ---------------------------------------------------------
//
TInt CEditorViewContainer::CountComponentControls() const
    {
    return 1; // return nbr of controls inside this container
    }

// ---------------------------------------------------------
// CEditorViewContainer::ComponentControl(TInt aIndex) const
// ---------------------------------------------------------
//
CCoeControl* CEditorViewContainer::ComponentControl(TInt aIndex) const
    {
    switch ( aIndex )
        {
        case 0:
            if (iEditor)
				 return iEditor;
			if (iSheet)
				return iSheet;
        default:
            return NULL;
        }
    }

// ---------------------------------------------------------
// CEditorViewContainer::Draw(const TRect& aRect) const
// ------------------------------------------------	---------
//
void CEditorViewContainer::Draw(const TRect& aRect) const
{
	CWindowGc& gc = SystemGc();
	gc.Clear(aRect);
}

// ---------------------------------------------------------
// CEditorViewContainer::HandleControlEventL(
//     CCoeControl* aControl,TCoeEvent aEventType)
// ---------------------------------------------------------
//
void CEditorViewContainer::HandleControlEventL(
    CCoeControl* /*aControl*/,TCoeEvent /*aEventType*/)
{
   // TODO: Add your control event handler code here
}

TKeyResponse CEditorViewContainer::OfferKeyEventL(const TKeyEvent& aKeyEvent,TEventCode aType)
{
	COpenDocument* iDoc = STATIC_CAST(CMobileOfficeAppUi*,iEikonEnv->EikAppUi())->OpenDocument();

	if (iEditor) {
		if (aType == EEventKey)
		{
			if (!iDoc->Editable())
			{
				if (aKeyEvent.iCode == EKeyDownArrow)
					{
					iEditor->MoveCursorL (TCursorPosition::EFPageDown, EFalse);
					return EKeyWasConsumed;
					}	
				else if (aKeyEvent.iCode == EKeyUpArrow)	
					{
					iEditor->MoveCursorL (TCursorPosition::EFPageUp, EFalse);
					return EKeyWasConsumed;
					}
				else if (aKeyEvent.iCode == EKeyLeftArrow)
					{
							iDoc->GetPreviousNote(iEikonEnv);
							return EKeyWasConsumed;
					}
				else if (aKeyEvent.iCode == EKeyRightArrow)
					{
							iDoc->GetNextNote(iEikonEnv);
							return EKeyWasConsumed;
					}
				else if ( (aKeyEvent.iCode == EKeyDevice3) || (aKeyEvent.iCode == EKeyEnter) )
					{
           					iDoc->GetCurrentNote(iEikonEnv,ETrue);
							return EKeyWasConsumed;
					}
				else if (aKeyEvent.iCode == 42)
				{
					STATIC_CAST(CMobileOfficeAppUi*,iEikonEnv->EikAppUi())->View(KEditorViewViewId)->HandleCommandL(EMobileOfficeCmdStart);
					return EKeyWasConsumed;
				}
				else if (aKeyEvent.iCode == 35)
				{
					STATIC_CAST(CMobileOfficeAppUi*,iEikonEnv->EikAppUi())->View(KEditorViewViewId)->HandleCommandL(EMobileOfficeCmdEnd);
					return EKeyWasConsumed;
				}
				else if (aKeyEvent.iCode == 48)
				{
					STATIC_CAST(CMobileOfficeAppUi*,iEikonEnv->EikAppUi())->View(KEditorViewViewId)->HandleCommandL(EMobileOfficeCmdFull);
					return EKeyWasConsumed;
				}
				else 
		//			return iEditor->OfferKeyEventL(aKeyEvent, aType);
					return EKeyWasNotConsumed;
			}
			else
			{
				return iEditor->OfferKeyEventL(aKeyEvent, aType);
			}
        }
		return EKeyWasNotConsumed;
	}

	if (iSheet)
	{
		if (aKeyEvent.iCode == 42)
		{
			STATIC_CAST(CMobileOfficeAppUi*,iEikonEnv->EikAppUi())->View(KEditorViewViewId)->HandleCommandL(EMobileOfficeCmdStart);
			return EKeyWasConsumed;
		}
		else if (aKeyEvent.iCode == 35)
		{
			STATIC_CAST(CMobileOfficeAppUi*,iEikonEnv->EikAppUi())->View(KEditorViewViewId)->HandleCommandL(EMobileOfficeCmdEnd);
			return EKeyWasConsumed;
		}
		else if (aKeyEvent.iCode == 48)
		{
			STATIC_CAST(CMobileOfficeAppUi*,iEikonEnv->EikAppUi())->View(KEditorViewViewId)->HandleCommandL(EMobileOfficeCmdFull);
			return EKeyWasConsumed;
		}
	}
 
	return CCoeControl::OfferKeyEventL(aKeyEvent, aType);
}

CEikRichTextEditor* CEditorViewContainer::Editor()
{
	return iEditor;
}

CSheetCtrl* CEditorViewContainer::Sheet()
{
	return iSheet;
}

void CEditorViewContainer::HandleResourceChange(TInt aType)
{
	CCoeControl::HandleResourceChange(aType);

	if (aType==KEikDynamicLayoutVariantSwitch)
	{
		TRect aRect;
		AknLayoutUtils::LayoutMetricsRect(AknLayoutUtils::EMainPane,aRect);
		SetRect(aRect);
		/*
		if (iEditor)		
			iEditor->SetRect(TRect(TSize(Rect().Width() - 10,Rect().Height())));
		if (iSheet)
			iSheet->SetRect(aRect);
			*/
	}	

}

void CEditorViewContainer::GetHelpContext(TCoeHelpContext &aContext) const
{
	aContext.iContext = _L("Viewing_a_file");
	aContext.iMajor = KUidMobileOffice;
}

// End of File  
