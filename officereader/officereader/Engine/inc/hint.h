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

#ifndef HINT_H
#define HINT_H

#define MAX_LINES 20

#include <eikenv.h>

    /* This class will draw a floating window with some
       text, nicely word-wrapped. You can anchor to window
       to any of the for corners. It'll calculate the minimum
       size needed based on the maximum width given, and use
       that */

class CTextBox : public CCoeControl {
public:
    enum TPosition {
        ETopLeft,
        ETopRight,
        EBottomLeft,
        EBottomRight
    };
public:
    TRgb iBgColor, iFgColor;
	
    void ZeroRect() {
        SetRect( TRect(TPoint(0, 0), TSize(0, 0)) );
    }
    void ConstructL(CEikonEnv* aEnv,TRgb aBackgroundColor, TRgb aForegroundColor,
            TRgb aBorderColor, TInt aBorder, TInt aMargin) {
        /* if we just CreateWindowL() we get a 'floating' window,
           since each new window is created on top of the
           previous ones. No key events will be routed if
           we don't add it to the control stack.*/
        CreateWindowL();
        /* default window covers the whole screen, we
           don't want that in case somebody Activate()s
           without calling SetText() */
        ZeroRect();
	
        iBgColor=aBackgroundColor;
        iFgColor=aForegroundColor;
        iFont=aEnv->DenseFont();
        /* 1.1 times font height is a standard estimate
           for line height when we don't know any better */
        iLineHeight=iFont->HeightInPixels()*1.1;
        iMargin=aMargin;
        iBorder=aBorder;
        iBorderColor=aBorderColor;
    }
    void Draw(const    TRect& aRect) const {
        /* we don't care if aRect is actually smaller than
           this window:
            a) it won't normally happen, since this will
            float on top of all other windows in the app,
            and menus and other transient windows use a
            copy of the window below to redraw
            b) if we did, we'd have to worry about how to
            draw the border correctly, instead of just
            using the pen on the rect
        */
        CWindowGc& gc =    SystemGc();
        gc.SetPenStyle(CGraphicsContext::ESolidPen);
        gc.SetPenSize(TSize(iBorder, iBorder));
        gc.SetPenColor(iBorderColor);
        gc.SetBrushColor(iBgColor);
        gc.SetBrushStyle(CGraphicsContext::ESolidBrush);
        gc.DrawRect(Rect());
	
        if (! iText || iText->Length()==0) return;
	
        gc.UseFont(iFont);
        TInt baseline=iFont->HeightInPixels()-iFont->DescentInPixels();
        TPoint p(iBorder+iMargin, baseline+iBorder+iMargin);
        TInt pos=0;
        gc.SetPenStyle(CGraphicsContext::ESolidPen);
        gc.SetPenColor(iFgColor);
        for (int i=0; i<iCurrentLineCount ; i++) {
            /* pos will be one over length of string at end of
               loop */
            TPtrC text=(*iText).Mid(pos, iLines[i]-pos);
            gc.DrawText(text, p);
            pos=iLines[i];
	
            /* we have to check whether the line was broken
               at whitespace, and advance if so */
            if ( pos < (*iText).Length() &&
                TChar((*iText)[pos]).IsSpace()) pos++;
            p.iY+=iLineHeight;
        }
        gc.DiscardFont();
    }

    const CFont* iFont;
    TInt iLines[MAX_LINES];
    TInt iCurrentWidth, iCurrentLineCount, iLineHeight;
    TInt iMargin, iBorder; TRgb iBorderColor;
    HBufC* iText;
    
/* doesn't take ownership of the text, you need to make
       sure its lifetime is long enough*/
    void SetText(TInt aMaxWidth, HBufC* aText, TPosition aPosFrom,
            TPoint aPos) {
        iText=aText;
        if (! iText || iText->Length()==0) {
            ZeroRect();
            return;
        }
	
        /* Calculate the line breaks. We go one over
         * the string length in the loop so that the
         * last line width is calculated.
         */
        TInt i, w, l, previous_space=0, max_w=0, previous_w=0;
        for (i=0, w=0, l=0; i<=aText->Length() && l<MAX_LINES ; i++) {
            if ( i==aText->Length() || TChar((*aText)[i]).IsSpace()) {
                previous_w=w;
                previous_space=i;
            }
            TInt this_w=0;
            if (i<aText->Length())
                this_w=iFont->CharWidthInPixels( (*aText)[i] );
            if (i==aText->Length() || w+this_w > aMaxWidth) {
                if (previous_space==0) {
                    /* if there is no break opportunity,
                       we just cut the word */
                    previous_w=w;
                    previous_space=i-1;
                }
                if (previous_w > max_w) max_w=previous_w;
                iLines[l]=previous_space;
                l++;
                i=previous_space;
                previous_space=0;
                w=0;
            } else {
                w+=this_w;
            }
        }
        iCurrentLineCount=l;
        iCurrentWidth=max_w+2*iMargin+2*iBorder;
        TPoint tl;
        TSize s(iCurrentWidth, iCurrentLineCount*iLineHeight+2*iMargin+2*iBorder);
        switch(aPosFrom) {
            case ETopLeft:
                tl=aPos;
                break;
            case ETopRight:
                tl.iX=aPos.iX-s.iWidth;
                tl.iY=aPos.iY;
                break;
            case EBottomLeft:
                tl.iX=aPos.iX;
                tl.iY=aPos.iY-s.iHeight;
                break;
            case EBottomRight:
                tl.iX=aPos.iX-s.iWidth;
                tl.iY=aPos.iY-s.iHeight;
                break;
        }
        SetRect(TRect(tl, s));
    }
};

#endif
