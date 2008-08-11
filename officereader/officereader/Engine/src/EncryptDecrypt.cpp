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

#include <e32cmn.h>
#include <hash.h>
#include <e32math.h>
#include <charconv.h>
#include <f32file.h>
#include "EncryptDecrypt.h"

#include "BlowFish.h"

#define SALT_LEN 16
#define INI_VECTOR_LEN 8
#define SHA_LEN 20
#define KEY_LEN 16
#define NR_OF_ITER 1024
#define KMaxPasswordLength 128

CEncryptDecrypt* CEncryptDecrypt::NewL()
{
    CEncryptDecrypt* self = CEncryptDecrypt::NewLC();
    CleanupStack::Pop(self);
    return self;
}

CEncryptDecrypt* CEncryptDecrypt::NewLC()
{
    CEncryptDecrypt* self = new (ELeave) CEncryptDecrypt;
    CleanupStack::PushL(self);
    self->ConstructL();
    return self;
}

CEncryptDecrypt::CEncryptDecrypt()
{
	iSalt = iIniVector = iKey = NULL;
}

CEncryptDecrypt::~CEncryptDecrypt()
{
	iSalt = iIniVector = iKey = NULL;
}

void CEncryptDecrypt::ConstructL()
{
//
}

//Get Renadom Sequence
void CEncryptDecrypt::RandomSeq(const TInt& Len, HBufC8* Seq)
{
	TTime time;
	time.HomeTime();
	TInt64 smallRandSeed = 0x00/*time.Int64()*/;
	for(int i = 0; i < Len; i++)
		Seq->Des().Append((TChar)(Math::Rand(smallRandSeed) % 128));//We need a 8 bit character
}

//Gets the salt for encryption
//Length of the Salt Depends on the input parameter
void CEncryptDecrypt::CreateSalt(const TInt& SaltLen)
{
	iSalt = HBufC8::NewL(SaltLen + 4);
	RandomSeq(SaltLen,iSalt);
}

//Gets the Initialization vector for Blowfish CFB
//Length of the Vector Depends on the input parameter
void CEncryptDecrypt::CreateInitializationVector(const TInt& VectorLen)
{
	iIniVector = HBufC8::NewL(VectorLen);
	RandomSeq(VectorLen,iIniVector);
}
//Uses salt and Inivector along with Password to create 16 byte Key
void CEncryptDecrypt::CreateKey(const TDesC8& Password)
{
	CreateInitializationVector(INI_VECTOR_LEN);
	PBKDF2 (Password, NR_OF_ITER);
}

#define n2l(c,l)        (l =((TUint32)(*(c)))<<24L, \
                         l|=((TUint32)(*((c)+1)))<<16L, \
                         l|=((TUint32)(*((c)+2)))<< 8L, \
                         l|=((TUint32)(*((c)+3))))
                         
#define l2n(l,c)        (*(c)=(TUint8)(((l)>>24L)&0xff), \
                         *((c)+1)=(TUint8)(((l)>>16L)&0xff), \
                         *((c)+2)=(TUint8)(((l)>> 8L)&0xff), \
                         *((c)+3)=(TUint8)(((l)     )&0xff))
                         
/*!
  @function Encrypt
  
  @discussion  Encrypt using Blowfish CFB
  @Param Password
  @Param Message to Encrypt
  @Param Salt Output Salt parameter FREEING THE MEMORY IS CALLER'S RESPONSIBILITY
  @Param IniVector Output Initialization Vector parameter FREEING THE MEMORY IS CALLER'S RESPONSIBILITY
  @Return Buffer with ENcrypted text of the same length as input text FREEING THE MEMORY IS CALLER'S RESPONSIBILITY
  */

HBufC8* CEncryptDecrypt::Encrypt(const TDesC8& Password, const TDesC8& Message, TPtr8 Salt, TPtr8 IniVector)
{
	CreateSalt(SALT_LEN);
	Salt.Copy(*iSalt);// This has to be done earlier since CreateKey may change the Salt
	CreateKey(Password);
	BlowFish blowfish(iKey->Des().Ptr(),iKey->Size());
	HBufC8* outbuff = HBufC8::NewL(Message.Size() + 1);
	TInt n = Message.Size();
	TUint8* in = new TUint8[8];
	const TUint8 * inPtr = Message.Ptr();
	memcpy(in, (*iIniVector).Ptr(),8);
	block out;
	TUint32 l,r;
	for(; n >= 8; n -= 8)
	{
		n2l(in,l);n2l((in + 4),r); 
		blowfish.Blowfish_encipher (&l,&r);
		l2n(l,in);l2n(r,in + 4);
		memcpy(out,inPtr,8);
		for(int k=0; k< 8;k++){
			out[k] ^= in[k];
			in[k] = out[k];
		}
		TPtrC8 outDes(out,8);
		outbuff->Des().Append(outDes);
		inPtr +=8;
	}
	if(n > 0)
	{
		n2l(in,l);n2l((in + 4),r); 
		blowfish.Blowfish_encipher (&l,&r);
		l2n(l,in);l2n(r,in + 4);
		memcpy(out,inPtr,n);
		for(int k=0; k< n;k++){
			out[k] ^= in[k];
			in[k] = out[k];
		}
		TPtrC8 outDes(out,n);
		outbuff->Des().Append(outDes);
		inPtr +=n;
	}
	IniVector.Copy(*iIniVector);
	if( NULL != iSalt)
		delete iSalt;
	if( NULL != iIniVector)
		delete iIniVector;
	if( NULL != iKey)
		delete iKey;
	delete in;
	return outbuff;
}

