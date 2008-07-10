#include "SyGen.h"

//////////////////////////////////////////////////////////////////////////////////////////
// CSyGeneratorBase
//////////////////////////////////////////////////////////////////////////////////////////
CSyGeneratorBase::TXMLElement::TXMLElement(const TDesC& aPrefix, const TDesC& aName)
:	iPrefix(aPrefix),
	iName(aName)
{
}

// XML Header Strings

// General Strings
_LIT(KEmptyString, "");
_LIT(KEmptyPrefix, "");
_LIT(KEmptyName, "");
_LIT(KPrefixColon, ":");
_LIT(KPrefixSpace, " ");
_LIT(KPrefixEquals,"=");
_LIT(KPrefixQuote, "\"");
_LIT(KXMLNamespace, "xmlns");

// Start Document specific strings 
_LIT(KDocStart, "<?xml version=\"1.0\"");
_LIT(KDocEncoding, " encoding=\"%S\"");
_LIT(KDocStandalone, " standalone = \"yes\"");
_LIT(KDocEnd,   " ?>");
_LIT(KDocTypeStart,  "<!DOCTYPE ");
_LIT(KDocTypeSystem, " SYSTEM \"");
_LIT(KDocTypeEnd,    "\">");

_LIT(KElementStart, "<");
_LIT(KElementClose, ">");	
_LIT(KElementEnd,   "</");
_LIT(KElementEmpty, "/>");

_LIT(KCommentStart, "<!-- ");
_LIT(KCommentEnd, " -->");

_LIT(KPIStart, "<?");
_LIT(KPIEnd, "?>");
_LIT(KPISpace, " ");

_LIT(KCDATAStart, "<![CDATA[");
_LIT(KCDATAEnd, "]]>");

_LIT(EscapedQuote, 			"&quot;");
_LIT(EscapedApostrophe, 	"&apos;");
_LIT(EscapedAmpersand,		"&amp;");
_LIT(EscapedLessThan,		"&lt;");
_LIT(EscapedGreaterThan,	"&gt;");


CSyGeneratorBase::CSyGeneratorBase(MSyOutputHandler& aOutput)
:	iOutput(aOutput)
{

}


EXPORT_C CSyGeneratorBase::~CSyGeneratorBase()
{
	__ASSERT_ALWAYS(iBuffer != NULL, Panic(KErrCorrupt));
	delete iBuffer;
	iBuffer = NULL;
	
	iContextStack.Close();
}


void CSyGeneratorBase::BaseConstructL(const TInt aBufferSize)
{
	// Do not put on the stack as is owned by the writer
	iBuffer = HBufC::NewL(aBufferSize);
}

TPtr CSyGeneratorBase::GetBufferL(const TInt aNewLength)
{
	__ASSERT_ALWAYS(iBuffer != NULL, Panic(KErrCorrupt));

	// if the existing buffer is too small we resize it
	const TInt maxLength = iBuffer->Des().MaxLength();
	if (maxLength < aNewLength)
	{
		delete iBuffer;
		iBuffer = HBufC::NewL(aNewLength);
	}
	else
	{
		iBuffer->Des().Zero();
	}
	
	return iBuffer->Des();
}

TPtr CSyGeneratorBase::ExpandBufferL(const TInt aExpandBy)
{
	__ASSERT_ALWAYS(iBuffer != NULL, Panic(KErrCorrupt));

	const TInt newLength = iBuffer->Length() + aExpandBy;
	const TInt maxLength = iBuffer->Des().MaxLength();
	
	if (maxLength < newLength )
	{
		iBuffer = iBuffer->ReAllocL(newLength);
	}
	
	return iBuffer->Des();
}

void CSyGeneratorBase::FinishElementDeclarationIfNeeded()
{
	if (!iNeedClosingBrace)
		return;
	else
		; // we need to close the current declaration
		
	// first we ensure the buffer has sufficient space		
	TPtr buffer = ExpandBufferL(2);

	// now get the last element
	const TInt		   lastIndex = iContextStack.Count() -1;
	const TXMLElement& lastElement = iContextStack[ lastIndex ];
	
	// if the last element on the context stack is empty then this was
	// an empty element so close the element marker
	// and pop it from the stack
	if (lastElement.iName.Length() == 0)
	{
		buffer.Append(KElementEmpty);
		iContextStack.Remove(lastIndex);
	}
	else
	{
		// the last element was a regular element so just close
		// the declaration - we will remove it from the stack when
		// the corresponding EndElement is encountered
		buffer.Append(KElementClose);
	}
		
	iOutput.WriteL(buffer, EFalse);
	
	iNeedClosingBrace = EFalse;
}

