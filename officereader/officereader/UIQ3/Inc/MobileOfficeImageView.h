#ifndef MOBILEOFFICEIMAGEVIEW_H
#define MOBILEOFFICEIMAGEVIEW_H

#include <QikViewBase.h>

#include <FBS.H>
#include <VWSDEF.H>


/**
CMobileOfficeImageView description.
*/
class CMobileOfficeImageView : public CQikViewBase
	{
public:
	static CMobileOfficeImageView* NewLC(CQikAppUi& aAppUi);
	~CMobileOfficeImageView();

	// from CQikViewBase
	TVwsViewId ViewId()const;
	void HandleCommandL(CQikCommand& aCommand);

	void Draw(const TRect& aRect) const;

	void HandleControlEventL(CCoeControl* aControl,TCoeEvent );
	void ViewActivatedL(  const TVwsViewId& /*aPrevViewId*/,TUid /*aCustomMessageId*/,const TDesC8& /*aCustomMessage*/);
	TVwsViewIdAndMessage ViewScreenDeviceChangedL();


protected:
	// from CQikViewBase
	void ViewConstructL();

private:
	CMobileOfficeImageView(CQikAppUi& aAppUi);
	void ConstructL();
	CFbsBitmap* iBitmap;
	};

#endif // MOBILEOFFICEIMAGEVIEW_H
