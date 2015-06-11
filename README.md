# myo-java-JNI-Library

C++/C Files required to build a JNI DLL for the myo-java project.

## Windows

** *MSVCP100.dll is required, install as part of VC++, Not sure about other methods.* **

####There are several environmental variables that need to be set up:

* *JAVA_HOME* - Points to the base installation of a JDK
* *MYO_HOME* - Points to the base of a Myo ZIP. 
* *VC* - The base installation of a Visual Studio directory

#### Script to compile
* *compile64.bat* - 64-bit DLL. This DLL will be located *x64/Release/JNIJavaMyoLib.dll*
* *compile32.bat* - 32-bit DLL. This DLL will be located *x86/Release/JNIJavaMyoLib.dll*

## OSX

#### There are several environmental variables that need to be set up:

* *MYO_HOME* - Points to the base of a Myo ZIP. 

#### Script to compile:
* *compile.sh* - 32-bit and 64-bit dynamic library. This JNLIB will be located *libmyo.jnilib*