#ifndef MOBILEOFFICEEDITORVIEW_H
#define MOBILEOFFICEEDITORVIEW_H

#include <QikViewBase.h>
#include <QikScrollableContainer.h>

#include <EIKRTED.H> // CeikRichtexteditor
#include <TXTRICH.H> // CRichtext 

#include <VWSDEF.H>

#include "sheet_ctrl.h"
#include "sheet_model.h"
#include "sheet_data.h"
#include "sheet_view.h"
#include "cell.h"


class CQikFullscreenFloatingMenu;


class CMobileOfficeEditorView : public CQikViewBase
	{
public:
	static CMobileOfficeEditorView* NewLC(CQikAppUi& aAppUi);
	~CMobileOfficeEditorView();

	// from CQikViewBase
	TVwsViewId ViewId()const;
	void GetHelpContext(TCoeHelpContext &aContext) const; 

	void ViewActivatedL(   
        const TVwsViewId& /*aPrevViewId*/,
        TUid /*aCustomMessageId*/,
        const TDesC8& /*aCustomMessage*/);
	void ViewDeactivated();

	void HandleCommandL(CQikCommand& aCommand);
	TKeyResponse OfferKeyEventL(const TKeyEvent& aKeyEvent,TEventCode aType);
	TVwsViewIdAndMessage ViewScreenDeviceChangedL();
	//void HandlePointerEventL(const TPointerEvent &aPointerEvent);

	void SetupText(void);
	void SetupSheet(void);
			
	CEikRichTextEditor* Editor();
	CSheetCtrl* Sheet();

protected:
	// from CQikViewBase
	void ViewConstructL();

private:
	CMobileOfficeEditorView(CQikAppUi& aAppUi);
	void ConstructL();

	TInt iLength;
	CEikRichTextEditor* rted;
	CSheetCtrl* iSheet;
	CQikScrollableContainer* container;

	CQikFullscreenFloatingMenu* iFloatingMenu;
	};

#endif // MOBILEOFFICEEDITORVIEW_H
