#include <eikstart.h>
#include "e32cmn.h"

#include "MobileOfficeApplication.h"
#include "MobileOfficeDocument.h"
#include "MobileOfficeExternalInterface.h" // contains the applications UID

/**
The function is called by the UI framework to ask for the
application's UID. The returned value is defined by the
constant KUidMobileOfficeApp and must match the second value
defined in the project definition file.
*/
TUid CMobileOfficeApplication::AppDllUid() const
	{
	return KUidMobileOfficeApp;
	}

/**
This function is called by the UI framework at application start-up.
It creates an instance of the document class.
*/
CApaDocument* CMobileOfficeApplication::CreateDocumentL()
	{
	return CMobileOfficeDocument::NewL(*this);
	}

/**
The function is called by the framework immediately after it has started the
application's EXE. It is called by the framework and is expected to have
exactly this prototype.
Function is a non-leaving function, this because we need to create the 
application object or return NULL on failure. Therefore we cant use the 
new(ELeave) operator.

@return Instance of the application class.
*/
CApaApplication* NewApplication()
	{
	return new CMobileOfficeApplication;
	}

/**
E32Main() contains the program's start up code, the entry point for an EXE.
*/
GLDEF_C TInt E32Main()
	{
		/*
		#ifdef _DEBUG
	RHeap *heap = UserHeap::ChunkHeap(0,100000,10000000,100000);
	User::SwitchHeap(heap);
#endif
	*/

	return EikStart::RunApplication(NewApplication);
	}
