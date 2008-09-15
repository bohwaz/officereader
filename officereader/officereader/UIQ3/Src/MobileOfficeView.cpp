#include <QikCommand.h>

#include <eikclbd.h> 
#include <barsread.h>
#include <BAUTILS.H>	// delete file
#include <QikSimpleDialog.h>
#include <EIKAPPUI.H>


#include <MobileOffice.rsg>
#include "MobileOfficeAppUi.h"
#include "MobileOfficeView.h"
#include "MobileOffice.hrh"
#include "MobileOfficeExternalInterface.h"

#include <QFiles.mbg>

#include <apparc.h>


/** Max size for TInt in text. */
const TInt KMaxSizeOfTIntInBytes = 11;

/** Max size for the default name defined in the resource */
const TInt KMaxSizeOfDefaultName = 32;

_LIT(cTab,"C:\\");
_LIT(dTab,"D:\\");
_LIT(c,"C:\\Media Files\\Document\\");
_LIT(d,"D:\\Document\\");

/**
Creates and constructs the view.

@param aAppUi Reference to the AppUi
@return Pointer to a CMobileOfficeView object
*/
CMobileOfficeView* CMobileOfficeView::NewLC(CQikAppUi& aAppUi)
	{
	CMobileOfficeView* self = new(ELeave) CMobileOfficeView(aAppUi);
	CleanupStack::PushL(self);
	self->ConstructL();
	return self;
	}

/**
Constructor for the view.
Passes the application UI reference to the construction of the super class.

KNullViewId should normally be passed as parent view for the applications
default view. The parent view is the logical view that is normally activated
when a go back command is issued. KNullViewId will activate the system
default view.

@param aAppUi Reference to the application UI
*/
CMobileOfficeView::CMobileOfficeView(CQikAppUi& aAppUi)
	: CQikViewBase(aAppUi, KNullViewId)
	{
	}

/**
Destructor for the view
*/
CMobileOfficeView::~CMobileOfficeView()
	{
	}

/**
2nd stage construction of the App UI.
*/
void CMobileOfficeView::ConstructL()
	{
	// Calls ConstructL that initialises the standard values.
	// This should always be called in the concrete view implementations.
	BaseConstructL();
	}

/**
Inherited from CQikViewBase and called upon by the UI Framework.
It creates the view from resource.
*/
void CMobileOfficeView::ViewConstructL()
	{
	// Loads information about the UI configurations this view supports
	// together with definition of each view.
	ViewConstructFromResourceL(R_MOBILEOFFICE_UI_CONFIGURATIONS);

	_LIT( KIconFile,"z:\\resource\\apps\\QFiles.mbm");
	//TFileName mbmFile( KIconFile );
	
	CFbsBitmap* pic1 = new (ELeave) CFbsBitmap();
	User::LeaveIfError(pic1->Load(KIconFile, EMbmQfilesInternalmemory));

	CFbsBitmap* mask1 = new (ELeave) CFbsBitmap();
	User::LeaveIfError(mask1->Load(KIconFile, EMbmQfilesInternalmemory_mask));

	CFbsBitmap* pic2 = new (ELeave) CFbsBitmap();
	User::LeaveIfError(pic2->Load(KIconFile, EMbmQfilesExternalmemory));

	CFbsBitmap* mask2 = new (ELeave) CFbsBitmap();
	User::LeaveIfError(mask2->Load(KIconFile, EMbmQfilesExternalmemory_mask));

	AddTabL(1,KNullDesC,pic1,mask1);
	AddTabL(2,KNullDesC,pic2,mask2);

	CQikListBox* listBox = LocateControlByUniqueHandle<CQikListBox>(EListBoxListViewListCtrl);
	listBox->SetListBoxObserver(this);

	ActivateTabL(1);


	#ifndef FREEVERSION
		CMobileOfficeAppUi* app = (CMobileOfficeAppUi*)iEikonEnv->EikAppUi();
		if (!app->Registered())
		{
			CQikCommandManager& cmdManager = CQikCommandManager::Static();
			cmdManager.SetInvisible(*this,EMobileOfficeCmdRegister,ETrue);
		}
	#else
		CQikCommandManager& cmdManager = CQikCommandManager::Static();
		cmdManager.SetInvisible(*this,EMobileOfficeCmdRegister,ETrue);
	#endif
}

