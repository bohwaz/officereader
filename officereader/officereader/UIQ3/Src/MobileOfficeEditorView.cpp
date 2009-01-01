#include <QikCommand.h>
#include <QikScrollableContainer.h>
#include <QikRowLayoutManager.h>
#include <QikGridLayoutManager.h>
#include <QikBuildingBlock.h>
#include <QikMultiPageViewBase.h>
#include <QikUiConfigDataTypes.h>
#include <QikFullscreenFloatingMenu.h>
#include <QikHelpLauncher.h>
#include <QikSimpleDialog.h>
#include <GDI.H>

#include <MobileOffice.rsg>
#include "MobileOffice.hrh"

#include "MobileOfficeEditorView.h"
#include "MobileOfficeExternalInterface.h"

#include "MobileOfficeAppui.h"

#include "sheet_view.h"
#include "sheet_drawer.h"

#include <HLPLCH.H>

/**
Creates and constructs the view.

@param aAppUi Reference to the AppUi
@return Pointer to a CMobileOfficeEditorView object
*/
CMobileOfficeEditorView* CMobileOfficeEditorView::NewLC(CQikAppUi& aAppUi)
	{
	CMobileOfficeEditorView* self = new(ELeave) CMobileOfficeEditorView(aAppUi);
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
CMobileOfficeEditorView::CMobileOfficeEditorView(CQikAppUi& aAppUi)
	: CQikViewBase(aAppUi, KNullViewId)
	{
		iSheet = NULL;
		rted = NULL;
	}

/**
Destructor for the view
*/
CMobileOfficeEditorView::~CMobileOfficeEditorView()
	{
		if(iFloatingMenu)
        {
			CQikCommandManager::Static(*iCoeEnv).RemoveCustomOperator(*this,*iFloatingMenu);
			iFloatingMenu = NULL; // is in the control array..
        }
	}

/**
2nd stage construction of the view.
*/
void CMobileOfficeEditorView::ConstructL()
	{
	// Calls ConstructL that initialises the standard values.
	// This should always be called in the concrete view implementations.
	BaseConstructL();
	}

/**
Inherited from CQikViewBase and called upon by the UI Framework.
It creates the view from resource.
*/
void CMobileOfficeEditorView::ViewConstructL()
	{
	// Loads information about the UI configurations this view supports
	// together with definition of each view.	
	ViewConstructFromResourceL(R_MOBILEOFFICE_MOBILEOFFICEEDITORVIEW_UI_CONFIGURATIONS);

	
	CQikGridLayoutManager* gridlayout = CQikGridLayoutManager::NewLC();
    SetLayoutManagerL(gridlayout);
    CleanupStack::Pop(gridlayout);

	/*
    // Create a container and add it to the view
    container = new (ELeave) CQikScrollableContainer();
    Controls().AppendLC(container);
    container->ConstructL(EFalse);
    CleanupStack::Pop(container);
     
    // Create a layout manager to be used inside the container
    CQikRowLayoutManager* rowlayout = CQikRowLayoutManager::NewLC();
    container->SetLayoutManagerL(rowlayout);
    CleanupStack::Pop(rowlayout);
*/

	iFloatingMenu = CQikFullscreenFloatingMenu::NewL(*this);
	AddControlLC(iFloatingMenu);
    CQikCommandManager::Static(*iCoeEnv).AddCustomOperatorL(*this,*iFloatingMenu);
    CleanupStack::Pop(iFloatingMenu);
	


	/*
	
	TQikViewMode vM;// = ViewMode();
	vM.SetFullscreen();
	vM.SetButtonOrSoftkeyBar(ETrue);
	//vM.SetToolbar(ETrue);
	SetViewModeL(vM);
	*/
	
		
}

/**
Returns the view Id

@return Returns the Uid of the view
*/
TVwsViewId CMobileOfficeEditorView::ViewId()const
	{
	// The app UID3 and the view UID need to be defined in MobileOfficeExternalInterface.h
	return TVwsViewId(KUidMobileOfficeApp, KUidMobileOfficeEditorView);
	}

/**
Handles all commands in the view
Called by the UI framework when a command has been issued.
The command Ids are defined in the .hrh file.

@param aCommand The command to be executed
@see CQikViewBase::HandleCommandL
*/
void CMobileOfficeEditorView::HandleCommandL(CQikCommand& aCommand)
{

	CMobileOfficeAppUi* app = (CMobileOfficeAppUi*)iEikonEnv->EikAppUi();			
	CQikCommandManager& cmdManager = CQikCommandManager::Static();

	app->CheckDemo();
	app->OpenDocument()->SetNextViewID(KUidMobileOfficeEditorView);	
	app->OpenDocument()->HideNote();
	
	switch(aCommand.Id())
	{
	    case EMobileOfficeCmdFindFirst:
			{
	
				CQikSimpleDialog* dialog = new(ELeave) CQikSimpleDialog;
				dialog->PrepareLC(R_MY_SIMPLE_DIALOG);
				TInt ret = dialog->RunL();
				if(ret == EEikBidYes)
				{
					CEikEdwin* edwin = dialog->LocateControlByUniqueHandle<CEikEdwin>(ESimpleDialogEdwin);
					if(edwin)
					{
							if (edwin->TextLength()>0)
							{
								iLength = edwin->TextLength();
								

								HBufC *text = HBufC::NewL(edwin->TextLength());
								TPtr ptr_text = text->Des();
								edwin->Text()->Extract(ptr_text);
	
								if (Editor())
								
								{
									TInt aPos =  Editor()->FindTextL(text,0,0);
									if	 (aPos != KErrNotFound )
									{
										Editor()->SetCursorPosL(aPos,EFalse);
										Editor()->SetCursorPosL(aPos + text->Length(),ETrue);
										cmdManager.SetInvisible(*this, EMobileOfficeCmdFindAgain,EFalse);
									}
								}
								
								if (Sheet())
								{
									TPoint aPoint;
									if ( Sheet()->Model()->FindFirstL(*text,aPoint,CSheetModel::enFindSubStr | CSheetModel::enFindOrderRows | CSheetModel::enFindCompareNoCase))
									{
										Sheet()->View()->SetCurrentCell(aPoint);
										Sheet()->DrawNow();
										cmdManager.SetInvisible(*this, EMobileOfficeCmdFindAgain,EFalse);
									}
								}
								delete text;
							}
					}
				}
				CleanupStack::PopAndDestroy(dialog);
				break;
			}
			
		case EMobileOfficeCmdFindAgain:
			{
				if (Editor())
				{
						TInt aPos = -1;
						aPos = Editor()->FindTextL(NULL,Editor()->CursorPos(),0);
						if (aPos<0)
							aPos = Editor()->FindTextL(NULL,0,0);
						if (aPos > -1)
						{
							Editor()->SetCursorPosL(aPos,EFalse);
							Editor()->SetCursorPosL(aPos + iLength,ETrue);
						}
				}

				if (Sheet())
				{
					TPoint aPoint;
					if (Sheet()->Model()->FindNextL(aPoint))
					{
							Sheet()->View()->SetCurrentCell(aPoint);
							Sheet()->DrawNow();
					}
				}

				break;
			}
		case EMobileOfficeCmdStart:
		{
			if (Editor())
				Editor()->SetCursorPosL(0, EFalse);
			else
			{
				Sheet()->View()->SetCurrentCell(TPoint(0,0));
				Sheet()->DrawNow();
			}

			break;
		}
		case EMobileOfficeCmdEnd:
		{
			if (Editor())
			{
				Editor()->SetCursorPosL(Editor()->RichText()->DocumentLength(),EFalse);
			}
			else
			{
				Sheet()->View()->SetCurrentCell(TPoint(0,app->OpenDocument()->CellEnd()));
				Sheet()->DrawNow();
			}
			break;
		}
		case EMobileOfficeCmdHelp:
			{
				TCoeHelpContext helpContext(KUidMobileOfficeApp,_L("Viewing_a_file"));
				CQikHelpLauncher::LaunchLD(helpContext);
				break;
			}
		case EMobileOfficeCmdAppAbout:
				{
					CQikSimpleDialog* dialog = new(ELeave) CQikSimpleDialog;
					dialog->PrepareLC(R_MY_SIMPLE_ABOUT_DIALOG);
					dialog->RunLD();
//					CleanupStack::PopAndDestroy(dialog);
					break;
				}
		case EMobileOfficeCmdZoomNormal:
		{
			if (Editor())
			{
				SetZoomFactorL(TZoomFactor::EZoomOneToOne,EAbsoluteZoom );
			}
			else
			{
				Sheet()->View()->SetZoomModeL( CSheetView::enZoomSmall );
			}
			break;
		}
		case EMobileOfficeCmdZoomMedium:
		{
			if (Editor())
			{
				SetZoomFactorL(TZoomFactor::EZoomOneToOne*3,EAbsoluteZoom );
			}
			else
			{
				Sheet()->View()->SetZoomModeL( CSheetView::enZoomMedium );
			}
			break;
		}
		case EMobileOfficeCmdZoomLarge:
		{
			if (Editor())
			{
				SetZoomFactorL(TZoomFactor::EZoomOneToOne*4,EAbsoluteZoom );
			}
			else
			{
				Sheet()->View()->SetZoomModeL( CSheetView::enZoomLarge );
			}
			break;
		}
		case EMobileOfficeCmdSheet:
		{
			if (Sheet())
			{
				Sheet()->View()->SetCellAjustmentMode( ETrue );
			}
			
			break;
		}
		case EMobileOfficeCmdFull:
		{


			if (iSheet)
			{
				iEikonEnv->EikAppUi()->RemoveFromStack(Sheet());
				iEikonEnv->EikAppUi()->RemoveFromStack(Sheet()->SheetWnd());
			}

			//CQikCommandManager& cmdManager = CQikCommandManager::Static();
			TQikViewMode viewMode = ViewMode();
			if (viewMode.IsNormal())
			{

				TQikViewMode vM;// = ViewMode(	);
				vM.SetFullscreen();
				TQikUiConfig aConfig = CQUiConfigClient::Static().CurrentConfig();
				TQikScreenMode aMode = aConfig.ScreenMode();
				if ( (aMode == EQikUiConfigSmallPortrait) || (aMode== EQikUiConfigSmallLandscape) )
				{
					vM.SetButtonOrSoftkeyBar(ETrue);
				}
				SetViewModeL(vM);

				//cmdManager.Command(*this,EQikCmdGoBack)->SetCpfFlags(EQikCpfFlagDuplicateInMenuPane);

				DrawNow();

			}
			else
			{
				TQikViewMode vm;
				vm.SetNormal();
				SetViewModeL(vm);				
				
				DrawNow();
				//cmdManager.Command(*this,EQikCmdGoBack)->SetCpfFlags(0);
				
			}

			if (iSheet)
			{
				iEikonEnv->EikAppUi()->AddToStackL(Sheet()->SheetWnd());
				iEikonEnv->EikAppUi()->AddToStackL(Sheet());
			}
			
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
				Sheet()->Model()->RemoveAllL();
				//iContainer->Sheet()->View()->ResetCellSizeL( const TSize& Size );
				//iContainer->Sheet()->View()->ResetCellTextColorL( KRgbBlack );
				//iContainer->Sheet()->View()->ResetCellColorL( KRgbWhite );
				User::Heap().Compress();
				app->OpenDocument()->GetContentSheet(Sheet(),aCommand.Id());	
				Sheet()->View()->SetCurrentCell(TPoint(0,0));
				iSheet->DrawNow();
				break;
			}

		case EMobileOfficeCmdPicture:
				{
					app->OpenDocument()->ClearNote();
					iQikAppUi.ActivateViewL(TVwsViewId(KUidMobileOfficeApp,KUidMobileOfficeImageView));
					break;
				}
	
		case EMobileOfficeCmdProp:
				{
					app->OpenDocument()->ClearNote();
					iQikAppUi.ActivateViewL(TVwsViewId(KUidMobileOfficeApp,KUidMobileOfficePropView));
					break;
				}
		case EEikCmdExit:
			{
				app->OpenDocument()->ClearNote();
				app->HandleCommandL( EEikCmdExit );
				break;
			}
		
	
        default:
            {
				app->OpenDocument()->ClearNote();
				iQikAppUi.ActivateViewL(TVwsViewId(KUidMobileOfficeApp,KUidMobileOfficeView));
            break;
            }
        }
 }


 void CMobileOfficeEditorView::SetupText(void)
 {
	CQikCommandManager& cmdManager = CQikCommandManager::Static();
	COpenDocument* iDoc = STATIC_CAST(CMobileOfficeAppUi*,iEikonEnv->EikAppUi())->OpenDocument();
	 					
	rted = new (ELeave) CEikRichTextEditor();
	AddControlLC(rted, EMyViewRichTextEditor);
	if (iDoc->Editable())
	{
		rted->ConstructL(1000,100000000,CEikRichTextEditor::ENoCursorWrap,1,0, CEikRichTextEditor::EDisplayModeView);
	}
	else
	{
		rted->ConstructL(1000,100000000,CEikRichTextEditor::EReadOnly|CEikRichTextEditor::ENoCursorWrap,1,0, CEikRichTextEditor::EDisplayModeView);
	}
	rted->SetObserver(this);
	rted->SetUniqueHandle(EMyViewRichTextEditor);
	rted-> CreateScrollBarFrameL()->SetScrollBarVisibilityL( CEikScrollBarFrame::EAuto, CEikScrollBarFrame::EOn );
	CleanupStack::Pop(rted);			
												
	if (iDoc->Encrypted())
	{
		
		/*
		CAknTextQueryDialog* FindQuery = CAknTextQueryDialog::NewL(text);
		CleanupStack::PushL(FindQuery);
		HBufC* prompt = prompt = StringLoader::LoadLC(R_PASSWORD_QUERY);
		FindQuery->SetPromptL(*prompt);
		CleanupStack::PopAndDestroy(prompt);
		CleanupStack::Pop(FindQuery);
		*/

		CQikSimpleDialog* dialog = new(ELeave) CQikSimpleDialog;
		dialog->PrepareLC(R_MY_SIMPLE_DIALOG);
		TInt ret = dialog->RunL();
		if(ret == EEikBidYes)
		{
			CEikEdwin* edwin = dialog->LocateControlByUniqueHandle<CEikEdwin>(ESimpleDialogEdwin);
			HBufC *text = HBufC::NewL(edwin->TextLength());
			TPtr ptr_text = text->Des();
			edwin->Text()->Extract(ptr_text);
			iDoc->SetPassword(*text);
			delete text;
			iDoc->GetContentText( rted);
		}
		CleanupStack::PopAndDestroy(dialog);			
	}
	else
	{
			iDoc->GetContentText( rted);
	}

	cmdManager.SetInvisible(*this,EMobileOfficeCmdSheet,ETrue);
	cmdManager.SetInvisible(*this,EMobileOfficeCmdTable,ETrue);
	//cmdManager.SetInvisible(*this,EMobileOfficeCmdEnd,EFalse);
	cmdManager.SetInvisible(*this, EMobileOfficeCmdFindAgain,ETrue);
	
 }


 void CMobileOfficeEditorView::SetupSheet(void)
 {
	 CQikCommandManager& cmdManager = CQikCommandManager::Static();
	 COpenDocument* iDoc = STATIC_CAST(CMobileOfficeAppUi*,iEikonEnv->EikAppUi())->OpenDocument();

	iSheet = new ( ELeave ) CSheetCtrl;
	AddControlLC(iSheet, EEditorListCtrl);

	iSheet->ConstructL();
	iSheet->SetContainerWindowL( *this );
	iSheet->SetMopParent( this );
	iSheet->SetRect(Rect());
	
	TSize MinSize, MaxSize;
	iSheet->View()->CellAjustmentLimits( MinSize, MaxSize );
	iSheet->View()->ResetCellSizeL( TSize( MinSize.iWidth + (MaxSize.iWidth - MinSize.iWidth)/2,
		MinSize.iHeight + (MaxSize.iHeight - MinSize.iHeight)/2 ) );

	iSheet->SetItemViewPos( ETrue );
	iSheet->Data()->SetLongTextShowMode( CSheetData::enOverNeighbour );
	CleanupStack::Pop( iSheet );
 
	if (iDoc->Encrypted())
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
			iDoc->SetPassword(*text);
			delete text;
			iDoc->GetContentSheet(iSheet,ETableIndex-1);
		}
		CleanupStack::PopAndDestroy(dialog);			
	}
	else
	{
		iDoc->GetContentSheet(iSheet,ETableIndex-1);
	}

	cmdManager.SetInvisible(*this,EMobileOfficeCmdSheet,EFalse);
	cmdManager.SetInvisible(*this,EMobileOfficeCmdTable,EFalse);
	//cmdManager.SetInvisible(*this,EMobileOfficeCmdEnd,ETrue);
	cmdManager.SetInvisible(*this, EMobileOfficeCmdFindAgain,ETrue);
	cmdManager.SetInvisible(*this,EMobileOfficeCmdZoom,EFalse);

	RPointerArray<HBufC>* iTables = STATIC_CAST(CMobileOfficeAppUi*,iEikonEnv->EikAppUi())->OpenDocument()->GetTableArray();

	for (TInt i=0; i<=iTables->Count()-1;i++)
	{
		CQikCommand *aCommand = CQikCommand::NewLC(ETableIndex + i);
		aCommand->SetTextL(*(*iTables)[i]);
		aCommand->SetNamedGroupId(EMobileOfficeCmdTable);
		aCommand->SetType(EQikCommandTypeScreen);
		aCommand->SetHandler(this);
		cmdManager.InsertCommandL(*this,aCommand);
		CleanupStack::Pop(aCommand);
	}

 }
		



