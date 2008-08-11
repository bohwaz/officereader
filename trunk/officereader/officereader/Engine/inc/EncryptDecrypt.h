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

#ifndef __ENCRYPTDECRYPT_H__
#define __ENCRYPTDECRYPT_H__
#include <e32des8.h>

class CEncryptDecrypt 
{
public:
/*!
  @function NewL
   
  @discussion Create a CEncryptDecrypt object, 
  */
    static CEncryptDecrypt* NewL();

/*!
  @function NewLC
   
 @result a pointer to the created instance of CEncryptDecrypt
  */
    static CEncryptDecrypt* NewLC();


/*!
  @function ~CEncryptDecrypt
  
  @discussion Destroy the object and release all memory objects
  */
     ~CEncryptDecrypt();
private:

/*!
  @function ConstructL
  
  @discussion  Perform the second phase construction of a CEncryptDecrypt object
  */
    static void ConstructL();
	
	//Constructor
	CEncryptDecrypt ();
public:
/*!
  @function Encrypt
  
  @discussion  Encrypt using Blowfish CFB
  @Param Password
  @Param Message to Encrypt
  @Param Salt Output Salt parameter 
  @Param IniVector Output Initialization Vector parameter 
  @Return Buffer with ENcrypted text of the same length as input text FREEING THE MEMORY IS CALLER'S RESPONSIBILITY
  */
	HBufC8* Encrypt(const TDesC8& Password, const TDesC8& Message, TPtr8 Salt, TPtr8 IniVector);
/*!
  @function Decrypt
  
  @discussion  Decrypt using Blowfish CFB
  @Param Password
  @Param Message to Decrypt
  @Param Salt Input Salt parameter obtained via Encrypt
  @Param IniVector Input Initialization Vector Obtained via encrypt
  @Return Buffer with ENcrypted text of the same length as input text FREEING THE MEMORY IS CALLER'S RESPONSIBILITY
  */

	HBufC8* Decrypt(const TDesC8& password, const TDesC8& Message, const TDesC8& Salt, const TDesC8& IniVector);
private:
	void CreateSalt(const TInt& SaltLen);
	void CreateInitializationVector(const TInt& SaltLen);
	void PBKDF2 (const TDesC8& P, const TInt c);
	void RandomSeq(const TInt& Len, HBufC8* Seq);
	//TDes8& Or(TPtrC8 mac, TInt i, TPtr8 targetbuffer);
	void xOr(TPtrC8 sourcebuffer,TPtr8 targetbuffer);
	void CreateKey(const TDesC8& Password);
	HBufC8* iSalt;
	HBufC8* iIniVector;
	HBufC8* iKey;
};
#endif
