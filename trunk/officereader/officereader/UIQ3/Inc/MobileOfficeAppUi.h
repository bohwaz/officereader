#ifndef MOBILEOFFICEAPPUI_H
#define MOBILEOFFICEAPPUI_H

#include <QikAppUi.h>
#include "OpenDocument.h"

#ifndef FREEVERSION
	#include "LicenseManager.h"
#endif

/**
This class represents the application UI in MobileOffice application,
It has responsibility to create the view.
*/
class CMobileOfficeAppUi : public CQikAppUi
    {
public:
	// from CQikAppUi

	~CMobileOfficeAppUi();
	COpenDocument* OpenDocument();
	void ConstructL();

	TFileName GetAppPath();

	CArrayFix<TCoeHelpContext>* HelpContextL() const;
	void CheckDemo();
	#ifndef FREEVERSION
		 bool Register();
		CLicenseManager LicMan();
		TBool Registered();
	#endif

	private:
		COpenDocument* iOpenDocument;
		#ifndef FREEVERSION
			CLicenseManager licMan;
		#endif
	};
#endif // MOBILEOFFICEAPPUI_H
