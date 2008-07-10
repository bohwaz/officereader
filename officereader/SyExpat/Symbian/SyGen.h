/** XML Generator for Symbian OS
	Copyright © 2004 ToddSoftware.com. All rights reserved.
	Copyright (c) 1998, 1999, 2000 Thai Open Source Software Center Ltd
    and Clark Cooper
	Copyright (c) 2001, 2002, 2003 Expat maintainers.

	Permission is hereby granted, free of charge, to any person obtaining
	a copy of this software and associated documentation files (the
	"Software"), to deal in the Software without restriction, including
	without limitation the rights to use, copy, modify, merge, publish,
	distribute, sublicense, and/or sell copies of the Software, and to
	permit persons to whom the Software is furnished to do so, subject to
	the following conditions:

	The above copyright notice and this permission notice shall be included
	in all copies or substantial portions of the Software.

	THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
	EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
	MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.
	IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY
	CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT,
	TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE
	SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
*/

/** @file SyGen.h
	This file contains the definition of the %CSyGeneratorBase and its associated 
	typedefs and constants.
	This is the main definition of the XML generator and its methods.
*/
#ifndef __SYGEN_H
#define __SYGEN_H

#include <e32base.h>

	/**
	 *	@class MSyOutputHandler.
		Mixin interface class for handling callbacks to write some data.
	 */
	class MSyOutputHandler
	{
		public:
		
		/**
			@fn					virtual void WriteL(const TDesC& aString, const TBool aIsRawData)
			Intended Usage:		This interface is intended to be aggregated onto another
								object and so the WriteL will transcode and save aString
								to the relevant output stream in the correct format.
			@exception			KErrNoMemory.
			@param				aString the text to be written out.
			@param				aIsRawData True if this is raw data, i.e. not part of an element
								definition.
			@return				none
			@pre 				none
			@post				none
		 */
		virtual void WriteL(const TDesC& aString, const TBool aIsRawData) = 0;
	};

	/**
		@class CSyGeneratorBase
		This class is intended to be derived from by the user adding functionality.
		The class will extend the basic functionality in order to support more efficient
		Generation of XML data.
	 */
	class CSyGeneratorBase : public CBase
	{
		public:
			
			/**
				@fn					~CSyGeneratorBase()
				Intended Usage	:	Standard destructor
		 	*/
			IMPORT_C ~CSyGeneratorBase();

		/**
			@fn					void StartDocumentL(const TDesC& aEncoding, const TBool aStandalone, const TDesC& aPublicId, const TDesC& aSystemId)
			Intended Usage:		Generates the XML header and sends it to the output handler.
			@exception			KErrNoMemory if no memory for buffer
								ELevelUnderRun if startdoc already called
			@param				aEncoding the document encoding.
			@param				aStandalone whether the document is standalone. i.e. it does not have an external DTD.
			@param				aPublicId the public id of the document.
			@param				aSystemId the system id of the document.
			@return				none
			@pre 				None
			@post				Nothing is left on the CleanupStack
		 */
			IMPORT_C void StartDocumentL(const TDesC& aEncoding, const TBool aStandalone, const TDesC& aPublicId, const TDesC& aSystemId);

		/**
			@fn					void StartDocumentL(const TBool aStandalone)
			Intended Usage:		Generates the default document handler.
			@exception			KErrNoMemory if no memory for buffer
								ELevelUnderRun if startdoc already called
			@param				aStandalone	indicates if the document is standalone.
			@return				none
			@pre 				None
			@post				Nothing is left on the CleanupStack
		 */
			IMPORT_C void StartDocumentL(const TBool aStandalone = ETrue);

		/**
			@fn					void EndDocumentL();
			Intended Usage:		Closes the XML document. If there are any pending unclosed
								elements then these will be flushed.
			@exception			KErrNoMemory if no memory for buffer
								ELevelUnderRun if startdoc already called
			@return				none
			@pre 				None
			@post				Nothing is left on the CleanupStack
		 */
			IMPORT_C void EndDocumentL();

		/**
			@fn					void StartFragmentL();
			Intended Usage:		Starts the xml fragment. i.e. no document header
			@exception			KErrNoMemory if no memory for buffer
								ELevelUnderRun if startdoc already called
			@return				none
			@pre 				None
			@post				Nothing is left on the CleanupStack
		 */
			IMPORT_C void StartFragmentL();
			
		/**
			@fn					void EndFragmentL();
			Intended Usage:		Closes the XML fragment.
			@exception			KErrNoMemory if no memory for buffer
								ELevelUnderRun if startdoc already called
			@return				none
			@pre 				None
			@post				Nothing is left on the CleanupStack
		 */
			IMPORT_C void EndFragmentL();

		/**
			@fn					void StartElementL(const TDesC& aName)
			Intended Usage:		Starts an element that does not have a prefix.
			@exception			KErrNoMemory if no memory for buffer.
			@param				aName the name of the element.
			@return				none
			@pre 				None
			@post				The name is added to the context stack so must be kept until its respective
								EndElementL function is called.
		 */
			IMPORT_C void StartElementL(const TDesC& aName);

		/**
			@fn					void StartElementL(const TDesC& aPrefix, const TDesC& aName)
			Intended Usage:		Starts an element that has a prefix. Call SetNamespaceL and SetAttributeL
								functions to add attributs and namespaces to the element
			@exception			KErrNoMemory if no memory for buffer.
			@param				aPrefix the namespace prefix of the element.
			@param				aName the name of the element.
			@return				none
			@pre 				None
			@post				The name is added to the context stack so must be kept until its respective
								EndElementL function is called.
		 */
			IMPORT_C void StartElementL(const TDesC& aPrefix, const TDesC& aName);

		/**
			@fn					void EmptyElementL(const TDesC& aName)
			Intended Usage:		Starts an element that is an empty element.
			@exception			KErrNoMemory if no memory for buffer.
			@param				aName the name of the element.
			@return				none
			@pre 				None
			@post				The name is added to the context stack so must be kept until its respective
								EndElementL function is called.
		 */
			IMPORT_C void EmptyElementL(const TDesC& aName);

		/**
			@fn					void EmptyElementL(const TDesC& aPrefix, const TDesC& aName)
			Intended Usage:		Creates and empty element. Call SetNamespaceL and SetAttributeL
								functions to add attributs and namespaces to the element.
			@exception			KErrNoMemory if no memory for buffer.
			@param				aPrefix the namespace prefix of the element.
			@param				aName the name of the element.
			@return				none
			@pre 				None
			@post				The name is added to the context stack so must be kept until its respective
								EndElementL function is called.
		 */
			IMPORT_C void EmptyElementL(const TDesC& aPrefix, const TDesC& aName);

		/**
			@fn					void SetNamespaceL(const TDesC& aURI)
			Intended Usage:		Declares a namespace within an element definition. This is used to set
								the default namespace for the element.
			@exception			KErrNoMemory if no memory for buffer.
			@param				aURI the URI of the namespace to be set as the default namespace.
			@return				none
			@pre 				StartElement must have been called, but not EndElement
			@post				this default namespace for this and all child elements will now be 
								this URI
		 */
			IMPORT_C void SetNamespaceL(const TDesC& aURI);

		/**
			@fn					void SetNamespaceL(const TDesC& aPrefix, const TDesC& aURI)
			Intended Usage:		Assigns a prefix to a a namespace within an element definition.
			@exception			KErrNoMemory if no memory for buffer.
			@param				aPrefix the prefix to be mapped to the specified URI
			@param				aURI the URI of the namespace to be set as the default namespace.
			@return				none
			@pre 				StartElement must have been called, but not EndElement
			@post				none
		 */
			IMPORT_C void SetNamespaceL(const TDesC& aPrefix, const TDesC& aURI);

		/**
			@fn					void SetAttributeL(const TDesC& aName, const TDesC& aValue)
			Intended Usage:		Assigns an attribute to an element. This form is used to
								write attributes with no namespace.
			@exception			KErrNoMemory if no memory for buffer.
			@param				aName the name of the attribute.
			@param				aValue the value of the attribute.
			@return				none
			@pre 				StartElement must have been called, but not EndElement
			@post				none
		 */
			IMPORT_C void SetAttributeL(const TDesC& aName, const TDesC& aValue);

		/**
			@fn					void SetAttributeL(const TDesC& aPrefix, const TDesC& aName, const TDesC& aValue)
			Intended Usage:		Assigns an attribute to an element. This form is the default form and
								write attributes with a namespace. If aPrefix is empty then the attribute name
								only will be written out.
			@exception			KErrNoMemory if no memory for buffer.
			@param				aPrefix the namespace prefix of the attribute.
			@param				aName the name of the attribute.
			@param				aValue the value of the attribute
			@return				none
			@pre 				StartElement must have been called, but not EndElement
			@post				none
		 */
			IMPORT_C void SetAttributeL(const TDesC& aPrefix, const TDesC& aName, const TDesC& aValue);

		/**
			@fn					void EndElementL();
			Intended Usage:		Writes out the end element text for the current element. If the current
								element is empty then it just closes the element
			@exception			KErrNoMemory if no memory for buffer.
			@return				none
			@pre 				StartElement must have been called
			@post				none
		 */
			IMPORT_C void EndElementL();

		/**
			@fn					void CharactersL(const TDesC& aCharacters)
			Intended Usage:		Writes out string to the output buffer. This is used to write out element
								contents and the data within a CDATA section.
								This can be called multiple times
			@exception			KErrNoMemory if no memory for buffer.
			@param				aCharacters the character data to be written out
			@return				none
			@pre 				StartElement must have been called
			@post				none
		 */
			IMPORT_C void CharactersL(const TDesC& aCharacters);

		/**
			@fn					void IgnorableWhitespaceL(const TDesC& aCharacters);
			Intended Usage:		Writes out string to the output buffer. This is used to write
								out data between elements that is in effect whitespace
								This can be called multiple times to write out strings
			@exception			KErrNoMemory if no memory for buffer.
			@param				aCharacters the character data to be written out
			@return				none
			@pre 				StartElement must have been called
			@post				none
		 */
			IMPORT_C void IgnorableWhitespaceL(const TDesC& aCharacters);

		/**
			@fn					void ProcessingInstructionL(const TDesC& aTarget, const TDesC& aData)
			Intended Usage:		Writes out string a processing instruction to the xml writer.
			@exception			KErrNoMemory if no memory for buffer.
			@param				aTarget the target of the processing instruction
			@param				aData the data for the processing instruction.
			@return				none
			@pre 				none
			@post				none
		 */
			IMPORT_C void ProcessingInstructionL(const TDesC& aTarget, const TDesC& aData);

		/**
			@fn					void CommentL(const TDesC& aComment)
			Intended Usage:		Writes out string a comment to the xml writer.
			@exception			KErrNoMemory if no memory for buffer.
			@param				aComment the comment that is to be written out
			@return				none
			@pre 				none
			@post				none
		 */
			IMPORT_C void CommentL(const TDesC& aComment);

		/**
			@fn					void StartCDATAL()
			Intended Usage:		Starts a CDATA Section. This is used to start the CDATA Section.
								Use CharactersL to write out the CData data.
								block. 
			@exception			KErrNoMemory if no memory for buffer.
			@return				none
			@pre 				A CDATA Section  must not already be open and an element must be open
			@post				none
		 */
			IMPORT_C void StartCDATAL();

		/**
			@fn					void EndCDATAL()
			Intended Usage:		Ends a CDATA Section. This is used to end the current CDataSection
			@exception			KErrNoMemory if no memory for buffer.
			@return				none
			@pre 				A CDATA Section  must be already be open
			@post				none
		 */
			IMPORT_C void EndCDATAL();

		/**
			@fn					const TVersion VersionInfo() const
			Intended Usage:		This should be used to get the version number of the XMLWriter.
			@exception			KErrNoMemory if no memory for buffer.
			@return				The version structure with the version info of the writer
			@pre 				none
			@post				none
		 */
			IMPORT_C const TVersion VersionInfo() const;
	protected:
			/**
				@fn					CSyGeneratorBase(MSyOutputHandler& aOutput)
				Intended Usage	:	Default constructor
				@param				aOutput the mixin interface that handles transcoding and serialization 
				@warning			Not exported as this class is not designed
									to be extended.
		 	*/
			IMPORT_C CSyGeneratorBase(MSyOutputHandler& aOutput);

			/**
				@fn					void BaseConstructL(const TInt aBufferSize)
				Intended Usage	:	Second phase of safe construction method.
									It is intended that this function be called from the
									ConstructL of dervied classes
				@param				aBufferSize the intial size of the buffer.
				@pre 				First phase of construction is complete.
				@post				Object if fully constructed.
		 	*/
			IMPORT_C void BaseConstructL(const TInt aBufferSize);
		
			/**
				@fn					void Panic(const TInt aPanic)
				Intended Usage	:	Helper function to generate a panic
				@param				aPanic the panic code for the error
				@pre 				First phase of construction is complete.
				@post				Never returns, only leaves!.
		 	*/
			IMPORT_C static void Panic(const TInt aPanic);

	private:
			/**
				@fn					void StartElement2L(const TDesC& aPrefix, const TDesC& aName, const TBool aEmptyElement)
				Intended Usage	:	This creates a new generic start element.
				@param				aPrefix the prefix of the element.
				@param				aName the name of the element.
				@param				aEmptyElement the element is empty.
				@return				none
				@pre 				none
				@post				none
		 	*/
			void StartElement2L(const TDesC& aPrefix, const TDesC& aName, const TBool aEmptyElement);
			
			/**
				@fn					TPtr GetBufferL(const TInt aNewLength)
				Intended Usage	:	Helper function to allocate a temporary working 
									buffer. The contents of the existing buffer are deleted!
									Note that by default this buffer is 1024 bytes which should
									provide a sufficient buffer for all but the most complex 
									element definitions with all the namespaces included.
									Note that characters are passed through unbuffered
				@param				aNewLength the new size of the buffer.
				@return				TPtr to the newly allocated buffer.
				@pre 				none
				@post				none
		 	*/
			TPtr GetBufferL(const TInt aNewLength = -1);

			/**
				@fn					TPtr ExpandBufferL(const TInt aExpandBy)
				Intended Usage	:	Helper function to expand the current buffer by the specified
									amount - the existing data is preserved
				@param				aExpandBy the number of characters to expand the buffer by.
				@return				none
				@pre 				A buffer must exist
				@post				none
		 	*/
			TPtr ExpandBufferL(const TInt aExpandBy);
			
			/**
				@fn					void FinishElementDeclarationIfNeeded()
				Intended Usage	:	If the last element was not terminated because for example we were adding
									namespaces or attrbiutes then we need to add a ">" character to the
									end of the output
				@return				none
				@pre 				none
				@post				none
		 	*/
			void FinishElementDeclarationIfNeeded();
			
	private:
		/** The instance of the mixin interface where data will be output to */
		MSyOutputHandler&					iOutput;
		
		/** The current depth of the element stack */
		TInt							iDepth;
		
		/** The temporary buffer used to hold the current line of text being output */
		HBufC*							iBuffer;

		/** Flag indicating if the generator is in a CData section or not */
		TBool							iInCDATASection;
		
		/** Flag indicating if the generator needs to close the previous element.
			This is used in the case where the current element needs to have
			Attribute or Namespaces.
		 */
		TBool							iNeedClosingBrace;

		/**
		 * @struct TXMLElement
		 * The temporary structure to hold a parsing context.
		 * If both name is empty then the generator is in the context of
		 * an empty element.
		 */
		struct TXMLElement
		{
			/**
			 * Constructor for the element item to go on the stack.
			 */
			TXMLElement(const TDesC& aPrefix, const TDesC& aName);
			
			/**
			 * The prefix of the element. This is optionally empty
			 */
			TPtrC iPrefix;
			
			/**
			 * The name of the element
			 */
			TPtrC iName;
		};

		/** The array of nested elements being generated */		
		RArray<TXMLElement>				iContextStack;
};

	/**
		@class CSyGenerator
		Simple C++ class for generating XML data, rather than using hard coded strings.
		In general as this class is dealing with memory, all the functions will leave if 
		an error is encountered allocating memory.
	 */
