	/* 	
	@fn					virtual void StartElement(const TDesC& aName, const RArray<TPtrC>& aAttributes)
	Intended Usage:		Called whenever a new element is parsed. The name of the element is stored
						in aName.
	@exception			none.
	@param				aName the name of the element
	@param				aAttributes array of character strings.
	@return				none
	@pre 				None
	@post				Nothing is left on the CleanupStack
	*/
	typedef void (* TExpatStartElement)(const TDesC& aName, const RArray<TAttribute>& aAttributes);

	/* 	
	@fn					virtual void EndElement(const TDesC& aName)
	Intended Usage:		Called whenever an end element declearation is encountered. The name of
						the element being closed.
	@exception			none.
	@param				aName the name of the element
	@return				none
	@pre 				None
	@post				Nothing is left on the CleanupStack
	*/
	typedef void (* TExpatEndElement)(const TDesC& aName);

	/* 	
	@fn					virtual void CharacterData(const TDesC& aData)
	Intended Usage:		Called whenever a block of character data is read.
	@exception			none.
	@param				aData The block of data
	@param				aLength The length of the data block
	@return				none
	@pre 				None
	@post				Nothing is left on the CleanupStack
	@warn				The aData block is not null terminated!
	*/
	typedef void (* TExpatCharacterData)(const TDesC& aData);

	/* 	
	@fn					virtual void ProcessingInstruction(const TDesC& aTarget, const TDesC& aData)
	Intended Usage:		Called whenever a block of character data is read.
	@exception			none.
	@param				aTarget The processing instruction.
	@param				aData the data for the processing instruction.
	@return				none
	@pre 				None
	@post				Nothing is left on the CleanupStack
	*/
	typedef void (* TExpatProcessingInstruction)(const TDesC& aTarget, const TDesC& aData);

	/* 	
	@fn					virtual void Comment(const TDesC& aData)
	Intended Usage:		Called whenever a comment is read.
	@exception			none.
	@param				aData the comment that has been read.
	@return				none
	@pre 				None
	@post				Nothing is left on the CleanupStack
	*/
	typedef void (* TExpatComment)(const TDesC& aComment);

	/* 	
	@fn					virtual void EndCDataSection() = 0
	Intended Usage:		Called whenever an end data section is encountered.
	@exception			none.
	@return				none
	@pre 				None
	@post				Nothing is left on the CleanupStack
	*/
	typedef void (* TExpatStartCDATASection)();
	
	/* 	
	@fn					virtual void EndCDataSection() = 0
	Intended Usage:		Called whenever an end data section is encountered.
	@exception			none.
	@return				none
	@pre 				None
	@post				Nothing is left on the CleanupStack
	*/
	typedef void (* TExpatEndCDATASection)();

	/* 	
	@fn					virtual void DefaultHandler(const TDesC& aData)
	Intended Usage:		This is called for any characters in the XML document for which
					    there is no applicable handler.  This includes both characters that
						are part of markup which is of a kind that is not reported
						(comments, markup declarations), or characters that are part of a
						construct which could be reported but for which no handler has been
						supplied. The characters are passed exactly as they were in the XML
						document except that they will be encoded in UTF-8 or UTF-16.
						Line boundaries are not normalized. Note that a byte order mark
						character is not passed to the default handler. There are no
						guarantees about how characters are divided between calls to the
						default handler: for example, a comment might be split between
						multiple calls.
	@exception			none.
	@param				aData The block of data that needs to be handled
	@param				aLength the length of the data block.
	@return				none
	@pre 				None
	@post				Nothing is left on the CleanupStack
	@warn				The string is not null terminated
	*/
	typedef void (* TExpatDefault)(const TDesC& aData);

	/* 	
	@fn					virtual void StartDocType(const TDesC& aDocType, const TDesC& aSysId, const TDesC& aPubId, const TBool aHasInternalSubset)
	Intended Usage:		 This is called for the start of the DOCTYPE declaration, before any DTD or internal subset is parsed.
	@exception			none.
	@return				none
	@pre 				None
	@post				Nothing is left on the CleanupStack
	*/
	typedef void (* TExpatStartDocTypeDecl)(const TDesC& aDocType, const TDesC& aSysId, const TDesC& aPubId, const TBool aHasInternalSubset);
					

	/* 	
		@fn					virtual void EndDocType() = 0
		Intended Usage:		This is called for the start of the DOCTYPE declaration when the closing > is encountered, but after 
							processing any external subset.
		@exception			none.
		@return				none
		@pre 				None
		@post				Nothing is left on the CleanupStack
	*/
	typedef void (* TExpatEndDocTypeDecl)();


	/* 	
		@fn					Entity(const TDesC& aEntityName, const TBool aIsParameterEntity, const TDesC& aValue, const TInt32 aLength, 
							const TDesC& aBase, const TDesC& aSysId, const TDesC& aPubId, const TDesC& aNotationName)
		Intended Usage:		This is called for entity declarations. The is_parameter_entity  argument will be non-zero if the entity 
							is a parameter entity, zero  otherwise.

							For internal entities (<!ENTITY foo "bar">), value will
							be non-NULL and systemId, publicID, and notationName will be NULL.
							The value string is NOT null-terminated; the length is provided in
							the value_length argument. Since it is legal to have zero-length
							values, do not use this argument to test for internal entities.

							For external entities, value will be NULL and systemId will be
							non-NULL. The publicId argument will be NULL unless a public
							identifier was provided. The notationName argument will have a
							non-NULL value only for unparsed entity declarations.
		@exception			none.
		@return				none
		@pre 				None
		@post				Nothing is left on the CleanupStack
	*/


	/* 	
		@fn					TSymbianExpatStatus TExpatNotStandalone()
		Intended Usage:		This is called if the document is not standalone, that is, it has an
							external subset or a reference to a parameter entity, but does not
							have standalone="yes". If this handler returns XML_STATUS_ERROR,
							then processing will not continue, and the parser will return a
							XML_ERROR_NOT_STANDALONE error.
							If parameter entity parsing is enabled, then in addition to the
							conditions above this handler will only be called if the referenced
							entity was actually read.
		@exception			none.
		@return				none
		@pre 				None
		@post				Nothing is left on the CleanupStack
	*/
	typedef TSymbianExpatStatus (* TExpatNotStandalone)();

	/* 	
		@fn					void StartNamespace(const TDesC& aPrefix, const TDesC& aURI)
		Intended Usage:		When namespace processing is enabled, this is called once for
							each namespace declaration. The call to the start and end element
							handlers occur between the calls to the start and end namespace
							declaration handlers. For an xmlns attribute, prefix will be
							NULL.  For an xmlns="" attribute, uri will be NULL.
		@exception			none.
		@return				none
		@pre 				None
		@post				Nothing is left on the CleanupStack
		*/
	typedef void (* TExpatStartNamespaceDecl)(const TDesC& aPrefix, const TDesC& aURI);

	/* 	
		@fn					void EndNamespace(const TDesC& aPrefix)
		Intended Usage:		When namespace processing is enabled, this is called once for
							each namespace declaration. The call to the start and end element
							handlers occur between the calls to the start and end namespace
							declaration handlers.
		@exception			none.
		@return				none
		@pre 				None
		@post				Nothing is left on the CleanupStack
		*/
	typedef void (* TExpatEndNamespaceDecl)(const TDesC& aPrefix);

struct ExpatDocumentHandlers
{
	TExpatStartElement				iFnStartElement;
	TExpatEndElement				iFnEndElement;
	TExpatCharacterData				iFnCharacterData;
	TExpatProcessingInstruction		iFnProcessingInstruction;
	TExpatComment					iFnComment;
	TExpatStartCDATASection			iFnStartCDATASection;
	TExpatEndCDATASection			iFnEndCDATASection;
	TExpatDefault					iFnDefault;
	TExpatStartDocTypeDecl			iFnStartDocTypeDecl;
	TExpatEndDocTypeDecl			iFnEndDocTypeDecl;
	TExpatNotStandalone				iFnNotStandalone;
	TExpatStartNamespaceDecl		iFnStartNamespaceDecl;
	TExpatEndNamespaceDecl			iFnEndNamespaceDecl;
};
