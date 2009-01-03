#ifndef MOBILEOFFICEDOCUMENT_H
#define MOBILEOFFICEDOCUMENT_H

#include <QikDocument.h>

/**
This class represents the document in MobileOffice application,
For an UIQ application to work properly the document class need to be
derived from CQikDocument.
*/
class CMobileOfficeDocument : public CQikDocument
	{
public:
	static CMobileOfficeDocument* NewL(CQikApplication& aApp);
	void OpenFileL(CFileStore *&aFileStore, RFile &aFile);

private:
	// from CQikDocument
	CEikAppUi* CreateAppUiL();

	CMobileOfficeDocument(CQikApplication& aApp);
	void ConstructL();
	};
#endif // MOBILEOFFICEDOCUMENT_H
