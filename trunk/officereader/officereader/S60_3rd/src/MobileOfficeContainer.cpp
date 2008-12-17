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
#include "MobileOfficeContainer.h"
#include "MobileOfficeAppUi.h"
#include "MobileOffice.hrh"
#include "MobileOfficeApp.h"
#include <MobileOffice.rsg> // resources

#include <eikclbd.h> 
#include <barsread.h>
#include <BAUTILS.H>	// delete file

#include <PathInfo.h>

// ================= MEMBER FUNCTIONS =======================

// ---------------------------------------------------------
// CMobileOfficeContainer::ConstructL(const TRect& aRect)
// EPOC two phased constructor
// ---------------------------------------------------------
//
void CMobileOfficeContainer::ConstructL(const TRect& aRect)
{
	CreateWindowL();
	SetRect(aRect);

	iListbox = new( ELeave )CAknSingleStyleListBox();
	iListbox->ConstructL(this, EAknListBoxSelectionList);
	iListbox->SetContainerWindowL(*this); 
	// add scrollbars to listbox 
	iListbox->CreateScrollBarFrameL(ETrue); 
	iListbox->ScrollBarFrame()->SetScrollBarVisibilityL( CEikScrollBarFrame::EAuto, CEikScrollBarFrame::EOn ); 
	iListbox->SetRect( Rect() );
	iListbox->ItemDrawer()->ColumnData()->EnableMarqueeL( ETrue );

	/*
    CAknFilteredTextListBoxModel* model = STATIC_CAST( CAknFilteredTextListBoxModel*, iListbox->Model() );
    iFindbox = CAknSearchField::NewL( *this, CAknSearchField::ESearch, NULL, 200);
    CleanupStack::PushL(iFindbox);
    model->CreateFilterL( iListbox, iFindbox );
    CleanupStack::Pop(iFindbox); // findbox	
	CAknColumnListBox* aknListBox = STATIC_CAST(CAknColumnListBox*,iListbox);
    AknFind::HandleFixedFindSizeChanged(this, aknListBox, iFindbox);
	*/

	iListbox->ActivateL();

	CEikStatusPane *sp = 
	( ( CAknAppUi* )iEikonEnv->EikAppUi() )->StatusPane();
        
	iNaviPane = ( CAknNavigationControlContainer * )
	sp->ControlL( TUid::Uid( EEikStatusPaneUidNavi ) );
 
	iNaviDecorator = iNaviPane->CreateTabGroupL();
 
	CAknTabGroup * tabGroup = (CAknTabGroup*) iNaviDecorator->DecoratedControl();
	tabGroup->AddTabL(0,_L("C:\\"));
	tabGroup->AddTabL(1,PathInfo::MemoryCardRootPath());
	tabGroup->SetActiveTabById(0);

	Search();

	ActivateL();
}

// Destructor
CMobileOfficeContainer::~CMobileOfficeContainer()
{
		delete iNaviDecorator;
		//delete iFindbox;
		delete iListbox;
}

// ---------------------------------------------------------
// CMobileOfficeContainer::SizeChanged()
// Called by framework when the view size is changed
// ---------------------------------------------------------
//
void CMobileOfficeContainer::SizeChanged()
{
	if (iListbox)
	{
		/*
		if (iFindbox)
        {
        	CAknColumnListBox* aknListBox = STATIC_CAST(CAknColumnListBox*, iListbox);
        	AknFind::HandleFixedFindSizeChanged(this, aknListBox, iFindbox);
        }
		*/
		iListbox->SetExtent( TPoint( 0, 0 ), iListbox->MinimumSize() );
	}

}

// ---------------------------------------------------------
// CMobileOfficeContainer::CountComponentControls() const
// ---------------------------------------------------------
//
TInt CMobileOfficeContainer::CountComponentControls() const
{
    return 1; // return nbr of controls inside this container
}

// ---------------------------------------------------------
// CMobileOfficeContainer::ComponentControl(TInt aIndex) const
// ---------------------------------------------------------
//
CCoeControl* CMobileOfficeContainer::ComponentControl(TInt aIndex) const
    {
    switch ( aIndex )
        {
        case 0:
			return iListbox;
       /*case 1:
			return iFindbox;
		*/
        default:
            return NULL;
        }
    }

