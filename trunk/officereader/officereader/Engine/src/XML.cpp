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

#include "XML.h"
#include "Para.h"
#include "XMLTags.h"
#include "Bullet.h"
#include "Encryption.h"

#include <txtfrmat.h>


TReal GetPtByDesC(const TDesC& aLength)
{
	_LIT(in,"in");
	_LIT(mm,"mm");
	_LIT(cm,"cm");
	_LIT(px,"px");
	_LIT(pc,"pc");
	_LIT(pt,"pt");
//	_LIT(pro,"%");
	
	TPtrC aType;
	TInt len;

		 aType.Set( aLength.Right(2) );
		 __ASSERT_DEBUG( KErrNone  == TLex(aLength.Left( aLength.Length()- 2) ).Val(len), User::Panic( aLength.Left( aLength.Length()- 2) ,1) );
		TLex(aLength.Left( aLength.Length()- 2) ).Val(len);

		if (aType==in)
		{
			return len * 72 * 1.25; //TODO:  ca. 1.5, how about 2nd edition?????????????????? TODO
		}
		else if (aType==pt)
		{
			return len;
		}
		else if (aType==cm)
		{
			return len  * 10 * (72/25.4) * 1.25; //TODO:  ca. 1.5, how about 2nd edition?????????????????? TODO
		}
		else if (aType==mm)
		{
			return len * (72/25.4);	//(25.40 mm = 1 Inch)
		}
		else if (aType==px)
		{
			
		}
		else if (aType==pc)
		{
			return len * 12;		// (6pc = 1 inch)
		}
	
		return 0;
}

CXML::CXML()
{
	// No implementation required
}


CXML::~CXML()
{
	delete iReader;

	delete iTags;

	#ifdef _log_
		iFileLogger.CloseLog();
		iFileLogger.Close();
	#endif
}

CXML* CXML::NewLC(COpenDocument* aParent)
{
	CXML* self = new (ELeave)CXML();
	CleanupStack::PushL(self);
	self->iOpenDocument = aParent;
	self->ConstructL();
	return self;
}

CXML* CXML::NewL(COpenDocument* aParent)
{
	CXML* self=CXML::NewLC(aParent);
	CleanupStack::Pop(); // self;
	return self;
}

void CXML::ConstructL()
{
	
	#ifdef _log_
		User::LeaveIfError(iFileLogger.Connect());
		iFileLogger.CreateLog(_L("Office"),_L("xml.txt"),EFileLoggingModeOverwrite);
	#endif

	iRFs = iOpenDocument->RF();

	iTags = CTags::NewL();
	iChange = EFalse;

	iReader = CSyParser::NewL(TPtrC());		
	iReader->SetHandler(this);	

}

void CXML::SetMode(TXMLMode aInfo)
{
	iMode = aInfo;
}

void CXML::SetParseDataL(const TDesC8& aData)
{
	iData.Set( aData ); 
}


void CXML::StartElement(const TQualified& aName, const RArray< ::TAttribute>& aAttributes)
{
	#ifdef _log_
		iFileLogger.Write(aName.iLocalName);
	#endif
	
	iTags->PushL( aName.iLocalName.Alloc() );

	
	/*********** Encrypation Data			********************/
	
	if (aName.iPrefix == manifest)
	{
			if (aName.iLocalName == file_entry)
			{
				for (TInt i=0 ; i<=aAttributes.Count()-1 ; i++)
				{
					TPtrC name = aAttributes[i].iName.iLocalName;
					if (name == full_path)
					{
						iOpenDocument->EncryptData()->SetCurrentFile(aAttributes[i].iValue);
					}
					else
					if (name == file_size)
					{
						TInt aValue;
						TLex16(aAttributes[i].iValue).Val(aValue);
						iOpenDocument->EncryptData()->SetCurrentFileSize(aValue);
					}
				}
			}
			else
			if (aName.iLocalName == encryption_data)
			{
				iOpenDocument->SetEncrypted(ETrue);

				for (TInt i=0 ; i<=aAttributes.Count()-1 ; i++)
				{
					TPtrC name = aAttributes[i].iName.iLocalName;
					if (name == checksum)
					{
						iOpenDocument->EncryptData()->SetCheckSum(aAttributes[i].iValue);
					}
				}
			}
			else 
			if (aName.iLocalName == algorithm )
			{
				for (TInt i=0 ; i<=aAttributes.Count()-1 ; i++)
				{
					TPtrC name = aAttributes[i].iName.iLocalName;
					if (name == initialisation_vector)
					{
						iOpenDocument->EncryptData()->SetInit(aAttributes[i].iValue);
					}
				}
			}
			else
			if (aName.iLocalName == key_derivation)
			{
				for (TInt i=0 ; i<=aAttributes.Count()-1 ; i++)
				{
					TPtrC name = aAttributes[i].iName.iLocalName;
					if (name == salt)
					{
						iOpenDocument->EncryptData()->SetSalt(aAttributes[i].iValue);
					}
				}
			}
	}
	/**********************************************************/
	
	
	/*********** META Information			********************/
	if (iMode==XMLModeMeta)
	{
		if (aName.iPrefix == meta)
		{
 			if  (aName.iLocalName == document_statistic)
			{
				for (TInt i=0 ; i<=aAttributes.Count()-1 ; i++)
				{
								
					if  (aAttributes[i].iName.iLocalName == page_count)
						iOpenDocument->SetPages(aAttributes[i].iValue);
					else
					if  (aAttributes[i].iName.iLocalName == paragraph_count)
						iOpenDocument->SetParagraphs(aAttributes[i].iValue);
					else
					if  (aAttributes[i].iName.iLocalName == word_count)
						iOpenDocument->SetWords(aAttributes[i].iValue);

				}
			}
		}
	}
}