void CMobileOfficeView::AddItem(const TDesC& Text)
{
	const CQikListBox* listBox = LocateControlByUniqueHandle<const CQikListBox>(EListBoxListViewListCtrl);
	MQikListBoxModel& model(listBox->Model());

	//model.ModelBeginUpdateLC();

	// Create a listBoxData item, the data returned is Open().
	MQikListBoxData* listBoxData = model.NewDataL(MQikListBoxModel::EDataNormal);
	// Pushes the data onto the cleanup stack. 
	// When CleanupStack::PopAndDestroy() is called, the data will be closed.
	CleanupClosePushL(*listBoxData);

	// Adds the text that will be visible in the list box
	listBoxData->AddTextL(Text, EQikListBoxSlotText1);
	
	// Removes the listBoxData from the stack and calls close on listBoxData
	CleanupStack::PopAndDestroy(listBoxData);

	// Informs that the update of the list box model has ended
	//model.ModelEndUpdateL();
	
}


/**
Returns the view Id

@return Returns the Uid of the view
*/
TVwsViewId CMobileOfficeView::ViewId()const
	{
	return TVwsViewId(KUidMobileOfficeApp, KUidMobileOfficeView);
	}

/*
Handles all commands in the view
Called by the UI framework when a command has been issued.
The command Ids are defined in the .hrh file.

@param aCommand The command to be executed
@see CQikViewBase::HandleCommandL
*/
void CMobileOfficeView::HandleCommandL(CQikCommand& aCommand)
{
	TFileName aFileName;
	TInt ListCount;


	
	CMobileOfficeAppUi* app = (CMobileOfficeAppUi*)iEikonEnv->EikAppUi();
	app->CheckDemo();

	const CQikListBox* iListbox = LocateControlByUniqueHandle<const CQikListBox>(EListBoxListViewListCtrl);
	ListCount = iListbox->Model().Count();

	if(ListCount>0)
	{

		if (ActiveTabId() ==1)
		{
			aFileName.Insert(0,c);
		}	
		else
		{
			aFileName.Insert(0,d);
		}
		MQikListBoxData* listBoxData = iListbox->Model().RetrieveDataL( iListbox->CurrentItemIndex());
		CleanupClosePushL(*listBoxData);
		aFileName.Append(listBoxData->Text(EQikListBoxSlotText1,0));

		app->OpenDocument()->SetFileName(aFileName);
		app->OpenDocument()->SetNextViewID(KUidMobileOfficeView);


		if (!BaflUtils::FileExists(iCoeEnv->FsSession(),aFileName))
		{
			ClearListbox();
			if (ActiveTabId()==1)
				SearchC();
			else
				SearchE();
		}
		else
		{
			switch(aCommand.Id())
			{
				case EMobileOfficeCmdOpen:	
					{
						iEikonEnv->EikAppUi()->ActivateViewL(TVwsViewId(KUidMobileOfficeApp,KUidMobileOfficeEditorView));
						break;
					}
				case EMobileOfficeCmdProp:
					{
							iEikonEnv->EikAppUi()->ActivateViewL(TVwsViewId(KUidMobileOfficeApp,KUidMobileOfficePropView));
						break;
					}
				case EMobileOfficeCmdPicture:
					{
						iEikonEnv->EikAppUi()->ActivateViewL(TVwsViewId(KUidMobileOfficeApp,KUidMobileOfficeImageView));
						break;
					}
			}
		}

		CleanupStack::PopAndDestroy(listBoxData);	
	}

	switch(aCommand.Id())
		{
		case EMobileOfficeCmdNewText:
			{
				app->OpenDocument()->SetEditable(ETrue);
				app->OpenDocument()->SetNextViewID(KUidMobileOfficeView);
				iEikonEnv->EikAppUi()->ActivateViewL(TVwsViewId(KUidMobileOfficeApp,KUidMobileOfficeEditorView));
				break;
			}
			case EMobileOfficeCmdDelete:
				{
					// count überprüfung in Delete
					Delete();
					break;
				}
			case EMobileOfficeCmdRefresh:
				{
					ClearListbox();
					if (ActiveTabId() ==1)
					{
						SearchC();
					}
					else
					{
						SearchE();
					}
					break;
				}
#ifndef FREEVERSION
			case EMobileOfficeCmdRegister:
			{
				app->Register();

				if (!app->Registered())
				{
					CQikCommandManager& cmdManager = CQikCommandManager::Static();
					cmdManager.SetInvisible(*this,EMobileOfficeCmdRegister,ETrue);
				};

				break;
			
			}
#endif
			case EMobileOfficeCmdAppAbout:
				{
					CQikSimpleDialog* dialog = new(ELeave) CQikSimpleDialog;
					dialog->PrepareLC(R_MY_SIMPLE_ABOUT_DIALOG);
					dialog->RunL();
					CleanupStack::PopAndDestroy(dialog);
					break;
				}
			case EEikCmdExit:
            {
				/*
				CEikAppUi *app = iEikonEnv->EikAppUi();			
				if (app->iDoorObserver)
					app->iDoorObserver->NotifyExit( MApaEmbeddedDocObserver::ENoChanges );
				*/
				
				CQikViewBase::HandleCommandL(aCommand);
				break;
            }
		default:
			CQikViewBase::HandleCommandL(aCommand);
			break;
		}
}

