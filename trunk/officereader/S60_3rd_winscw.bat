#/*************************************************************************
# *
# * DO NOT ALTER OR REMOVE COPYRIGHT NOTICES OR THIS FILE HEADER.
# * 
# * Copyright 2008,2009,2010 by ODENDAHL SEPT-Solutions
# *
# * Office Reader - a OpenDocument viewer for Symbian OS
# *
# * $Id:$
# *
# * This file is part of Office Reader
# *
# * Office Reader is free software: you can redistribute it and/or modify
# * it under the terms of the GNU General Public License version 3 or GNU 
# * General Lesser Public License version 3 , as published by the Free Software 
# * Foundation.
# *
# * Office Reader is distributed in the hope that it will be useful,
# * but WITHOUT ANY WARRANTY; without even the implied warranty of
# * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
# * GNU General (Lesser) Public License version 3 for more details
# * (a copy is included in the LICENSE file that accompanied this code).
# *
# * You should have received a copy of the GNU (Lesser) General Public License
# * version 3 along with Office Reader.  If not, 
# * see <http://www.gnu.org/licenses/>.
# *
# ************************************************************************/


echo "Compiling for S60 3nd Edition"
devices -setdefault @S60_3rd_MR:com.nokia.s60

cd SyExpat/Symbian
bldmake bldfiles
START ABLD.bat build winscw udeb
cd ../..
echo "Finished compiling SyExpat XML parser"

echo "Start compiling Sheet Control"

cd sheetcontrol/group
bldmake bldfiles
START ABLD.bat build winscw udeb
cd ../..
echo "Finished compiling Sheet Control

echo "Start compiling Engine"
cd officereader\Engine\group
bldmake bldfiles
START ABLD.bat build winscw udeb
cd ../../..
echo "Finished compiling Engine


echo "Start compiling Application"
cd officereader\S60_3rd\group
bldmake bldfiles
START ABLD.bat build winscw udeb
echo "Finished compiling Application"

echo "Finished for S60 3nd Edition"