/*TDes8& CEncryptDecrypt::Or(TPtrC8 mac, TInt i, TPtr8 targetbuffer)
{
    TUint8 byte[4];
	byte[0]= 0xFF || i;
	byte[1]= (0xFF00 || i >> 8);
	byte[2]= (0xFF0000 || i >> 16);
	byte[3]= (0xFF000000 || i >> 24);
	for(int i = 0; i< mac.Size(); i++)
		targetbuffer[i] = mac[i] | byte[i % 4];
	return mac;
}*/

// Ex Or routine for 2 TPtr8 buffers
void CEncryptDecrypt::xOr(TPtrC8 sourcebuffer,TPtr8 targetbuffer)
{
	if(targetbuffer.Size() == 0)
		targetbuffer.Copy(sourcebuffer);
	else
		for(int i = 0; i< targetbuffer.Size(); i++)
			targetbuffer[i] ^= sourcebuffer[i];
}

// PBKDF2 taken from [RFC2898]
void CEncryptDecrypt::PBKDF2 (const TDesC8& P, const TInt c) 
{
	// Convert the password to UTF8 first
/*	TBuf<KMaxPasswordLength> temppwd;
	TInt temp1,temp2;
	CCnvCharacterSetConverter::DoConvertToUnicode(CCnvCharacterSetConverter::AsciiConversionData(),
						CCnvCharacterSetConverter::EBigEndian,temppwd,P,temp1,temp2);
	HBufC8* pattern = SenXmlUtils::ToUtf8LC(temppwd); */
	CSHA1* sha1 = CSHA1::NewL(); //Will be deleted along with HMac
	CHMAC* hMac;
	HBufC8* pwd_hash = 0;
	//if(P.Size() > 16)
	//{
		sha1->Update(P);
		TInt hash_size = sha1->BlockSize();
		pwd_hash = HBufC8::NewL(hash_size);
		pwd_hash->Des().Copy(sha1->Final());
		hMac = CHMAC::NewL(*pwd_hash,sha1);
	//}
	/*else
	{
		hMac = CHMAC::NewL(P,sha1);
	}*/
	
	TInt block_size = hMac->BlockSize();

	iKey = HBufC8::NewL(block_size);//Will be deleted in Encrypt/ decrypt function
	TUint8* tempint = new TUint8[4];
	*tempint = 0x00;*(tempint +1) = 0x00;*(tempint +2) = 0x00;*(tempint +3) = 0x01;
	TPtr8 temp(tempint,4);
	temp.SetLength(4);
	hMac->Update(*iSalt);
	hMac->Update(temp);
	iSalt->Des().Copy(hMac->Final());
	iKey->Des().Copy(*iSalt);
	for(int j = 1; j< c ;j ++){
		hMac->Update(*iSalt);
		iSalt->Des().Copy(hMac->Final());
		xOr(*iSalt, iKey->Des());
	}
	iKey->Des().SetLength(16);
	delete hMac;
	if(0 != pwd_hash)
		delete pwd_hash;
	delete tempint;
}

/*!
  @function Decrypt
  
  @discussion  Decrypt using Blowfish CFB
  @Param Password
  @Param Message to Decrypt
  @Param Salt Input Salt parameter obtained via Encrypt
  @Param IniVector Input Initialization Vector Obtained via encrypt
  @Return Buffer with ENcrypted text of the same length as input text freeng the buffer is calle's responsibility
  */
HBufC8* CEncryptDecrypt::Decrypt(const TDesC8& password, const TDesC8& Message, const TDesC8& Salt, const TDesC8& IniVector)
{
	iSalt = HBufC8::NewL(64/*Salt.Size() + 4*//*for adding the block number*/);
	iSalt->Des().Copy(Salt);
	iIniVector = HBufC8::NewL(IniVector.Size());
	iIniVector->Des().Copy(IniVector);
	PBKDF2 (password, NR_OF_ITER);
	BlowFish blowfish(iKey->Des().Ptr(),iKey->Size());
	HBufC8* outbuff = HBufC8::NewL(Message.Size());
	TInt n = Message.Size();
	TUint8* in = new TUint8[8];
	const TUint8 * inPtr = Message.Ptr();
	memcpy(in, IniVector.Ptr(),8);
	block out;
	block crypt;
	TUint32 l,r;
	for(; n >= 8; n -= 8)
	{
		memcpy(out, inPtr,8);
		inPtr += 8;
		n2l(in,l);n2l(in+4,r); 
		blowfish.Blowfish_encipher (&l,&r);
		l2n(l,in);l2n(r,in + 4);

		for(int k=0; k< 8;k++){
			crypt[k] = out[k];
			out[k] ^= in[k];
			in[k] = crypt[k];
		}
		TPtrC8 outDes(out,8);
		outbuff->Des().Append(outDes);
	}
	if(n > 0)
	{
		memcpy(out,inPtr,n);
		inPtr +=n;
		n2l(in,l);n2l((in + 4),r); 
		blowfish.Blowfish_encipher (&l,&r);
		l2n(l,in);l2n(r,in + 4);
		for(int k=0; k< n;k++){
			crypt[k] = out[k];
			out[k] ^= in[k];
			in[k] = crypt[k];
		}
		TPtrC8 outDes(out,n);
		outbuff->Des().Append(outDes);
	}
	if( NULL != iSalt)
		delete iSalt;
	if( NULL != iIniVector)
		delete iIniVector;
	if( NULL != iKey)
		delete iKey;
	delete in;
	return outbuff;
}