class CSyGenerator : public CSyGeneratorBase
{
public:
		/**
			@fn					~CSyGenerator()
			Intended Usage	:	Standard destructor
	 	*/
		IMPORT_C virtual ~CSyGenerator();
		
		/**
			@fn					CSyGeneratorBase* NewLC(MSyOutputHandler& aOutput, const TInt aBufferSize)
			Intended Usage:		Standardized safe construction which leaves the instance of the generator on
								the cleanup stack.
								Constructs a new generator to generate the XML. The generator will need to 
								allocate an intermediate buffer internally, the intial size of which can be 
								configured. The Characters function will not use this buffer, but pass the data
								straight through to the output mixin class.
			@exception			KErrNoMemory.
			@param				aOutput the output buffer
			@param				aBufferSize the initial size of the output buffer
			@return				CSyGeneratorBase* A pointer to the newly created class.
			@pre 				None
			@post				An instance of the parser is left on the CleanupStack
		 */
			static IMPORT_C CSyGenerator* NewLC(MSyOutputHandler& aOutput, const TInt aBufferSize = 1024);

		/**
			@fn					CSyGeneratorBase* NewL(MSyOutputHandler& aOutput, const TInt aBufferSize)
			Intended Usage:		Standardized safe construction which leaves nothing on the cleanup stack.
								Constructs a new generator to generate the XML. The generator will need to 
								allocate an intermediate buffer internally, the intial size of which can be 
								configured. The Characters function will not use this buffer, but pass the data
								straight through to the output mixin class.
			@exception			KErrNoMemory.
			@param				aOutput the output buffer
			@param				aBufferSize the initial size of the output buffer
			@return				CSyGeneratorBase* A pointer to the newly created class.
			@pre 				None
			@post				Nothing is left on the CleanupStack
		 */
			static IMPORT_C CSyGenerator* NewL(MSyOutputHandler& aOutput, const TInt aBufferSize = 1024);	
	
protected:
		/**
			@fn					CSyGenerator(MSyOutputHandler& aOutput)
			Intended Usage:		Standardized constructor for a generator object
			@exception			none
			@param				aOutput the output mixin interface
			@return				None.
			@pre 				None.
			@post				Nothing is left on the CleanupStack
		 */
		CSyGenerator(MSyOutputHandler& aOutput);
			/**
				@fn					void ConstructL(const TInt aBufferSize)
				Intended Usage	:	Second phase of safe construction method.
									Just calls BaseConstructL
				@param				aBufferSize the intial size of the buffer.
				@pre 				First phase of construction is complete.
				@post				Object if fully constructed.
		 	*/
		void ConstructL(const TInt aBufferSize);
};
#endif // __SYGEN_H
