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

#ifndef MOBILEOFFICEDOCUMENT_H
#define MOBILEOFFICEDOCUMENT_H

// INCLUDES
#include <akndoc.h>
   
// CONSTANTS

// FORWARD DECLARATIONS
class  CEikAppUi;

// CLASS DECLARATION

/**
*  CMobileOfficeDocument application class.
*/
class CMobileOfficeDocument : public CAknDocument
    {
    public: // Constructors and destructor
        /**
        * Two-phased constructor.
        */
        static CMobileOfficeDocument* NewL(CEikApplication& aApp);

        /**
        * Destructor.
        */
        virtual ~CMobileOfficeDocument();

    public: // New functions

		
    public: // Functions from base classes
		CFileStore* OpenFileL( TBool aDoOpen, const TDesC& aFilename, RFs& aFs );
			 void OpenFileL(CFileStore *&aFileStore, RFile &aFile);
	//	CFileStore* OpenFileL( TBool aDoOpen, const TDesC& aFilename, RFs& aFs );
    protected:  // New functions

    protected:  // Functions from base classes

    private:

        /**
        * EPOC default constructor.
        */
        CMobileOfficeDocument(CEikApplication& aApp);
        void ConstructL();

    private:

        /**
        * From CEikDocument, create CMobileOfficeAppUi "App UI" object.
        */
        CEikAppUi* CreateAppUiL();
    };

#endif



