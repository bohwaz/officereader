#ifndef __XML_HELPER_H
#define __XML_HELPER_H

class CDocumentHandler : public CBase, public MSyDocHandler
{
public:
	CDocumentHandler(RTest& aTest);
	virtual ~CDocumentHandler();
	
	virtual void StartElement(const TQualified& aName, const RArray<TAttribute>& attributes);
	virtual void EndElement(const TQualified& aName);
	virtual void CharacterData(const TDesC& aData);
	virtual void ProcessingInstruction(const TDesC& aTarget, const TDesC& aData);
	virtual void Comment(const TDesC& aData);
	virtual void StartCDataSection();
	virtual void EndCDataSection();
	virtual void Default(const TDesC& /*aData*/);
	
	virtual void SetParser(CExpatParserBase* aParser);
private:
	void Indent(const TInt aDepth);
	
protected:
	RTest&			iTest;
	TInt			iDepth;
};

class CDeclarationHandler : public CBase, public MSyDeclHandler
{
public:
	CDeclarationHandler(RTest& aTest);
	virtual ~CDeclarationHandler();
	
	virtual TSyExpatStatus NotStandalone();
	virtual void StartDocType( const TDesC& aDocType, const TDesC& aSystemId, const TDesC& aPublicId, const TBool aHasInternalSubset);
	
	virtual void EndDocType();
	virtual void StartNamespace(const TDesC& aPrefix, const TDesC& aURI);
	virtual void EndNamespace(const TDesC& aPrefix);
	virtual void Entity(const TDesC& aName, const TBool aIsParameter, const TDesC& aValue, const TDesC& aBase, const TDesC& aSystemId, const TDesC& aPublicId, const TDesC& aNotation);
	virtual void SkippedEntity(const TDesC& aEntity, const TBool aIsParameter);
	virtual void Notation(const TDesC& aNotationName, const TDesC& aBase, const TDesC& aSystemId, const TDesC& aPublicId);
	virtual void SetParser(CExpatParserBase* aParser);
	
private:
	void Indent(const TInt aDepth);
	
protected:
	RTest&	iTest;	
	TInt			iDepth;	
};

#endif // __XML_HELPER_H