#ifdef _log_
void CXML::EndElement(const TQualified& aName)
#else
void CXML::EndElement(const TQualified& /*aName*/)
#endif
{	
		#ifdef _log_
			iTags->Pop(aName.iLocalName);
		#else
			iTags->Pop();
		#endif
}

void CXML::CharacterData(const TDesC& aName)
{
	
	#ifdef _log_
		iFileLogger.Write(aName);
	#endif

	if (iMode==XMLModeMeta)
	{		
		if (iTags->IsTitle() )
		{
			iOpenDocument->SetTitle(aName);
		}
		else
		if (iTags->IsDescription() )
		{
			iOpenDocument->SetDescription(aName);
		}
		else
		if (iTags->IsSubject() )
		{
			iOpenDocument->SetSubject(aName);
		}
		else
		if (iTags->IsKeyword() )
		{
			iOpenDocument->SetKeywords(aName);
		}
	}
}

void CXML::ProcessingInstruction(const TDesC& /*aTarget*/, const TDesC& /*aData*/)
{
}

void CXML::Comment(const TDesC& /*aData*/) 
{
}

void CXML::StartCDataSection()
{
}

void CXML::EndCDataSection()
{}

void CXML::Default(const TDesC& /*aData*/)
{
}


void CXML::SetParser(CExpatParserBase* /*aParser*/)
{}

/*
HBufC* CXML::GetValue(const TDesC8& aData,const TDesC& aName, const TDesC&aPrefix)
{
		

	//RFile			file;
	//User::LeaveIfError(file.Open( *iRFs, _L("c:\\content.xml"), EFileStream));
	//CleanupClosePushL(file);
		
	// 3. We set up the buffer with a default size of 128 bytes.
	//    This buffer is set purposly small so that we execute the
	//    parse stuff as often as possible to show aynchronous parsing
	//    We alse set up some loop variables
	const int KMaxBufLen = 500;
	TBuf8<KMaxBufLen> buf;
	TBool isFinal = ETrue;
	TSyExpatStatus status = EStatusOk;
	do
	{
		// 4. First read some data
		//    Note that if there was an error reading there
		//    data an error will be returned, this is not the
		//    case with eof where the buffer will be smaller than
		//    maxBufLen.
		//User::LeaveIfError(file.Read(buf, KMaxBufLen));
		

		// 5. Next we attempt to parse the result buffer
		//    or if we reached the end of the file the 
		//    partial buffer
		isFinal = (aData.Length() != KMaxBufLen);
//		status = iReader->Parse(buf, isFinal);
		status = iReader->Parse(aData, isFinal);		
		// 6. When the parser returns an error
		//    we log it 
		//    Otherwise if there is no more data
		//    then we report we have finished
		if (status != EStatusOk)
		{
			_LIT(KError,"Error on Line %d Column %d\n");
		}
		else
		{
			if (isFinal)
			{
				_LIT(KFinished, "Finished parsing file\n");
			}
		}
		
		// 7. finally we see if we have to loop round
		//    any more - we stop if we find an error or
		//    we have reached the end of the file
	} while (status == EStatusOk && !isFinal);

	//CleanupStack::PopAndDestroy();

	return iValue.AllocL();
}
*/

void CXML::GetContent()
{

	// TODO: in kleinen Teilen lesen

	TBool isFinal = ETrue;
	TSyExpatStatus status = EStatusOk;
	status = iReader->Parse(iData, isFinal);		
	
		if (status != EStatusOk)	//TODO: Assert_debug
		{

		}
		else
		{
		}

}