void CMobileOfficeView::SearchC(void)
{
	CQikListBox* listBox = LocateControlByUniqueHandle<CQikListBox>(EListBoxListViewListCtrl);

	listBox->Model().ModelBeginUpdateLC();


	ScanDirL(iCoeEnv->FsSession(),_L("C:\\Media files\\Document\\"),_L("*.odt"));
	ScanDirL(iCoeEnv->FsSession(),_L("C:\\Media files\\Document\\"),_L("*.ott"));
	ScanDirL(iCoeEnv->FsSession(),_L("C:\\Media files\\Document\\"),_L("*.ods"));
	ScanDirL(iCoeEnv->FsSession(),_L("C:\\Media files\\Document\\"),_L("*.ots"));
	ScanDirL(iCoeEnv->FsSession(),_L("C:\\Media files\\Document\\"),_L("*.odp"));
	ScanDirL(iCoeEnv->FsSession(),_L("C:\\Media files\\Document\\"),_L("*.otp"));

	/*
	ScanDirL(iCoeEnv->FsSession(),_L("C:\\"),_L("*.odt"));
	ScanDirL(iCoeEnv->FsSession(),_L("C:\\"),_L("*.ott"));
	ScanDirL(iCoeEnv->FsSession(),_L("C:\\"),_L("*.ods"));
	ScanDirL(iCoeEnv->FsSession(),_L("C:\\"),_L("*.ots"));
	ScanDirL(iCoeEnv->FsSession(),_L("C:\\"),_L("*.odp"));
	ScanDirL(iCoeEnv->FsSession(),_L("C:\\"),_L("*.otp"));

	
	SearchFiles(iCoeEnv->FsSession(),_L("C:\\Data\\"),_L("*.odt"));
	SearchFiles(iCoeEnv->FsSession(),_L("C:\\Data\\"),_L("*.ott"));
	SearchFiles(iCoeEnv->FsSession(),_L("C:\\Data\\"),_L("*.ods"));
	SearchFiles(iCoeEnv->FsSession(),_L("C:\\Data\\"),_L("*.ots"));
	SearchFiles(iCoeEnv->FsSession(),_L("C:\\Data\\"),_L("*.odp"));
	SearchFiles(iCoeEnv->FsSession(),_L("C:\\Data\\"),_L("*.otp"));
	*/

	listBox->Model().ModelEndUpdateL();
}

void CMobileOfficeView::SearchE(void)
{
	CQikListBox* listBox = LocateControlByUniqueHandle<CQikListBox>(EListBoxListViewListCtrl);
	listBox->Model().ModelBeginUpdateLC();

	//SearchFiles(iCoeEnv->FsSession(),_L("D:\\"),_L("*.odt"));
	
	ScanDirL(iCoeEnv->FsSession(),_L("D:\\Document\\"),_L("*.odt"));
	ScanDirL(iCoeEnv->FsSession(),_L("D:\\Document\\"),_L("*.ott"));
	ScanDirL(iCoeEnv->FsSession(),_L("D:\\Document\\"),_L("*.ods"));
	ScanDirL(iCoeEnv->FsSession(),_L("D:\\Document\\"),_L("*.ots"));
	ScanDirL(iCoeEnv->FsSession(),_L("D:\\Document\\"),_L("*.odp"));
	ScanDirL(iCoeEnv->FsSession(),_L("D:\\Document\\"),_L("*.otp"));

	/*
	SearchFiles(iCoeEnv->FsSession(),_L("E:\\Data\\"),_L("*.odt"));
	SearchFiles(iCoeEnv->FsSession(),_L("E:\\Data\\"),_L("*.ott"));
	SearchFiles(iCoeEnv->FsSession(),_L("E:\\Data\\"),_L("*.ods"));
	SearchFiles(iCoeEnv->FsSession(),_L("E:\\Data\\"),_L("*.ots"));
	SearchFiles(iCoeEnv->FsSession(),_L("E:\\Data\\"),_L("*.odp"));
	SearchFiles(iCoeEnv->FsSession(),_L("E:\\Data\\"),_L("*.otp"));
	*/

	listBox->Model().ModelEndUpdateL();
}



