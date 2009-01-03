#include "MobileOfficeAppUi.h"
#include "MobileOfficeView.h"

#include "MobileOfficeEditorView.h"
#include "MobileOfficeImageView.h"
#include "MobileOfficePropView.h"
#include "MobileOfficeExternalInterface.h"
#include <MobileOffice.rsg>

#include <EIKAPP.H>
#include <QikSimpleDialog.h>
#include <MobileOffice.hrh>

#include <EIKFUTIL.H>
#include <HLPLCH.H>

#ifndef FREEVERSION
	#include "CIMEI.h"
#endif

void CMobileOfficeAppUi::ConstructL()
	{
	// Calls ConstructL that initiate the standard values.
	CQikAppUi::ConstructL();
	
	iOpenDocument = COpenDocument::NewL();
	
	#ifndef FREEVERSION
		TBuf<50> nein;
		#ifndef __WINSCW__
			CIMEIRetriever::GetIMEIL(nein);	
		#endif

		licMan.SetIMEI(nein);
		licMan.Start(this);
	#endif	

	CMobileOfficeView* mobileofficeView = CMobileOfficeView::NewLC(*this);
	AddViewL(*mobileofficeView);
	CleanupStack::Pop(mobileofficeView);
	

	CMobileOfficeEditorView* mobileofficeEditorView = CMobileOfficeEditorView::NewLC(*this);
	AddViewL(*mobileofficeEditorView);
	CleanupStack::Pop(mobileofficeEditorView);
	
	CMobileOfficeImageView* mobileofficeImageView = CMobileOfficeImageView::NewLC(*this);
	AddViewL(*mobileofficeImageView);
	CleanupStack::Pop(mobileofficeImageView);

	CMobileOfficePropView* mobileofficePropView = CMobileOfficePropView::NewLC(*this);
	AddViewL(*mobileofficePropView);
	CleanupStack::Pop(mobileofficePropView);
	
	SetDefaultViewL(*mobileofficeView);

	CheckDemo();
}

CMobileOfficeAppUi::~CMobileOfficeAppUi()
{
	delete iOpenDocument;
}

COpenDocument* CMobileOfficeAppUi::OpenDocument()
{
	return iOpenDocument;
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
		
		_LIT(mobileOffice,"Mobile Office");
		_LIT(demoText,"Demonstration Version");

//		HBufC* mobileSearchText = StringLoader::LoadLC( R_MOBILESEARCH_MOBILESEARCH );
//		HBufC* demoText = StringLoader::LoadLC( R_MOBILESEARCH_DEMO );
	
		CEikonEnv::Static()->InfoWinL( mobileOffice, demoText );

//		CleanupStack::PopAndDestroy( demoText );
//		CleanupStack::PopAndDestroy( mobileSearchText );
    }
#endif
}

#ifndef FREEVERSION
bool CMobileOfficeAppUi::Register()
{
    TBuf <64> buff;
	CQikSimpleDialog* dialog = new(ELeave) CQikSimpleDialog;
	dialog->PrepareLC(R_MY_SIMPLE_DIALOG_REG);
	TInt ret = dialog->RunL();
	if(ret == EEikBidYes)
	{
		CEikEdwin* edwin = dialog->LocateControlByUniqueHandle<CEikEdwin>(ESimpleDialogEdwin);
		if(edwin)
		{
			edwin->Text()->Extract(buff,0);

			int user_number = 0;
			TLex lx;
			lx.Assign( buff );
			lx.Val( user_number );

			licMan.SetUnlockCode( user_number );

			TBuf<64> aImei;
			#ifndef __WINSCW__
				int unique_number = 0;

				for( int n = 0; n < aImei.Length(); n++ )
				{
					TChar ch = aImei[n];

					if( ch.IsDigit() )
					{
						TBuf <2> conv;
						conv.Append( ch );

						int num;
						TLex lx;
						lx.Assign( conv );
						lx.Val( num );

						unique_number += num;
					}
				}

				int numaro = unique_number;
				if( (22222 + numaro * 1000 + numaro * 100 + numaro) == user_number );
							unique_number++;
			#endif

			
			if( licMan.Is_dm_1() )
			{
			//	HBufC* errorText = StringLoader::LoadLC( R_MOBILESEARCH_ERROR );
			//	HBufC* invalidCodeText = StringLoader::LoadLC( R_MOBILESEARCH_INVALIDCODE );

				_LIT(err,"Error");
				_LIT(invalidCodeText,"Invalid Code");

				//CEikonEnv::Static()->InfoWinL( *errorText, *invalidCodeText );
				CEikonEnv::Static()->InfoWinL( err, invalidCodeText );
					
			//	CleanupStack::PopAndDestroy( invalidCodeText );
			//CleanupStack::PopAndDestroy( errorText );
			}
			else 
			{
				//HBufC* correctCodeText = StringLoader::LoadLC( R_MOBILESEARCH_CORRECTCODE );
				//	HBufC* thanksText = StringLoader::LoadLC( R_MOBILESEARCH_THANKS );
				_LIT(corr,"Correct Code");
				_LIT(thanks,"Thanks for buying");
			
				//CEikonEnv::Static()->InfoWinL( *correctCodeText, *thanksText );
				CEikonEnv::Static()->InfoWinL( corr, thanks );

				//CleanupStack::PopAndDestroy( thanksText );	
				//CleanupStack::PopAndDestroy( correctCodeText );
			}

			licMan.End();
				
		}
	}
	CleanupStack::PopAndDestroy(dialog);
				
	return true;
}

TBool CMobileOfficeAppUi::Registered()
{
	return licMan.Is_dm_1();
}
CLicenseManager CMobileOfficeAppUi::LicMan()
{ 
	return licMan;
}
#endif

CArrayFix<TCoeHelpContext>* CMobileOfficeAppUi::HelpContextL() const
{
	CArrayFixFlat<TCoeHelpContext>* array = new(ELeave)CArrayFixFlat<TCoeHelpContext>(1);
	CleanupStack::PushL(array);
	array->AppendL(TCoeHelpContext(KUidMobileOfficeApp, _L("Introduction")));
	CleanupStack::Pop(array);
	return array;
}