CTableXML* CTableXML::NewLC(COpenDocument* aParent)
{
	CTableXML* self = new (ELeave)CTableXML();
	CleanupStack::PushL(self);
	self->iOpenDocument = aParent;
	self->ConstructL();
	self->iColumnRepeated = 1;
	self->iRowRepeated = 1;
	self->iCorrectTable = EFalse;
	return self;
}

CTableXML* CTableXML::NewL(COpenDocument* aParent)
{
	CTableXML* self=CTableXML::NewLC(aParent);
	CleanupStack::Pop(); // self;
	return self;
}



void CTableXML::StartElement(const TQualified& aName, const RArray< ::TAttribute>& aAttributes)
{
	CXML::StartElement(aName,aAttributes);	// hier stack befüllt und meta, für alle gleich

	if (iMode==XMLModeCompleteText)
	{
		if (aName.iPrefix == table)
		{
			if (aName.iLocalName == table_cell)
			{
				// we only set a format if there is a name set inside the attributes
				iAddFormat = EFalse;
				//if there is a Name, override the formatting settings, else set them directly
				TBool aStyleName = EFalse;

				for (TInt i=0 ; i<=aAttributes.Count()-1 ; i++)
				{
					TPtrC name = aAttributes[i].iName.iLocalName;
					if (name == style_name)
					{
						iOpenDocument->Para()->GetIndexByName(aAttributes[i].iValue,EFalse);
						aStyleName = ETrue;
						// we can now apply a formattting
						iAddFormat = ETrue;
					}
					else
					if (name == value_type)
					{
						if (iCorrectTable)	
							iOpenDocument->SetValueType(aAttributes[i].iValue,aStyleName);
					}
					else
					if (name == number_columns_repeated )
					{
						TPtrC value = aAttributes[i].iValue;
						TLex16(value).Val(iColumnRepeated);
					}	
					else
					if (name == number_columns_spanned)
					{
						TPtrC value = aAttributes[i].iValue;
						TLex16(value).Val(iColumnSpanned);
					}

				}
			}
			else 
			if (aName.iLocalName == table_row)
			{
				for (TInt i=0 ; i<=aAttributes.Count()-1 ; i++)
				{
					TPtrC name = aAttributes[i].iName.iLocalName;
				
				/*	if (name == style_name)
					{
						iOpenDocument->Para()->GetIndexByName(aAttributes[0].iValue,EFalse);
					}
					else
				*/
					if (name == number_rows_repeated )
					{
						TPtrC value = aAttributes[i].iValue;
						TLex16(value).Val(iRowRepeated);
						
						// TODO:
						// What to do with end items such as
						// <table:table-row table:style-name="ro1" table:number-rows-repeated="65461">
						if (iRowRepeated>100)
							iRowRepeated = 1;
					}	

				}
			}
			else
			if (aName.iLocalName==table)    			
			{
				HBufC* aName = aAttributes[0].iValue.Alloc();
				iOpenDocument->AddTableName(aName);
				if (iTable)
				{
					//name is set, so use the correct table
					if (*aName == *iTable)
					{
						iCorrectTable = ETrue;
						iOpenDocument->CellPositionReset();
					}
				}
				else	// nothing set, so we talk the first one
					iCorrectTable = ETrue;
			}
		}
		else
		if (aName.iPrefix == style)
		{
			if ( (aName.iLocalName==style) || (aName.iLocalName==default_style) )
			{
				HBufC* value = NULL;

				for (TInt i=0 ; i<=aAttributes.Count()-1 ; i++)
				{

					TPtrC aName = aAttributes[i].iName.iLocalName;

					if (aName==name)
					{
						value = aAttributes[i].iValue.AllocL();
					}
					else
					if (aName==family )
					{
						TPtrC familyvalue = aAttributes[i].iValue;
						
						if (familyvalue==table_cell)
						{
							iOpenDocument->Para()->AddCellItem(value);
						}
						
					}
					else
					if (aName==parent_style_name)
					{
						
						/* TODO:
						TPtrC aParent = aAttributes[i].iValue;
						iOpenDocument->Para()->SetParent(aParent);
						*/
					}
				}
				delete value;
			}//style:style
			else
			if (aName.iLocalName == table_cell_properties)
			{
				for (TInt i=0 ; i<=aAttributes.Count()-1 ; i++)
				{

						TPtrC name = aAttributes[i].iName.iLocalName;
						
						if (name == background_color)
						{
							TPtrC aColor = aAttributes[i].iValue;
							if (aColor != transparent)
							{
								iOpenDocument->Para()->SetCellHighlightColor(aColor.Right(6));
							}
						}
				}
			
			}
			else
			if  (aName.iLocalName ==text_properties)
			{
				for (TInt i=0 ; i<=aAttributes.Count()-1 ; i++)
				{

						TPtrC name = aAttributes[i].iName.iLocalName;
						
						if (name == color)
						{
							TPtrC aColor = aAttributes[i].iValue;
							if (aColor != transparent)
							{
								iOpenDocument->Para()->SetCellFontColor(aColor.Right(6));
							}
						}
				}
			} // style:text-properties
			else		
			if (aName.iLocalName == paragraph_properties)
			{
				for (TInt i=0 ; i<=aAttributes.Count()-1 ; i++)
				{
					TPtrC name = aAttributes[i].iName.iLocalName;
					if (name==text_align)
					{
						iOpenDocument->Para()->SetCellAlign(aAttributes[i].iValue);
					}
					
				}
				
				
			} // style:paragraph-properties
		}	// style:					
		else
		if ((aName.iPrefix== text) && (aName.iLocalName==s))
		{
			if (aAttributes.Count()> 0)
			{
				TPtrC value = aAttributes[0].iValue;
				TInt aInt;
				TLex16(value).Val(aInt);
				iOpenDocument->AddCellWhiteSpace(aInt);
			}	
			else
			{
				iOpenDocument->AddCellWhiteSpace(1);
			}

		}
	}	//XMLModeCompleteText
}

