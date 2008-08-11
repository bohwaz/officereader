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

#include "Bullet.h"
#include <gulutil.h>

CBullet::CBullet()
{
	// No implementation required
}


CBullet::~CBullet()
{
}

CBullet* CBullet::NewLC()
{
	CBullet* self = new (ELeave)CBullet();
	CleanupStack::PushL(self);
	self->ConstructL();
	return self;
}

CBullet* CBullet::NewL()
{
	CBullet* self=CBullet::NewLC();
	CleanupStack::Pop(); // self;
	return self;
}

void CBullet::ConstructL()
{
    iIndent = 0;
}

void CBullet::ResetChar(TChar aValue)
{
	if (iType==EBTNumeric)
		iCurrentChar = aValue;
}

void CBullet::IncreaseChar()
{
	if (iType == EBTNumeric)
	{

		// : auch noch für A..... und a....
		if ( (iCurrentChar==0x39) || (iCurrentChar==0x5A) || (iCurrentChar==0x7A) || (iCurrentChar == 0x2022))
		{
			iCurrentChar = 0x2022;
		}
		else
			iCurrentChar += 1;
	}
	
}


void CBullet::SetChar(TChar aChar)
{
	iCurrentChar = aChar;
}

void CBullet::SetType(TBulletType aType)
{
	iType = aType;
}

TChar CBullet::Char()
{
	return iCurrentChar;
}

void CBullet::SetIndent(TInt aIndent)
{
	iIndent = aIndent;
}

TInt CBullet::Indent()
{
	return iIndent;
}





/////BULLETS
CBullets::CBullets()
{
	// No implementation required
}


CBullets::~CBullets()
{
	
	__ASSERT_DEBUG(iLevel==-1,User::Panic(_L("~CBullets"),0));
	iBullets->ResetAndDestroy();
	delete iBullets;

	delete iName;
}

CBullets* CBullets::NewLC()
{
	CBullets* self = new (ELeave)CBullets();
	CleanupStack::PushL(self);
	self->ConstructL();
	return self;
}

CBullets* CBullets::NewL()
{
	CBullets* self=CBullets::NewLC();
	CleanupStack::Pop(); // self;
	return self;
}

void CBullets::ConstructL()
{
	iLevel = -1;
	iPos = -1;
	iBullets = new (ELeave) RPointerArray<CBullet>;
}



void CBullets::AddBullet(TBulletType aType)
{
	CBullet* aBullet = CBullet::NewL();
	aBullet->SetType(aType);
	iBullets->Append(aBullet);
	iPos++;
}

void CBullets::VerifyBullets(void)
{
	if (iPos==-1)
	{
		// we have never added a bullet, so we add a default one
		// maybe we need to add all ten levels??
		AddBullet(EBTNumeric);
		SetChar(0x31);
		SetIndent(150);
		
	}
}

TInt CBullets::GetLevel()
{
	return iLevel;
}

void CBullets::SetChar(TChar aChar)
{
	(*iBullets)[iPos]->SetChar(aChar);
}

void CBullets::SetIndent(TInt aIndent)
{
	(*iBullets)[iPos]->SetIndent(aIndent);
}


void CBullets::SetName(const TDesC& aName)
{
	iName = aName.Alloc();
}

TPtrC CBullets::Name()
{
	return *iName;
}

void CBullets::IncLevel()
{
	iLevel ++;
}

void CBullets::DecLevel()
{
	iLevel --;
}

TChar CBullets::Char()
{
	return (*iBullets)[iLevel]->Char();
}

TInt CBullets::Indent()
{
	return (*iBullets)[iLevel]->Indent();
}

void CBullets::IncChar()
{
	(*iBullets)[iLevel]->IncreaseChar();
}

void CBullets::ResetBullet(TChar aValue)
{
	(*iBullets)[iLevel]->ResetChar(aValue);
}
