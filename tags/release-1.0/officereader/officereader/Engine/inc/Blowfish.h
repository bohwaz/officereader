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

#include <e32def.h>
#define bf_N 16

/*
extern TUint32 bf_S[4][256];
extern TUint32 bf_P[bf_N + 2];
extern TUint32 tempbf_S[4][256];
extern TUint32 tempbf_P[bf_N + 2];
*/

#define MAXKEYBYTES 56

typedef TUint8 block[8];


union aword
    {
    TUint32 word;
    TUint8 byte[4];
    struct
        {
TUint byte3:
        8;
TUint byte2:
        8;
TUint byte1:
        8;
TUint byte0:
        8;
        }
    w;
    };

class BlowFish
    {
    public:
        BlowFish (const TUint8 *key, int keylen);
        //Not needed
        //void Encrypt (block in, block out);
        //Not needed
        //void Decrypt (block in, block out);

        // Blow fish encryption for 8 bytes
        void Blowfish_encipher (TUint32 *xl, TUint32 *xr);
        // Blow fish Decryption for 8 bytes in CFB mode you do not need it
        void Blowfish_decipher (TUint32 *xl, TUint32 *xr);
    private:
        void InitializeBlowfish (const TUint8 key[], TInt16 keybytes);

    private:
        TUint32 bf_S[4][256];
        TUint32 bf_P[bf_N + 2];
    };

