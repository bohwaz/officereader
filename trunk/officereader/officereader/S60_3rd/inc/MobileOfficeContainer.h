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

#ifndef MOBILEOFFICECONTAINER_H
#define MOBILEOFFICECONTAINER_H

// INCLUDES
#include <coecntrl.h>
   
// FORWARD DECLARATIONS


#include <coecntrl.h>
#include <eikenv.h>
#include <eikrted.h>
#include <txtrich.h>
#include <eiktbar.h>
#include <eikaufty.h>
#include <eikspane.h>
#include <eikdoc.h>

#include <eikspane.h>
#include <aknnavide.h> 
#include <akntabgrp.h>
#include <akntabobserver.h> 

#include <aknsfld.h> 
#include <aknlists.h>    
#include <EIKLBX.H>


// CLASS DECLARATION

/**
*  CMobileOfficeContainer  container control class.
*  
*/
class CMobileOfficeContainer : public CCoeControl, MCoeControlObserver
    {
    public: // Constructors and destructor
        
        /**
        * EPOC default constructor.
        * @param aRect Frame rectangle for container.
        */
        void ConstructL(const TRect& aRect);

        /**
        * Destructor.
        */
        ~CMobileOfficeContainer();

    public: // New functions

		void SearchFiles(RFs& aFs,const TDesC& aPath, const TDesC& aWild);
		void ScanDirL(RFs& aFs, const TDesC& aDir, const TDesC& aWild) ;
		
		void Search(void);
		void ClearListbox(void);
		void Delete();

		void AddItem(TDes& Text);
		TPtrC ListboxText();
		TInt ListboxIndex(void);
		CAknSingleStyleListBox* iListbox;

		void HideTab();
		void ShowTab();

    private: // Functions from base classes
       /**
        * From CoeControl,SizeChanged.
        */
        void SizeChanged();

       /**
        * From CoeControl,CountComponentControls.
        */
        TInt CountComponentControls() const;

       /**
        * From CCoeControl,ComponentControl.
        */
        CCoeControl* ComponentControl(TInt aIndex) const;

       /**
        * From CCoeControl,Draw.
        */
        void Draw(const TRect& aRect) const;

       	
		/**
		* From MCoeControlObserver
		* Acts upon changes in the hosted control's state. 
		*
		* @param	aControl	The control changing its state
		* @param	aEventType	The type of control event 
		*/
        void HandleControlEventL(CCoeControl* aControl,TCoeEvent aEventType);
		TKeyResponse OfferKeyEventL(const TKeyEvent& aKeyEvent,TEventCode aType);   
		void HandleResourceChange(TInt aType);
	
		CAknNavigationControlContainer* iNaviPane;
		CAknNavigationDecorator* 		iNaviDecorator;
	private: //data
		CAknSearchField* 			iFindbox;	
    
    };		

#endif


