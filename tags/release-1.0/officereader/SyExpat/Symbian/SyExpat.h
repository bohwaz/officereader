/** @mainpage  XML Reader for Symbian OS
	@n
	@n
	This is the documentation for the XML Reader class
	The parser is currently based on the 1.95.8 parser code from
	the Expat.org site.
	@n
	@n
	Copyright  © 2004 ToddSoftware.com. All rights reserved.
	@n
	Copyright (c) 1998, 1999, 2000 Thai Open Source Software Center Ltd
    and Clark Cooper
	@n
	Copyright (c) 2001, 2002, 2003 Expat maintainers.
	@n
	Permission is hereby granted, free of charge, to any person obtaining
	a copy of this software and associated documentation files (the
	"Software"), to deal in the Software without restriction, including
	without limitation the rights to use, copy, modify, merge, publish,
	distribute, sublicense, and/or sell copies of the Software, and to
	permit persons to whom the Software is furnished to do so, subject to
	the following conditions:
	@n
	@n
	The above copyright notice and this permission notice shall be included
	in all copies or substantial portions of the Software.
	@n
	@n
	THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
	EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
	MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.
	IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY
	CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT,
	TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE
	SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
*/

/** @file SyExpat.h
	This file contains the definition of the %CExpatParserBase class,
	and its associated typedefs and constants.
	This is the main definition of the XML Parser and its methods.
*/

#ifndef __SYEXPAT_H
#define __SYEXPAT_H

