# Introduction #

Office Reader can be compiled just like any other Symbian OS project, there is nothing special about it. So if you are completly new to Symbian OS, reading some ["Getting Started"](http://wiki.forum.nokia.com/index.php/How_do_I_start_programming_for_Symbian_OS%3F) guides first will help you a lot. Please note that Symbian only support a Windows host for building, but there are some project to get this to run on Linux as well.

# Check out the source #

svn checkout http://officereader.googlecode.com/svn/trunk/ officereader-read-only

# Getting the right SDK #

Before you can compile Office Reader, you have to install the right SDK.

You have to download(free of charge) and install one of these two, or both of course, depending on your target platform:

  * S60 3rd Edition Maintanance Release: [Forum Nokia](http://www.forum.nokia.com/info/sw.nokia.com/id/4a7149a5-95a5-4726-913a-3c6f21eb65a5/S60-SDK-0616-3.0-mr.html)
  * UIQ3 3.0 [UIQ](http://developer.uiq.com/devtools_uiqsdk.html)

Please note: later SDK's such as S60 3rd Edition FP X or UIQ3 3.x will work as well, but these builds will then not work on all available phones


# Fix some SDK tools #

You have to fix some issues of the Symbian tooling:

http://www3.symbian.com/faq.nsf/AllByDate/E9DF3257FD565A658025733900805EA2?OpenDocument

http://developer.uiq.com/devtools_other.html#sdk_30_missing_libs (only if you are building for UIQ, this is not neccessary for S60 3rd Edition)

http://www3.symbian.com/faq.nsf/0/E9DF3257FD565A658025733900805EA2?OpenDocument

# Compiling by command line #

To compile the project, you have to run these two commands several times:

**bldmake bldfiles**

This will prepare the workspace based on target, then either run for emulator build

**ABLD.bat build winscw udeb**

or this one for target device:

**ABLD.bat build gcce urel**


These two commands have to be run in the following folders:

SyExpat/Symbian  // compile SyExpat parser

sheetcontrol/group // compile the sheet engine for ods files

officereader/Engine/group // compile the OpenDocument Engine

officereader/S60\_3rd/group // compile application for S60 3rd

or

officereader/UIQ3/group // compile application for UIQ3


In the top source directory there are also two bat files, which will run these commands for a target device build automatically. These can be easily adapted.

# Running the application #

If you have build for emulator (winscw), you can now simply issue the command

**epoc**

This will start the emulator, Office Reader can be found in the subfolder _Applications_

For installing the application on target device, you first have to package it into a sis file. Run this command either in \officereader\S60\_3rd\sis or \officereader\UIQ3\sis

**makesis MobileOffice.pkg**

After this, the created MobileOffice.sis file has to be signed just like any other Symbian application. This is explained very detailed in the SDK documentation, just look out for self-signing, as the application does not need any capabilities and does not need to be signed by another authority.

# IDE's #

Instead of compiling and packagin by command line, you can also use one of the available IDE's which will make your life a little bit easier:

  * Carbide.c++, all editions
  * Microsoft Visual Studio.NET 2003 with Carbide.vs 2.0 plugin
  * Microsoft Visual Studio 2005 with Carbide.vs 3.0 plugin

You just have to import the mmp files into the IDE and compile and build the project there. For additionl information, see the manuals of the corresponding IDEs.