void CMobileOfficeEditorView::ViewActivatedL(   
        const TVwsViewId& /*aPrevViewId*/,
        TUid /*aCustomMessageId*/,
        const TDesC8& /*aCustomMessage*/)
        {

		switch ( STATIC_CAST(CMobileOfficeAppUi*,iEikonEnv->EikAppUi())->OpenDocument()->GetType() )
		{
			
			case ODT: {
						SetupText();
						break; 
					}
			case OTT: { 
						SetupText();
						break; 
					}
			case ODP: { 
						//SetupPresentation();
						SetupText();
						break; 
					}
			case OTP: { 
						//SetupPresentation();
						SetupText();
						break; 
					}
			case ODS: { 
						SetupSheet();
						break; 
					}
			case OTS: { 
						SetupSheet();
						break; 
					}
			case ODFUnknown:{
						break;
						}
		}
		
	PerformLayout();
	ActivateL();

	if (rted) 
	{
		rted->SetFocus(ETrue);
		rted->SetCursorPosL(0, EFalse);
		rted->TextView()->SetCursorVisibilityL(TCursor::EFCursorInvisible,TCursor::EFCursorInvisible);
	}
	else
	{
		iSheet->SetFocus(ETrue);
	}

}

/**
Called every time a view is deactivated
*/
void CMobileOfficeEditorView::ViewDeactivated()
{
	
	if (iSheet)
	{
		RPointerArray<HBufC>* iTables = STATIC_CAST(CMobileOfficeAppUi*,iEikonEnv->EikAppUi())->OpenDocument()->GetTableArray();
		CQikCommandManager& cmdManager = CQikCommandManager::Static();
		for (TInt i=iTables->Count()-1;i>=0;i--)
		{
			cmdManager.DeleteCommand(*this,ETableIndex + i);
		}
		//container->RemoveControl(*iSheet);
		RemoveControl(*iSheet);
		delete iSheet;
		iSheet = NULL;
	}

	if (rted)
	{
		//container->RemoveControl(*rted);
		RemoveControl(*rted);
		delete rted;
		rted = NULL;
	}
}


