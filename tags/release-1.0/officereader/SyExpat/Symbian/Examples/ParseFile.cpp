/*****************************************************************
 * ParseFile.cpp
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
 * Read an XML document filename from the console and then verify it is 
 * valid and well-formed.
 *
 *  You will need to replace the filename specified in KTxtFilename
 * by default this resides in the root of the symbian c driver
 * normally {EPOC}\epoc32\winscw\c
 *
 * Almost all the guts of the system revolve around the callParseFile
 * function which does everything important. The rest is just plumbing
 */
#include <e32base.h>
#include <SyExpat.h>
#include <f32file.h>
#include <e32test.h>
#include "XMLHelper.h"

// String constants
_LIT(KTxtEPOC32EX,"OUTLINE");
_LIT(KTxtExampleCode,"Expat Example Code");
_LIT(KFormatFailed,"failed: leave code=%d");
_LIT(KTxtOK,"ok");
_LIT(KTxtPressAnyKey," [press any key]");

// TODO: Insert your filename here
// put this file into %EPOCROOT%\epoc32\winscw\c
_LIT(KTxtFilename, "doGoogleSearchResponse.xml");

LOCAL_D RTest test(_L("Text XML parse on file"));
LOCAL_D CTrapCleanup* theCleanup;

LOCAL_C void callParseFile(RFs& fs, const TDesC& aXMLFilename)
{
	// 1. try to open the file containing the xml and if it 
	// was opened successfully put it on the cleanup stack 
	// with a close
	RFile			file;
	User::LeaveIfError(file.Open(fs, aXMLFilename, EFileStream));
	CleanupClosePushL(file);
	
	
	// 2. Create the XML Parser	and set the default document handlers
	CSyParser*   reader = CSyParser::NewLC(TPtrC());		

	CDocumentHandler docHandler(test);
	reader->SetHandler(&docHandler);

	CDeclarationHandler declHandler(test);
	reader->SetHandler(&declHandler);
	
	// 3. We set up the buffer with a default size of 128 bytes.
	//    This buffer is set purposly small so that we execute the
	//    parse stuff as often as possible to show aynchronous parsing
	//    We alse set up some loop variables
	const int KMaxBufLen = 128;
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
		User::LeaveIfError(file.Read(buf, KMaxBufLen));

		// 5. Next we attempt to parse the result buffer
		//    or if we reached the end of the file the 
		//    partial buffer
		isFinal = (buf.Length() != KMaxBufLen);
		status = reader->Parse(buf, isFinal);
		
		// 6. When the parser returns an error
		//    we log it 
		//    Otherwise if there is no more data
		//    then we report we have finished
		if (status != EStatusOk)
		{
			_LIT(KError,"Error on Line %d Column %d\n");
			test.Printf(KError, reader->GetCurrentLineNumber(), reader->GetCurrentColumnNumber());
		}
		else
		{
			if (isFinal)
			{
				_LIT(KFinished, "Finished parsing file\n");
				test.Printf(KFinished);
			}
		}
		
		// 7. finally we see if we have to loop round
		//    any more - we stop if we find an error or
		//    we have reached the end of the file
	} while (status == EStatusOk && !isFinal);

	// 8.  Finally delete the parser
	//     and close the file handle
	CleanupStack::PopAndDestroy(2);
}

LOCAL_C void doExampleL()
{
	// Create a session to the server
	RFs    fs;
	User::LeaveIfError(fs.Connect());
	CleanupClosePushL(fs);

	TParse filename;
	User::LeaveIfError(fs.Parse(KTxtFilename, filename));
	
	callParseFile(fs, filename.FullName());	
	
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
	test.Start(_L("ParseFile"));
	test.Title();

	theCleanup=CTrapCleanup::New();

	TRAPD(ret,callExampleL());	

	delete theCleanup;	

	test.End();

	test.Close();

	__UHEAP_MARKEND;

	return(KErrNone);
}

