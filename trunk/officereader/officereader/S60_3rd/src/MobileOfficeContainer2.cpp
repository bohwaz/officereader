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
#include "MobileOfficeContainer2.h"
#include "MobileOfficeAppUi.h"
#include "MobileOfficeApp.h"

#include <eiklabel.h>  // for example label control
#include <eikclbd.h> //marquee

#include <stringloader.h>

#include <aknnotewrappers.h> 
#include <AknQueryDialog.h>  // findquery

#include  "MobileOffice.hrh"
#include <MobileOffice.rsg> // resources


// ================= MEMBER FUNCTIONS =======================

// ---------------------------------------------------------
// CMobileOfficeContainer2::ConstructL(const TRect& aRect)
// EPOC two phased constructor
// ---------------------------------------------------------
//
void CMobileOfficeContainer2::ConstructL(const TRect& aRect)
    {
	CreateWindowL();
	SetRect(aRect);
	
	iListbox = new( ELeave )CAknDoubleStyleListBox();
	iListbox->ConstructL(this, EAknListBoxSelectionList);
	iListbox->SetContainerWindowL(*this); 
	iListbox->CreateScrollBarFrameL(ETrue); 
	iListbox->ScrollBarFrame()->SetScrollBarVisibilityL( CEikScrollBarFrame::EOn, CEikScrollBarFrame::EAuto ); 
	iListbox->SetRect( Rect() );
	iListbox->ItemDrawer()->ColumnData()->EnableMarqueeL( ETrue );
	iListbox->ActivateL();
    
	
	COpenDocument* iDoc = STATIC_CAST(CMobileOfficeAppUi*,iEikonEnv->EikAppUi())->OpenDocument();
	if (iDoc->Encrypted())
	{
		TBuf<50> text;
		CAknTextQueryDialog* FindQuery = CAknTextQueryDialog::NewL(text);
		CleanupStack::PushL(FindQuery);
		HBufC* prompt = prompt = StringLoader::LoadLC(R_PASSWORD_QUERY);
		FindQuery->SetPromptL(*prompt);
		CleanupStack::PopAndDestroy(prompt);
		CleanupStack::Pop(FindQuery);
		if (FindQuery->ExecuteLD(R_DATAQUERY_DATA_QUERY))
		{
			iDoc->SetPassword(text);
			iDoc->LoadMetaInfomation();
		}
	}
	else
		iDoc->LoadMetaInfomation();

	ShowMeta();
	ActivateL();
}

// Destructor
CMobileOfficeContainer2::~CMobileOfficeContainer2()
    {
    delete iListbox;
   
    }

// ---------------------------------------------------------
// CMobileOfficeContainer2::SizeChanged()
// Called by framework when the view size is changed
// ---------------------------------------------------------
//
void CMobileOfficeContainer2::SizeChanged()
{
   	if (iListbox)
	{
		iListbox->SetExtent( TPoint( 0, 0 ), iListbox->MinimumSize() );
	}
}

// ---------------------------------------------------------
// CMobileOfficeContainer2::CountComponentControls() const
// ---------------------------------------------------------
//
TInt CMobileOfficeContainer2::CountComponentControls() const
    {
    return 1; // return nbr of controls inside this container
    }

// ---------------------------------------------------------
// CMobileOfficeContainer2::ComponentControl(TInt aIndex) const
// ---------------------------------------------------------
//
CCoeControl* CMobileOfficeContainer2::ComponentControl(TInt aIndex) const
    {
    switch ( aIndex )
        {
        case 0:
            return iListbox;
        default:
            return NULL;
        }
    }

// ---------------------------------------------------------
// CMobileOfficeContainer2::Draw(const TRect& aRect) const
// ---------------------------------------------------------
//
void CMobileOfficeContainer2::Draw(const TRect& /*aRect*/) const
{
	/*CWindowGc& gc = SystemGc();
	gc.SetPenStyle(CGraphicsContext::ENullPen);
	gc.SetBrushStyle(CGraphicsContext::ESolidBrush);
	gc.DrawRect(aRect);
	*/
}

// ---------------------------------------------------------
// CMobileOfficeContainer2::HandleControlEventL(
//     CCoeControl* aControl,TCoeEvent aEventType)
// ---------------------------------------------------------
//
void CMobileOfficeContainer2::HandleControlEventL(
    CCoeControl* /*aControl*/,TCoeEvent /*aEventType*/)
    {
    // TODO: Add your control event handler code here
    }


TKeyResponse CMobileOfficeContainer2::OfferKeyEventL(const TKeyEvent& aKeyEvent,TEventCode aType)
{
	if (iListbox)
		return iListbox->OfferKeyEventL(aKeyEvent,aType);
	else
		return EKeyWasNotConsumed;

}

void CMobileOfficeContainer2::AddItem(TDes& Text)
{
	CDesC16Array * list_array = ( CDesC16Array * ) iListbox->Model()->ItemTextArray();
	Text.Insert(0,_L("\t"));
	list_array->AppendL(Text);
    iListbox->HandleItemAdditionL();
}

void CMobileOfficeContainer2::ShowMeta(void)
{

	COpenDocument *doc =STATIC_CAST(CMobileOfficeAppUi* ,iEikonEnv->EikAppUi())->OpenDocument();
	
		TBuf<250> Dateiname;
	Dateiname.Append(_L("Filename\t"));
	Dateiname.Append( doc->Filename() );
	AddItem(Dateiname);

/*	TBuf<100> Typ;
	Typ.Append(_L("Typ\t"));
	Typ.Append( doc->OpenDocument()->Filename() );
	AddItem(Typ);
*/

	TBuf<150> Title;
	Title.Append(_L("Title\t"));
	Title.Append( doc->GetTitle() );
	AddItem(Title);

	TBuf<150> Subject;
	Subject.Append(_L("Subject\t"));
	Subject.Append( doc->GetSubject() );
	AddItem(Subject);

	TBuf<150> Description;
	Description.Append(_L("Description\t"));
	Description.Append( doc->GetDescription() );
	AddItem(Description);

	TBuf<150> Keywords;
	Keywords.Append(_L("Keywords\t"));
	Keywords.Append( doc->GetKeywords() );
	AddItem(Keywords);

	/*
	TBuf<100> Info;
	Info.Append(_L("Info\t"));
	Info.Append( STATIC_CAST(CAppUi* ,iEikonEnv->EikAppUi())->OpenDocument()->GetInfo1() );
	AddItem(Info);
*/
	

	TBuf<150> Words;
	Words.Append(_L("Words\t"));
	Words.Append( doc->GetWords() );
	AddItem(Words);
	
	TBuf<150> Para;
	Para.Append(_L("Paragraphs\t"));
	Para.Append( doc->GetParagraphs() );
	AddItem(Para);

	TBuf<150> Pages;
	Pages.Append(_L("Pages\t"));
	Pages.Append( doc->GetPages() );
	AddItem(Pages);

}

  

void CMobileOfficeContainer2::HandleResourceChange(TInt aType)
{
	CCoeControl::HandleResourceChange(aType);

	if (aType==KEikDynamicLayoutVariantSwitch)
	{
		TRect aRect;
		AknLayoutUtils::LayoutMetricsRect(AknLayoutUtils::EMainPane,aRect);
		SetRect(aRect);
		//iListbox->SetRect(aRect);
	}
}

void CMobileOfficeContainer2::GetHelpContext(TCoeHelpContext &aContext) const
{
	aContext.iContext = _L("Properties");
	aContext.iMajor = KUidMobileOffice;
}
  