void CTableXML::CharacterData(const TDesC& aName)
{
	CXML::CharacterData(aName);

	if ( iTags->IsText() && !iTags->IsAnnotation() && !iTags->IsFooter() && !iTags->IsHeader() && iCorrectTable)
	{
		for (TInt i=1;i<=iColumnRepeated;i++)
		{
			iOpenDocument->AddCellText(aName);
			
			if (iAddFormat)
			{
				iOpenDocument->AddCellFormat();
			}
			
			if (i!=iColumnRepeated)
				iOpenDocument->AddColumnCell();
		}

		iColumnRepeated = 1;
	}
}


void CTableXML::EndElement(const TQualified& aName)
{
		
		CXML::EndElement(aName);

		if (aName.iPrefix == table)
		{
			if (aName.iLocalName == table_row)
			{
				iOpenDocument->AddRowCell(iRowRepeated);	
				iRowRepeated = 1;
			}
			else
			if (aName.iLocalName == table_cell)
			{	
				if (iColumnRepeated > iColumnSpanned)
				{
					for (TInt i=1;i<= iColumnRepeated;i++)
					{
						// TODO: erzeuige ich hier für alle Cellen ein CCell object??	
						if (iAddFormat)
							iOpenDocument->AddCellFormat();
						iOpenDocument->AddColumnCell();
					}
				}
				else
						iOpenDocument->AddColumnCell(iColumnSpanned);
				
				iColumnRepeated = 1;
				iColumnSpanned = 1;
			}
			else
			if (aName.iLocalName==table)    			
			{
				iCorrectTable = EFalse;
			}
		}
}




CTextXML* CTextXML::NewLC(COpenDocument* aParent)
{
	CTextXML* self = new (ELeave)CTextXML();
	CleanupStack::PushL(self);
	self->iOpenDocument = aParent;
	self->ConstructL();
	return self;
}

CTextXML* CTextXML::NewL(COpenDocument* aParent)
{
	CTextXML* self=CTextXML::NewLC(aParent);
	CleanupStack::Pop(); // self;
	return self;
}

void CTextXML::ConstructL(void)
{
	CXML::ConstructL();

	iHeight = NULL;
	iWidth = NULL;
}



