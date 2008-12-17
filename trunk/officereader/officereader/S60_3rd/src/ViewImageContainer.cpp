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
#include "ViewImageContainer.h"

#include <eiklabel.h>  // for example label control

#include <ImageConversion.h>

#include "OpenDocument.h"

#include "MobileOfficeAppUi.h"
#include "MobileOfficeApp.h"
#include <e32base.h>
#include <gdi.h>

// ================= MEMBER FUNCTIONS =======================

// ---------------------------------------------------------
// CViewImageContainer::ConstructL(const TRect& aRect)
// EPOC two phased constructor
// ---------------------------------------------------------
//
void CViewImageContainer::ConstructL(const TRect& aRect)
    {
    CreateWindowL();
	SetRect(aRect);
    

	iBgContext = CAknsBasicBackgroundControlContext::NewL( KAknsIIDQsnBgAreaMain,aRect,EFalse );
    
	STATIC_CAST(CMobileOfficeAppUi*,iEikonEnv->EikAppUi())->OpenDocument()->GetThubnail(this,TSize(aRect.Width(),aRect.Height()));
	
	ActivateL();
	    }


TTypeUid::Ptr CViewImageContainer::MopSupplyObject(TTypeUid aId) 
{ 
	if (aId.iUid == MAknsControlContext::ETypeId) 
	{ 
		if (iBgContext )
			return MAknsControlContext::SupplyMopObject( aId, iBgContext );
	}
	return CCoeControl::MopSupplyObject(aId);
}

// Destructor
CViewImageContainer::~CViewImageContainer()
    {
		
	if (iBgContext)
	{
		delete iBgContext;
		iBgContext = NULL;
	}
	
}


// ---------------------------------------------------------
// CViewImageContainer::SizeChanged()
// Called by framework when the view size is changed
// ---------------------------------------------------------
//
void CViewImageContainer::SizeChanged()
    {
    // TODO: Add here control resize code etc.
    }

// ---------------------------------------------------------
// CViewImageContainer::CountComponentControls() const
// ---------------------------------------------------------
//
TInt CViewImageContainer::CountComponentControls() const
    {
    return 0; // return nbr of controls inside this container
    }

// ---------------------------------------------------------
// CViewImageContainer::ComponentControl(TInt aIndex) const
// ---------------------------------------------------------
//
CCoeControl* CViewImageContainer::ComponentControl(TInt aIndex) const
    {
    switch ( aIndex )
        {
        default:
            return NULL;
        }
    }

// ---------------------------------------------------------
// CViewImageContainer::Draw(const TRect& aRect) const
// ---------------------------------------------------------
//
void CViewImageContainer::Draw(const TRect& aRect) const
    {
	CWindowGc& gc = SystemGc();
	gc.Clear(aRect);    

//	gc.SetBrushStyle( CGraphicsContext::ENullBrush );

// font color siehe Mobile Search Label
	const CFont* normalFont = iEikonEnv->NormalFont();
	gc.UseFont(normalFont);
	
		MAknsSkinInstance* skin = AknsUtils::SkinInstance(); 
	MAknsControlContext* cc = AknsDrawUtils::ControlContext(this); 

	AknsDrawUtils::Background(skin, cc,this,gc,aRect);
	    
	if (iBitmap )
		gc.BitBlt(TPoint(0,0),iBitmap);
	else
		gc.DrawText(_L("no image available"),TPoint(20	,30));

}

// ---------------------------------------------------------
// CViewImageContainer::HandleControlEventL(
//     CCoeControl* aControl,TCoeEvent aEventType)
// ---------------------------------------------------------
//
void CViewImageContainer::HandleControlEventL(
    CCoeControl* /*aControl*/,TCoeEvent /*aEventType*/)
    {

	iBitmap = STATIC_CAST(CMobileOfficeAppUi* ,iEikonEnv->EikAppUi())->OpenDocument()->Bitmap();
	DrawNow();
}


void CViewImageContainer::HandleResourceChange(TInt aType)
{
	CCoeControl::HandleResourceChange(aType);

	if (aType==KEikDynamicLayoutVariantSwitch)
	{
		TRect aRect;
		AknLayoutUtils::LayoutMetricsRect(AknLayoutUtils::EMainPane,aRect);
		SetRect(aRect);
	}
}

void CViewImageContainer::GetHelpContext(TCoeHelpContext &aContext) const
{
	aContext.iContext = _L("Thumbnail");
	aContext.iMajor = KUidMobileOffice;
}
// End of File  
