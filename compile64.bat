if not defined DevEnvDir (
	call "%GCC%\vcvarsall.bat" x86_amd64
)

mkdir x64\Release\
mkdir x64\Objects\

cl /c /EHsc /O2 /MD /fp:precise /I"%MYO_HOME%\include" /I"%JAVA_HOME%\include" /I"%JAVA_HOME%\include\win32" /Fo"x64\Objects\\" /Fd"x64\Objects\vc100.pdb" com_thalmic_myo_Hub.cpp com_thalmic_myo_Myo.cpp handle.cpp jnidevicelistener.cpp

link /OUT:"x64\Release\JNIJavaMyoLib.dll" /LIBPATH:"%MYO_HOME%\lib" myo64.lib /MACHINE:X64 /IMPLIB:x64\Objects\JNIJavaMyoLib.lib /DLL x64\Objects\com_thalmic_myo_Hub.obj x64\Objects\com_thalmic_myo_Myo.obj x64\Objects\handle.obj x64\Objects\jnidevicelistener.obj