TKeyResponse CMobileOfficeEditorView::OfferKeyEventL(const TKeyEvent& aKeyEvent,TEventCode aType)
	{
	if (rted) {
		if (aType == EEventKey)
		{
			if ( (aKeyEvent.iCode == EKeyLeftArrow) || (aKeyEvent.iCode == EKeyDevice6) )
				{
						STATIC_CAST(CMobileOfficeAppUi*,iEikonEnv->EikAppUi())->OpenDocument()->GetPreviousNote(iEikonEnv);
						return EKeyWasConsumed;
				}
			else if ((aKeyEvent.iCode == EKeyRightArrow)|| (aKeyEvent.iCode == EKeyDevice7) )
				{
						STATIC_CAST(CMobileOfficeAppUi*,iEikonEnv->EikAppUi())->OpenDocument()->GetNextNote(iEikonEnv);
						return EKeyWasConsumed;
				}
			else if ( (aKeyEvent.iCode == EKeyDevice8) || (aKeyEvent.iCode==EKeyEnter))
				{
           				STATIC_CAST(CMobileOfficeAppUi*,iEikonEnv->EikAppUi())->OpenDocument()->GetCurrentNote(iEikonEnv,ETrue);
						return EKeyWasConsumed;
				}
			else if (aKeyEvent.iCode == 42)
				{
						CQikCommand *aCommand = CQikCommand::NewLC(EMobileOfficeCmdStart);
						HandleCommandL(*aCommand);
						CleanupStack::PopAndDestroy(aCommand);
						return EKeyWasConsumed;
				}
			else if (aKeyEvent.iCode == 35)
				{
					CQikCommand *aCommand = CQikCommand::NewLC(EMobileOfficeCmdEnd);
					HandleCommandL(*aCommand);
					CleanupStack::PopAndDestroy(aCommand);
					return EKeyWasConsumed;
				}
			else if (aKeyEvent.iCode == 48)
				{
					CQikCommand *aCommand = CQikCommand::NewLC(EMobileOfficeCmdFull);
					HandleCommandL(*aCommand);
					CleanupStack::PopAndDestroy(aCommand);
					return EKeyWasConsumed;
				}
			else
				return rted->OfferKeyEventL(aKeyEvent, aType);
				//return EKeyWasNotConsumed;
		}
	}

	if (iSheet)
	{
		if (aKeyEvent.iCode == 42)
		{
			CQikCommand *aCommand = CQikCommand::NewLC(EMobileOfficeCmdStart);
			HandleCommandL(*aCommand);
			CleanupStack::PopAndDestroy(aCommand);
			return EKeyWasConsumed;
		}
		else if (aKeyEvent.iCode == 35)
		{
			CQikCommand *aCommand = CQikCommand::NewLC(EMobileOfficeCmdEnd);
			HandleCommandL(*aCommand);
			CleanupStack::PopAndDestroy(aCommand);
			return EKeyWasConsumed;
		}
		else if (aKeyEvent.iCode == 48)
		{
			CQikCommand *aCommand = CQikCommand::NewLC(EMobileOfficeCmdFull);
			HandleCommandL(*aCommand);
			CleanupStack::PopAndDestroy(aCommand);
			return EKeyWasConsumed;
		}
	}
 
	return CCoeControl::OfferKeyEventL(aKeyEvent, aType);	
}	


CEikRichTextEditor* CMobileOfficeEditorView::Editor()
{
	return rted;
}

CSheetCtrl* CMobileOfficeEditorView::Sheet()
{
	return iSheet;
}

TVwsViewIdAndMessage CMobileOfficeEditorView::ViewScreenDeviceChangedL()
{
	return TVwsViewIdAndMessage(ViewId());
}

void CMobileOfficeEditorView::GetHelpContext(TCoeHelpContext &aContext) const
{
	aContext.iContext = _L("Viewing_a_file");
	aContext.iMajor = KUidMobileOfficeApp;
}
