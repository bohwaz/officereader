/*****************************************************************
 * Elements.cpp
 *
 * Copyright 1999, Clark Cooper, 2004 ToddSoftware
 * All rights reserved.
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the license contained in the
 * COPYING file that comes with the expat distribution.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
 * EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
 * MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.
 * IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY
 * CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT,
 * TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE
 * SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
 *
 * Read an XML document that is stored in a string and display the results
 *
 * Almost all the guts of the system revolve around the doExampleL()
 * function which does everything important. 
 * The rest is just plumbing
 */
#include <e32base.h>
#include <SyGen.h>
#include <e32test.h>

// String constants
_LIT(KTxtEPOC32EX,"EXAMPLES");
_LIT(KTxtExampleCode,"Expat Elements");

_LIT(KFormatFailed, "Failed to generate XML\n");
_LIT(KTxtOK, "Successfully generated XML\n");
_LIT(KTxtPressAnyKey," [press any key]\n");

LOCAL_D RTest test(_L("XML String"));

LOCAL_D CTrapCleanup* theCleanup;


class CTestOutput : public CBase, public MSyOutputHandler
{
public:
	virtual void WriteL(const TDesC& aString, const TBool aIsRawData)
	{
		TPtrC ptr(aString);
		
		_LIT(KFormat, "%S");
		test.Printf(KFormat, &ptr);
		
		if (!aIsRawData)
		{
			_LIT(KNewLine, "\n");
			
			const TInt last = aString.Length()-1;
			
			if (aString[last-1] == TChar('/') && aString[last] == TChar('>'))
			{
				test.Printf(KNewLine);
			}
			else
			{			
				// if the start is "</" or "<!" or "<?" or the end is "/>" and it is not raw data
				// then print a new line 
				if (aString[0] == TChar('<'))
				{
					
					if (aString[1] == TChar('/') || aString[1] == TChar('!') || aString[1] == TChar('?'))
					{
						test.Printf(KNewLine);
					}
				}
			}
		}
	}
};

_LIT(KNoPrefix, "");

_LIT(KElementDoSearch,      "doGoogleSearch");
_LIT(KElementBody,			"body");
_LIT(KElementEnvelope,		"envelope");
_LIT(KElementKey, 			"key");
_LIT(KElementQ,	 			"q");
_LIT(KElementStart,			"start");
_LIT(KElementMaxResults,	"maxResults");
_LIT(KElementFilter,        "filter");
_LIT(KElementRestrict,		"restrict");
_LIT(KElementSafeSearch,    "safesearch");
_LIT(KElementLR,			"lr");
_LIT(KElementIE,			"ie");
_LIT(KElementOE,			"oe");

_LIT(KSoapEnvelope,"SOAP-ENV");
_LIT(KSoapEnvelopeURI, "http://schemas.xmlsoap.org/soap/envelope/");

_LIT(KSoapSchema, "xsd");
_LIT(KSoapSchemaURI, "http://www.w3.org/1999/XMLSchema");

_LIT(KSoapSchemaInstance, "xsi");
_LIT(KSoapSchemaInstanceURI,"http://www.w3.org/1999/XMLSchema-instance");
 
_LIT(KNamespace1, "ns1");
_LIT(KNamespace1URI, "urn:GoogleSearch");

_LIT(KSoapEncodingStyle,  "encodingStyle");
_LIT(KSoapEncodingStyleVal, "http://schemas.xmlsoap.org/soap/encoding/");

_LIT(KSoapType, "type");
_LIT(KSoapTypeInt,    "xsd:int");
_LIT(KSoapTypeString, "xsd:string");
_LIT(KSoapTypeBool,   "xsd:bool");


_LIT(KGoogleKey, 	"0000000000000");
_LIT(KGoogleSearch, "bbc");
_LIT(KGoogleStart,  "0");
_LIT(KGoogleMaxResults, "10");
_LIT(KGoogleFilter, "true");
_LIT(KGoogleSafeSearch, "false");
_LIT(KGoogleLR,			"lr");
_LIT(KGoogleCodePage,	"latin1");

