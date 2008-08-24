echo "Compiling for UIQ3"
devices -setdefault @UIQ3:com.symbian.UIQ

cd SyExpat/Symbian
bldmake bldfiles
START ABLD.bat build gcce urel
cd ../..
echo "Finished compiling SyExpat XML parser"

echo "Start compiling Sheet Control"

cd sheetcontrol/group
bldmake bldfiles
START ABLD.bat build gcce urel
cd ../..
echo "Finished compiling Sheet Control

echo "Start compiling Engine"
cd officereader\Engine\group
bldmake bldfiles
START ABLD.bat build gcce urel
cd ../../..
echo "Finished compiling Engine


echo "Finished for UIQ3"