void CSyGeneratorBase::StartElement2L(const TDesC& aPrefix, const TDesC& aName, const TBool aEmptyElement)
{
	// Check if the last call was a start element that has not been closed
	FinishElementDeclarationIfNeeded();

	const TInt maxSize = KElementStart.BufferSize + aPrefix.Length() + 
						 KPrefixColon.BufferSize + aName.Length();
	
	// and get a new buffer
	TPtr buffer = GetBufferL(maxSize);
	// start with a "<"
	buffer.Append(KElementStart);
	
	// add the namespace prefix if needed e.g. "test:"
	if (aPrefix.Length() > 0)
	{
		buffer.Append(aPrefix);
		buffer.Append(KPrefixColon);
	}
	
	// add the name
	buffer.Append(aName);

	// and save the context information so we know what the index
	// is when we close the element
	if (aEmptyElement)
	{
		// Empty elements are indicated with an empty name
		// NB This is removed in the FinishElementDeclarationIfNeeded
		// function!
		iContextStack.Append(TXMLElement(KEmptyPrefix, KEmptyName));
	}
	else
	{
		// Just save the context for the EndElement call
		iContextStack.Append(TXMLElement(aPrefix, aName));	
		iDepth++;
	}
		
	// finally we indicate that we need to be able to close the brace
	// sometime later
	iNeedClosingBrace = TRUE;
}

EXPORT_C void CSyGeneratorBase::EndElementL()
{
	FinishElementDeclarationIfNeeded();
	
	// we get the top element on the stack
	const TInt lastIndex =  iContextStack.Count() -1;
	const TXMLElement lastElement = iContextStack[ lastIndex];

	iContextStack.Remove(lastIndex);
	
	// Calculate how much space we need	and get a buffer of at least
	// this size
	const TInt maxSize = KElementEnd.BufferSize + 
						 lastElement.iPrefix.Length() + KPrefixColon.BufferSize +
						 lastElement.iName.Length() +
						 KElementClose.BufferSize;
	TPtr buffer = GetBufferL(maxSize);
	
	// now write out the "</"
	buffer.Append(KElementEnd);

	// write out the prefix if it is present
	if (lastElement.iPrefix.Length() > 0)
	{
		buffer.Append(lastElement.iPrefix);
		buffer.Append(KPrefixColon);
	}
	else
		; // no namepsace prefix 
		
	// finally write out the element name
	buffer.Append(lastElement.iName);
	// and write out the ">"
	buffer.Append(KElementClose);

	 iOutput.WriteL(buffer, EFalse);
	 iDepth--;
}

///////////////////////////////////////////////////////////////////////////////////////////////////////
EXPORT_C void CSyGeneratorBase::StartDocumentL(const TDesC& aEncoding, const TBool aStandalone, 
										 const TDesC& aPublicId, const TDesC& aSystemId)
{
	// now calculate how much data we need and make sure the buffer is big enoung
	const TInt maxSize = KDocStart.BufferSize +
						 KDocEncoding.BufferSize + aEncoding.Length() + 
						 KDocStandalone.BufferSize +
						 KDocTypeStart.BufferSize + aPublicId.Length() + 
						 KDocTypeSystem.BufferSize + aSystemId.Length() + 
						 KDocTypeEnd.BufferSize +						 
						 KDocEnd.BufferSize;
	TPtr buffer = GetBufferL(maxSize);

	// now start building the XML file header
	buffer.Append(KDocStart);

	// Add an encoding if needed
	if (aEncoding.Length() > 0)
	{
		
		TBuf<128> encoding;
		encoding.Format(KDocEncoding, &aEncoding);
		buffer.Append(encoding);
	}
	
	// Add a standalone declaration if needed
	if (aStandalone)
		buffer.Append(KDocStandalone);
	
	if (aPublicId.Length() != 0)
	{
		buffer.Append(KDocTypeStart);
		buffer.Append(aPublicId);
		buffer.Append(KDocTypeSystem);
		buffer.Append(aSystemId);
		buffer.Append(KDocTypeEnd);
	}

	// Finally append the end document marker
	buffer.Append(KDocEnd);

	// and write out the whole string
	iOutput.WriteL(buffer, EFalse);

	StartFragmentL();
}

