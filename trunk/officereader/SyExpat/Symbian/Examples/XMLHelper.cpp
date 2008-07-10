#include "SyExpat.h"
#include "e32test.h"
#include "XMLHelper.h"

CDocumentHandler::CDocumentHandler(RTest& aTest)
:	iTest(aTest),
	iDepth(0)
{
}
	
CDocumentHandler::~CDocumentHandler()
{
}
	
void CDocumentHandler::Indent(const TInt aDepth)
{
	for (TInt i=0; i < aDepth; i++)
	{
//		_LIT(KTab, "   ");
//		iTest.Printf(KTab);
	}
}
	
void CDocumentHandler::StartElement(const TQualified& aName, const RArray<TAttribute>& attributes)
{

	TPtrC elemName(aName.iLocalName);
	TPtrC elemPrefix(aName.iPrefix);
	TPtrC elemURI(aName.iURI);
	
	if (elemPrefix.Length() == 0)
	{
		// locally named element
		_LIT(KElementFormatString,"LN:%S ");
		Indent(iDepth);iTest.Printf(KElementFormatString, &elemName);
	}
	else
	{
		if (elemURI.Length() > 0)
		{
			// namespace declaration for URI
			_LIT(KNamespaceFormatString,"ELEM:%S ");
			Indent(iDepth); iTest.Printf(KNamespaceFormatString, &elemURI);
		}
		
	
		// Fully qualified element
		_LIT(KElementFormatString, ":%S:%S ");
		Indent(iDepth);iTest.Printf(KElementFormatString, &elemPrefix, &elemName);
	}
		
	TInt i = 0;
	const TInt attrCount = attributes.Count() /2;
	for (i = 0; i < attrCount; i++)
	{
		const TAttribute& attribute = attributes[i];
		const TPtrC attrPrefix(attribute.iName.iPrefix);
		const TPtrC attrName(attribute.iName.iLocalName);
		const TPtrC attrValue(attribute.iValue);
		
		if (attrPrefix.Length() == 0)
		{
			// Locally Named attribute
			_LIT(KAttributeFormatString, "LNA:%S = %S\n");
			Indent(iDepth);iTest.Printf(KAttributeFormatString, &attrName, &attrValue);
		}
		else
		{
			// Fully Qualified attribute
			_LIT(KAttributeFormatString, "QNA:%S:%S = %S\n");
			Indent(iDepth);iTest.Printf(KAttributeFormatString, &attrPrefix, &attrName, &attrValue);
			
		}
	}
		
	if (i == 0)
	{
		_LIT(KNewLine, "\n");
		iTest.Printf(KNewLine);
	}
		
	iDepth++;
}
	
void CDocumentHandler::EndElement(const TQualified& /*aName*/)
{
//	_LIT(KElementFormatString,"\n");
//	Indent(iDepth);iTest.Printf(KElementFormatString);
}
	
void CDocumentHandler::CharacterData(const TDesC& aData)
{
//	_LIT(KElementFormatString,"%S");
//	TPtrC tmp(aData);
//	Indent(iDepth);iTest.Printf(KElementFormatString, &tmp);
}
	
void CDocumentHandler::ProcessingInstruction(const TDesC& aTarget, const TDesC& aData)
{
	_LIT(KElementFormatString,"PI %S = %S\n");

	TPtrC target(aTarget);
	TPtrC data(aData);
	Indent(iDepth);iTest.Printf(KElementFormatString, &target, &data);
}

void CDocumentHandler::Comment(const TDesC& aData)
{
	_LIT(KElementFormatString,"Comment - %S\n");

	TPtrC tmp(aData);
	Indent(iDepth); iTest.Printf(KElementFormatString, &tmp);
}
	
void CDocumentHandler::StartCDataSection()
{
	_LIT(KDefaultFormatString,"CDATA Start\n");
	Indent(iDepth); iTest.Printf(KDefaultFormatString);
}
	
void CDocumentHandler::EndCDataSection()
{
	_LIT(KDefaultFormatString,"CDATA End\n");
	Indent(iDepth); iTest.Printf(KDefaultFormatString);
}
	
void CDocumentHandler::Default(const TDesC& aData)
{
//	_LIT(KDefaultFormatString,"Default - S\n");
//	TPtrC tmp(aData);
//	Indent(iDepth); iTest.Printf(KDefaultFormatString, &tmp);
}

void CDocumentHandler::SetParser(CExpatParserBase* /*aParser*/)
{
	// Does nothing
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////
CDeclarationHandler::CDeclarationHandler(RTest& aTest)
:	iTest(aTest),
	iDepth(0)
{
}

CDeclarationHandler::~CDeclarationHandler()
{
}

void CDeclarationHandler::Indent(const TInt aDepth)
{
	for (TInt i=0; i < aDepth; i++)
	{
		_LIT(KTab, "   ");
//		iTest.Printf(KTab);
	}
}
	

TSyExpatStatus CDeclarationHandler::NotStandalone()
{
	_LIT(KDefaultFormatString,"This document is not standalone!\n");
	Indent(iDepth); iTest.Printf(KDefaultFormatString);

	return EStatusOk;
}

void CDeclarationHandler::StartDocType( const TDesC& aDocType, const TDesC& /*aSystemId*/, const TDesC& /*aPublicId*/, const TBool /*aHasInternalSubset*/)
{
	_LIT(KDefaultFormatString,"DT: %S\n");
	
	TPtrC doctype(aDocType);
	Indent(iDepth); iTest.Printf(KDefaultFormatString, &doctype);
	iDepth++;

}

void CDeclarationHandler::EndDocType()
{
	iDepth--;
}

void CDeclarationHandler::StartNamespace(const TDesC& aPrefix, const TDesC& aURI)
{
	_LIT(KDefaultFormatString,"NS: %S:%S\n");
	
	TPtrC prefix(aPrefix);
	TPtrC uri(aURI);
	
	Indent(iDepth); iTest.Printf(KDefaultFormatString, &prefix, &uri);
	
	iDepth++;
}

void CDeclarationHandler::EndNamespace(const TDesC& /*aPrefix*/)
{
	iDepth--;
}

void CDeclarationHandler::Entity(const TDesC& aName, const TBool /*aIsParameter*/, const TDesC& aValue, const TDesC& /*aBase*/, const TDesC& /*aSystemId*/, const TDesC& /*aPublicId*/, const TDesC& /*aNotation*/)
{
	_LIT(KDefaultFormatString,"ENT: %S = %S\n");
	
	TPtrC name(aName);
	TPtrC value(aValue);
	
	Indent(iDepth); iTest.Printf(KDefaultFormatString, &name, &value);

}

void CDeclarationHandler::SkippedEntity(const TDesC& aEntity, const TBool /*aIsParameter*/)
{
	_LIT(KDefaultFormatString,"SE: %S\n");
	
	TPtrC entity(aEntity);
	
	Indent(iDepth); iTest.Printf(KDefaultFormatString, &entity);

}

void CDeclarationHandler::Notation(	const TDesC& aNotationName, const TDesC& /*aBase*/, const TDesC& /*aSystemId*/, const TDesC& /*aPublicId*/)
{
	_LIT(KDefaultFormatString,"NOT: %S\n");
	
	TPtrC notationName(aNotationName);
	
	Indent(iDepth); iTest.Printf(KDefaultFormatString, &notationName);
}

void CDeclarationHandler::SetParser(CExpatParserBase* /*aParser*/)
{
	// Does nothing
}
