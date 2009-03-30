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

#ifndef MOBILEOFFICEAPPUI_H
#define MOBILEOFFICEAPPUI_H

// INCLUDES
#include <aknviewappui.h>
#include <aknappui.h>  // orientation
#include <akntabgrp.h>
#include <aknnavide.h>


#include <akntabobserver.h>

#ifndef FREEVERSION
	#include "LicenseManager.h"
#endif

#include "OpenDocument.h"


// FORWARD DECLARATIONS
class CMobileOfficeContainer;


// CLASS DECLARATION

/**
* Application UI class.
* Provides support for the following features:
* - EIKON control architecture
* - view architecture
* - status pane
* 
*/
class CMobileOfficeAppUi : public CAknViewAppUi, MAknTabObserver
    {
    public: // // Constructors and destructor

        /**
        * EPOC default constructor.
        */      
        void ConstructL();

        /**
        * Destructor.
        */      
        ~CMobileOfficeAppUi();
        
    public: // New functions

		COpenDocument* OpenDocument();
		TFileName GetAppPath();
		void CheckDemo();
#ifndef FREEVERSION
		 bool Register();
		TBool Registered();
#endif

    public: // Functions from base classes
		/**
        * From MAknTabObserver.
        * @param aIndex tab index
        */
		void TabChangedL(TInt aIndex);
		void HandleCommandL(TInt aCommand);	
	void OpenFileL( const TDesC& aFileName );
		CArrayFix<TCoeHelpContext>* HelpContextL() const;

    private:
        // From MEikMenuObserver
        void DynInitMenuPaneL(TInt aResourceId,CEikMenuPane* aMenuPane);

		COpenDocument* iOpenDocument;
    private:
        

        /**
        * From CEikAppUi, handles key events.
        * @param aKeyEvent Event to handled.
        * @param aType Type of the key event. 
        * @return Response code (EKeyWasConsumed, EKeyWasNotConsumed). 
        */
        virtual TKeyResponse HandleKeyEventL(
            const TKeyEvent& aKeyEvent,TEventCode aType);

    private: //Data
        CAknNavigationControlContainer* iNaviPane;
        CAknTabGroup*                   iTabGroup;
        CAknNavigationDecorator*        iDecoratedTabGroup;
	#ifndef FREEVERSION
		CLicenseManager licMan;
	#endif
    };

#endif


