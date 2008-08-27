/*
============================================================================
 Name        : CMobileOfficeApplication from MobileOfficeApplication.h
 Author      : 
 Version     :
 Copyright   : Your copyright notice
 Description : Declares main application class.
============================================================================
*/

#ifndef MOBILEOFFICEAPPAPPLICATION_H
#define MOBILEOFFICEAPPAPPLICATION_H

#include <QikApplication.h>

class CMobileOfficeApplication : public CQikApplication
	{
private:
	CApaDocument* CreateDocumentL();
	TUid AppDllUid() const;
	CApaApplication* NewApplication();
	};

#endif // MOBILEOFFICEAPPAPPLICATION_H
