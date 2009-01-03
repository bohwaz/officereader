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

#ifndef EDITORVIEWCONTAINER_H
#define EDITORVIEWCONTAINER_H

// INCLUDES
#include <coecntrl.h>

#include "sheet_ctrl.h"
#include "sheet_model.h"
#include "sheet_data.h"
#include "sheet_view.h"
#include "cell.h"

#include <aknwaitdialog.h>



#include <eikrted.h>

// CLASS DECLARATION

/**
*  CEditorViewContainer  container control class.
*  
*/
class CEditorViewContainer : public CCoeControl, MCoeControlObserver
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
        ~CEditorViewContainer();

    public: // New functions
		void SetupText(void);
		void SetupSheet(void);
		CEikRichTextEditor* Editor();
		CSheetCtrl* Sheet();
		

    public: // Functions from base classes
		void GetHelpContext(TCoeHelpContext &aContext) const;

	private:
	

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
    private: //data
        
		CEikRichTextEditor* iEditor;
		CSheetCtrl* iSheet;
		CAknWaitDialog* iWaitDialog;

    };

#endif

// End of File
		