void CTextXML::StartElement(const TQualified& aName, const RArray< ::TAttribute>& aAttributes)
{
	CXML::StartElement(aName,aAttributes);	// hier stack befüllt und meta, für alle gleich

	if (iMode==XMLModeCompleteText)
	{
		if (aName.iPrefix==style)
		{
			if ( (aName.iLocalName==style) || (aName.iLocalName==default_style) || (aName.iLocalName==master_page) )
			{
				HBufC* value = NULL;

				for (TInt i=0 ; i<=aAttributes.Count()-1 ; i++)
				{

					TPtrC aName = aAttributes[i].iName.iLocalName;

					if (aName==name)
					{
						value = aAttributes[i].iValue.AllocL();
					}
					else
					if ( (aName==family) || (aName==page_layout_name))
					{
						TPtrC familyvalue = aAttributes[i].iValue;
						
						if (familyvalue==text)
						{
							iOpenDocument->Para()->AddTextItem(EFalse, value);
						}
						else 
						if (familyvalue==paragraph)
						{
							iOpenDocument->Para()->AddTextItem(ETrue, value);
						}	
						else
						{
							iOpenDocument->Para()->AddTextItem(ETrue, value);
						}
					}
					else
					if (aName==parent_style_name)
					{
						iOpenDocument->Para()->SetParent(aAttributes[i].iValue);
					}
					else
					if (aName==list_style_name)
					{
						iOpenDocument->Para()->SetBullet();
					}
				}

				delete value;
			}//style:style
			else
			if  (aName.iLocalName ==text_properties)
			{				
				for (TInt i=0 ; i<=aAttributes.Count()-1 ; i++)
				{
					TPtrC name = aAttributes[i].iName.iLocalName;

					if (name==text_position)
					{
						iOpenDocument->Para()->SetTextPos(aAttributes[i].iValue);
					}
					else
					if (name==color)
					{
						iOpenDocument->Para()->SetColor(aAttributes[i].iValue.Right(6));
					}
					else
					if (name==font_size)
					{
						iOpenDocument->Para()->SetHeight(aAttributes[i].iValue);
					}
					else
					if (name == font_weight )
					{
						iOpenDocument->Para()->SetWeight(aAttributes[i].iValue);
					}
					else
					if (name == background_color )
					{
						TPtrC aColor = aAttributes[i].iValue;
						if (aColor != transparent)
						{
							iOpenDocument->Para()->SetHighlightColor(aColor.Right(6));
						}
					}			
					else
					if (name == font_style )
					{
						iOpenDocument->Para()->SetStyle(aAttributes[i].iValue);
					}
					else
					if (name == text_underline_style)
					{
						iOpenDocument->Para()->SetUnderline(aAttributes[i].iValue);
					}
					else
					if (name == text_underline_type)
					{
						iOpenDocument->Para()->SetUnderline(aAttributes[i].iValue);
					}
					else
					if (name == text_line_through_style)
					{
						iOpenDocument->Para()->SetStrikeThrough(aAttributes[i].iValue);
					}
				} //for loop
			} // style:text-properties
			else		
			if (aName.iLocalName == paragraph_properties)
			{
				for (TInt i=0 ; i<=aAttributes.Count()-1 ; i++)
				{
					TPtrC name = aAttributes[i].iName.iLocalName;
				
					if (name == text_align )
					{
						iOpenDocument->Para()->SetAlign(aAttributes[i].iValue);
					}			
					else
					if (name == margin_left )
					{
						iOpenDocument->Para()->SetLeftMargin(aAttributes[i].iValue);
					}	
					else
					if (name == margin_right )
					{
						iOpenDocument->Para()->SetRightMargin(aAttributes[i].iValue);
					}
					else
					/*
					if (name.Find(padding) != KErrNotFound)
					{
						iOpenDocument->Para()->SetPadding(aAttributes[i].iValue);
					}
					else
					*/					
					if (name == background_color )
					{
						TPtrC aColor = aAttributes[i].iValue;
						if (aColor != transparent)
						{
							iOpenDocument->Para()->SetParaHighlightColor(aColor.Right(6));
						}
					}			
					else
					if (name == text_indent )
					{
						iOpenDocument->Para()->SetIndent(aAttributes[i].iValue);
					}
					else
					if ( name.Find(border) != KErrNotFound) 
					{
						iOpenDocument->Para()->SetBorder(name ,aAttributes[i].iValue);
					}			

				}
				
			} // style:paragraph-properties
			else
			if (aName.iLocalName == list_level_properties)
			{

				for (TInt i=0 ; i<=aAttributes.Count()-1 ; i++)
				{
					TPtrC name = aAttributes[i].iName.iLocalName;
				
					if (name == space_before)
						iOpenDocument->Para()->SetBulletIndent(aAttributes[i].iValue);	
				}
			}
			
			else			
			if (aName.iLocalName == graphic_properties)
			{
				for (TInt i=0 ; i<=aAttributes.Count()-1 ; i++)
				{
					TPtrC name = aAttributes[i].iName.iLocalName;
				
					if (name == horizontal_pos)
						iOpenDocument->Para()->SetAlign(aAttributes[i].iValue);			
				} 
			}// style:graphic-properties
		}	// style:
		else	
		if (aName.iPrefix == text)
		{
			if  (aName.iLocalName  == p)
			{
				if ( (!iTags->IsNote()) && (!iTags->IsAnnotation()) ) // do not switch to new format inside the note's text:p
				{
					if (aAttributes.Count()>0)
					{
						if ( (!iOpenDocument->Para()->GetIndexByName(aAttributes[0].iValue,ETrue)) || iTags->IsList())
						{
							// if we are at the same global paragrah as before, do not insert new paragraph
							// otherwis, if there are e.g borders, there are way to many	
							//but if this is a list, we still need it, or no bullet will be shown
							iOpenDocument->DeleteL();// delete inserted enter char from endelement
							iOpenDocument->AppendParagraphL();
						}

						if (iTags->IsList())
						{
							// if no attributes, there is no paragrah, so we do it just here
							iOpenDocument->Para()->SetBulletChar();
						}

					}

					if ( (aAttributes.Count()==0) && (iTags->IsList()) )
					{
							// if we are at the same global paragrah as before, do not insert new paragraph
							// otherwis, if there are e.g borders, there are way to many	
							//but if this is a list, we still need it, or no bullet will be shown
							iOpenDocument->DeleteL();// delete inserted enter char from endelement
							iOpenDocument->AppendParagraphL();
						
					}

				}
			}
			else
			if (aName.iLocalName == span)
			{
				//<text:span text:style-name="T1">
				if (aAttributes.Count()>0)
					iOpenDocument->Para()->GetIndexByName(aAttributes[0].iValue,EFalse);
			}
			else
			if (aName.iLocalName == h)
			{
				if ((aAttributes.Count()>0) && !iTags->IsNote() && !iTags->IsAnnotation())
				{
					if ( (!iOpenDocument->Para()->GetIndexByName(aAttributes[0].iValue,ETrue)) || (iTags->IsList()))
					{
						// if we are at the same global paragrah as before, do not insert new paragraph
						// otherwis, if there are e.g borders, there are way to many	
						iOpenDocument->DeleteL();// delete inserted enter char from endelement	
						iOpenDocument->AppendParagraphL();
					}
				}
			}
			else
			if (aName.iLocalName == tab)
			{
				iOpenDocument->AddTabL();
			}
			else
			if (aName.iLocalName == s)
			{
				if (aAttributes.Count()> 0)
				{
					//<text:s text:c="3" /> 
					TPtrC value = aAttributes[0].iValue;
					TInt aInt;
					TLex16(value).Val(aInt);
					iOpenDocument->AddWhiteSpace(aInt);
				}	
				else
				{
					iOpenDocument->AddWhiteSpace(1);
				}
			}
			else
			if (aName.iLocalName == list)
			{

				if (aAttributes.Count() > 0)
				{
					iOpenDocument->Para()->GetBulletIndex(aAttributes[0].iValue);
				}
				iOpenDocument->Para()->IncBulletLevel();

			}
			else
			if  (aName.iLocalName == list_item)
			{
				for (TInt i=0 ; i<=aAttributes.Count()-1 ; i++)
				{
					TPtrC name = aAttributes[i].iName.iLocalName;
					
					if (name==start_value)
					{
						iOpenDocument->Para()->ResetBullet(aAttributes[i].iValue);
					}
				}
		
				
			}
			else
			if (aName.iLocalName == list_style)
			{
				if (aAttributes.Count() > 0)
					iOpenDocument->Para()->AddBulletItem(aAttributes[0].iValue);
				else
				{
					_LIT(empty,"");
					iOpenDocument->Para()->AddBulletItem(empty);
				}
			}
			else
			if (aName.iLocalName == list_level_style_number)
			{
				iOpenDocument->Para()->AddBullet(EBTNumeric);

				for (TInt i=0 ; i<=aAttributes.Count()-1 ; i++)
				{
					TPtrC name = aAttributes[i].iName.iLocalName;
				
					if (name == num_format)
					{
						if (aAttributes[i].iValue.Length()>0)
						{
							__ASSERT_DEBUG(aAttributes[i].iValue.Length() == 1,User::Panic(_L("num-format: Länge > 1"),0) );
							iOpenDocument->Para()->SetChar(aAttributes[i].iValue[0]);
						}
					}
				}
			}
			else
			if (aName.iLocalName == list_level_style_image)
			{
				iOpenDocument->Para()->AddBullet(EBTImage);
				iOpenDocument->Para()->SetChar(0x2022);

			}
			else
			if (aName.iLocalName == list_level_style_bullet)
			{
				iOpenDocument->Para()->AddBullet(EBTFixed);

				for (TInt i=0 ; i<=aAttributes.Count()-1 ; i++)
				{
					TPtrC name = aAttributes[i].iName.iLocalName;
				
					if (name == bullet_char)
					{
						if (aAttributes[i].iValue.Length()>0)
						{
							__ASSERT_DEBUG(aAttributes[i].iValue.Length() == 1,User::Panic(_L("bullet-char: Länge > 1"),0) );						
							iOpenDocument->Para()->SetChar(aAttributes[i].iValue[0]);
						}
					}
				}
			}
			else
 			if (aName.iLocalName == change_start)
			{
				iChange = ETrue;
			}
			
		} //text:
		else
		if (aName.iPrefix==draw)
		{
			if (aName.iLocalName == frame)
			{

				for (TInt i=0 ; i<=aAttributes.Count()-1 ; i++)
				{
					TPtrC name = aAttributes[i].iName.iLocalName;
					
					if (name == width)
					{
						delete iWidth;					
						iWidth = aAttributes[i].iValue.Alloc();
					}

					if (name == height)
					{
						delete iHeight;
						iHeight = aAttributes[i].iValue.Alloc();
					}
					
				}
			}

			if (aName.iLocalName == image)
			{	
				#ifdef _log_
					iFileLogger.Write(_L(""));
					iFileLogger.Write(_L("PICTURE"));
					iFileLogger.Write(*iWidth);
					iFileLogger.Write(*iHeight);
					iFileLogger.Write(_L(""));
				#endif
				
					/* TODO TODO*/
				if (iWidth && iHeight)
					iOpenDocument->GetPicture(aAttributes[0].iValue, TSize(TInt(GetPtByDesC(*iWidth)) ,TInt(GetPtByDesC(*iHeight)) ));     //20,50));
			}

			if (aName.iLocalName == page)
			{	
				iOpenDocument->AddNewPageL();
			}
		}
		}  // if completetext
}

