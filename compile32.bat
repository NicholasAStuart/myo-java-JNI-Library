if not defined DevEnvDir (
	call "%VC%\vcvarsall.bat" x86
)

mkdir x86\Release\
mkdir x86\Objects\

cl /c /EHsc /O2 /MD /fp:precise /I"%MYO_HOME%\include" /I"%JAVA_HOME%\include" /I"%JAVA_HOME%\include\win32" /Fo"x86\Objects\\" /Fd"x86\Objects\vc100.pdb" com_thalmic_myo_Hub.cpp com_thalmic_myo_Myo.cpp handle.cpp jnidevicelistener.cpp

link /OUT:"x86\Release\JNIJavaMyoLib.dll" /LIBPATH:"%MYO_HOME%\lib" myo32.lib /MACHINE:x86 /IMPLIB:x86\Objects\JNIJavaMyoLib.lib /DLL x86\Objects\com_thalmic_myo_Hub.obj x86\Objects\com_thalmic_myo_Myo.obj x86\Objects\handle.obj x86\Objects\jnidevicelistener.obj