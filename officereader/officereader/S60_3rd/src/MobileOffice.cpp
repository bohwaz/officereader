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
#include <eikstart.h>
#include "MobileOfficeApplication.h"

#include "e32cmn.h"



LOCAL_C CApaApplication* NewApplication()
	{
	return new CMobileOfficeApplication;
	}

GLDEF_C TInt E32Main()
	{
		
#ifdef _DEBUG
	RHeap *heap = UserHeap::ChunkHeap(0,100000,10000000,100000);
	User::SwitchHeap(heap);
#endif

	return EikStart::RunApplication( NewApplication );
	}
