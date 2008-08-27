#ifndef MOBILEOFFICEVIEW_H
#define MOBILEOFFICEVIEW_H

#include <QikViewBase.h>
#include <QikMultiPageViewBase.h> 

#include <MQikListBoxObserver.h>

#include <VWSDEF.H> 

#include <QikListBoxModel.h>
#include <QikListBox.h>
#include <QikListBoxData.h>

#include <f32file.h> //rfs


class CMobileOfficeView : public CQikViewBase,MQikListBoxObserver 
	{
public:
	static CMobileOfficeView* NewLC(CQikAppUi& aAppUi);
	~CMobileOfficeView();

	// from CQikViewBase
	TVwsViewId ViewId()const;
	void HandleCommandL(CQikCommand& aCommand);
	TKeyResponse OfferKeyEventL(const TKeyEvent& aKeyEvent,TEventCode aType);

	
		void SearchFiles(RFs& aFs,const TDesC& aPath, const TDesC& aWild);
		void ScanDirL(RFs& aFs, const TDesC& aDir, const TDesC& aWild) ;
		
		void SearchC(void);
		void SearchE(void);
		void AddItem(const TDesC& Text);
		void Delete();
		void ClearListbox();
		//TPtrC ListboxText();

		void HandleListBoxEventL(CQikListBox*  aListBox , TQikListBoxEvent aEventType, TInt , TInt);
		TVwsViewIdAndMessage ViewScreenDeviceChangedL();
		void TabActivatedL(TInt aTabId);

protected:
	// from CQikViewBase
	void ViewConstructL();

private:
	CMobileOfficeView(CQikAppUi& aAppUi);
	void ConstructL();
	};

#endif // MOBILEOFFICEVIEW_H