LOCAL_C void doExampleL()
{
	
	// 1. Create the XML Parser	and set the default document handler	
	CTestOutput	xmlOutput;
	CSyGenerator* writer = CSyGenerator::NewLC(xmlOutput);
	
	writer->StartDocumentL(ETrue);
		writer->StartElementL(KSoapEnvelope, KElementEnvelope);
		writer->SetNamespaceL(KSoapEnvelope, KSoapEnvelopeURI);
		writer->SetNamespaceL(KSoapSchema, KSoapSchemaURI);
		writer->SetNamespaceL(KSoapSchemaInstance, KSoapSchemaInstanceURI);
			
			writer->StartElementL(KSoapEnvelope, KElementBody);
				
				writer->StartElementL(KNamespace1, KElementDoSearch);
				writer->SetNamespaceL(KNamespace1, KNamespace1URI);
				writer->SetAttributeL(KSoapEnvelope, KSoapEncodingStyle, KSoapEncodingStyleVal);
					
					writer->StartElementL(KElementKey);
					writer->SetAttributeL(KSoapSchemaInstance, KSoapType, KSoapTypeString);
						writer->CharactersL(KGoogleKey);
					writer->EndElementL();
					
					writer->StartElementL(KElementQ);
					writer->SetAttributeL(KSoapSchemaInstance, KSoapType, KSoapTypeString);
						writer->CharactersL(KGoogleSearch);
					writer->EndElementL();
					
					writer->StartElementL(KElementStart);
					writer->SetAttributeL(KSoapSchemaInstance, KSoapType, KSoapTypeInt);
						writer->CharactersL(KGoogleStart);
					writer->EndElementL();
					
					writer->StartElementL(KElementMaxResults);
					writer->SetAttributeL(KSoapSchemaInstance, KSoapType, KSoapTypeInt);
						writer->CharactersL(KGoogleMaxResults);
					writer->EndElementL();
					
					writer->StartElementL(KElementFilter);
					writer->SetAttributeL(KSoapSchemaInstance, KSoapType, KSoapTypeBool);
						writer->CharactersL(KGoogleFilter);
					writer->EndElementL();
					
					writer->EmptyElementL(KElementRestrict);
					writer->SetAttributeL(KSoapSchemaInstance, KSoapType, KSoapTypeString);
					
					writer->StartElementL(KElementSafeSearch);
					writer->SetAttributeL(KSoapSchemaInstance, KSoapType, KSoapTypeString);
						writer->CharactersL(KGoogleSafeSearch);
					writer->EndElementL();
					
					writer->StartElementL(KElementLR);
					writer->SetAttributeL(KSoapSchemaInstance, KSoapType, KSoapTypeString);
					writer->EndElementL();
					
					writer->StartElementL(KElementIE);
					writer->SetAttributeL(KSoapSchemaInstance, KSoapType, KSoapTypeString);
						writer->CharactersL(KGoogleCodePage);					
					writer->EndElementL();
					
					writer->StartElementL(KElementOE);
					writer->SetAttributeL(KSoapSchemaInstance, KSoapType, KSoapTypeString);
						writer->CharactersL(KGoogleCodePage);					
					writer->EndElementL();
					
				writer->EndElementL();
			writer->EndElementL();
		writer->EndElementL();
	writer->EndDocumentL();
				
	// finally clean up the parser and return
	CleanupStack::PopAndDestroy();
}

LOCAL_C void callExampleL() // initialize and call example code under cleanup stack
{
	TRAPD(error,doExampleL()); // perform example function
	if (error)
		test.Printf(KFormatFailed, error);
	else
		test.Printf(KTxtOK);
	test.Printf(KTxtPressAnyKey);
	test.Getch(); // get and ignore character
}

GLDEF_C TInt E32Main() // main function called by E32
{
	__UHEAP_MARK;
	test.Start(_L("ParseXMLString"));
	test.Title();

	theCleanup=CTrapCleanup::New();

	TRAPD(ret,callExampleL());	

	delete theCleanup;	

	test.End();

	test.Close();

	__UHEAP_MARKEND;

	return(KErrNone);
}

