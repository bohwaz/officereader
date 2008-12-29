#include <QikCommand.h>
#include <e32base.h>
#include <gdi.h>
#include <QikSimpleDialog.h>


#include <MobileOffice.rsg>

#include "MobileOfficeImageView.h"
#include "MobileOfficeExternalInterface.h"
#include "MobileOffice.hrh"

#include "OpenDocument.h"

#include "MobileOfficeAppUi.h"

#include <HLPLCH.H>

/**
Creates and constructs the view.

@param aAppUi Reference to the AppUi
@return Pointer to a CMobileOfficeImageView object
*/
CMobileOfficeImageView* CMobileOfficeImageView::NewLC(CQikAppUi& aAppUi)
	{
	CMobileOfficeImageView* self = new(ELeave) CMobileOfficeImageView(aAppUi);
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
CMobileOfficeImageView::CMobileOfficeImageView(CQikAppUi& aAppUi)
	: CQikViewBase(aAppUi, KNullViewId)
	{
	}

/**
Destructor for the view
*/
CMobileOfficeImageView::~CMobileOfficeImageView()
	{
	}

/**
2nd stage construction of the view.
*/
void CMobileOfficeImageView::ConstructL()
	{
	// Calls ConstructL that initialises the standard values.
	// This should always be called in the concrete view implementations.
	BaseConstructL();
	}

/**
Inherited from CQikViewBase and called upon by the UI Framework.
It creates the view from resource.
*/	
void CMobileOfficeImageView::ViewConstructL()
	{
	// Loads information about the UI configurations this view supports
	// together with definition of each view.
	ViewConstructFromResourceL(R_MOBILEOFFICE_MOBILEOFFICEIMAGEVIEW_UI_CONFIGURATIONS);
	}

/**
Returns the view Id

@return Returns the Uid of the view
*/
TVwsViewId CMobileOfficeImageView::ViewId()const
	{
	// The app UID3 and the view UID need to be defined in MobileOfficeExternalInterface.h
	return TVwsViewId(KUidMobileOfficeApp, KUidMobileOfficeImageView);
	}

/**
Handles all commands in the view
Called by the UI framework when a command has been issued.
The command Ids are defined in the .hrh file.

@param aCommand The command to be executed
@see CQikViewBase::HandleCommandL
*/
void CMobileOfficeImageView::HandleCommandL(CQikCommand& aCommand)
{
	CMobileOfficeAppUi* app = (CMobileOfficeAppUi*)iEikonEnv->EikAppUi();			
	app->CheckDemo();

	switch(aCommand.Id())
		{
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
	
       	case EMobileOfficeCmdOpen:	
				{
					iEikonEnv->EikAppUi()->ActivateViewL(TVwsViewId(KUidMobileOfficeApp,KUidMobileOfficeEditorView));
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
		case EQikCmdGoBack:
		{
			iEikonEnv->EikAppUi()->ActivateViewL(TVwsViewId(KUidMobileOfficeApp,app->OpenDocument()->NextViewID()));
			break;
		}
		// Go back and exit command will be passed to the CQikViewBase to handle.
		default:
			CQikViewBase::HandleCommandL(aCommand);
			break;
		}
	}

void CMobileOfficeImageView::Draw(const TRect& aRect) const
    {
	CWindowGc& gc = SystemGc();
	gc.Clear(aRect);    

	const CFont* normalFont = iEikonEnv->NormalFont();
	gc.UseFont(normalFont);

	if (iBitmap )
		gc.BitBlt(TPoint(0,0),iBitmap);
	else
		gc.DrawText(_L("no image available"),TPoint(20	,30));
	}

void CMobileOfficeImageView::HandleControlEventL(
    CCoeControl* /*aControl*/,TCoeEvent /*aEventType*/)
{
	iBitmap = STATIC_CAST(CMobileOfficeAppUi* ,iEikonEnv->EikAppUi())->OpenDocument()->Bitmap();
	DrawNow();
}

void CMobileOfficeImageView::ViewActivatedL(   const TVwsViewId& /*aPrevViewId*/,TUid /*aCustomMessageId*/,const TDesC8& /*aCustomMessage*/)
{
		STATIC_CAST(CMobileOfficeAppUi*,iEikonEnv->EikAppUi())->OpenDocument()->GetThubnail(this,TSize(Rect().Width(),Rect().Height()));
}

TVwsViewIdAndMessage CMobileOfficeImageView::ViewScreenDeviceChangedL()
{
	return TVwsViewIdAndMessage(ViewId());
}

void CMobileOfficeImageView::GetHelpContext(TCoeHelpContext &aContext) const
{
	aContext.iContext = _L("Thumbnail");
	aContext.iMajor = KUidMobileOfficeApp;
}