void CMobileOfficeView::ScanDirL(RFs& aFs, const TDesC& aDir, const TDesC& aWild) 
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
         /*
			 parse.Set((*dir)[i].iName, &spec, NULL); 
	
		 TBuf<256> Name;
			Name.Copy(parse.FullName());
			AddItem(Name); 
			*/
			 AddItem((*dir)[i].iName);
         } 

      CleanupStack::PopAndDestroy(dir); 
      } 
   } 

void CMobileOfficeView::SearchFiles(RFs& aFs,const TDesC& aPath, const TDesC& aWild)
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

      ScanDirL(aFs, scan->FullPath	(), aWild); 
      }; 
   CleanupStack::PopAndDestroy(scan); 
  
}

/*
TPtrC CMobileOfficeView::ListboxText()
{
	const CQikListBox* iListbox = LocateControlByUniqueHandle<const CQikListBox>(EListBoxListViewListCtrl);

	MQikListBoxData* listBoxData = iListbox->Model().RetrieveDataL( iListbox->CurrentItemIndex());
	//CleanupClosePushL(*listBoxData);
	return listBoxData->Text(EQikListBoxSlotText1,0);


}
*/

TKeyResponse CMobileOfficeView::OfferKeyEventL(const TKeyEvent& aKeyEvent,TEventCode aType)
{
	if (aKeyEvent.iCode == EKeyBackspace)
	{
			Delete();
			return EKeyWasConsumed;
	}

	/*
	if ( (aKeyEvent.iCode == EKeyLeftArrow) || (aKeyEvent.iCode == EKeyRightArrow) )
	{
		
	}
	*/

	return CQikViewBase::OfferKeyEventL(aKeyEvent,aType);
}

void CMobileOfficeView::Delete()
{

	/*
	CQikSimpleDialog* dialog = new(ELeave) CQikSimpleDialog;
	dialog->PrepareLC(R_MY_SIMPLE_DIALOG_DELETE);
	TInt ret = dialog->RunL();
	if(ret == EEikBidYes)
	{
	*/
				
		CQikListBox* listBox = LocateControlByUniqueHandle<CQikListBox>(EListBoxListViewListCtrl);
		TInt ListCount = listBox->Model().Count();
		if(ListCount>0)
		{
			//TPtrC listtext;
			TFileName aFileName;
			MQikListBoxData* listBoxData = listBox->Model().RetrieveDataL( listBox->CurrentItemIndex());
			CleanupClosePushL(*listBoxData);

			if (ActiveTabId() ==1)
			{
				aFileName.Insert(0,c);
			}	
			else
			{
				aFileName.Insert(0,d);
			}
			aFileName.Append(listBoxData->Text(EQikListBoxSlotText1,0));
			//listtext.Set(listBoxData->Text(EQikListBoxSlotText1,0));

			if ( (BaflUtils::DeleteFile(iCoeEnv->FsSession(),aFileName)==KErrNone) && (listBox->CurrentItemIndex() >-1))
			{
				listBox->RemoveItemL(listBox->CurrentItemIndex());
				listBox->DrawNow();
			}

			CleanupStack::PopAndDestroy(listBoxData);
		}
	//}
}

void CMobileOfficeView::ClearListbox(void)
{
	CQikListBox* listBox = LocateControlByUniqueHandle<CQikListBox>(EListBoxListViewListCtrl);
	listBox->RemoveAllItemsL();
}


void CMobileOfficeView::HandleListBoxEventL(CQikListBox*,TQikListBoxEvent aEventType,TInt,TInt)
{     
	switch(aEventType)     
	{     
	case EEventSelectionChanged: break;
		case EEventMatchBufferChanged: break;
		case EEventTopReached: break;
		case EEventBottomReached: break;
		case EEventEmptyListBoxActioned: break;
		case EEventHighlightMoved: break;
		case EEventSlotIndexChanged: break;
		case EEventDimmedItemConfirmedAttempt: break;
		case EEventMatchBufferFull: break;
		case EEventCustomStart: break;
		case EEventItemHighlighted:         break;     
		case EEventItemTapped:    break;
		case EEventItemConfirmed:     
		{ 
			CQikCommand* cmd = CQikCommand::NewL(EMobileOfficeCmdOpen); 
			HandleCommandL(*cmd); 
			//CleanupStack::PopAndDestroy(cmd);  
			delete cmd;
			break;
		}     
		
	}
}


TVwsViewIdAndMessage CMobileOfficeView::ViewScreenDeviceChangedL()
{
	return TVwsViewIdAndMessage(ViewId());
}

void CMobileOfficeView::TabActivatedL(TInt aTabId)
{
		ClearListbox();
		if (aTabId ==1)
		{
			SearchC();
		}	
		else
		{
			SearchE();
		}
}