// ---------------------------------------------------------
// CMobileOfficeContainer::Draw(const TRect& aRect) const
// ---------------------------------------------------------
//
void CMobileOfficeContainer::Draw(const TRect& aRect) const
           {
	CWindowGc& gc = SystemGc();
	gc.Clear(aRect);
    }


// ---------------------------------------------------------
// CMobileOfficeContainer::HandleControlEventL(
//     CCoeControl* aControl,TCoeEvent aEventType)
// ---------------------------------------------------------
//
void CMobileOfficeContainer::HandleControlEventL(
    CCoeControl* /*aControl*/,TCoeEvent /*aEventType*/)
    {
    // TODO: Add your control event handler code here
    }


void CMobileOfficeContainer::AddItem(TDes& Text)
{	
	TBool bAdd = ETrue;
	COpenDocument* iDoc = STATIC_CAST(CMobileOfficeAppUi*,iEikonEnv->EikAppUi())->OpenDocument();
	CDesC16Array * list_array = ( CDesC16Array * ) iListbox->Model()->ItemTextArray();

	if ( (Text.Find(_L(".pdf")) != KErrNotFound) && !iDoc->IsHybridPDF(Text) )
			bAdd = EFalse;
	if (bAdd)
	{
		Text.Insert(0,_L("\t"));
		list_array->AppendL(Text);
		iListbox->HandleItemAdditionL();
	}
}

void CMobileOfficeContainer::ClearListbox(void)
{
	CDesC16Array * list_array = ( CDesC16Array * ) iListbox->Model()->ItemTextArray();
	list_array->Reset();
	iListbox->HandleItemRemovalL();
}

TKeyResponse CMobileOfficeContainer::OfferKeyEventL(const TKeyEvent& aKeyEvent,TEventCode aType)
{

	CMobileOfficeAppUi* app = (CMobileOfficeAppUi*)iAvkonAppUi;
 
	CAknTabGroup* iTabGroup = (CAknTabGroup*) iNaviDecorator->DecoratedControl();
	if ( iTabGroup )
	{
		TInt active = iTabGroup->ActiveTabIndex();
		switch ( aKeyEvent.iCode )
			{
				case EKeyLeftArrow:
				{
					if ( active == 1 )
					{
						iTabGroup->SetActiveTabById(0);
						ClearListbox();
						Search();
						return EKeyWasConsumed;
					}
					else
						return EKeyWasNotConsumed;
					break;
				}
				case EKeyRightArrow:
				{
					if ( active == 0 )
					{
						iTabGroup->SetActiveTabById(1);
						ClearListbox();
						Search();
						return EKeyWasConsumed;
					}
					else
						return EKeyWasNotConsumed;
					break;
				}
				default:
					break;
			}
	}

	if (aKeyEvent.iCode == EKeyBackspace)
	{
			app->CheckDemo();
	
			TInt ListCount;
			CTextListBoxModel* model =  iListbox->Model();  // not taking ownership
			model->SetOwnershipType (ELbmOwnsItemArray);
			CDesCArray* itemArray = STATIC_CAST(CDesCArray*, model->ItemTextArray());
			ListCount=itemArray->Count();

			if (ListCount>0)
			{
				Delete();
			}
			return EKeyWasConsumed;
	}
	
	if ( (iListbox->CurrentItemIndex()!=-1) && ( (aKeyEvent.iCode == EKeyDevice3) || (aKeyEvent.iCode == EKeyEnter))) 
	{
		if (BaflUtils::FileExists(iCoeEnv->FsSession(),ListboxText()))
		{
			app->OpenDocument()->SetFileName(ListboxText());
			iAvkonAppUi->HandleCommandL(EMobileOfficeCmdOpen);	 
		}
		else
		{	
			ClearListbox();
			Search();
		}
		return EKeyWasConsumed;
	}

	if(iListbox)
	{
		/*
		if ( iFindbox )
		{
           TBool needRefresh( EFalse );
            
            // Offers the key event to find box.
            if ( AknFind::HandleFindOfferKeyEventL( aKeyEvent, aType, this, iListbox, iFindbox,EFalse, needRefresh ) == EKeyWasConsumed )
            {
               	if ( needRefresh )
                {
              	    SizeChanged();
                  	DrawNow();
                }
 
               	return EKeyWasConsumed;
            }
		}
		*/
		return iListbox->OfferKeyEventL(aKeyEvent,aType);
	}
	
	return EKeyWasNotConsumed;
}