EXPORT_C void CSyGeneratorBase::StartDocumentL(const TBool aStandalone)
{
	StartDocumentL(KEmptyString, aStandalone, KEmptyString, KEmptyString);
}


EXPORT_C void CSyGeneratorBase::EndDocumentL()
{
	EndFragmentL();
}

EXPORT_C void CSyGeneratorBase::StartFragmentL()
{
//	iDepth++;
}

EXPORT_C void CSyGeneratorBase::EndFragmentL()
{
	while (iContextStack.Count() > 0)
	{
		EndElementL();
	}

//	iDepth--;
}

EXPORT_C void CSyGeneratorBase::StartElementL(const TDesC& aName)
{
	
	StartElement2L(KEmptyPrefix, aName, EFalse);
}

EXPORT_C void CSyGeneratorBase::StartElementL(const TDesC& aPrefix, const TDesC& aName)
{
	StartElement2L(aPrefix, aName, EFalse);
}

EXPORT_C void CSyGeneratorBase::EmptyElementL(const TDesC& aName)
{
	StartElement2L(KEmptyPrefix, aName, ETrue);
}

EXPORT_C void CSyGeneratorBase::EmptyElementL(const TDesC& aPrefix, const TDesC& aName)
{
	StartElement2L(aPrefix, aName, ETrue);
}

EXPORT_C void CSyGeneratorBase::SetNamespaceL(const TDesC& aURI)
{
	SetNamespaceL(KEmptyPrefix, aURI);
}

EXPORT_C void CSyGeneratorBase::SetNamespaceL(const TDesC& aPrefix, const TDesC& aURI)
{
	// do not close the element here as we are still wriitng out namespace stuff etc
	const TInt maxSize = KPrefixSpace.BufferSize + 
						 KXMLNamespace.BufferSize + KPrefixColon.BufferSize + 
						 aPrefix.Length() + KPrefixEquals.BufferSize + 
						 KPrefixQuote.BufferSize +
						 aURI.Length() + 
						 KPrefixQuote.BufferSize;
						 
						 
	TPtr buffer = ExpandBufferL(maxSize);
	
	buffer.Append(KPrefixSpace);
	buffer.Append(KXMLNamespace);
	
	if (aPrefix.Length() == 0)
	{
		// this is a default namespace
		buffer.Append(KPrefixEquals);
	}
	else
	{
		buffer.Append(KPrefixColon);
		buffer.Append(aPrefix);
		buffer.Append(KPrefixEquals);
	}
	
	buffer.Append(KPrefixQuote);
	buffer.Append(aURI);
	buffer.Append(KPrefixQuote);
	
	// dont write it out - we will flush it all when we call something that
	// generates a new intrinsic type
}

EXPORT_C void CSyGeneratorBase::SetAttributeL(const TDesC& aName, const TDesC& aValue)
{
	SetAttributeL(KEmptyPrefix, aName, aValue);
}

EXPORT_C void CSyGeneratorBase::SetAttributeL(const TDesC& aPrefix, const TDesC& aName, const TDesC& aValue)
{
	const TInt maxSize = KPrefixSpace.BufferSize + 
						 aPrefix.Length() + KPrefixColon.BufferSize + aName.Length() + 
						 KPrefixEquals.BufferSize + 
						 KPrefixQuote.BufferSize + aValue.Length() + KPrefixQuote.BufferSize;
	TPtr buffer = ExpandBufferL(maxSize);
	
	buffer.Append(KPrefixSpace);
	
	if (aPrefix.Length() != 0)
	{
		buffer.Append(aPrefix);
		buffer.Append(KPrefixColon);
	}
	
	buffer.Append(aName);
	buffer.Append(KPrefixEquals);
	buffer.Append(KPrefixQuote);
	buffer.Append(aValue);
	buffer.Append(KPrefixQuote);	
}

