#include <s32strm.h>
#include <QikApplication.h>

#include "MobileOfficeDocument.h"
#include "MobileOfficeAppUi.h"
#include "MobileOfficeExternalInterface.h"

#include <BAUTILS.H>

/**
Creates and constructs the document. This is called by
CMobileOfficeApplication::CreateDocumentL() which in turn is called by the
UI framework.
*/
CMobileOfficeDocument* CMobileOfficeDocument::NewL(CQikApplication& aApp)
	{
	CMobileOfficeDocument* self = new (ELeave) CMobileOfficeDocument(aApp);
	CleanupStack::PushL(self);
	self->ConstructL();
	CleanupStack::Pop(self);
	return self;
	}

/**
The constructor of the document class just passes the
supplied reference to the constructor initialization list.
*/
CMobileOfficeDocument::CMobileOfficeDocument(CQikApplication& aApp)
	: CQikDocument(aApp)
	{
	}

/**
2nd stage construction of the model.
All code that shall be called in initializing phase and might leave shall be
added here.
*/
void CMobileOfficeDocument::ConstructL()
	{
	}

/**
This is called by the UI framework as soon as the document has been created.
It creates an instance of the ApplicationUI. The Application UI class is an
instance of a CEikAppUi derived class.
*/
CEikAppUi* CMobileOfficeDocument::CreateAppUiL()
	{
	return new (ELeave) CMobileOfficeAppUi;
	}


void CMobileOfficeDocument::OpenFileL(CFileStore *&aFileStore, RFile &aFile)
{
	
	aFileStore = NULL;
	const TUid KViewId = {1};
	TFileName fileName;
	User::LeaveIfError(aFile.FullName(fileName));
	
	CMobileOfficeAppUi* app = (CMobileOfficeAppUi*)iAppUi;
	app->OpenDocument()->SetNextViewID(KViewId);

	if (fileName.Find(_L("\\Private\\")) == KErrNotFound)
	{
		app->OpenDocument()->SetFileName(fileName);
		aFile.Close(); 
	}
	else
	{	// copy the file, as it is data-caged by another application		
		RFs aRfs;
		aRfs.Connect();
		
		TInt size;
		aFile.Size(size);
		
		HBufC8 *temp = HBufC8::NewL(size);
		TPtr8 aPtr = temp->Des();
		aFile.Read(aPtr);

		User::LeaveIfError(aFile.Name(fileName));
		fileName.Insert(0,_L("C:\\Media files\\Document\\"));
		
		TInt i = 1;
		while ( BaflUtils::FileExists(aRfs,fileName) )
		{
			_LIT(start,"(");
			_LIT(end,")");
			TBuf<10> number;

			number.Num(i);
			number.Insert(0,start);
			number.Append(end);
			if (i!=1)
			{
				fileName.Delete(fileName.Length()-7,3);
			}

			fileName.Insert(fileName.Length()-4,number);
			i++;
		}
		
		RFile newFile;
		if (newFile.Create(aRfs,fileName,EFileWrite)==KErrNone)
		{
			newFile.Write(aPtr);
			newFile.Close();
		}
		delete temp;
		app->OpenDocument()->SetFileName(fileName);
		aFile.Close(); 

		aRfs.Close();
		aFile.Close();

	}
	
	iAppUi->ActivateViewL(TVwsViewId(KUidMobileOfficeApp,KUidMobileOfficeEditorView));


}
