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
 * Generic interface for hash functions (e.g. checksums,
 * cryptographic hash functions).
 *
 * Version: 1.0
 *
 * bit-side GmbH 2005
 * www.bit-side.com
 *
 * Author: Philipp Henkel <p.henkel@bit-side.com>
 *
 *
 * This code is placed in the public domain.
 *
 *
 * The code is distributed on an 'AS IS' basis, WITHOUT
 * WARRANTY OF ANY KIND, EITHER EXPRESS OR IMPLIED.
 *
 */


#ifndef __HASHFUNC_H__
#define __HASHFUNC_H__

#include <w32std.h>
#include <f32file.h>

//namespace util {

/** Interface for hash functions.
  * A hash function is a function that converts an input from a (typically)
  * large domain into an output in a (typically) smaller range.
  *
  */
class MHashFunc {
public:
  /** Appends data to domain. Can be called multiple times. */
  virtual void AppendToHashL(const TDesC8& aData) = 0;

  /** Returns hash value. AppendToHash has to be called first.
    * The Caller takes the ownership of the hash buffer.
    */
  virtual HBufC8* GetHashAndResetL() = 0;


  /** Resets internal buffers.
    */
  virtual void ResetHash() = 0;


public:
  virtual ~MHashFunc();
};


inline MHashFunc::~MHashFunc(){};

//} // namespace util


#endif // __HASHFUNC_H__


