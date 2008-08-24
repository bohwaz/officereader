echo "Compiling for S60 2nd Edition"
devices -setdefault @Series60_v20:com.nokia.series60

echo "Start compiling SyExpat XML parser"
cd SyExpat/Symbian
bldmake bldfiles
START ABLD.bat build armi urel
cd ../..
echo "Finished compiling SyExpat XML parser"

echo "Start compiling Sheet Control"
cd sheetcontrol/group
bldmake bldfiles
START ABLD.bat build armi urel
cd ../..
echo "Finished compiling Sheet Control

echo "Start compiling Engine"
cd officereader\Engine\group
bldmake bldfiles
START ABLD.bat build armi urel
cd ../../..
echo "Finished compiling Engine

echo "Finsihed compiling for S60 2nd Edition"