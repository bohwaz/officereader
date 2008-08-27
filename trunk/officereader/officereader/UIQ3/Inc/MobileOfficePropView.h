#ifndef MOBILEOFFICEPROPVIEW_H
#define MOBILEOFFICEPROPVIEW_H

#include <QikViewBase.h>
#include <QikListBoxModel.h>
#include <QikListBox.h>
#include <QikListBoxData.h>
#include <VWSDEF.H>

/**
CMobileOfficePropView description.
*/
class CMobileOfficePropView : public CQikViewBase
	{
public:
	static CMobileOfficePropView* NewLC(CQikAppUi& aAppUi);
	~CMobileOfficePropView();

	// from CQikViewBase
	TVwsViewId ViewId()const;
	void HandleCommandL(CQikCommand& aCommand);
	TVwsViewIdAndMessage ViewScreenDeviceChangedL();

	void AddItem(TDes& Text1,HBufC* Text2);
	void ViewActivatedL(  const TVwsViewId& /*aPrevViewId*/,TUid /*aCustomMessageId*/,const TDesC8& /*aCustomMessage*/);
	void ViewDeactivated();
	CQikCommand *DynInitOrDeleteCommandL(CQikCommand *aCommand, const CCoeControl &aControlAddingCommands);

protected:
	// from CQikViewBase
	void ViewConstructL();

private:
	CMobileOfficePropView(CQikAppUi& aAppUi);
	void ConstructL();
	};

#endif // MOBILEOFFICEPROPVIEW_H
