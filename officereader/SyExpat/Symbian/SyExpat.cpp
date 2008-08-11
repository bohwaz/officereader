// SyExpat.cpp
// Copyright © 2004 ToddSoftware.com. All rights reserved.
// Copyright (c) 1998, 1999, 2000 Thai Open Source Software Center Ltd
//                               and Clark Cooper
// Copyright (c) 2001, 2002, 2003 Expat maintainers.
//
// Permission is hereby granted, free of charge, to any person obtaining
// a copy of this software and associated documentation files (the
// "Software"), to deal in the Software without restriction, including
// without limitation the rights to use, copy, modify, merge, publish,
// distribute, sublicense, and/or sell copies of the Software, and to
// permit persons to whom the Software is furnished to do so, subject to
// the following conditions:
//
// The above copyright notice and this permission notice shall be included
// in all copies or substantial portions of the Software.
//
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
// EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
// MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.
// IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY
// CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT,
// TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE
// SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.


/** 
	@file SyExpat.cpp
	Comments : This file contains the implementations of the class CExpatParserBase.
 */ 



#include <expat.h>

#undef NULL

#include <e32base.h>
#include <e32def.h>

#include "SyExpat.h"

_LIT(KSeperator, "\n");

namespace SyExpat
{
static void SplitElementName(const TText* aName, TQualified& aItem)
{
	TPtrC tmp(aName);
	
	//URI + sep + local_name + sep + prefix.
	
	const TInt EndURI = tmp.Find(KSeperator);
	if (EndURI != KErrNotFound)
	{
		aItem.iURI.Set(tmp.Ptr(), EndURI);
		
		tmp.Set(aName + EndURI + KSeperator.BufferSize);
		
		const TInt EndLocalName = tmp.Find(KSeperator);
		if (EndLocalName != KErrNotFound)
		{
			aItem.iLocalName.Set(tmp.Ptr(), EndLocalName);
			
			tmp.Set(tmp.Ptr() + EndLocalName +  KSeperator.BufferSize);
			
			aItem.iPrefix.Set(tmp);
		}
		else
		{
			; // there was no local name - should we assert?
		}
	}
	else
	{
		; // as there is no namespace information we assume we only have a local name so do nothing
		aItem.iLocalName.Set(tmp);
	}
}

//////////////////////////////////////////////////////////////////////////////////////////////////////
// MSyDocHandler implmenetation

//////////////////////////////////////////////////////////////////////////////////////////////////////
// MSyDeclHandler implementation
/*
This internal structure is used to hold state data that is used for the default handlers
*/
struct TExpatDocumentHandlers
{
	MSyDocHandler*		iDocHandler;
	MSyDeclHandler*		iDeclHandler;
};

// Define helper macros to get the data out of the user data
#define RECAST_TO_DOC_HANDLER(parserdata)		(REINTERPRET_CAST(TExpatDocumentHandlers*, parserdata)->iDocHandler)
#define RECAST_TO_DECL_HANDLER(parserdata)		(REINTERPRET_CAST(TExpatDocumentHandlers*, parserdata)->iDeclHandler)

#define RECAST_FROM_XML_CHAR(s)		REINTERPRET_CAST(const TText*, s)
#define RECAST_TO_XML_CHAR(s)		REINTERPRET_CAST(const XML_Char*, s)

// The E32Dll() entry point function


#ifdef PLATFORM_2ND

GLDEF_C TInt E32Dll(TDllReason)
{
	return(KErrNone);
}
#endif


static void XMLCALL StartElementHandler(void* userData, const XML_Char* aName, const XML_Char** aAttributes)
{
	TQualified name;
	SplitElementName(RECAST_FROM_XML_CHAR(aName), name);
	
	RArray<TAttribute> attributes;
	
	XML_Char** attrTemp = (XML_Char**)aAttributes;
	
	int i = 0;
	while (attrTemp	 != NULL && attrTemp[i] != NULL)
	{
		TAttribute attribute;
		SplitElementName(RECAST_FROM_XML_CHAR(attrTemp[i++]), attribute.iName);
		
		attribute.iValue.Set(RECAST_FROM_XML_CHAR(attrTemp[i++]));
		attributes.Append( attribute );
	}

	MSyDocHandler* handler = RECAST_TO_DOC_HANDLER(userData);
	if (handler != NULL)
		handler->StartElement(name, attributes);

	attributes.Close();
}

static void XMLCALL EndElementHandler(void* userData, const XML_Char* aName)
{
	TQualified name;
	SplitElementName(RECAST_FROM_XML_CHAR(aName), name);

	MSyDocHandler* handler = RECAST_TO_DOC_HANDLER(userData);
	if (handler != NULL)
		handler->EndElement(name);
}


static void XMLCALL CharacterDataHandler(void* userData, const XML_Char* aData, int aLength)
{
	const TPtrC  data(RECAST_FROM_XML_CHAR(aData), aLength);
	
	MSyDocHandler* handler = RECAST_TO_DOC_HANDLER(userData);
	if (handler != NULL)
		handler->CharacterData(data);
}


static void ProcessingInstructionHandler(void* userData, const XML_Char* aTarget, const XML_Char* aData)
{
	const TPtrC target(RECAST_FROM_XML_CHAR(aTarget));
	const TPtrC data(RECAST_FROM_XML_CHAR(aData));

	MSyDocHandler* handler = RECAST_TO_DOC_HANDLER(userData);
	if (handler != NULL)
		handler->ProcessingInstruction(target, data);
}

static void CommentHandler(void* userData, const XML_Char* aData)
{
	const TPtrC data(RECAST_FROM_XML_CHAR(aData));

	MSyDocHandler* handler = RECAST_TO_DOC_HANDLER(userData);
	if (handler != NULL)
		handler->Comment(data);
}

static void StartCDataSectionHandler(void* userData)
{
	MSyDocHandler* handler = RECAST_TO_DOC_HANDLER(userData);
	if (handler != NULL)
		handler->StartCDataSection();	
	
}

static void EndCDataSectionHandler(void* userData)
{
	MSyDocHandler* handler = RECAST_TO_DOC_HANDLER(userData);
	if (handler != NULL)
		handler->EndCDataSection();
}


static void DefaultHandler(void* userData, const XML_Char* aS, int aLen)
{
	const TPtrC s(RECAST_FROM_XML_CHAR(aS), aLen);
	
	MSyDocHandler* handler = RECAST_TO_DOC_HANDLER(userData);
	if (handler != NULL)
		handler->Default(s);
}

static void StartDocTypeHandler(void *userData, const XML_Char *aDocTypeName, const XML_Char *aSysId, const XML_Char *aPubId, int aHas_internal_subset)
{
	const TPtrC docTypeName(RECAST_FROM_XML_CHAR(aDocTypeName));
	const TPtrC sysid(RECAST_FROM_XML_CHAR(aSysId));
	const TPtrC pubid(RECAST_FROM_XML_CHAR(aPubId));
	const TBool hasInternalSubset = aHas_internal_subset == 0 ? EFalse : ETrue;


	MSyDeclHandler* handler = RECAST_TO_DECL_HANDLER(userData);
	if (handler != NULL)
		handler->StartDocType(docTypeName, sysid, pubid, hasInternalSubset);

}

static void EndDocTypeHandler(void* userData)
{
	MSyDeclHandler* handler = RECAST_TO_DECL_HANDLER(userData);
	if (handler != NULL)
		handler->EndDocType();
}


static int NotStandaloneHandler(void *userData)
{
	MSyDeclHandler* handler = RECAST_TO_DECL_HANDLER(userData);
	if (handler != NULL)
		return STATIC_CAST(int, handler->NotStandalone());
		
	return 0;
}

static void StartNamespaceHandler(void *userData, const XML_Char *aPrefix, const XML_Char *aURI)
{
	const TPtrC	prefix(RECAST_FROM_XML_CHAR(aPrefix));
	const TPtrC uri(RECAST_FROM_XML_CHAR(aURI));
	
	MSyDeclHandler* handler = RECAST_TO_DECL_HANDLER(userData);
	if (handler != NULL)
		handler->StartNamespace(prefix, uri);	

}

static void EndNamespaceHandler(void *userData, const XML_Char* aPrefix)
{
	const TPtrC	prefix(RECAST_FROM_XML_CHAR(aPrefix));

	MSyDeclHandler* handler = RECAST_TO_DECL_HANDLER(userData);
	if (handler != NULL)
		handler->EndNamespace(prefix);
}

static void EntityHandler(void *userData, const XML_Char *aEntityName, int aIs_parameter_entity, 
										  const XML_Char *aValue, int aValue_length, const XML_Char *aBase, 
										  const XML_Char *aSystemId, const XML_Char *aPublicId, 
										  const XML_Char *aNotationName)
{
	const TPtrC entityName(RECAST_FROM_XML_CHAR(aEntityName));
	const TBool isParameterEntity(aIs_parameter_entity);
	const TPtrC value(RECAST_FROM_XML_CHAR(aValue), aValue_length);
	const TPtrC base(RECAST_FROM_XML_CHAR(aBase));
	const TPtrC systemId(RECAST_FROM_XML_CHAR(aSystemId));
	const TPtrC publicId(RECAST_FROM_XML_CHAR(aPublicId));
	const TPtrC notationName(RECAST_FROM_XML_CHAR(aNotationName));
	
	MSyDeclHandler* handler = RECAST_TO_DECL_HANDLER(userData);
	if (handler != NULL)
		handler->Entity(entityName, isParameterEntity, value, base, systemId, publicId, notationName);
		
	
}

static void SkippedEntityHandler(void *userData, const XML_Char *aEntityName, int aIs_parameter_entity)
{
	const TPtrC entityName(RECAST_FROM_XML_CHAR(aEntityName));
	const TBool parameterEntity(aIs_parameter_entity);

	MSyDeclHandler* handler = RECAST_TO_DECL_HANDLER(userData);
	if (handler != NULL)
		handler->SkippedEntity(entityName, parameterEntity);
}

static void NotationHandler(void *userData, const XML_Char *aNotationName, const XML_Char *aBase, 
											const XML_Char *aSystemId, const XML_Char *aPublicId)
{
	const TPtrC notationName(RECAST_FROM_XML_CHAR(aNotationName));
	const TPtrC base(RECAST_FROM_XML_CHAR(aBase));
	const TPtrC systemId(RECAST_FROM_XML_CHAR(aSystemId));
	const TPtrC publicId(RECAST_FROM_XML_CHAR(aPublicId));
	
	MSyDeclHandler* handler = RECAST_TO_DECL_HANDLER(userData);
	if (handler != NULL)
		handler->Notation(notationName, base, systemId, publicId);

}

///////////////////////////////////////////////////////////////////////////////////////
// CExpatParserBase
///////////////////////////////////////////////////////////////////////////////////////
CExpatParserBase::CExpatParserBase()
{
}

EXPORT_C  CExpatParserBase::~CExpatParserBase()
{
	// First we get the parser object which could  be NULL if the ConstructL failed

	XML_Parser parser = REINTERPRET_CAST(XML_Parser,iParser);
	if (parser != NULL)
	{
		// first we delete the document handlers
		TExpatDocumentHandlers* tmp = REINTERPRET_CAST(TExpatDocumentHandlers*, XML_GetUserData(parser));
		delete tmp;
		// next we delete the parser
		XML_ParserFree( parser );
		iParser = NULL;
	}
	else
		;// the parser was NULL so do nothing
}

void CExpatParserBase::BaseConstructL(const TDesC& aEncoding)
{
	const TDesC& tmp = KSeperator;
	
	const TChar c = tmp[0];
	iParser = XML_ParserCreateNS( RECAST_TO_XML_CHAR(aEncoding.Ptr()), c);

	TExpatDocumentHandlers* docHandlers = new (ELeave) TExpatDocumentHandlers;
	docHandlers->iDeclHandler = NULL;
	docHandlers->iDocHandler = NULL;

	XML_Parser expatParser = REINTERPRET_CAST(XML_Parser,iParser);

	// Set the doc type handlers
	XML_SetReturnNSTriplet(expatParser, XML_TRUE);
	XML_SetUserData(expatParser, docHandlers);
	XML_SetStartElementHandler(expatParser, StartElementHandler);
	XML_SetEndElementHandler(expatParser, EndElementHandler);
	XML_SetCharacterDataHandler(expatParser, CharacterDataHandler);
	XML_SetProcessingInstructionHandler(expatParser, ProcessingInstructionHandler);
	XML_SetCommentHandler(expatParser, CommentHandler);
	XML_SetStartCdataSectionHandler(expatParser, StartCDataSectionHandler);
	XML_SetEndCdataSectionHandler(expatParser, EndCDataSectionHandler);
	XML_SetDefaultHandler(expatParser, DefaultHandler);

	// Set the declaration handler
	XML_SetStartDoctypeDeclHandler(expatParser, StartDocTypeHandler);
	XML_SetEndDoctypeDeclHandler(expatParser, EndDocTypeHandler);
	XML_SetNotStandaloneHandler(expatParser, NotStandaloneHandler);	
	XML_SetStartNamespaceDeclHandler(expatParser, StartNamespaceHandler);
	XML_SetEndNamespaceDeclHandler(expatParser, EndNamespaceHandler);
	XML_SetEntityDeclHandler(expatParser, EntityHandler);
	XML_SetSkippedEntityHandler(expatParser, SkippedEntityHandler);
	XML_SetNotationDeclHandler(expatParser, NotationHandler);

}

EXPORT_C TBool CExpatParserBase::Reset(const TDesC& aEncoding)
{
	return XML_ParserReset(REINTERPRET_CAST(XML_Parser,iParser), RECAST_TO_XML_CHAR(aEncoding.Ptr()));
}

EXPORT_C TSyExpatStatus  CExpatParserBase::Parse(const TPtrC8 aBuffer, const TBool aIsFinal)
{
	return (TSyExpatStatus)XML_Parse(REINTERPRET_CAST(XML_Parser,iParser), (const char*)aBuffer.Ptr(), aBuffer.Length(), aIsFinal);
}

EXPORT_C TAny* CExpatParserBase::GetBuffer(TInt aLength)
{
	return XML_GetBuffer(REINTERPRET_CAST(XML_Parser,iParser), aLength);
}

EXPORT_C TSyExpatStatus  CExpatParserBase::ParseBuffer(const TInt aLength, const TBool aIsFinal)
{
	return (TSyExpatStatus)XML_ParseBuffer(REINTERPRET_CAST(XML_Parser,iParser), aLength, aIsFinal);
}

EXPORT_C TSyExpatStatus CExpatParserBase::Stop(const TBool bResumable)
{
	return (TSyExpatStatus)XML_StopParser(REINTERPRET_CAST(XML_Parser,iParser), bResumable ? XML_TRUE : XML_FALSE);
}

// Parse position and error reporting functions
EXPORT_C TSyExpatError CExpatParserBase::GetErrorCode() const
{
	return (TSyExpatError)XML_GetErrorCode(REINTERPRET_CAST(XML_Parser,iParser));
}
	
EXPORT_C TInt32 CExpatParserBase::GetCurrentByteIndex() const
{
	return XML_GetCurrentByteIndex(REINTERPRET_CAST(XML_Parser,iParser));
}

EXPORT_C TInt CExpatParserBase::GetCurrentLineNumber() const
{
	return XML_GetCurrentLineNumber(REINTERPRET_CAST(XML_Parser,iParser));
}

EXPORT_C TInt CExpatParserBase::GetCurrentColumnNumber() const
{
	return XML_GetCurrentColumnNumber(REINTERPRET_CAST(XML_Parser,iParser));
}

EXPORT_C TInt CExpatParserBase::GetCurrentByteCount() const
{
	return XML_GetCurrentByteCount(REINTERPRET_CAST(XML_Parser,iParser));
}

EXPORT_C TSyExpatStatus CExpatParserBase::GetParsingStatus(TBool* aFinal) const
{
	XML_ParsingStatus status;
	XML_GetParsingStatus(REINTERPRET_CAST(XML_Parser,iParser), &status);

	if (aFinal == NULL)
		*aFinal = status.finalBuffer;
		
	return (TSyExpatStatus)status.parsing;
}

// Set the handlers
EXPORT_C void CExpatParserBase::SetHandler(MSyDocHandler* aHandler)
{
	XML_Parser expatParser = REINTERPRET_CAST(XML_Parser, iParser);

	TExpatDocumentHandlers* docHandlers = REINTERPRET_CAST(TExpatDocumentHandlers*, XML_GetUserData(expatParser));

	docHandlers->iDocHandler = aHandler;
	
	if (aHandler != NULL)
		aHandler->SetParser(this);
}

EXPORT_C void CExpatParserBase::SetHandler(MSyDeclHandler* aHandler)
{
	XML_Parser expatParser = REINTERPRET_CAST(XML_Parser, iParser);
	
	TExpatDocumentHandlers* docHandlers = REINTERPRET_CAST(TExpatDocumentHandlers*, XML_GetUserData(expatParser));

	docHandlers->iDeclHandler = aHandler;
	
	if (aHandler != NULL)	
		aHandler->SetParser(this);	
}

void CExpatParserBase::SetBase(const TDesC& aBase)
{
	XML_Parser expatParser = REINTERPRET_CAST(XML_Parser, iParser);
	
	XML_SetBase(expatParser, RECAST_TO_XML_CHAR(aBase.Ptr()));
}

TPtrC CExpatParserBase::GetBase() const
{
	XML_Parser expatParser = REINTERPRET_CAST(XML_Parser, iParser);

	const XML_Char* base = XML_GetBase(expatParser);
	TPtrC basePtr(RECAST_FROM_XML_CHAR(base));
	
	return basePtr;
}

void CExpatParserBase::Panic(const TInt aPanic)
{
	_LIT(KPanicCategory, "SymbianExpat");
		
	User::Panic(KPanicCategory, aPanic);
}

EXPORT_C const TVersion CExpatParserBase::VersionInfo() const
{
	const TVersion v(XML_MAJOR_VERSION, XML_MINOR_VERSION, XML_MICRO_VERSION);
	
	return v;
}

////////////////////////////////////////////////////////////////////////////////////////
// CSyParser
////////////////////////////////////////////////////////////////////////////////////////
EXPORT_C CSyParser* CSyParser::NewL(const TDesC& aEncoding)
{	CSyParser* self = NewLC(aEncoding);
	
	CleanupStack::Pop(self);
	
	return self;
}

EXPORT_C CSyParser* CSyParser::NewLC(const TDesC& aEncoding)
{
	CSyParser* self = new (ELeave) CSyParser();
	
	CleanupStack::PushL(self);
	
	self->ConstructL(aEncoding);
	
	return self;		
	
}

EXPORT_C CSyParser::~CSyParser()
{
	// Does nothing as all the work is in the CExpatParserBase
}

void CSyParser::ConstructL(const TDesC& aEncoding)
{
	BaseConstructL(aEncoding);
}

////////////////////////////////////////////////////////////////////////////////////////
// CSyContextualParser
////////////////////////////////////////////////////////////////////////////////////////
EXPORT_C CSyContextualParser* CSyContextualParser::NewL(const TDesC& aEncoding)
{	CSyContextualParser* self = NewLC(aEncoding);
	
	CleanupStack::Pop(self);
	
	return self;
}

EXPORT_C CSyContextualParser* CSyContextualParser::NewLC(const TDesC& aEncoding)
{
	CSyContextualParser* self = new (ELeave) CSyContextualParser();
	
	CleanupStack::PushL(self);
	
	self->ConstructL(aEncoding);
	
	return self;		
	
}

//////////////////////////////////////////////////////////////////////////////////////////
// CSyContextualParser
//////////////////////////////////////////////////////////////////////////////////////////
CSyContextualParser::~CSyContextualParser()
{
	// Does nothing except empty the context stacks.
	iDocHandlerStack.Close();
	iDeclHandlerStack.Close();
}

void CSyContextualParser::ConstructL(const TDesC& aEncoding)
{
	BaseConstructL(aEncoding);
}

EXPORT_C void CSyContextualParser::PushDocHandler(MSyDocHandler* aHandler)
{
	// push the new handler onto the stack
	iDocHandlerStack.Append(aHandler);
	
	// and set it to be the default handler for the parser
	SetHandler(aHandler);
}

EXPORT_C void CSyContextualParser::PushDeclHandler(MSyDeclHandler* aHandler)
{
	// push the new handler onto the stack
	iDeclHandlerStack.Append(aHandler);
	
	// and set it to be the default handler
	SetHandler(aHandler);
	
}

EXPORT_C MSyDocHandler* CSyContextualParser::PopDocHandler()
{
	__ASSERT_ALWAYS(iDocHandlerStack.Count() >0, Panic(KErrUnderflow));
	
	// first remove the top item on the stack which is
	// the current document handler 
	const TInt top = iDocHandlerStack.Count() - 1;
	MSyDocHandler* ret = iDocHandlerStack[top];
	iDocHandlerStack.Remove(top);

	// And replace it with a new one
	if (iDocHandlerStack.Count() > 0)
	{
		const TInt newtop = iDocHandlerStack.Count()-1;
		MSyDocHandler* newHandler = iDocHandlerStack[newtop];
		SetHandler(newHandler);
	}
	else
	{
		// there is nothing on the stack so its okay
		SetHandler((MSyDocHandler*)NULL);
	}
	
	return ret;
}

EXPORT_C MSyDeclHandler* CSyContextualParser::PopDeclHandler()
{
	__ASSERT_ALWAYS(iDeclHandlerStack.Count() > 0, Panic(KErrUnderflow));
	
	const TInt top = iDeclHandlerStack.Count() - 1;
	
	MSyDeclHandler* ret = iDeclHandlerStack[top];
	
	iDeclHandlerStack.Remove(top);

	// And replace it with a new one
	if (iDeclHandlerStack.Count() > 0)
	{
		const TInt newtop = iDeclHandlerStack.Count()-1;
		MSyDeclHandler* newHandler = iDeclHandlerStack[newtop];
		SetHandler(newHandler);
	}
	else
	{
		// there is nothing on the stack so its okay
		SetHandler((MSyDeclHandler*)NULL);
	}
	
	return ret;
}
}