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
#include "MobileOfficeDocument.h"
#include "MobileOfficeAppui.h"

#include <BAUTILS.H>


// ================= MEMBER FUNCTIONS =======================

// constructor
CMobileOfficeDocument::CMobileOfficeDocument(CEikApplication& aApp)
: CAknDocument(aApp)    
    {
    }

// destructor
CMobileOfficeDocument::~CMobileOfficeDocument()
    {
    }

// EPOC default constructor can leave.
void CMobileOfficeDocument::ConstructL()
    {
    }

// Two-phased constructor.
CMobileOfficeDocument* CMobileOfficeDocument::NewL(
        CEikApplication& aApp)     // CMobileOfficeApp reference
    {
    CMobileOfficeDocument* self = new (ELeave) CMobileOfficeDocument( aApp );
    CleanupStack::PushL( self );
    self->ConstructL();
    CleanupStack::Pop();

    return self;
    }
    
// ----------------------------------------------------
// CMobileOfficeDocument::CreateAppUiL()
// constructs CMobileOfficeAppUi
// ----------------------------------------------------
//
CEikAppUi* CMobileOfficeDocument::CreateAppUiL()
    {
    return new (ELeave) CMobileOfficeAppUi;
    }

 CFileStore* CMobileOfficeDocument::OpenFileL( TBool aDoOpen, const TDesC& aFilename, RFs& aFs )
 {
	 	return  CAknDocument::OpenFileL (aDoOpen, aFilename, aFs);
 }

void CMobileOfficeDocument::OpenFileL(CFileStore *&aFileStore, RFile &aFile)
{
	aFileStore = NULL;
	const TUid KViewId = {1};
	TFileName fileName;
	User::LeaveIfError(aFile.FullName(fileName));
	
	CMobileOfficeAppUi* app = (CMobileOfficeAppUi*) iAvkonAppUi;
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
		fileName.Insert(0,_L("C:\\"));
		
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
	
	iAvkonAppUi->HandleCommandL(3);
	
}