TPtrC CMobileOfficeContainer::ListboxText()
{
	TPtrC temp;
	temp.Set(iListbox->Model()->ItemText( iListbox->CurrentItemIndex()));
	temp.Set(temp.Right(temp.Length()-1));	
	return temp;
}


void CMobileOfficeContainer::SearchFiles(RFs& aFs,const TDesC& aPath, const TDesC& aWild)
{
	CDirScan* scan = CDirScan::NewLC(aFs); 
   scan->SetScanDataL(aPath, KEntryAttDir|KEntryAttMatchExclusive, ESortNone, CDirScan::EScanDownTree); 

   FOREVER 
      { 
      CDir* dir = NULL; 
      TRAPD(error, scan->NextL(dir)); 
      if (error || !dir) 
         break; 

		delete dir; 

      ScanDirL(aFs, scan->FullPath(), aWild); 
      }; 
   CleanupStack::PopAndDestroy(scan); 
  
}

void CMobileOfficeContainer::Search(void)
{
   CAknTabGroup* iTabGroup = (CAknTabGroup*) iNaviDecorator->DecoratedControl();
 
    switch (iTabGroup->ActiveTabId())
	{
		case 0: 
		{
			ScanDirL(iCoeEnv->FsSession(),_L("C:\\"),_L("*.odt"));
			ScanDirL(iCoeEnv->FsSession(),_L("C:\\"),_L("*.ott"));
			ScanDirL(iCoeEnv->FsSession(),_L("C:\\"),_L("*.ods"));
			ScanDirL(iCoeEnv->FsSession(),_L("C:\\"),_L("*.ots"));
			ScanDirL(iCoeEnv->FsSession(),_L("C:\\"),_L("*.odp"));
			ScanDirL(iCoeEnv->FsSession(),_L("C:\\"),_L("*.otp"));
			ScanDirL(iCoeEnv->FsSession(),_L("C:\\"),_L("*.pdf"));

			SearchFiles(iCoeEnv->FsSession(),PathInfo::PhoneMemoryRootPath(),_L("*.odt"));
			SearchFiles(iCoeEnv->FsSession(),PathInfo::PhoneMemoryRootPath(),_L("*.ott"));
			SearchFiles(iCoeEnv->FsSession(),PathInfo::PhoneMemoryRootPath(),_L("*.ods"));
			SearchFiles(iCoeEnv->FsSession(),PathInfo::PhoneMemoryRootPath(),_L("*.ots"));
			SearchFiles(iCoeEnv->FsSession(),PathInfo::PhoneMemoryRootPath(),_L("*.odp"));
			SearchFiles(iCoeEnv->FsSession(),PathInfo::PhoneMemoryRootPath(),_L("*.otp"));
			SearchFiles(iCoeEnv->FsSession(),PathInfo::PhoneMemoryRootPath(),_L("*.pdf"));
			break;
		}
		case 1: 
		{
			ScanDirL(iCoeEnv->FsSession(),PathInfo::MemoryCardRootPath(),_L("*.odt"));
			ScanDirL(iCoeEnv->FsSession(),PathInfo::MemoryCardRootPath(),_L("*.ott"));
			ScanDirL(iCoeEnv->FsSession(),PathInfo::MemoryCardRootPath(),_L("*.ods"));
			ScanDirL(iCoeEnv->FsSession(),PathInfo::MemoryCardRootPath(),_L("*.ots"));
			ScanDirL(iCoeEnv->FsSession(),PathInfo::MemoryCardRootPath(),_L("*.odp"));
			ScanDirL(iCoeEnv->FsSession(),PathInfo::MemoryCardRootPath(),_L("*.otp"));
			ScanDirL(iCoeEnv->FsSession(),PathInfo::MemoryCardRootPath(),_L("*.pdf"));

			ScanDirL(iCoeEnv->FsSession(),_L("E:\\Documents\\"),_L("*.odt"));
			ScanDirL(iCoeEnv->FsSession(),_L("E:\\Documents\\"),_L("*.ott"));
			ScanDirL(iCoeEnv->FsSession(),_L("E:\\Documents\\"),_L("*.ods"));
			ScanDirL(iCoeEnv->FsSession(),_L("E:\\Documents\\"),_L("*.ots"));
			ScanDirL(iCoeEnv->FsSession(),_L("E:\\Documents\\"),_L("*.odp"));
			ScanDirL(iCoeEnv->FsSession(),_L("E:\\Documents\\"),_L("*.otp"));
			ScanDirL(iCoeEnv->FsSession(),_L("E:\\Documents\\"),_L("*.pdf"));

			SearchFiles(iCoeEnv->FsSession(),_L("E:\\Data\\"),_L("*.odt"));
			SearchFiles(iCoeEnv->FsSession(),_L("E:\\Data\\"),_L("*.ott"));
			SearchFiles(iCoeEnv->FsSession(),_L("E:\\Data\\"),_L("*.ods"));
			SearchFiles(iCoeEnv->FsSession(),_L("E:\\Data\\"),_L("*.ots"));
			SearchFiles(iCoeEnv->FsSession(),_L("E:\\Data\\"),_L("*.odp"));
			SearchFiles(iCoeEnv->FsSession(),_L("E:\\Data\\"),_L("*.otp"));
			SearchFiles(iCoeEnv->FsSession(),_L("E:\\Data\\"),_L("*.pdf"));
			break;
		}
	}

	CTextListBoxModel* model =  iListbox->Model();  // not taking ownership
	model->SetOwnershipType (ELbmOwnsItemArray);
	CDesCArray* itemArray = STATIC_CAST(CDesCArray*, model->ItemTextArray());
	if (itemArray->Count()>0)
		iListbox->SetCurrentItemIndex(0);
		
	iListbox->DrawNow();
}



