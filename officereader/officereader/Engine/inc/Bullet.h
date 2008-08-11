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

#ifndef BULLET_H
#define BULLET_H

// INCLUDES
#include <e32std.h>
#include <e32base.h>

// CLASS DECLARATION


enum TBulletType
{
	EBTNumeric = 1000,
	EBTRoman,
	EBTFixed,
	EBTImage
};

class CBullet : public CBase
{
public:

	~CBullet();

	static CBullet* NewL();

	static CBullet* NewLC();

	void SetIndent(TInt aIndent);
	TInt Indent();
	
	void ResetChar(TChar aValue);
	void IncreaseChar();
	void SetType(TBulletType aType);
	void SetChar(TChar aChar);
	TChar Char();

private:

	CBullet();
	
	TBulletType iType;
	TChar iCurrentChar;
	TInt iIndent;

	void ConstructL();
};

class CBullets : public CBase
{
public: // Constructors and destructor

	~CBullets();

	static CBullets* NewL();

	static CBullets* NewLC();

	TInt GetLevel();

	void VerifyBullets(void);

	void AddBullet(TBulletType aType);
	void SetType(TBulletType aType);
	void SetIndent(TInt aIndent);
	void IncChar();
	
	void SetName(const TDesC& aName);
	TPtrC Name();

	void SetChar(TChar aChar);
	
	//void SetPrefix();
	// SetSuffic
	//TDesC gettext -> text mit prefix, suffix z.B iii)
	TChar Char();
	
	TInt Indent();

	void IncLevel();
	void DecLevel();

	void ResetBullet(TChar aValue);
private:

	CBullets();

	TInt iPos;
	TInt iLevel;
	void ConstructL();

	HBufC* iName;
	RPointerArray<CBullet>* iBullets;

};

#endif // BULLET_H
	
