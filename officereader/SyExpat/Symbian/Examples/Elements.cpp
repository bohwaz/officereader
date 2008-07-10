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
#include "SyExpat.h"
#include <e32test.h>
#include "xmlhelper.h"

// String constants
_LIT(KTxtEPOC32EX,"EXAMPLES");
_LIT(KTxtExampleCode,"Expat Elements");

_LIT(KTxtParsedOk, "XML Successfully parser");
_LIT(KTxtParsedFailedOn, "XML Failed on Line %d Column %d");

_LIT(KTxtFailed, "Failed parsing");
_LIT(KTxtOk, "Ok");

_LIT(KTxtPressAnyKey," [press any key]");

LOCAL_D RTest test(_L("XML String"));

LOCAL_D CTrapCleanup* theCleanup;


LOCAL_C void doExampleL()
{
	
	// 1. Create the XML Parser	and set the default document handler	
	CDocumentHandler	xmlOutput(test);
	CDeclarationHandler	xmlOutput2(test);
	
#define _CONTEXTUAL_PARSER
#ifdef _CONTEXTUAL_PARSER
	CSyContextualParser* reader = CSyContextualParser::NewLC(TPtrC());
	reader->PushDocHandler(&xmlOutput);
	reader->PushDeclHandler(&xmlOutput2);
#else
	CSyParser* reader = CSyParser::NewLC(TPtrC());
	reader->SetHandler(&xmlOutput);
	reader->SetHandler(&xmlOutput2);
#endif

	
	_LIT8(KDocument, "<?xml version=\"1.0\" standalone=\"yes\"?>"
					 "<test>"
					 "   <elem1>"
					 "     <elem11>"
						      "cdata11"
					 "     </elem11>"
					 "     <elem12>"
						    "cdata12"
					 "     </elem12>"
					 "   </elem1>"
					 "<!-- This is a comment -->"
					 "   <elem2 attr=\"test\" attr2=\"test2\" attr3=\"test3\"/>"
					 "   <elem3 attr=\"test3\"/>"
					 "   <elem4>"
					       "text"
					 "   </elem4>"
					 "</test>");

	TPtrC8 doc(KDocument);
	TSyExpatStatus status = reader->Parse(doc, ETrue);
	if (status == EStatusOk)
	{
		test.Printf(KTxtParsedOk);
	}
	else
	{
		test.Printf(KTxtParsedFailedOn, reader->GetCurrentLineNumber(), reader->GetCurrentColumnNumber());
	}
	
#ifdef _CONTEXTUAL_PARSER
	reader->PopDocHandler();
	reader->PopDeclHandler();
#endif

	// finally clean up the parser and return
	CleanupStack::PopAndDestroy();
}

LOCAL_C void callExampleL() // initialize and call example code under cleanup stack
{
	TRAPD(error,doExampleL()); // perform example function
	if (error)
		test.Printf(KTxtFailed, error);
	else
		test.Printf(KTxtOk);
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