void CMobileOfficeContainer::ScanDirL(RFs& aFs, const TDesC& aDir, const TDesC& aWild) 
{ 
   TParse parse; 
   parse.Set(aWild, &aDir, NULL); 
   TPtrC spec(parse.FullName()); 

   TFindFile find(aFs); 
   CDir* dir; 
    
   if (!find.FindWildByPath(parse.FullName(), NULL, dir)) 
      { 
      CleanupStack::PushL(dir); 

      for(int i = 0; i < dir->Count(); i++) 
         { 
         parse.Set((*dir)[i].iName, &spec, NULL); 
	
		 TBuf<256> Name;
		Name.Copy(parse.FullName());
		AddItem(Name); 
         } 

      CleanupStack::PopAndDestroy(dir); 
      } 
   } 

void CMobileOfficeContainer::HandleResourceChange(TInt aType)
{
	CCoeControl::HandleResourceChange(aType);

	if (aType==KEikDynamicLayoutVariantSwitch)
	{
		TRect aRect;
		AknLayoutUtils::LayoutMetricsRect(AknLayoutUtils::EMainPane,aRect);
		SetRect(aRect);
	}
}

TInt CMobileOfficeContainer::ListboxIndex(void)
{
	return iListbox->CurrentItemIndex();
}

void CMobileOfficeContainer::Delete()
{
	CDesC16Array * list_array = ( CDesC16Array * ) iListbox->Model()->ItemTextArray();
	if ( (BaflUtils::DeleteFile(iCoeEnv->FsSession(),ListboxText())==KErrNone) && (iListbox->CurrentItemIndex() >-1))
	{
		list_array->Delete(iListbox->CurrentItemIndex(),1);
		iListbox->HandleItemRemovalL();
		iListbox->DrawNow();
	}
}

void CMobileOfficeContainer::ShowTab()
{
	iNaviPane->PushL(*iNaviDecorator);
}


void CMobileOfficeContainer::HideTab()
{
	iNaviPane->Pop();
}

void CMobileOfficeContainer::GetHelpContext(TCoeHelpContext &aContext) const
{
	aContext.iContext = _L("File_Overview");
	aContext.iMajor = KUidMobileOffice;
}