EXPORT_C void CSyGeneratorBase::CharactersL(const TDesC& aCharacters)
{
	FinishElementDeclarationIfNeeded();

	if (iInCDATASection)
	{
		// note we do not put the data into a temporary
		// as we can avoid duplicating the cdata stuff!
		iOutput.WriteL(aCharacters, ETrue);
		return;
	}
	else
		; // we need to escape the data
		
		
	TInt iEscapedCharacters = 0;
	TInt i = 0;
	for (i=0; i < aCharacters.Length(); i++)
	{
		switch (aCharacters[i])
		{
			case '\"': // "
			case '\'':
			case '&':
			case '<':
			case '>':
				iEscapedCharacters++;
		}
	}

	const TInt maxSize = aCharacters.Length() + (iEscapedCharacters * EscapedQuote.BufferSize);
	TPtr buffer = GetBufferL(maxSize);
	
	for (i = 0; i < aCharacters.Length(); i++)
	{
		switch (aCharacters[i])
		{
			case '\"': //" //- stupid context parser does'nt know this is a string character not a string!
				buffer.Append(EscapedQuote);
				break;
			case '\'':
				buffer.Append(EscapedApostrophe);
				break;
			case '&':
				buffer.Append(EscapedAmpersand);
				break;
			case '<':
				buffer.Append(EscapedLessThan);
				break;
			case '>':
				buffer.Append(EscapedGreaterThan);
				break;
			default:
				buffer.Append(aCharacters[i]);
				break;
		}
	}
	
	iOutput.WriteL(buffer, ETrue);
}

EXPORT_C void CSyGeneratorBase::StartCDATAL()
{
	FinishElementDeclarationIfNeeded();

	iOutput.WriteL(KCDATAStart, ETrue);
	
	iInCDATASection = ETrue;
}

EXPORT_C void CSyGeneratorBase::EndCDATAL()
{
	// we never need to finish an element declaration
	// here as it would have been closed in the StartCDATAL
	// of course this assumes you called StartCDATAL

	iOutput.WriteL(KCDATAEnd, ETrue);
	
	iInCDATASection = EFalse;
}

EXPORT_C void CSyGeneratorBase::IgnorableWhitespaceL(const TDesC& aCharacters)
{
	FinishElementDeclarationIfNeeded();

	CharactersL(aCharacters);
}

EXPORT_C void CSyGeneratorBase::ProcessingInstructionL(const TDesC& aTarget, const TDesC& aData)
{
	FinishElementDeclarationIfNeeded();

	const TInt maxSize = KPIStart.BufferSize + 	aTarget.Length() + 
						 KPISpace.BufferSize + aData.Length() + 
						 KPIEnd.BufferSize;
						 
	TPtr buffer = GetBufferL(maxSize);
	
	buffer.Append(KPIStart);
	buffer.Append(aTarget);
	
	if (aData.Length() > 0)
	{
		buffer.Append(KPISpace);
		buffer.Append(aData);
	}
	
	buffer.Append(KPIEnd);
	
	iOutput.WriteL(buffer, EFalse);
}

EXPORT_C void CSyGeneratorBase::CommentL(const TDesC& aComment)
{
	FinishElementDeclarationIfNeeded();

	const TInt maxSize = KCommentStart.BufferSize + aComment.Length() + KCommentEnd.BufferSize;
	
	TPtr buffer = GetBufferL(maxSize);
		
	buffer.Append(KCommentStart);
	buffer.Append(aComment);
	buffer.Append(KCommentEnd);
	
	iOutput.WriteL(buffer, EFalse);
}



EXPORT_C void CSyGeneratorBase::Panic(const TInt aPanic)
{
	_LIT(KPanicCategory, "TSXMLWriter");
		
	User::Panic(KPanicCategory, aPanic);
}

EXPORT_C const TVersion CSyGeneratorBase::VersionInfo() const
{
	const TVersion v(1, 1, 1);
	
	return v;
}

//////////////////////////////////////////////////////////////////////////////////////////
// CSyGenerator
//////////////////////////////////////////////////////////////////////////////////////////
EXPORT_C CSyGenerator* CSyGenerator::NewL(MSyOutputHandler& aOutput, const TInt aBufferSize)
{
	CSyGenerator* self =  NewLC(aOutput, aBufferSize);
	CleanupStack::Pop();
	return self;

}

EXPORT_C CSyGenerator* CSyGenerator::NewLC(MSyOutputHandler& aOutput, const TInt aBufferSize)
{
	CSyGenerator* self = new (ELeave) CSyGenerator(aOutput);
	
	CleanupStack::PushL(self);
	
	self->ConstructL(aBufferSize);
	
	return self;
}

CSyGenerator::CSyGenerator(MSyOutputHandler& aOutput)
: CSyGeneratorBase(aOutput)
{
}

CSyGenerator::~CSyGenerator()
{
}

void CSyGenerator::ConstructL(const TInt aBufferSize)
{
	BaseConstructL(aBufferSize);
}

