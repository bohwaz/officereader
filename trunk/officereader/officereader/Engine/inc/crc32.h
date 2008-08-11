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

/*
 * Table-driven implementation of the checksum
 * algorithm CRC-32 for Symbian OS.
 *
 * Version: 1.0
 *
 * bit-side GmbH 2005
 * www.bit-side.com
 *
 * Author: Philipp Henkel <p.henkel@bit-side.com>
 * Based on Ross Williams' "Painless Guide to CRC Error
 *                          Detection Algorithms".
 *
 * This code is placed in the public domain.
 *
 *
 * The code is distributed on an 'AS IS' basis, WITHOUT
 * WARRANTY OF ANY KIND, EITHER EXPRESS OR IMPLIED.
 *
 */



#ifndef __CRC32_H__
#define __CRR32_H__

#include <e32base.h>

#include "hashfunc.h"

//namespace util {


/** This class implements CRC-32.
  * Polynom is 0x04C11DB7, reflection is used.
  */
class CCrc32
: public CBase, public MHashFunc
{
public:
  static CCrc32* NewL();
  static CCrc32* NewLC();
  ~CCrc32();

public:  // implements MHashFunc
  void    AppendToHashL(const TDesC8& aData);
  HBufC8* GetHashAndResetL();
  void    ResetHash();

protected:
  CCrc32();
  void ConstructL();

public: // helper
  static void HexBytesToHexAscii(const TDesC8& aText, TDes8& aHexCode);


protected:
  TUint32 iCrc;
  static const TUint32 iCrcTable[];  /** lookup table */

#if defined(_DEBUG)
private:
  TBool RunSelfTestL();
#endif

};


//} // namespace util


#endif // __CRC32_H__
