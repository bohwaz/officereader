#include <QikCommand.h>
#include <MobileOffice.rsg>

#include "MobileOfficeAppUi.h"
#include "MobileOfficePropView.h"
#include "MobileOfficeExternalInterface.h"
#include "MobileOffice.hrh"

#include <QikListBoxLayout.h>
#include <QikListBoxLayoutElement.h>
#include <QikListBoxLayoutProperties.h>
#include <QikListBoxLayoutPair.h>

#include <QikSimpleDialog.h>
#include <GDI.H>
#include <HLPLCH.H>


/**
Creates and constructs the view.

@param aAppUi Reference to the AppUi
@return Pointer to a CMobileOfficePropView object
*/
CMobileOfficePropView* CMobileOfficePropView::NewLC(CQikAppUi& aAppUi)
	{
	CMobileOfficePropView* self = new(ELeave) CMobileOfficePropView(aAppUi);
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
CMobileOfficePropView::CMobileOfficePropView(CQikAppUi& aAppUi)
	: CQikViewBase(aAppUi, KNullViewId)
	{
	}

/**
Destructor for the view
*/
CMobileOfficePropView::~CMobileOfficePropView()
	{
	}

/**
2nd stage construction of the view.
*/
void CMobileOfficePropView::ConstructL()
	{
	// Calls ConstructL that initialises the standard values.
	// This should always be called in the concrete view implementations.
	BaseConstructL();
	}

/**
Inherited from CQikViewBase and called upon by the UI Framework.
It creates the view from resource.
*/
void CMobileOfficePropView::ViewConstructL()
	{
	// Loads information about the UI configurations this view supports
	// together with definition of each view.
	ViewConstructFromResourceL(R_MOBILEOFFICE_MOBILEOFFICEPROPVIEW_UI_CONFIGURATIONS);

	
	CQikListBox* listBox = LocateControlByUniqueHandle<CQikListBox>(EListBoxListViewListCtrl2);
	CQikListBoxLayoutPair* layoutPair = listBox->GetLayout(listBox->DefaultLayoutId());
    
    if(layoutPair)
    {
        CQikListBoxLayoutElement* theFirstRow = layoutPair->NormalLayout()->GetElement(EQikListBoxSlotText1);   
        if(theFirstRow)
        {
            TQikListBoxPropertiesText textProperties;
            theFirstRow->GetContentTypeProperties(textProperties);
            textProperties.iTextStyle =  EQikListBoxFontBold;
            RArray<TInt> dummy;            
            theFirstRow->SetContentTypePropertiesL(textProperties, dummy);
            //listBox->LayoutUpdatedL();

		}
	}
	
}

/**
Returns the view Id

@return Returns the Uid of the view
*/
TVwsViewId CMobileOfficePropView::ViewId()const
	{
	// The app UID3 and the view UID need to be defined in MobileOfficeExternalInterface.h
	return TVwsViewId(KUidMobileOfficeApp, KUidMobileOfficePropView);
	}

/**
Handles all commands in the view
Called by the UI framework when a command has been issued.
The command Ids are defined in the .hrh file.

@param aCommand The command to be executed
@see CQikViewBase::HandleCommandL
*/
void CMobileOfficePropView::HandleCommandL(CQikCommand& aCommand)
	{

	CMobileOfficeAppUi* app = (CMobileOfficeAppUi*)iEikonEnv->EikAppUi();			
	app->CheckDemo();

	switch(aCommand.Id())
		{
			case EMobileOfficeCmdOpen:	
				{
					iEikonEnv->EikAppUi()->ActivateViewL(TVwsViewId(KUidMobileOfficeApp,KUidMobileOfficeEditorView));
					break;
				}

			case EMobileOfficeCmdPicture:
				{
					iEikonEnv->EikAppUi()->ActivateViewL(TVwsViewId(KUidMobileOfficeApp,KUidMobileOfficeImageView));
					break;
				}
	
			case EMobileOfficeCmdProp:
				{
					iEikonEnv->EikAppUi()->ActivateViewL(TVwsViewId(KUidMobileOfficeApp,KUidMobileOfficePropView));
					break;
				}
			case EMobileOfficeCmdHelp:
			{
				CArrayFix<TCoeHelpContext>* buf = iEikonEnv->EikAppUi()->AppHelpContextL();
				HlpLauncher::LaunchHelpApplicationL(iEikonEnv->WsSession(), buf);
				break;
			}
			case EMobileOfficeCmdAppAbout:
				{
					CQikSimpleDialog* dialog = new(ELeave) CQikSimpleDialog;
					dialog->PrepareLC(R_MY_SIMPLE_ABOUT_DIALOG);
					dialog->RunL();
					CleanupStack::PopAndDestroy(dialog);
					break;
				}
		// Go back and exit command will be passed to the CQikViewBase to handle.
		case EQikCmdGoBack:
		{
			iEikonEnv->EikAppUi()->ActivateViewL(TVwsViewId(KUidMobileOfficeApp,app->OpenDocument()->NextViewID()));
			break;
		}
		default:
			CQikViewBase::HandleCommandL(aCommand);
			break;
		}
	}

void CMobileOfficePropView::AddItem(TDes& Text1,HBufC* Text2)
{
	
	CQikListBox* listBox = LocateControlByUniqueHandle<CQikListBox>(EListBoxListViewListCtrl2);

	//model.ModelBeginUpdateLC();

	
	MQikListBoxData* listBoxData = listBox->Model().NewDataL(MQikListBoxModel::EDataNormal);
	// Pushes the data onto the cleanup stack. 
	// When CleanupStack::PopAndDestroy() is called, the data will be closed.
	CleanupClosePushL(*listBoxData);

	// Adds the text that will be visible in the list box
	listBoxData->AddTextL(Text1, EQikListBoxSlotText1);
	listBoxData->AddTextL(Text2, EQikListBoxSlotText2);
	
	// Removes the listBoxData from the stack and calls close on listBoxData
	CleanupStack::PopAndDestroy(listBoxData);

	// Informs that the update of the list box model has ended
	//model.ModelEndUpdateL();

}

void CMobileOfficePropView::ViewActivatedL(  const TVwsViewId& /*aPrevViewId*/,TUid /*aCustomMessageId*/,const TDesC8& /*aCustomMessage*/)
{
	

	const CQikListBox* listBox = LocateControlByUniqueHandle<const CQikListBox>(EListBoxListViewListCtrl2);
	

	listBox->Model().ModelBeginUpdateLC();


	COpenDocument* doc = STATIC_CAST(CMobileOfficeAppUi* ,iEikonEnv->EikAppUi())->OpenDocument();
	if (doc->Encrypted())
	{
		CQikSimpleDialog* dialog = new(ELeave) CQikSimpleDialog;
		dialog->PrepareLC(R_MY_SIMPLE_DIALOG);
		TInt ret = dialog->RunL();
		if(ret == EEikBidYes)
		{
			CEikEdwin* edwin = dialog->LocateControlByUniqueHandle<CEikEdwin>(ESimpleDialogEdwin);
			HBufC *text = HBufC::NewL(edwin->TextLength());
			TPtr ptr_text = text->Des();
			edwin->Text()->Extract(ptr_text);
			doc->SetPassword(*text);
			delete text;
			doc->LoadMetaInfomation();
		}
		CleanupStack::PopAndDestroy(dialog);			
	}
	else
	{
			doc->LoadMetaInfomation();
	}


	TBuf<10> Dateiname1;
	HBufC* Dateiname2;
	Dateiname2 = doc->Filename().Alloc();
	Dateiname1.Append(_L("Filename\t"));
	AddItem(Dateiname1,Dateiname2);
	//delete Dateiname2;

	
	TBuf<150> Title1;
	HBufC* Title2;
	Title1.Append(_L("Title\t"));
	Title2 = doc->GetTitle().Alloc();
	AddItem(Title1,Title2);
	//delete Title2;

	TBuf<150> Subject1;
	HBufC* Subject2;
	Subject1.Append(_L("Subject\t"));
	Subject2 = doc->GetSubject().Alloc();
	AddItem(Subject1,Subject2);
	//delete Subject2;

	TBuf<250> Description1;
	HBufC* Description2;
	Description1.Append(_L("Description\t"));
	Description2 = doc->GetDescription().Alloc() ;
	AddItem(Description1,Description2);
	//delete Description2;

	TBuf<150> Keywords1;
	HBufC* Keywords2;
	Keywords1.Append(_L("Keywords\t"));
	Keywords2 = doc->GetKeywords().Alloc();
	AddItem(Keywords1,Keywords2);
	//delete Keywords2;
	
	
	TBuf<50> Words1;
	HBufC* Words2;
	Words1.Append(_L("Words\t"));
	Words2 = doc->GetWords().Alloc();
	AddItem(Words1,Words2);
	//delete Words2;
	
	TBuf<50> Para1;
	HBufC* Para2;
	Para1.Append(_L("Paragraphs\t"));
	Para2 = doc->GetParagraphs().Alloc();
	AddItem(Para1,Para2);
	//delete Para2;

	TBuf<50> Pages1;
	HBufC* Pages2;
	Pages1.Append(_L("Pages\t"));
	Pages2  = doc->GetPages().Alloc();
	AddItem(Pages1,Pages2);
	//delete Pages2;

	


	listBox->Model().ModelEndUpdateL();
	

}

void CMobileOfficePropView::ViewDeactivated()
{
	CQikListBox* listBox = LocateControlByUniqueHandle<CQikListBox>(EListBoxListViewListCtrl2);
	listBox->RemoveAllItemsL();
}


CQikCommand *CMobileOfficePropView::DynInitOrDeleteCommandL(CQikCommand *aCommand, const CCoeControl &/*aControlAddingCommands*/)
{
	if (aCommand->Type()==EQikCommandTypeItem)
	{
		delete aCommand;
		return NULL;
	}
	else
		return aCommand;
}


TVwsViewIdAndMessage CMobileOfficePropView::ViewScreenDeviceChangedL()
{
	return TVwsViewIdAndMessage(ViewId());
}

void CMobileOfficePropView::GetHelpContext(TCoeHelpContext &aContext) const
{
	aContext.iContext = _L("Properties");
	aContext.iMajor = KUidMobileOfficeApp;
}