#include <e32base.h>

	 /**
	 *	Status of the parser object.
	 */
	enum TSyExpatStatus
	{ 
		EStatusError, 						///< The parser encountered an error.
		EStatusOk, 							///< The parser is ok.6		
		EStatusSuspended					///< The parser is currently suspended.
	};
	
	 /**
	 *	Error codes enumeration for the parser.
	 */
	
	enum TSyExpatError
	{
		ESyExpatErrorNone,							///< There is no error.
		ESyExpatErrorMemory,						///< The memory is exhausted.
		ESyExpatErrorSyntax,						///< There was a syntax error.
		ESyExpatErrorNoElements,					///< There are no elements in the xml file.
		ESyExpatErrorInvalidToken,					///< The parser encountered an invalid token.
		ESyExpatErrorUnclosedToken,					///< The parser encountered an unclosed token.
		ESyExpatErrorPartialCharacter,				///< A partial character was encountered.
		ESyExpatErrorTagMismatch,					///< A mismatched tag was found during parsing.
		ESyExpatErrorDuplicateAttribute,			///< A duplicate attribute name was found in an element.
		ESyExpatErrorJunkAfterDocElement,			///< Illegal data found after the doc element.
		ESyExpatErrorParamEntityRef,				///< Parameter Entity Reference error.
		ESyExpatErrorUndefinedEntity,				///< An entity was undefined.
		ESyExpatErrorRecursiveEntity,				///< An entity is recursivly defined.
		ESyExpatErrorAsyncEntity,					///< An entity is ayncronously defined.
		ESyExpatErrorBadCharRef,					///< An illegal character reference was found.
		ESyExpatErrorBinaryEntityRef,				///< A binary entity reference was found.
		ESyExpatErrorMisplacedPIInstruction,		///< A misplaced processing instruction was found.
		ESyExpatErrorUnknownEncoding,				///< An unknown encoding was found.
		ESyExpatErrorIncorrectEncoding,				///< An incorrect encoding was found.
		ESyExpatErrorUnclosedCDataSection,			///< An unclosed data section was found.
		ESyExpatErrorExternalEntityHandling,		///< An error handling external entity references.
		ESyExpatErrorNotStandalone,					///< The XML document is not standalone.
		ESyExpatErrorUnexpectedState,				///< The parser has got into an unexpected state.
		ESyExpatErrorEntityDeclaredInPE,			///< Entity declared as external parameter entity.
		ESyExpatErrorFeaturesRequiresXMLDTD,		///< Features require an XML DTD.
		ESyExpatErrorFeatureCannotChange,			///< Feature cannot  be changed
		ESyExpatErrorUnboundPrefix,					///< Attribute prefix is unbound
		ESyExpatErrorUndeclaringPrefix,				///< A prefix cannot be reset to empty
		ESyExpatErrorIncompletePE,					///< Incomplete parameter entity
		ESyExpatErrorXMLDecl,						///< Error in DECL XML declaration
		ESyExpatErrorTextDecl,						///< Error in TEXT XML declaration
		ESyExpatErrorPublicId,						///< Error in public id
		ESyExpatErrorSuspended,						///< Parser is suspended error
		ESyExpatErrorNonSuspended,					///< Parser is not suspended error
		ESyExpatErrorAborted,						///< Parsing was aborted
		ESyExpatErrorFinished,						///< Parsing was finished
		ESyExpatErrorSuspendPE						///< Parsing was suspended due to a parameter entity
	};


	/**
	 * @struct TQualified
	 * A struct to help build a qualified name.
	 * The only field that will never be empty
	 * will be the iLocalName field, the prefix
	 * and URI could be empty
	 */
	struct TQualified
	{
		/**
		 * The URI of the element in the current scope
		 */
		 TPtrC  iPrefix;
		 
		 /**
		  * The local name, or just the name of the element or attribute
		  */
		 TPtrC	iLocalName;
		 
		 /**
		  * the URI of the element or the attribute in the form of
		  * schema:URI
		  */
		 TPtrC	iURI;
	};
	
	/**
	 *	@struct	TAttribute
	 *	Struct to help build an attribute.
	 *  An attribute consists not just of a local name, but also possibly a prefix and
	 *  a URI.
	 */
	struct TAttribute
	{
		/**
		 * The attribute name which includes the optional namespace.
		 */
		TQualified iName;
		
		/**
		 * The value of the attribute
		 */
		TPtrC		iValue;
	};
	
	/**
	 * forward declare of this class to assist in saving the parser instance
	 */
	class CExpatParserBase;
	
	/**
		@class MSyDocHandler
		Mixin interface class that is intended to be overridden in an inherited class.
		Default stub implementations of all the functions are provided to make it easier 
		to inherit from. Provides the default handlers for the document
	*/
	class MSyDocHandler
	{
	public:
		/** 	
		@fn					virtual void StartElement(const TQualified& aName, const RArray< ::TAttribute>& aAttributes)
		Intended Usage:		Called whenever a new element is parsed. The name of the element is stored
							in aName.
		@exception			none.
		@param				aName the  element.
		@param				aAttributes array of attributes, consisting of a prefix and a name and a value.
		@return				none
		@pre 				None
		@post				Nothing is left on the CleanupStack
		*/
		virtual void StartElement(const TQualified& aName, const RArray< ::TAttribute>& aAttributes) = 0;

		/** 	
		@fn					virtual void EndElement(const TQualified& aName)
		Intended Usage:		Called whenever an end element declearation is encountered. The name of
							the element being closed.
		@exception			none.
		@param				aName the name of the element
		@return				none
		@pre 				None
		@post				Nothing is left on the CleanupStack
		*/
		virtual void EndElement(const TQualified& aName) = 0;
		
		
		/** 	
		@fn					virtual void CharacterData(const TDesC& aData)
		Intended Usage:		Called whenever a block of character data is read.
		@exception			none.
		@param				aData The block of data
		@return				none
		@pre 				None
		@post				Nothing is left on the CleanupStack
		@warning			The aData block is not null terminated!
		*/
		virtual void CharacterData(const TDesC& aName) = 0;
		
		/** 	
		@fn					virtual void ProcessingInstruction(const TDesC& aTarget, const TDesC& aData)
		Intended Usage:		Called whenever a block of character data is read.
		@exception			none.
		@param				aTarget The processing instruction.
		@param				aData the data for the processing instruction.
		@return				none
		@pre 				None
		@post				Nothing is left on the CleanupStack
		*/
		virtual void ProcessingInstruction(const TDesC& aTarget, const TDesC& aData) = 0;
		
		/** 	
		@fn					IMPORT_C virtual void Comment(const TDesC& aData)
		Intended Usage:		Called whenever a comment is read.
		@exception			none.
		@param				aData the comment that has been read.
		@return				none
		@pre 				None
		@post				Nothing is left on the CleanupStack
		*/
		virtual void Comment(const TDesC& aData) = 0;

		/** 	
		@fn					virtual void StartCDataSection()
		Intended Usage:		Called whenever an end data section is encountered.
		@exception			none.
		@return				none
		@pre 				None
		@post				Nothing is left on the CleanupStack
		*/
		virtual void StartCDataSection() = 0;
		
		/** 	
		@fn					virtual void EndCDataSection() = 0
		Intended Usage:		Called whenever an end data section is encountered.
		@exception			none.
		@return				none
		@pre 				None
		@post				Nothing is left on the CleanupStack
		*/
		virtual void EndCDataSection() = 0;

		/** 	
		@fn					void Default(const TDesC& aData)
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
		@return				none
		@pre 				None
		@post				Nothing is left on the CleanupStack
		@warning			The string is not null terminated
		*/
		virtual void Default(const TDesC& aData) = 0;

		
		/** 	
		@fn					virtual void SetParser(CExpatParserBase* aParser);
		Intended Usage:		This sets the parser that the mixin class is associated with
							This function is guaranteed to be called before the first
							method on the interface is called, so the developer will always
							have access to the parser that is currently parsing the data.
							The default
		@exception			none.
		@param				aParser the parser instance associated with the interface
		@return				none
		@pre 				None
		@post				Nothing is left on the CleanupStack
		*/
		virtual void SetParser(CExpatParserBase* aParser) = 0;

	protected:

	};

	/**
		@class MSyDeclHandler
		Mixin interface class that is intended to be overridden in an inherited class.
		Default stub implementations of all the functionions are provided to make it 
		easier to inherit from.	Provides the callback functionality for the declaration 
		handlers in	the document
	*/
	class MSyDeclHandler
	{
	public:
		/** 	
			@fn					virtual TSyExpatStatus NotStandalone();
			Intended Usage:		This is called if the document is not standalone, that is, it has an
								external subset or a reference to a parameter entity, but does not
								have standalone="yes". If this handler returns EStatusError,
								then processing will not continue, and the parser will return a
								ENotStandalone error.
								If parameter entity parsing is enabled, then in addition to the
								conditions above this handler will only be called if the referenced
								entity was actually read.
			@exception			none.
			@return				none
			@pre 				None
			@post				Nothing is left on the CleanupStack
		*/
		virtual TSyExpatStatus NotStandalone() = 0;
		
		/** 	
		@fn					void StartDocType(	const TDesC& aDocType, const TDesC& aSystemId, 
												const TDesC& aPublicId, const TBool aHasInternalSubset)
		Intended Usage:		This is called for the start of the DOCTYPE declaration, before any DTD or 
							internal subset is parsed.
		@exception			none.
		@param				aDocType the type of doc
		@param				aSystemId the SystemId of the document.
		@param				aPublicId the public id of the document.
		@param				aHasInternalSubset Flag indicating if the document has an internal subset.
		@return				none
		@pre 				None
		@post				Nothing is left on the CleanupStack
		*/
		virtual void StartDocType( const TDesC& aDocType, const TDesC& aSystemId, const TDesC& aPublicId, const TBool aHasInternalSubset) = 0;
		
		/** 	
			@fn					virtual void EndDocType();
			Intended Usage:		This is called for the start of the DOCTYPE declaration when the closing > is encountered, but after 
								processing any external subset.
			@exception			none.
			@return				none
			@pre 				None
			@post				Nothing is left on the CleanupStack
		*/
		virtual void EndDocType() = 0;
		
		/** 	
			@fn					void StartNamespace(const TDesC& aPrefix, const TDesC& aURI)
			Intended Usage:		When namespace processing is enabled, this is called once for
								each namespace declaration. The call to the start and end element
								handlers occur between the calls to the start and end namespace
								declaration handlers. For an xmlns attribute, prefix will be
								NULL.  For an xmlns="" attribute, uri will be NULL.
			@exception			none.
			@param				aPrefix the prefix of the namespace being opened.
			@param				aURI the URI of the namespace
			@return				none
			@pre 				None
			@post				Nothing is left on the CleanupStack
		*/
		virtual void StartNamespace(const TDesC& aPrefix, const TDesC& aURI) = 0;

		/** 	
		@fn					void EndNamespace(const TDesC& aPrefix)
		Intended Usage:		When namespace processing is enabled, this is called once for
							each namespace declaration. The call to the start and end element
							handlers occur between the calls to the start and end namespace
							declaration handlers.
		@exception			none.
		@param				aPrefix the prefix of the current namespace being closed.
		@return				none.
		@pre 				None
		@post				Nothing is left on the CleanupStack
		*/
		virtual void EndNamespace(const TDesC& aPrefix) = 0;


		/** 	
		@fn					virtual void Entity(const TDesC& aName, const TBool aIsParameter, 
												const TDesC& aValue, const TDesC& aBase, 
												const TDesC& aSystemId, const TDesC& aPublicId,
									 			const TDesC& aNotation);

		Intended Usage:		This is called for entity declarations. The aIsParameter
   							argument will be non-zero if the entity is a parameter entity, zero
   							otherwise.

   							For internal entities (<!ENTITY foo "bar">), value will
   							be non-NULL and systemId, publicID, and notationName will be empty.
   							The value string is NOT null-terminated. Since it is legal to 
   							have zero-length values, do not use this argument to test for 
   							internal entities.

   							For external entities, value will be empty and systemId will not be
   							empty. The publicId argument will be empty unless a public
   							identifier was provided. The notationName argument will have a
   							non empty value only for unparsed entity declarations.
		@exception			none.
		@param				aName the name of the entity
		@param				aIsParameter TRUE if the parameter is a parameter entity
		@param				aValue the value of the entity.
		@param				aBase the base URI of the entity.
		@param				aSystemId the SystemId of the entity.
		@param				aPublicId the PublicId of the entity.
		@param				aNotation the notation for the entity.
		@return				none
		@pre 				None
		@post				Nothing is left on the CleanupStack
		*/
		virtual void Entity(const TDesC& aName, const TBool aIsParameter, const TDesC& aValue, 
							const TDesC& aBase, const TDesC& aSystemId, const TDesC& aPublicId,
							const TDesC& aNotation) = 0;

		/** 	
		@fn					void SkippedEntity(const TDesC& aEntity, const TBool aIsParameter)
		Intended Usage:		This is called in two situations:
							1) 	An entity reference is encountered for which no declaration
      							has been read *and* this is not an error.
							2) 	An internal entity reference is read, but not expanded, because
      							the default has been called.
   							Note: skipped parameter entities in declarations and skipped general
         					entities in attribute values cannot be reported, because
         					the event would be out of sync with the reporting of the
         					declarations or attribute values.
		@exception			none.
		@param				aEntity the entity name which will not be blank
		@param				aIsParameter is TRUE if the entity is a parameter entity.
		@return				none
		@pre 				None
		@post				Nothing is left on the CleanupStack
		*/
		virtual void SkippedEntity(const TDesC& aEntity, const TBool aIsParameter) = 0;
									
		/** 	
		@fn					Notation(	const TDesC& aNotationName, const TDesC& aBase, 
										const TDesC& aSystemId, const TDesC& aPublicId);
		Intended Usage:		This is called for a declaration of notation.
		@exception			none.
		@param				aNotationName the name of the notation. This will never be empty.
		@param				aBase the base URI of the notation.
		@param				aSystemId the system id.
		@param				aPublicId the public id.
		@return				none
		@pre 				None
		@post				Nothing is left on the CleanupStack
		*/
		virtual void Notation(	const TDesC& aNotationName, const TDesC& aBase, 
								const TDesC& aSystemId, const TDesC& aPublicId) = 0;
										

		/** 	
		@fn					virtual void SetParser(CExpatParserBase* aParser);
		Intended Usage:		This sets the parser that the mixin class is associated with
							This function is guaranteed to be called before the first
							method on the interface is called, so the developer will always
							have access to the parser that is currently parsing the data.
							The default
		@exception			none.
		@param				aParser the parser instance associated with the interface
		@return				none
		@pre 				None
		@post				Nothing is left on the CleanupStack
		*/
		virtual void SetParser(CExpatParserBase* aParser) = 0;
	};

	/**
		@class CExpatParserBase.
		C++ wrapper over the Expat parser C classes. This class provides all
		the functionality required for using the Expat parser in Symbian.
		Note that unlike conventional Symbian applications which throw when they
		get an error, this class returns back a status variable from most functions
		which must then be checked for an error status and if an error is present
		call GetErrorCode to get the actual error.
		It is intended that this is the class any specializations of the parser are
		derived from.
		@see CSyContextualParser
		@see CSyParser
	 */
	class CExpatParserBase : public CBase
	{
		public:
			/**
				@fn					~CExpatParserBase()
				Intended Usage	:	Standard destructor.
									This distructor is marked as virtual as this class intended
									to be inherited from. so calling delete on CExpatParserBaseObject
									will correctly delete inherited classes.
		 	*/
			IMPORT_C virtual ~CExpatParserBase();
			
			/**
				@fn					TBool Reset(const TDesC& aEncoding);
				Intended Usage:		Prepare a parser object to be re-used.  This is particularly
	   								valuable when memory allocation overhead is disproportionatly high,
	   								such as when a large number of small documents need to be parsed.
	   								All handlers are cleared from the parser, except for the
	   								unknownEncodingHandler. The parser's external state is re-initialized
	   								except for the values of ns and ns_triplets.
	.			@exception			none.
				@param				aEncoding The default encoding to use when parsing new input
				@return				ETrue if the parser was successfully reset
				@pre 				None
				@post				Nothing is left on the CleanupStack
			 */
			IMPORT_C TBool Reset(const TDesC& aEncoding);

			/**
				@fn					TSyExpatStatus  Parse(const TPtrC8 aBuffer, const TBool aIsFinal)
				Intended Usage:		Parses a block of memory held in the buffer.
									Parses some input. Returns TExpatStatusError if a fatal error is
	   								detected.  The last call to Parse must have aIsFinal true; 
	   								The buffer may be a zero length.
				@exception			none.
				@param				aBuffer Pointer to a block of memory holding the data to be parsed.
				@param				aIsFinal Set to ETrue if this buffer is the last buffer to be parsed.
				@return				Either TExpatStatusOk if the buffer was successfully parsed, error
									otherwise.
				@pre 				none
				@post				Nothing is left on the CleanupStack.
			 */
			IMPORT_C TSyExpatStatus  Parse(const TPtrC8 aBuffer, const TBool aIsFinal);

			/**
				@fn					TSyExpatStatus  ParseBuffer(const TInt aLength, const TBool aIsFinal)
				Intended Usage:		Parses a block of memory held in the internal buffer.
									Parses some input. Returns TExpatStatus if a fatal error is
	   								detected.  The last call to Parse must have aIsFinal true; 
	   								The buffer may be a zero length.
				@exception			none.
				@param				aLength the length of the internal buffer to parse
				@param				aIsFinal ETrue if this is the last portion of the internal buffer 
									to parse.
				@return				Either TExpatStatusOk if the buffer was successfully parsed, error
									otherwise.
				@see				GetBuffer.
				@pre 				The parser state is not suspended and it is not in finished state.
				@post				Nothing is left on the CleanupStack.
			 */
			 IMPORT_C TSyExpatStatus  ParseBuffer(const TInt aLength, const TBool aIsFinal);

			/**
				@fn					TAny* GetBuffer(const TInt aLength)
				Intended Usage:		Reallocates or allocates a new internal buffer of the specified length.
									This buffer is owned by the parser and should not be deleted.
				@exception			none.
				@param				aLength The length of the internal buffer to parse
				@return				A pointer to the internal buffer or NULL if there was an error either
									the parser is in an inconsistent state or the memory cannot be allocated.
				@pre 				None
				@post				Nothing is left on the CleanupStack.
				@warning			The returned buffer is owned by the parser and must not be deleted.
			 */
			IMPORT_C TAny* GetBuffer(const TInt aLength);

			/**
				@fn					TSyExpatStatus  Stop(const TBool aResumable)
				Intended Usage:		Stops parsing, causing Parse() or ParseBuffer() to return.
								    Must be called from within a call-back handler, except when aborting
								    (aResumable = 0) an already suspended parser. Some call-backs may
								    still follow because they would otherwise get lost. Examples:
								    - endElementHandler() for empty elements when stopped in
								      startElementHandler(), 
								    - endNameSpaceDeclHandler() when stopped in endElementHandler(), 
								      and possibly others.

								    Can be called from most handlers, including DTD related call-backs,
								    except when parsing an external parameter entity and resumable != 0.
								    Returns TExpatStatusOk when successful, TExpatStatusError otherwise.
								    Possible error codes: 
								    - ESuspended: when suspending an already suspended parser.
								    - EFinished: when the parser has already finished.

								    When aResumable != 0 (true) then parsing is suspended, that is, 
								    Parse() and ParseBuffer() return TExpatStatusSuspended. 
								    Otherwise, parsing is aborted, that is, Parse() and ParseBuffer()
								    return TExpatStatusError with error code TSymbianExpatErrorAborted.

								    *Note*:
								    This will be applied to the current parser instance only, that is, if
								    there is a parent parser then it will continue parsing when the
								    externalEntityRefHandler() returns. It is up to the implementation of
								    the externalEntityRefHandler() to call StopParser() on the parent
								    parser (recursively), if one wants to stop parsing altogether.

								    When suspended, parsing can be resumed by calling ResumeParsing(). 
				@exception			none.
				@param				aResumable ETrue if parsing can be resumed later on.
				@return				TExpatStatusOk if the parser was successfully stopped, error otherwise.
				@pre 				None
				@post				Nothing is left on the CleanupStack
			 */
			 IMPORT_C TSyExpatStatus  Stop(const TBool aResumable);

			/**
				@fn					TSyExpatStatus  Resume()
				Intended Usage:		Resumes parsing after it has been suspended with Stop().
	   								Must not be called from within a handler call-back. Returns same
		   							status codes as Parse() or ParseBuffer().
								    Additional error code EExpatErrorNotSuspended possible.   

								    *Note*:
	   								This must be called on the most deeply nested child parser instance
	   								first, and on its parent parser only after the child parser has finished,
	   								to be applied recursively until the document entity's parser is restarted.
	   								That is, the parent parser will not resume by itself and it is up to the
	   								application to call Resume() on it at the appropriate moment.
				@exception			none.
				@return				TExpatStatusOk if ok, error otherwise.
				@pre 				None
				@post				Nothing is left on the CleanupStack
			 */
			IMPORT_C TSyExpatStatus  Resume();
		  
			/**
				@fn					TSymbianExpatError   GetErrorCode() const
				Intended Usage:		If Parse or ParseBuffer have returned TExpatStatusError, then
	   								GetErrorCode returns information about the error.
				@exception			none.
				@return				TSymbianExpatError enum value
				@pre 				None.
				@post				Nothing is left on the CleanupStack
			 */
			IMPORT_C TSyExpatError   GetErrorCode() const;

			/**
				@fn					TInt32 GetCurrentByteIndex() const
				Intended Usage:		These functions return information about the current parse
								    location.  They may be called from any callback called to report
	   								some parse event; in this case the location is the location of the
	   								first of the sequence of characters that generated the event.  When
	   								called from callbacks generated by declarations in the document
	   								prologue, the location identified isn't as neatly defined, but will
	   								be within the relevant markup.  When called outside of the callback
	   								functions, the position indicated will be just past the last parse
	   								event (regardless of whether there was an associated callback).
	   
	   								They may also be called after returning from a call to Parse
	   								or ParseBuffer.  If the return value is EExpatStatusError then
	   								the location is the location of the character at which the error
	   								was detected; otherwise the location is the location of the last
	   								parse event, as described above.
				@exception			none.
				@return				The index of where the parser currently is.
				@pre 				None.
				@post				Nothing is left on the CleanupStack
			 */
			IMPORT_C TInt32 GetCurrentByteIndex() const;

			/**
				@fn					TInt GetCurrentByteCount() const
				Intended Usage:		Gets the current number of bytes that have been parsed so far.
									See GetCurrentByteIndex for limitations and notes on this function
				@exception			none.
				@return				The current byte count in the document.
				@see				GetCurrentByteCount
				@pre 				None
				@post				Nothing is left on the CleanupStack
			 */
			IMPORT_C TInt   GetCurrentByteCount() const;

			/**
				@fn					TInt   GetCurrentLineNumber() const
				Intended Usage:		Returns the current line number in the document.
									See GetCurrentByteIndex for limitations and notes on this function
				@exception			none.
				@return				The current line number being parsed.
				@see				GetCurrentByteIndex
				@pre 				None
				@post				Nothing is left on the CleanupStack
			 */
			IMPORT_C TInt		GetCurrentLineNumber() const;

			/**
				@fn					TInt GetCurrentColumnNumber() const
				Intended Usage:		Returns the current column number in the document.
									See GetCurrentByteIndex for limitations and notes on this function
				@exception			none.
				@return				The current column number being parsed.
				@see				GetCurrentByteIndex
				@pre 				None
				@post				Nothing is left on the CleanupStack
			 */
			IMPORT_C TInt   GetCurrentColumnNumber() const;

			/**
				@fn					TSyExpatStatus GetParsingStatus(TBool* aFinal = NULL) const
				Intended Usage:		Returns the current state of the parser. The flag indicating
									if this is the final buffer can optionally be returned
				@exception			none.
				@param				aFinal optional parameter to return whether the parser is parsing
									the final block. If this parameter is NULL then the final block status
									will not be returned.
				@return				The current parse status.
				@pre 				None
				@post				Nothing is left on the CleanupStack
			 */
			IMPORT_C TSyExpatStatus GetParsingStatus(TBool* aFinal = NULL) const;
		  
			/**
				@fn					void SetHandler(MSyDocHandler* aHandler)
				Intended Usage:		Sets a new document handler for the parser
									This will replace the existing handler
				@exception			KErrArgument if the handler is NULL
				@param				aHandler paramater with the new document handler which replaces
									the existing handler
				@return				none.
				@pre 				none
				@post				Nothing is left on the CleanupStack. 
			 */
			IMPORT_C void SetHandler(MSyDocHandler* aHandler);	  

			/**
				@fn					void SetHandler(MSyDeclHandler* aHandler)
				Intended Usage:		Sets a new declaration handler for the parser
									This will replace the existing handler
				@exception			KErrArgument if the handler is NULL
				@param				aHandler paramater with the new document handler which replaces
									the existing handler
				@return				none.
				@pre 				none
				@post				Nothing is left on the CleanupStack. 
			 */
			IMPORT_C void SetHandler(MSyDeclHandler* aHandler);
			
			/**
				@fn					void  SetBase(const TDesC& aBase);
				Intended Usage:		Sets the base to be used for resolving relative URIs in system
   									identifiers in declarations.  Resolving relative identifiers is
   									left to the application: this value will be passed through as the
   									base argument to the Notation method.
				@exception			KErrMemory
				@param				aBase paramater with the new base URI
				@return				none.
				@pre 				none
				@post				Nothing is left on the CleanupStack. 
			 */

			IMPORT_C void  SetBase(const TDesC& aBase);

			/**
				@fn					TPtrC GetBase() const
				Intended Usage:		Gets the currently specified base entity URI that was set with
									the previous SetBase call.
				@exception			none

				@return				pointer to a string holding the base URI.
				@pre 				none
				@post				Nothing is left on the CleanupStack. 
			 */
			IMPORT_C TPtrC GetBase() const; 

			/**
				@fn					static const TVersion VersionInfo() const
				Intended Usage:		Returns the build version of the DLL.
				@exception			none.
				@return				TVersion with the versioning information of the DLL.
				@pre 				None
				@post				Nothing is left on the CleanupStack
			 */
			IMPORT_C const TVersion VersionInfo() const;
			
		protected:
		
			/**
				@fn					CExpatParserBase()
				Intended Usage	:	Default constructor
				@warning			Not exported as this class is not designed
									to be extended.
		 	*/
			EXPORT_C CExpatParserBase();
			
			/**
				@fn					void BaseConstructL(const TDesC& aEncoding)
				Intended Usage	:	Second phase of safe construction method
				@param				aEncoding the default encoding of for the data to be parsed. Will 
									override the encoding in the xml declaration line
				@return				none.
				@pre 				First phase of construction is complete.
				@post				Object if fully constructed.
		 	*/
		 	EXPORT_C void BaseConstructL(const TDesC& aEncoding);

			/**
				@fn					void Panic(const TInt aPanic)
				Intended Usage	:	Helper function to generate a panic. Note that
									this function is not expected to return as it always
									leaves.
				@param				aPanic the panic code for the error
				@return				none
				@pre 				First phase of construction is complete.
				@post				Never returns, only leaves!.
		 	*/
			EXPORT_C static void Panic(const TInt aPanic);
			

			
		private:
			/** Pointer to the parser object*/ 
			TAny*	iParser;

		protected:
		
		public:
		
	};

	/**
	 * @class CSyParser.
	 * This is a specialization of the parser class to provide generic handling of parsing.
	 * This provides a wrapper over the basic parser class that allows specialization of
	 * the parser into a concrete form. This is the class that should be used when a parser
	 * is required. 
	 */
	class CSyParser : public CExpatParserBase
	{
	public:
	
		/**
			@fn					IMPORT_C static CSyParser* NewL(const TDesC& aEncoding)
			Intended Usage:		Standardized safe construction which leaves nothing on the cleanup stack.
								Constructs a new parser and namespace processor.  Element type
								names and attribute names that belong to a namespace will be
								expanded; unprefixed attribute names are never expanded; unprefixed
								element type names are expanded only if there is a default
								namespace. The expanded name is the concatenation of the namespace
								URI, the namespace separator character, and the local part of the
								name.  If the namespace separator is a null char then the namespace URI
								and the local part will be concatenated without any separator.
								When a namespace is not declared, the name and prefix will be
								passed through without expansion.			
			@exception			KErrNoMemory.
			@param				aEncoding The encoding of the data to be parsed, this will override the definitions
								in the XML file or byte headers.
			@return				CExpatParserBase* A pointer to the newly created class.
			@pre 				None
			@post				Nothing is left on the CleanupStack
		 */
		IMPORT_C static CSyParser* NewL(const TDesC& aEncoding);

		/**
			@fn					IMPORT_C static CSyParser* NewLC(const TDesC& aEncoding)
			Intended Usage	:	Two phase constructor which leaves a CExpatParserBase* on the cleanup stack
								Constructs a new parser and namespace processor.  Element type
								names and attribute names that belong to a namespace will be
								expanded; unprefixed attribute names are never expanded; unprefixed
								element type names are expanded only if there is a default
								namespace. The expanded name is the concatenation of the namespace
								URI, the namespace separator character, and the local part of the
								name.  If the namespace separator is a null char then the namespace URI
								and the local part will be concatenated without any separator.
								When a namespace is not declared, the name and prefix will be
								passed through without expansion.			
			Error Condition	: 	Leave with an error code
			@exception			KErrNoMemory.
			@param				aEncoding The encoding of the data to be parsed, this will override the definitions
								in the XML file or byte headers.
			@return				CExpatParserBase* A pointer to the newly created class.
			@pre 				None
			@post				A CExpatParserBase is on the CleanupStack
		 */
		IMPORT_C static CSyParser* NewLC(const TDesC& aEncoding);

		/**
			@fn					virtual ~CSyParser()
			Intended Usage	:	Standard destructor
			@exception			none.			
			@return				none.
			@pre 				First phase of construction is complete.
			@post				none
	 	*/
		IMPORT_C virtual ~CSyParser();
	protected:
		/**
			@fn					void ConstructL(const TDesC& aEncoding);
			Intended Usage	:	Second phase constructor for the %CSyParser object.
			@exception			anything.			
			@param				aEncoding the encoding of the data to be parsed.
			@return				none.
			@pre 				First phase of construction is complete.
			@post				none
	 	*/
		void ConstructL(const TDesC& aEncoding);
					
	};
	
	/**
	 * @class CSyContextualParser.
	 * This class is used to build a parser stack when parsing. Each time the document handler
	 * needs to be changed in the middle of parsing, the current handler can be put onto the 
	 * stack and a new handler replace the current handler. When the new handler has finished,
	 * the handler can be popped off the stack and the old handler will replace the existing handler
	 * This makes it easier to map elements to structs via handlers.
	 */
	class CSyContextualParser : public CExpatParserBase
	{
		public:
			/**
				@fn					IMPORT_C static CSyContextualParser* NewL(const TDesC& aEncoding)
				Intended Usage:		Standardized safe construction which leaves nothing on the cleanup stack.
									Constructs a new parser and namespace processor.  Element type
									names and attribute names that belong to a namespace will be
									expanded; unprefixed attribute names are never expanded; unprefixed
									element type names are expanded only if there is a default
									namespace. The expanded name is the concatenation of the namespace
									URI, the namespace separator character, and the local part of the
									name.  If the namespace separator is a null char then the namespace URI
									and the local part will be concatenated without any separator.
									When a namespace is not declared, the name and prefix will be
									passed through without expansion.			
				@exception			KErrNoMemory.

				@param				aEncoding The encoding of the data to be parsed, this will override the definitions
									in the XML file or byte headers.
				@return				CSyContextualParser* A pointer to the newly created class.
				@pre 				None
				@post				Nothing is left on the CleanupStack
			 */
			IMPORT_C static CSyContextualParser* NewL(const TDesC& aEncoding);

			/**
				@fn					IMPORT_C static CSyContextualParser* NewLC(const TDesC& aEncoding)
				Intended Usage	:	Two phase constructor which leaves a CSyContextualParser* on the cleanup stack
									Constructs a new parser and namespace processor.  Element type
									names and attribute names that belong to a namespace will be
									expanded; unprefixed attribute names are never expanded; unprefixed
									element type names are expanded only if there is a default
									namespace. The expanded name is the concatenation of the namespace
									URI, the namespace separator character, and the local part of the
									name.  If the namespace separator is a null char then the namespace URI
									and the local part will be concatenated without any separator.
									When a namespace is not declared, the name and prefix will be
									passed through without expansion.			
				Error Condition	: 	Leave with an error code
				@exception			KErrNoMemory.
				@param				aEncoding The encoding of the data to be parsed, this will override the definitions
									in the XML file or byte headers.
				@return				CSyContextualParser* A pointer to the newly created class.
				@pre 				None
				@post				A CExpatParserBase is on the CleanupStack
			 */
			IMPORT_C static CSyContextualParser* NewLC(const TDesC& aEncoding);

			/**
				@fn					virtual ~CSyContextualParser()
				Intended Usage	:	This is the standard destructor and it empties the internal
									context stacks
				@exception			none.			
				@return				none.
				@pre 				First phase of construction is complete.
				@post				none
	 		*/
			IMPORT_C virtual ~CSyContextualParser();
			
			/**
				@fn					void PushDocHandler(MSyDocHandler* aHandler)
				Intended Usage	:	Pushes the current document handler onto the internal
									stack so that the existing handler can be replaced with this
									new handler.
				@exception			KErrArguement if the handler is NULL.			
				@param				aHandler the new handler to replace the existing handler
				@return				none.
				@pre 				First phase of construction is complete.
				@post				none
	 		*/
			IMPORT_C void PushDocHandler(MSyDocHandler* aHandler);

			/**
				@fn					void PushDeclHandler(MSyDeclHandler* aHandler)
				Intended Usage	:	Pushes the current declaration handler onto the internal
									stack so that the existing handler can be replaced with this
									new handler
				@exception			KErrArguement if the handler is NULL.			
				@param				aHandler the new declaration handler to replace the existing handler
				@return				none.
				@pre 				First phase of construction is complete.
				@post				none
	 		*/
			IMPORT_C void PushDeclHandler(MSyDeclHandler* aHandler);

			/**
				@fn					MSyDocHandler* PopDocHandler()
				Intended Usage	:	Replaces the existing handler with the previous handler that was
									placed on the internal handler stack.
				@exception			KErrUnderflow if the document handler stack is empty.			
				@return				The document handler that was being used and has now been replaced.
				@pre 				First phase of construction is complete.
				@post				The document handler will have been replaced with the handler 
									on the internal stack.
	 		*/
			IMPORT_C MSyDocHandler* PopDocHandler();

			/**
				@fn					MSyDocHandler* PopDeclHandler()
				Intended Usage	:	Replaces the existing handler with the previous handler that was
									placed on the internal handler stack.
				@exception			KErrUnderflow if the declaration handler stack is empty.			
				@return				The declaration handler that was being used and has now been 
									replaced.
				@pre 				First phase of construction is complete.
				@post				The declaration handler will have been replaced with the handler 
									on the internal stack.
	 		*/
			IMPORT_C MSyDeclHandler* PopDeclHandler();
			
		protected:
			/**
				@fn					void ConstructL(const TDesC& aEncoding);
				Intended Usage	:	Second phase constructor for the %CSyContextualParser object.
				@exception			anything.			
				@param				aEncoding the encoding of the data to be parsed.
				@return				none.
				@pre 				First phase of construction is complete.
				@post				none
	 		*/
			void ConstructL(const TDesC& aEncoding);

		private:
			/**
			 * The stack of document handlers.
			 */
			RArray<MSyDocHandler*>	iDocHandlerStack;
			
			/**
			 * the stack of declaration handlers.
			 */
			RArray<MSyDeclHandler*>	iDeclHandlerStack;	
	};
	
#endif // __SYMBIAN_EXPAT_H