void CTextXML::CharacterData(const TDesC& aName)
{
	CXML::CharacterData(aName);

	if ( iTags->IsText() && !iTags->IsNote() && !iTags->IsNoteCitation() && 
		!iTags->IsFooter() && !iTags->IsAnnotation() && !iChange && !iTags->IsTrackedChanges() )
		{
			iOpenDocument->AddTextL(aName,iTags->IsList());
		}
		else
    	if (iTags->IsNoteCitation())
		{
			temp = aName.AllocL();
			iOpenDocument->AddFootnoteLink(aName);
		}
		else
		if (iTags->IsNote())		//(iNote)
		{
			if (temp)
			{
				CleanupStack::PushL(temp);
				TInt len;
				len = temp->Length()+aName.Length();
				temp = temp->ReAllocL(len);
				CleanupStack::Pop();
				temp->Des().Append(aName);
			}
			else
			{
				temp = aName.AllocL();
			}
			
		}
		else
		if (iChange)
		{
			iOpenDocument->AddTrackedChangeInsertion(aName);

		}
}


void CTextXML::EndElement(const TQualified& aName)
{	

	CXML::EndElement(aName);

	if (iMode==XMLModeCompleteText)
	{
		#ifdef _log_
		if (aName.iPrefix==style)
		{
			if (aName.iLocalName==style)
				iOpenDocument->Para()->StyleEnd();
		}
		else
		#endif

		if (aName.iPrefix == text)
		{

			if  (aName.iLocalName == p)
			{	
				if (!iTags->IsTable() && !iTags->IsNote() && !iTags->IsTrackedChanges() && !iTags->IsAnnotation())
				{
					iOpenDocument->AddCarriageReturnL();
					// now in startelement
					//iOpenDocument->AppendParagraphL();
				}
			}
			else
			if  (aName.iLocalName == h)
			{	
				if ( !iTags->IsTable() )
				{
					iOpenDocument->AddCarriageReturnL();
					// now in startelement
					//iOpenDocument->AppendParagraphL();
				}
			}	
			else
			if (aName.iLocalName == span) 
			{
				iOpenDocument->Para()->SetPosToGlobal();
				
			}
			else
			if  (aName.iLocalName == line_break)
			{	
				iOpenDocument->AddCarriageReturnL();
			}
			else
			if (aName.iLocalName == list)
			{
				iOpenDocument->Para()->DecBulletLevel();
			}
			if  (aName.iLocalName == list_item)
			{
				iOpenDocument->Para()->IncChar();
			}
			if (aName.iLocalName == list_style)
			{
				iOpenDocument->Para()->VerifyBullets();
			}
			else
			if  (aName.iLocalName == note_body)
			{
				temp->Des().TrimAll();
				iOpenDocument->Notes()->Append(temp);
				temp = NULL;
				
			}
			else
			if  (aName.iLocalName == note_citation) 
			{
				CleanupStack::PushL(temp);
				temp = temp->ReAllocL( temp->Length()+ 2);
				CleanupStack::Pop();
				temp->Des().Append(note_addition);		
			}
			else
			if (aName.iLocalName == change_end)
			{
				iChange = EFalse;
			}			
		}	// text

		else
		if (aName.iPrefix == table )
		{
	
			if  (aName.iLocalName == table_row)
			{	
				iOpenDocument->AddCarriageReturnL();
				//iOpenDocument->AppendParagraphL();
			}


			if (aName.iLocalName == table_cell)
			{	
				iOpenDocument->AddTabL();
				iOpenDocument->AddTabL();
			}
		}
		else
		if (aName.iPrefix==draw)
		{
			if (aName.iLocalName == frame)
			{
				delete iHeight;
				iHeight = NULL;
				delete iWidth;
				iWidth = NULL;
				
			}
		}
	}

}


// CTAGS

CTags::CTags()
{
}

CTags::~CTags()
{
	__ASSERT_DEBUG(iTags->Count() == 0 , User::Panic( *(*iTags)[0] ,0));

	iTags->ResetAndDestroy();
	delete iTags;
}

CTags* CTags::NewL()
{
	CTags* self=CTags::NewLC();
	CleanupStack::Pop(); // self;
	return self;
}


CTags* CTags::NewLC()
{
	CTags* self = new (ELeave)CTags();
	CleanupStack::PushL(self);
	self->ConstructL();
	return self;
}

void CTags::ConstructL()
{
	iTags = new (ELeave) RPointerArray<HBufC>;
}

#ifdef _log_
void CTags::Pop(TPtrC aName)
#else
void CTags::Pop()
#endif
{
	#ifdef _log_
		TBuf<300> hi;
		hi.Append(aName);
		hi.Append(_L("		"));
		hi.Append(*(*iTags)[0]);
	#endif


	//__ASSERT_DEBUG(aName == *(*iTags)[0] , User::Panic(hi ,0));
	
	delete (*iTags)[0];
	iTags->Remove(0);
}

void CTags::PushL(HBufC* aName)
{
	iTags->Insert(aName,0);

}	

TBool CTags::IsNote()
{
	for (TInt i=0;i<=iTags->Count()-1;i++)
	{
		if ( (*(*iTags)[i] ) == note_body) 
		{	
			return ETrue;
		}
	}
	return EFalse;
}

TBool CTags::IsNoteCitation()
{
	return  ( (* (*iTags)[0] ) == note_citation );
}


TBool CTags::IsTrackedChanges()
{
	for (TInt i=0;i<=iTags->Count()-1;i++)
	{
		if (  (*(*iTags)[i]) == tracked_changes )
		{	
			return ETrue;
		}
	}
	return EFalse;

}

TBool CTags::IsText()
{
	for (TInt i=0;i<=iTags->Count()-1;i++)
	{
		if ( ((*(*iTags)[i]) == p) || ((*(*iTags)[i]) == h) || ((*(*iTags)[i]) == span) || ((*(*iTags)[i]) == a) )  //TODO:: a?
		{	
			return ETrue;
		}
	}
	return EFalse;
}

TBool CTags::IsFooter()
{
	for (TInt i=0;i<=iTags->Count()-1;i++)
	{
		if (  (*(*iTags)[i]) == footer )
		{	
			return ETrue;
		}
	}
	return EFalse;

}


TBool CTags::IsHeader()
{
	for (TInt i=0;i<=iTags->Count()-1;i++)
	{
		if (  (*(*iTags)[i]) == header )
		{	
			return ETrue;
		}
	}
	return EFalse;

}


TBool CTags::IsAnnotation()
{
	for (TInt i=0;i<=iTags->Count()-1;i++)
	{
		if (  (*(*iTags)[i]) == annotation )
		{	
			return ETrue;
		}
	}
	return EFalse;

}

TBool CTags::IsTable()
{
	return  ( ( (*(*iTags)[0]) == table_cell) || ((*(*iTags)[0]) == table_row)  );
}

TBool CTags::IsTitle()
{
	return  ( (* (*iTags)[0] ) == title );
}


TBool CTags::IsSubject()
{
	return  ( (* (*iTags)[0] ) == subject );
}

TBool CTags::IsDescription()
{
	return  ( (* (*iTags)[0] ) == description );
}

TBool CTags::IsKeyword()
{
	return  ( (* (*iTags)[0] ) == keyword );
}

TBool CTags::IsList()
{
	if (iTags->Count()>1)	
		return  ( (* (*iTags)[1] ) == list_item );
	else
		return EFalse;
}
